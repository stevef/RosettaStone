// FidelitySlice: inject one logged pre-action state, play one card, dump Tier A tags.
//
//   FidelitySlice <CardDefs.xml> <slice.json> <out.json>
//
// slice.json comes from research/card-cost/fidelity_slice.py (hs-winprob). Logged
// entity IDs are kept as engine entity IDs (+ID_OFFSET) so the comparison is a
// dictionary diff. Entities the engine creates during the play are reported
// under "new" with their card IDs.

#include <Rosetta/PlayMode/Actions/Generic.hpp>
#include <Rosetta/PlayMode/Cards/Cards.hpp>
#include <Rosetta/PlayMode/Games/Game.hpp>
#include <Rosetta/PlayMode/Games/GameConfig.hpp>
#include <Rosetta/PlayMode/Models/Enchantment.hpp>
#include <Rosetta/PlayMode/Models/Entity.hpp>
#include <Rosetta/PlayMode/Models/Hero.hpp>
#include <Rosetta/PlayMode/Models/HeroPower.hpp>
#include <Rosetta/PlayMode/Models/Location.hpp>
#include <Rosetta/PlayMode/Models/Minion.hpp>
#include <Rosetta/PlayMode/Models/Player.hpp>
#include <Rosetta/PlayMode/Models/Weapon.hpp>
#include <Rosetta/PlayMode/Tasks/PlayerTasks/ChooseTask.hpp>
#include <Rosetta/PlayMode/Tasks/PlayerTasks/EndTurnTask.hpp>
#include <Rosetta/PlayMode/Tasks/PlayerTasks/PlayCardTask.hpp>
#include <Rosetta/PlayMode/Tasks/PlayerTasks/PlayLocationTask.hpp>
#include <Rosetta/PlayMode/Zones/FieldZone.hpp>
#include <Rosetta/PlayMode/Zones/HandZone.hpp>
#include <Rosetta/PlayMode/Zones/SetasideZone.hpp>

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace RosettaStone;
using namespace PlayMode;
using namespace PlayerTasks;
using json = nlohmann::json;

constexpr int ID_OFFSET = 10000;

static int tagOf(const json& e, const char* name, int def = 0)
{
    if (e.contains("tags") && e["tags"].contains(name))
    {
        return e["tags"][name].get<int>();
    }
    return def;
}

