// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// Hearthstone++ is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/PlayMode/CardSets/EscapeFromVioletHoldCardsGen.hpp>
#include <Rosetta/PlayMode/Cards/CardPowers.hpp>

namespace RosettaStone::PlayMode
{
void EscapeFromVioletHoldCardsGen::AddHeroes(
    std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddHeroPowers(
    std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddDruid(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------------ SPELL - DRUID
    // [JAIL_436] Widow's Bite - COST:2
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Common
    // --------------------------------------------------------
    // Text: Give your hero +1 Attack this turn. Gain 1 Armor.
    //       Add "Widow's Feast" to your hand.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("JAIL_436e", EntityType::HERO));
    cardDef.power.AddPowerTask(std::make_shared<ArmorTask>(1));
    cardDef.power.AddPowerTask(
        std::make_shared<AddCardTask>(EntityType::HAND, "JAIL_436t"));
    cards.emplace("JAIL_436", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddDruidNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------ ENCHANTMENT - DRUID
    // [JAIL_436e] Threads - COST:0
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: +1 Attack this turn.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("JAIL_436e"));
    cards.emplace("JAIL_436e", cardDef);

    // ------------------------------------ ENCHANTMENT - DRUID
    // [JAIL_436e2] Webs - COST:0
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: +2 Attack this turn.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("JAIL_436e2"));
    cards.emplace("JAIL_436e2", cardDef);

    // ------------------------------------ ENCHANTMENT - DRUID
    // [JAIL_436e3] Fates - COST:0
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: +4 Attack this turn.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("JAIL_436e3"));
    cards.emplace("JAIL_436e3", cardDef);

    // ------------------------------------------ SPELL - DRUID
    // [JAIL_436t] Widow's Feast - COST:2
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: Give your hero +2 Attack this turn. Gain 2 Armor.
    //       Add "Widow's Banquet" to your hand.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("JAIL_436e2", EntityType::HERO));
    cardDef.power.AddPowerTask(std::make_shared<ArmorTask>(2));
    cardDef.power.AddPowerTask(
        std::make_shared<AddCardTask>(EntityType::HAND, "JAIL_436t2"));
    cards.emplace("JAIL_436t", cardDef);

    // ------------------------------------------ SPELL - DRUID
    // [JAIL_436t2] Widow's Banquet - COST:2
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: Give your hero +4 Attack this turn. Gain 4 Armor.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("JAIL_436e3", EntityType::HERO));
    cardDef.power.AddPowerTask(std::make_shared<ArmorTask>(4));
    cards.emplace("JAIL_436t2", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddHunter(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------------- SPELL - HUNTER
    // [JAIL_879] Beast Tripwire - COST:4
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Rare
    // --------------------------------------------------------
    // Text: Summon a random 5-Cost Beast.
    //       Shuffle 2 spells into your deck
    //       that do it again when drawn.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<RandomMinionTask>(TagValues{
        { GameTag::COST, 5, RelaSign::EQ },
        { GameTag::CARDRACE, static_cast<int>(Race::BEAST), RelaSign::EQ } }));
    cardDef.power.AddPowerTask(std::make_shared<SummonTask>());
    cardDef.power.AddPowerTask(
        std::make_shared<AddCardTask>(EntityType::DECK, "JAIL_879t", 2));
    cards.emplace("JAIL_879", cardDef);

    // -------------------------------------- LOCATION - HUNTER
    // [JAIL_877] Underbelly Network - COST:2 [HP:2]
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Common
    // --------------------------------------------------------
    // Text: Summon a 2/1 Rat with "<b>Deathrattle:</b> Draw a card."
    // --------------------------------------------------------
    // RefTag:
    // - DEATHRATTLE = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(
        std::make_shared<SummonTask>("JAIL_877t", SummonSide::RIGHT));
    cards.emplace("JAIL_877", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------------- SPELL - HUNTER
    // [JAIL_879t] Tripped Beast Tripwire - COST:4
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: <b>Casts When Drawn</b>
    //       Summon a random 5-Cost Beast.
    // --------------------------------------------------------
    // RefTag:
    // - CASTS_WHEN_DRAWN = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddTopdeckTask(std::make_shared<RandomMinionTask>(TagValues{
        { GameTag::COST, 5, RelaSign::EQ },
        { GameTag::CARDRACE, static_cast<int>(Race::BEAST), RelaSign::EQ } }));
    cardDef.power.AddTopdeckTask(std::make_shared<SummonTask>());
    cardDef.power.AddPowerTask(std::make_shared<RandomMinionTask>(TagValues{
        { GameTag::COST, 5, RelaSign::EQ },
        { GameTag::CARDRACE, static_cast<int>(Race::BEAST), RelaSign::EQ } }));
    cardDef.power.AddPowerTask(std::make_shared<SummonTask>());
    cards.emplace("JAIL_879t", cardDef);

    // ---------------------------------------- MINION - HUNTER
    // [JAIL_877t] Snoot Hoarder - COST:2 [ATK:2/HP:1]
    // - Race: Beast, Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: <b>Deathrattle:</b> Draw a card.
    // --------------------------------------------------------
    // GameTag:
    // - DEATHRATTLE = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddDeathrattleTask(std::make_shared<DrawTask>(1));
    cards.emplace("JAIL_877t", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddMage(std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddMageNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddPaladin(
    std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddPaladinNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddPriest(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ---------------------------------------- MINION - PRIEST
    // [CAP_803] Lingering Spirit - COST:4 [ATK:3/HP:3]
    // - Race: Undead, Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Rare
    // --------------------------------------------------------
    // Text: <b>Reborn</b> <b>Deathrattle:</b> Restore #3 Health
    //       to your hero. Excess healing damages a random enemy.
    // --------------------------------------------------------
    // GameTag:
    // - DEATHRATTLE = 1
    // - REBORN = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddDeathrattleTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            const auto playable = dynamic_cast<Playable*>(source);
            Hero* hero = player->GetHero();
            const int excess = std::max(0, 3 - hero->GetDamage());
            hero->TakeHeal(playable, 3);
            if (excess == 0)
            {
                return;
            }
            std::vector<Character*> enemies{ player->opponent->GetHero() };
            for (auto& minion : player->opponent->GetFieldZone()->GetAll())
            {
                enemies.emplace_back(minion);
            }
            const auto idx = Random::get<std::size_t>(0, enemies.size() - 1);
            Generic::TakeDamageToCharacter(playable, enemies[idx], excess,
                                           false);
        }));
    cards.emplace("CAP_803", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddPriestNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddRogue(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------------- MINION - ROGUE
    // [CAP_004] Disguised Operator - COST:1 [ATK:4/HP:2]
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Legendary
    // --------------------------------------------------------
    // Text: Can be played on either side. <b>Rush</b>.
    //       <b>Deathrattle:</b> Your opponent draws 2 cards.
    // --------------------------------------------------------
    // GameTag:
    // - DEATHRATTLE = 1
    // - RUSH = 1
    // - DISGUISED = 1
    // --------------------------------------------------------
    // NOTE: "Can be played on either side" (DISGUISED, CAP_004a/b) has no
    //       engine support; this plays on the friendly side only.
    cardDef.ClearData();
    cardDef.power.AddDeathrattleTask(std::make_shared<DrawOpTask>(2));
    cards.emplace("CAP_004", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddRogueNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddShaman(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ---------------------------------------- MINION - SHAMAN
    // [JAIL_800] Mug'Zee - COST:6 [ATK:6/HP:7]
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Start of Game:</b> If your deck has no other minions,
    //       get Mug's Hero Power. If it has no spells, get Zee's!
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - START_OF_GAME_KEYWORD = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddTrigger(std::make_shared<Trigger>(TriggerType::GAME_START));
    cardDef.power.GetTrigger()->triggerActivation = TriggerActivation::DECK;
    cardDef.power.GetTrigger()->removeAfterTriggered = true;
    cardDef.power.GetTrigger()->tasks = { std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            bool otherMinion = false, spell = false;
            for (auto& playable : player->GetDeckZone()->GetAll())
            {
                const auto type = playable->card->GetCardType();
                otherMinion |= type == CardType::MINION && playable != source;
                spell |= type == CardType::SPELL;
            }
            const char* heroPower = !otherMinion ? "JAIL_800hp1"
                                    : !spell     ? "JAIL_800hp2"
                                                 : nullptr;
            if (heroPower)
            {
                player->ReplaceHeroPower(dynamic_cast<HeroPower*>(
                    Entity::GetFromCard(player, Cards::FindCardByID(heroPower))));
            }
        }) };
    cards.emplace("JAIL_800", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddShamanNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------ HERO_POWER - SHAMAN
    // [JAIL_800hp1] Mug's Magic - COST:0
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: <b>Passive</b> Your first minion each turn costs (2) less.
    //       <i>(Unlocks on Turn 3.)</i>
    // --------------------------------------------------------
    // GameTag:
    // - TAG_SCRIPT_DATA_NUM_1 = 3
    // - HIDE_COST = 1
    // --------------------------------------------------------
    // NOTE: "Turn 3" is read as the owner's 3rd turn, computed from the
    //       game turn counter assuming player 1 went first. A restless HAND
    //       aura is not supported by Aura::RenewAll, so this is a start-of-
    //       turn trigger that gives the player JAIL_800hp1e.
    cardDef.ClearData();
    cardDef.power.AddTrigger(std::make_shared<Trigger>(TriggerType::TURN_START));
    cardDef.power.GetTrigger()->conditions = SelfCondList{
        std::make_shared<SelfCondition>([](const Playable* playable) {
            const Player* player = playable->player;
            const int ownTurn =
                (player->game->GetTurn() +
                 (player == player->game->GetPlayer1() ? 1 : 0)) /
                2;
            return ownTurn >= 3;
        })
    };
    cardDef.power.GetTrigger()->tasks = { std::make_shared<AddEnchantmentTask>(
        "JAIL_800hp1e", EntityType::PLAYER) };
    cards.emplace("JAIL_800hp1", cardDef);

    // ------------------------------------ ENCHANTMENT - SHAMAN
    // [JAIL_800hp1e] Mug's Magic - COST:0
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: The next minion you play costs (2) less.
    // --------------------------------------------------------
    // GameTag:
    // - CANT_BE_SILENCED = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddAura(std::make_shared<Aura>(
        AuraType::HAND, EffectList{ Effects::ReduceCost(2) }));
    {
        const auto aura = dynamic_cast<Aura*>(cardDef.power.GetAura());
        aura->condition =
            std::make_shared<SelfCondition>(SelfCondition::IsMinion());
        aura->removeTrigger = { TriggerType::PLAY_MINION, nullptr };
    }
    cards.emplace("JAIL_800hp1e", cardDef);

    // ------------------------------------ HERO_POWER - SHAMAN
    // [JAIL_800hp2] Zee's Might - COST:0
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: <b>Passive</b> Every fifth minion you play triggers
    //       its <b>Battlecry</b> twice.
    // --------------------------------------------------------
    // GameTag:
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    // NOTE: PLAY_MINION fires before the battlecry, so the flag is set on
    //       the 5th minion and cleared on the next one; a weapon battlecry
    //       played in between would also double, and a concurrent Brann
    //       would be clobbered.
    cardDef.ClearData();
    cardDef.power.AddTrigger(std::make_shared<Trigger>(TriggerType::PLAY_MINION));
    cardDef.power.GetTrigger()->triggerSource = TriggerSource::FRIENDLY;
    cardDef.power.GetTrigger()->tasks = { std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            const int count = source->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_2) + 1;
            source->SetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_2, count);
            player->playerAuraEffects.SetValue(GameTag::EXTRA_BATTLECRIES_BASE,
                                               count % 5 == 0 ? 1 : 0);
        }) };
    cards.emplace("JAIL_800hp2", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddWarlock(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------- LOCATION - WARLOCK
    // [JAIL_511] Spire of Solitude - COST:5 [HP:2]
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Epic
    // --------------------------------------------------------
    // Text: Summon a Demon with stats equal to your hand size.
    //       It attacks a random enemy minion.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            if (player->GetFieldZone()->IsFull())
            {
                return;
            }
            const int size = player->GetHandZone()->GetCount();
            const auto demon = dynamic_cast<Minion*>(Entity::GetFromCard(
                player, Cards::FindCardByID("JAIL_511t")));
            demon->SetAttack(size);
            demon->SetBaseHealth(size);
            Generic::Summon(demon, -1, source);
            const auto enemies = player->opponent->GetFieldZone()->GetAll();
            if (enemies.empty())
            {
                return;
            }
            Generic::AddEnchantment(Cards::FindCardByID("JAIL_511e"),
                                    dynamic_cast<Playable*>(source), demon);
            const auto idx = Random::get<std::size_t>(0, enemies.size() - 1);
            Generic::Attack(player, demon, enemies[idx], true);
        }));
    cards.emplace("JAIL_511", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddWarlockNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ---------------------------------- ENCHANTMENT - WARLOCK
    // [JAIL_511e] Solitary - COST:0
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: <b>Immune</b> this turn.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("JAIL_511e"));
    cards.emplace("JAIL_511e", cardDef);

    // --------------------------------------- MINION - WARLOCK
    // [JAIL_511t] Shivarra Infiltrator - COST:1 [ATK:1/HP:1]
    // - Race: Demon, Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(nullptr);
    cards.emplace("JAIL_511t", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddWarrior(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // --------------------------------------- WEAPON - WARRIOR
    // [CAP_103] Hand Cannon - COST:3 [ATK:3/HP:2]
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Rare
    // --------------------------------------------------------
    // Text: After your hero attacks, your Cannoneers FIRE!
    // --------------------------------------------------------
    // GameTag:
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    // NOTE: "FIRE!" is read as the Cannoneer's own end-of-turn effect
    //       (CAP_107t: deal 1 damage to a random enemy).
    cardDef.ClearData();
    cardDef.power.AddTrigger(
        std::make_shared<Trigger>(TriggerType::AFTER_ATTACK));
    cardDef.power.GetTrigger()->triggerSource = TriggerSource::HERO;
    cardDef.power.GetTrigger()->tasks = { std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            for (auto& minion : player->GetFieldZone()->GetAll())
            {
                if (minion->card->id != "CAP_107t")
                {
                    continue;
                }
                std::vector<Character*> enemies{ player->opponent->GetHero() };
                for (auto& enemy : player->opponent->GetFieldZone()->GetAll())
                {
                    enemies.emplace_back(enemy);
                }
                const auto idx = Random::get<std::size_t>(0, enemies.size() - 1);
                Generic::TakeDamageToCharacter(minion, enemies[idx], 1, false);
            }
        }) };
    cards.emplace("CAP_103", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddWarriorNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // --------------------------------------- MINION - WARRIOR
    // [CAP_107t] Cannoneer - COST:1 [ATK:1/HP:1]
    // - Race: Pirate, Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: At the end of your turn, deal 1 damage to a random enemy.
    // --------------------------------------------------------
    // GameTag:
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddTrigger(std::make_shared<Trigger>(TriggerType::TURN_END));
    cardDef.power.GetTrigger()->tasks = {
        std::make_shared<RandomTask>(EntityType::ENEMIES, 1),
        std::make_shared<DamageTask>(EntityType::STACK, 1)
    };
    cards.emplace("CAP_107t", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddDemonHunter(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------ SPELL - DEMONHUNTER
    // [JAIL_732] Void Soul - COST:1
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Rare
    // - Spell School: Shadow
    // --------------------------------------------------------
    // Text: Summon a random @-Cost Demon.
    //       Improve your future Void Souls.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_SCRIPT_DATA_NUM_1 = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            // The level lives on the player: 1 + Void Souls cast this game.
            const int level =
                1 + player->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1);
            auto cards = Cards::GetAllCards();
            std::vector<Card*> pool;
            for (Card* card : cards)
            {
                if (card->IsCollectible() &&
                    card->GetCardType() == CardType::MINION &&
                    card->GetRace() == Race::DEMON && card->GetCost() == level)
                {
                    pool.emplace_back(card);
                }
            }
            if (!pool.empty() && !player->GetFieldZone()->IsFull())
            {
                const auto idx = Random::get<std::size_t>(0, pool.size() - 1);
                Generic::Summon(dynamic_cast<Minion*>(Entity::GetFromCard(
                                    player, pool[idx])),
                                -1, source);
            }
            player->SetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1, level);
            for (auto& playable : player->GetHandZone()->GetAll())
            {
                if (playable->card->id == "JAIL_732")
                {
                    playable->SetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1,
                                         level + 1);
                }
            }
        }));
    cards.emplace("JAIL_732", cardDef);

    // ----------------------------------- MINION - DEMONHUNTER
    // [JAIL_733] Vicious Voidscale - COST:3 [ATK:3/HP:4]
    // - Race: Naga, Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Rare
    // --------------------------------------------------------
    // Text: <b>Taunt</b> <b>Deathrattle:</b> Get a Void Soul.
    // --------------------------------------------------------
    // GameTag:
    // - TAUNT = 1
    // - DEATHRATTLE = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddDeathrattleTask(
        std::make_shared<AddCardTask>(EntityType::HAND, "JAIL_732"));
    cards.emplace("JAIL_733", cardDef);

    // ----------------------------------- WEAPON - DEMONHUNTER
    // [JAIL_730] Stardust Scythe - COST:4 [ATK:3/HP:3]
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Common
    // --------------------------------------------------------
    // Text: After your hero attacks, get a Void Soul.
    // --------------------------------------------------------
    // GameTag:
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddTrigger(
        std::make_shared<Trigger>(TriggerType::AFTER_ATTACK));
    cardDef.power.GetTrigger()->triggerSource = TriggerSource::HERO;
    cardDef.power.GetTrigger()->tasks = { std::make_shared<AddCardTask>(
        EntityType::HAND, "JAIL_732") };
    cards.emplace("JAIL_730", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddDemonHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void EscapeFromVioletHoldCardsGen::AddNeutral(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // --------------------------------------- MINION - NEUTRAL
    // [JAIL_101] Violet Punisher - COST:3 [ATK:4/HP:3]
    // - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Common
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> Choose an enemy minion.
    //       Steal its <b>Bonus Effects</b> and gain +1/+1
    //       for each stolen.
    // --------------------------------------------------------
    // GameTag:
    // - BATTLECRY = 1
    // --------------------------------------------------------
    // RefTag:
    // - BONUS_EFFECTS = 1
    // --------------------------------------------------------
    // PlayReq:
    // - REQ_TARGET_IF_AVAILABLE = 0
    // - REQ_ENEMY_TARGET = 0
    // - REQ_MINION_TARGET = 0
    // --------------------------------------------------------
    // NOTE: "Bonus Effects" has no rules text in CardDefs.xml; read here as
    //       the enchantments attached to the target.
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, Playable* target) {
            if (!target)
            {
                return;
            }
            const auto self = dynamic_cast<Playable*>(source);
            const auto stolen = target->appliedEnchantments;
            for (const auto& enchantment : stolen)
            {
                Generic::AddEnchantment(enchantment->card, self, self,
                                        enchantment->GetScriptTag1(),
                                        enchantment->GetScriptTag2(), 0);
                enchantment->Remove();
                std::erase(target->appliedEnchantments, enchantment);
                Generic::AddEnchantment(Cards::FindCardByID("JAIL_101e"), self,
                                        self, 0, 0, 0);
            }
        }));
    cardDef.property.playReqs = PlayReqs{ { PlayReq::REQ_TARGET_IF_AVAILABLE, 0 },
                                          { PlayReq::REQ_ENEMY_TARGET, 0 },
                                          { PlayReq::REQ_MINION_TARGET, 0 } };
    cards.emplace("JAIL_101", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddNeutralNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ---------------------------------- ENCHANTMENT - NEUTRAL
    // [JAIL_101e] Always Punished - COST:0
    // - Set: ESCAPEFROM_VIOLET_HOLD
    // --------------------------------------------------------
    // Text: +1/+1 and a stolen <b>Bonus Effect.</b>
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("JAIL_101e"));
    cards.emplace("JAIL_101e", cardDef);
}

void EscapeFromVioletHoldCardsGen::AddAll(std::map<std::string, CardDef>& cards)
{
    AddHeroes(cards);
    AddHeroPowers(cards);

    AddDruid(cards);
    AddDruidNonCollect(cards);

    AddHunter(cards);
    AddHunterNonCollect(cards);

    AddMage(cards);
    AddMageNonCollect(cards);

    AddPaladin(cards);
    AddPaladinNonCollect(cards);

    AddPriest(cards);
    AddPriestNonCollect(cards);

    AddRogue(cards);
    AddRogueNonCollect(cards);

    AddShaman(cards);
    AddShamanNonCollect(cards);

    AddWarlock(cards);
    AddWarlockNonCollect(cards);

    AddWarrior(cards);
    AddWarriorNonCollect(cards);

    AddDemonHunter(cards);
    AddDemonHunterNonCollect(cards);

    AddNeutral(cards);
    AddNeutralNonCollect(cards);
}
}  // namespace RosettaStone::PlayMode