static const char* KEYWORD_TAGS[] = {
    "TAUNT",     "DIVINE_SHIELD", "FROZEN",     "STEALTH",  "WINDFURY",
    "RUSH",      "CHARGE",        "LIFESTEAL",  "POISONOUS", "IMMUNE",
    "SILENCED",  "REBORN",        "DORMANT",    "SPELLPOWER", "CANT_ATTACK",
    "LOCATION_ACTION_COOLDOWN", "NUM_ATTACKS_THIS_TURN", "EXHAUSTED"
};

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "usage: FidelitySlice <CardDefs.xml> <slice.json> <out.json>\n";
        return 2;
    }
    Cards::SetCardDefsXmlPath(argv[1]);
    Cards::GetInstance();  // load before any static lookup
    json slice;
    std::ifstream(argv[2]) >> slice;

    json jp = slice["players"];  // [actor, other] from the extractor
    int actorIndex = 0;
    if (jp[1]["tags"].value("FIRST_PLAYER", 0) == 1)
    {
        std::swap(jp[0], jp[1]);  // engine player 1 is the first player
        actorIndex = 1;
    }
    GameConfig config;
    const auto classOf = [](const json& p) {
        const std::string id = p["hero"]["card_id"].get<std::string>();
        const Card* hero = Cards::FindCardByID(id);
        if (hero->id != id || !hero->gameTags.contains(GameTag::CLASS))
        {
            return CardClass::WARRIOR;
        }
        const CardClass c = hero->GetCardClass();
        // The engine has no Death Knight hero (Cards::GetHeroCard has no case
        // for it), so a DK side is started as a Warrior; hero tags are set
        // from the log afterwards.
        return c == CardClass::INVALID || c == CardClass::NEUTRAL ||
                       c == CardClass::DEATHKNIGHT
                   ? CardClass::WARRIOR
                   : c;
    };
    config.player1Class = classOf(jp[0]);
    config.player2Class = classOf(jp[1]);
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.doShuffle = false;
    config.autoRun = false;
    Card* wisp = Cards::FindCardByID("CS2_231");
    for (std::size_t i = 0; i < START_DECK_SIZE; ++i)
    {
        config.player1Deck[i] = wisp;
        config.player2Deck[i] = wisp;
    }
    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);
    if (actorIndex == 1)
    {
        game.Process(game.GetPlayer1(), PlayerTasks::EndTurnTask());
        game.ProcessUntil(Step::MAIN_ACTION);
    }
    game.SetTurn(slice["game_tags"].value("TURN", game.GetTurn()));

    Player* players[2] = { game.GetPlayer1(), game.GetPlayer2() };
    Player* actor = players[actorIndex];
    const int loggedPlayerID[2] = { jp[0]["player_id"].get<int>(),
                                    jp[1]["player_id"].get<int>() };
    const auto controllerOf = [&](const Player* pl) {
        return pl == players[0] ? loggedPlayerID[0] : loggedPlayerID[1];
    };
    std::map<int, Entity*> idmap;  // logged id -> engine entity (fixed entities)
    std::map<Entity*, int> back;   // engine entity -> logged id
    std::vector<std::string> warnings;

    for (int side = 0; side < 2; ++side)
    {
        Player* player = players[side];
        const json& p = jp[side];
        for (auto& card : player->GetHandZone()->GetAll())
        {
            player->GetHandZone()->Remove(card);
            player->GetSetasideZone()->Add(card);
        }
        idmap[p["entity_id"].get<int>()] = player;
        back[player] = p["entity_id"].get<int>();
        player->SetTotalMana(tagOf(p, "RESOURCES", 0));
        player->SetUsedMana(tagOf(p, "RESOURCES_USED", 0));
        player->SetTemporaryMana(tagOf(p, "TEMP_RESOURCES", 0));
        player->SetOverloadLocked(tagOf(p, "OVERLOAD_LOCKED", 0));

        Hero* hero = player->GetHero();
        const json& h = p["hero"];
        idmap[h["id"].get<int>()] = hero;
        back[hero] = h["id"].get<int>();
        hero->SetBaseHealth(tagOf(h, "HEALTH", 30));
        hero->SetDamage(tagOf(h, "DAMAGE", 0));
        hero->SetArmor(tagOf(h, "ARMOR", 0));
        hero->SetExhausted(tagOf(h, "EXHAUSTED", 0) != 0);
        hero->SetNumAttacksThisTurn(tagOf(h, "NUM_ATTACKS_THIS_TURN", 0));
        if (!p["hero_power"].is_null())
        {
            idmap[p["hero_power"]["id"].get<int>()] = hero->heroPower;
            back[hero->heroPower] = p["hero_power"]["id"].get<int>();
        }
        if (!p["weapon"].is_null())
        {
            const json& w = p["weapon"];
            Card* card = Cards::FindCardByID(w["card_id"].get<std::string>());
            const auto weapon = dynamic_cast<Weapon*>(Entity::GetFromCard(
                player, card, std::nullopt, nullptr, w["id"].get<int>() + ID_OFFSET));
            weapon->SetAttack(tagOf(w, "ATK", card->GetCost()));
            // the log keeps a weapon's durability in HEALTH (max) and DAMAGE (used)
            weapon->SetDurability(tagOf(w, "HEALTH", tagOf(w, "DURABILITY", 1)) -
                                  tagOf(w, "DAMAGE", 0));
            hero->AddWeapon(*weapon);
        }
        if (const int atk = tagOf(h, "ATK", 0); atk > 0 && hero->GetAttack() != atk)
        {
            hero->SetGameTag(GameTag::ATK, atk - (hero->HasWeapon() ? hero->weapon->GetAttack() : 0));
        }
        for (const json& b : p["board"])
        {
            Card* card = Cards::FindCardByID(b["card_id"].get<std::string>());
            if (card->id != b["card_id"].get<std::string>())
            {
                warnings.emplace_back("board card not found: " + b["card_id"].get<std::string>());
                continue;
            }
            Playable* pl = Entity::GetFromCard(player, card, std::nullopt,
                                               player->GetFieldZone(),
                                               b["id"].get<int>() + ID_OFFSET);
            if (const auto minion = dynamic_cast<Minion*>(pl))
            {
                minion->SetAttack(tagOf(b, "ATK", minion->GetAttack()));
                minion->SetBaseHealth(tagOf(b, "HEALTH", minion->GetBaseHealth()));
            }
            if (const auto ch = dynamic_cast<Character*>(pl))
            {
                ch->SetDamage(tagOf(b, "DAMAGE", 0));
            }
            player->GetFieldZone()->Add(pl);
            // after Add: the zone marks a new minion exhausted, the log says otherwise
            for (const char* name : KEYWORD_TAGS)
            {
                if (b["tags"].contains(name))
                {
                    pl->SetGameTag(StrToEnum<GameTag>(name), b["tags"][name].get<int>());
                }
            }
            if (const int cr = tagOf(b, "CREATOR", 0)) pl->SetGameTag(GameTag::CREATOR, cr);
        }
        for (const json& c : p["hand"])
        {
            Card* card = c["card_id"].is_null() ? wisp : Cards::FindCardByID(c["card_id"].get<std::string>());
            Playable* pl = Entity::GetFromCard(player, card, std::nullopt, player->GetHandZone(),
                                               c["id"].get<int>() + ID_OFFSET);
            player->GetHandZone()->Add(pl);
            if (c["tags"].contains("COST"))
            {
                pl->SetCost(c["tags"]["COST"].get<int>());
            }
            if (const int cr = tagOf(c, "CREATOR", 0)) pl->SetGameTag(GameTag::CREATOR, cr);
        }
    }

    const auto find = [&](int id) -> Playable* {
        if (id <= 0) return nullptr;
        if (const auto it = idmap.find(id); it != idmap.end()) return dynamic_cast<Playable*>(it->second);
        if (const auto it = game.entityList.find(id + ID_OFFSET); it != game.entityList.end()) return it->second;
        return nullptr;
    };
    // Enchantment entities that existed before the block (slice "enchantments"
    // per player): created with their logged IDs, attached to the logged
    // target, buff already folded into the injected stats so not re-applied.
    for (int side = 0; side < 2; ++side)
    {
        for (const json& en : jp[side].value("enchantments", json::array()))
        {
            if (en["card_id"].is_null()) continue;
            Card* card = Cards::FindCardByID(en["card_id"].get<std::string>());
            if (card->id != en["card_id"].get<std::string>()) { warnings.emplace_back("enchantment card not found: " + en["card_id"].get<std::string>()); continue; }
            const int attached = tagOf(en, "ATTACHED", 0), creator = tagOf(en, "CREATOR", 0);
            Entity* target = find(attached);
            if (!target) { if (const auto it = idmap.find(attached); it != idmap.end()) target = it->second; }
            if (!target) { warnings.emplace_back("enchantment target not injected: " + en["card_id"].get<std::string>()); continue; }
            Playable* owner = find(creator);
            if (!owner) owner = players[side]->GetHero();
            auto inst = Enchantment::GetInstance(owner, card, target, 0, 0, en["id"].get<int>() + ID_OFFSET);
            inst->SetGameTag(GameTag::CREATOR, creator);
            inst->SetGameTag(GameTag::ATTACHED, attached);
            inst->SetGameTag(GameTag::ZONE, tagOf(en, "ZONE", 1));
        }
    }

    // The action
    const json& a = slice["action"];
    Playable* source = find(a["entity"].get<int>());
    if (!source)
    {
        std::cerr << "played entity not injected\n";
        return 1;
    }
    Playable* target = find(a["target"].get<int>());
    const int pos = a.value("position", 0);
    const int sub = a.value("suboption", -1);
    std::string status = "ok";
    try
    {
        if (const auto location = dynamic_cast<Location*>(source);
            location && location->GetZoneType() == ZoneType::PLAY)
        {
            // using a location already on the board
            game.Process(actor, PlayLocationTask(location, target));
        }
        else
        {
            game.Process(actor, PlayCardTask(source, target, pos > 0 ? pos - 1 : -1,
                                             sub < 0 ? 0 : sub));
        }
    }
    catch (const std::exception& e)
    {
        status = std::string("exception: ") + e.what();
    }

    // Choices the log resolved inside the block (Discover picks, Rewind's
    // Keep/Rewind): pick the engine's option with the same card ID, in order.
    try
    {
        for (const auto& chosen : a.value("chosen", json::array()))
        {
            if (!actor->choice)
            {
                warnings.emplace_back("logged choice but engine offered none: " + chosen.get<std::string>());
                break;
            }
            int pick = -1;
            for (const int id : actor->choice->choices)
            {
                if (game.entityList[id]->card->id == chosen.get<std::string>())
                {
                    pick = id;
                    break;
                }
            }
            if (pick < 0)
            {
                warnings.emplace_back("logged choice not among engine options: " + chosen.get<std::string>());
                break;
            }
            game.Process(actor, ChooseTask::Pick(actor, pick));
        }
    }
    catch (const std::exception& e)
    {
        status = std::string("exception in choice: ") + e.what();
    }

    // Dump Tier A tags per logged entity id
    json out;
    out["status"] = status;
    out["warnings"] = warnings;
    json newEntities = json::array();
    const auto dumpTags = [&](Entity* e) {
        json t;
        for (const auto& name : slice["tier_a"])
        {
            const std::string n = name.get<std::string>();
            GameTag tag;
            try { tag = StrToEnum<GameTag>(n); } catch (...) { continue; }
            if (tag == GameTag::INVALID) continue;
            t[n] = e->GetGameTag(tag);
        }
        // Entity-valued tags: map engine IDs back to logged IDs.
        for (const char* ref : { "CREATOR", "ATTACHED" })
        {
            if (!t.contains(ref) || t[ref].get<int>() == 0) continue;
            const int v = t[ref].get<int>();
            if (v >= ID_OFFSET) t[ref] = v - ID_OFFSET;
            else if (const auto it = game.entityList.find(v); it != game.entityList.end() && back.contains(it->second) && !game.entityList.contains(v + ID_OFFSET)) t[ref] = back[it->second];
        }
        // Tags the engine keeps as pointers/fields rather than as GameTags.
        if (const auto pl = dynamic_cast<Playable*>(e))
        {
            t["CONTROLLER"] = controllerOf(pl->player);
            // The engine's CardType enum is positional (LOCATION = 16, HS = 39);
            // CardDefs.xml's own value is kept in hsTags.
            if (pl->card && pl->card->hsTags.contains(202))
            {
                t["CARDTYPE"] = pl->card->hsTags.at(202);
            }
        }
        if (const auto ch = dynamic_cast<Character*>(e))
        {
            t["ATK"] = ch->GetAttack();
        }
        if (const auto pl = dynamic_cast<Player*>(e))
        {
            t["CONTROLLER"] = controllerOf(pl);
            t["FIRST_PLAYER"] = pl == players[0] ? 1 : 0;
            t["CARDTYPE"] = 2;
            t["ZONE"] = 1;
            t["PLAYSTATE"] = static_cast<int>(pl->playState);
            t["CURRENT_PLAYER"] = pl == game.GetCurrentPlayer() ? 1 : 0;
            t["HERO_ENTITY"] = back[pl->GetHero()];
            t["MAXHANDSIZE"] = 10;
            t["MAXRESOURCES"] = 10;
            t["NUM_TURNS_LEFT"] = 1;
            t["MULLIGAN_STATE"] = 4;
            t.erase("TURN");  // the player's own turn counter is not kept
        }
        return t;
    };
    for (const auto& [id, pl] : game.entityList)
    {
        if (id >= ID_OFFSET)
        {
            out[std::to_string(id - ID_OFFSET)] = dumpTags(pl);
        }
        else if (const auto it = back.find(pl); it != back.end())
        {
            out[std::to_string(it->second)] = dumpTags(pl);
        }
        else if (pl->card && pl->card->id != "CS2_231")
        {
            newEntities.push_back({ { "engine_id", id }, { "card_id", pl->card->id }, { "tags", dumpTags(pl) } });
        }
    }
    for (Player* player : players)
    {
        out[std::to_string(back[player])] = dumpTags(player);
    }
    out["new"] = newEntities;
    out["1"] = { { "TURN", game.GetTurn() } };  // game entity: only the turn is kept
    std::ofstream(argv[3]) << out.dump(1);
    std::cout << "status " << status << ", entities dumped " << out.size() - 3
              << ", new " << newEntities.size() << ", warnings " << warnings.size() << '\n';
    for (const auto& w : warnings) std::cout << "  " << w << '\n';
    return 0;
}
