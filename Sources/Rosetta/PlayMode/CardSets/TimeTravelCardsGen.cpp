// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// Hearthstone++ is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/PlayMode/CardSets/TimeTravelCardsGen.hpp>
#include <Rosetta/PlayMode/Cards/CardPowers.hpp>

namespace RosettaStone::PlayMode
{
void TimeTravelCardsGen::AddHeroes(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddHeroPowers(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddDruid(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddDruidNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddHunter(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddMage(std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------------- SPELL - MAGE
    // [TIME_000] Semi-Stable Portal - COST:2
    // - Set: TIME_TRAVEL, Rarity: Rare
    // - Spell School: Arcane
    // --------------------------------------------------------
    // Text: <b>Rewind</b> Add a random minion to your hand.
    //       It costs (3) less.
    // --------------------------------------------------------
    // GameTag:
    // - REWIND = 1
    // --------------------------------------------------------
    // Rewind itself is generic: Generic::OfferRewind after any REWIND card.
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, [[maybe_unused]] Entity* source,
           [[maybe_unused]] Playable* target) {
            std::vector<Card*> pool;
            for (Card* card : Cards::GetAllCards())
            {
                if (card->IsCollectible() &&
                    card->GetCardType() == CardType::MINION)
                {
                    pool.emplace_back(card);
                }
            }
            Card* pick = pool[Random::get<std::size_t>(0, pool.size() - 1)];
            Playable* minion = Entity::GetFromCard(player, pick);
            if (Generic::AddCardToHand(player, minion))
            {
                minion->SetCost(std::max(0, minion->GetCost() - 3));
            }
        }));
    cards.emplace("TIME_000", cardDef);
}

void TimeTravelCardsGen::AddMageNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddPaladin(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddPaladinNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddPriest(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------- MINION - DEATHKNIGHT
    // [TIME_619] Talanji of the Graves - COST:4 [ATK:4/HP:5]
    // - Race: Undead, Set: TIME_TRAVEL, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Fabled.</b> <b>Battlecry:</b> Draw Bwonsamdi
    //       <i>(or resurrect him if he has died)</i>.
    //       Choose a Boon to give him.
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - BATTLECRY = 1
    // - FABLED = 1
    // --------------------------------------------------------
    // NOTE: Filed under Priest because the engine has no Death Knight
    //       class. Fabled has no rules text in CardDefs.xml; read here as
    //       "the bundle cards (TIME_619t, TIME_619t2) start in your deck".
    //       Boons: player tag TAG_SCRIPT_DATA_ENT_1 = count,
    //       TAG_SCRIPT_DATA_ENT_2 = bitmask (1 Taunt, 2 Lifesteal, 4 Rush).
    cardDef.ClearData();
    cardDef.power.AddTrigger(std::make_shared<Trigger>(TriggerType::GAME_START));
    cardDef.power.GetTrigger()->triggerActivation = TriggerActivation::DECK;
    cardDef.power.GetTrigger()->removeAfterTriggered = true;
    cardDef.power.GetTrigger()->tasks = {
        std::make_shared<AddCardTask>(EntityType::DECK, "TIME_619t"),
        std::make_shared<AddCardTask>(EntityType::DECK, "TIME_619t2")
    };
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            bool died = false;
            for (auto& playable : player->GetGraveyardZone()->GetAll())
            {
                died |= playable->card->id == "TIME_619t";
            }
            if (died && !player->GetFieldZone()->IsFull())
            {
                const auto bwonsamdi = dynamic_cast<Minion*>(Entity::GetFromCard(
                    player, Cards::FindCardByID("TIME_619t")));
                Generic::Summon(bwonsamdi, -1, source);
                const int mask = player->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_2);
                const char* boons[] = { "TIME_619e4", "TIME_619e5", "TIME_619e6" };
                for (int i = 0; i < 3; ++i)
                {
                    if (mask & (1 << i))
                    {
                        Generic::AddEnchantment(Cards::FindCardByID(boons[i]),
                                                dynamic_cast<Playable*>(source),
                                                bwonsamdi);
                    }
                }
            }
            else
            {
                for (auto& task : ComplexTask::DrawCardFromDeck(
                         1, SelfCondList{ std::make_shared<SelfCondition>(
                                SelfCondition::IsCardID("TIME_619t")) }))
                {
                    task->SetPlayer(player);
                    task->SetSource(source);
                    task->Run();
                }
            }
        }));
    cardDef.power.AddPowerTask(std::make_shared<DiscoverTask>(
        std::vector<Card*>{ Cards::FindCardByID("TIME_619t3"),
                            Cards::FindCardByID("TIME_619t4"),
                            Cards::FindCardByID("TIME_619t5") },
        DiscoverType::INVALID, CardType::INVALID, CardClass::INVALID,
        Race::INVALID, Rarity::INVALID, ChoiceAction::CAST_SPELL, 3, false));
    cards.emplace("TIME_619", cardDef);
}

void TimeTravelCardsGen::AddPriestNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------- MINION - DEATHKNIGHT
    // [TIME_619t] Bwonsamdi - COST:6 [ATK:6/HP:6]
    // - Race: Undead, Set: TIME_TRAVEL
    // --------------------------------------------------------
    // Text: <b>Deathrattle:</b> Summon a random @-Cost minion.
    //       <i>(Any Boons given to Bwonsamdi carry over.)</i>
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - DEATHRATTLE = 1
    // - TAG_SCRIPT_DATA_NUM_1 = 4
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddDeathrattleTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            const int cost = source->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1) +
                             2 * player->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1);
            auto task = std::make_shared<RandomMinionTask>(
                TagValues{ { GameTag::COST, cost, RelaSign::EQ } });
            task->SetPlayer(player);
            task->SetSource(source);
            if (task->Run() == TaskStatus::COMPLETE)
            {
                auto summon = std::make_shared<SummonTask>();
                summon->SetPlayer(player);
                summon->SetSource(source);
                summon->Run();
            }
        }));
    cards.emplace("TIME_619t", cardDef);

    // ------------------------------------ SPELL - DEATHKNIGHT
    // [TIME_619t2] What Befell Zandalar - COST:3
    // - Set: TIME_TRAVEL
    // --------------------------------------------------------
    // Text: Deal $2 damage to all enemies.
    //       Choose a Boon to give to Bwonsamdi.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(
        std::make_shared<DamageTask>(EntityType::ENEMIES, 2, true));
    cardDef.power.AddPowerTask(std::make_shared<DiscoverTask>(
        std::vector<Card*>{ Cards::FindCardByID("TIME_619t3"),
                            Cards::FindCardByID("TIME_619t4"),
                            Cards::FindCardByID("TIME_619t5") },
        DiscoverType::INVALID, CardType::INVALID, CardClass::INVALID,
        Race::INVALID, Rarity::INVALID, ChoiceAction::CAST_SPELL, 3, false));
    cards.emplace("TIME_619t2", cardDef);

    // ------------------------------------ SPELL - DEATHKNIGHT
    // [TIME_619t3] Boon of Power - COST:0
    // - Set: TIME_TRAVEL
    // --------------------------------------------------------
    // Text: Give Bwonsamdi <b>Taunt</b> <i>permanently</i>.
    //       Minions summoned by his <b>Deathrattle</b> cost (2) more.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, [[maybe_unused]] Entity* source,
           [[maybe_unused]] Playable* target) {
            player->SetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1,
                               player->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1) + 1);
            player->SetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_2,
                               player->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_2) | 1);
        }));
    cardDef.power.AddPowerTask(
        std::make_shared<IncludeTask>(EntityType::MINIONS_HAND_DECK_FIELD));
    cardDef.power.AddPowerTask(std::make_shared<FilterStackTask>(SelfCondList{
        std::make_shared<SelfCondition>(SelfCondition::IsCardID("TIME_619t")) }));
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("TIME_619e4", EntityType::STACK));
    cards.emplace("TIME_619t3", cardDef);
    // ------------------------------------ SPELL - DEATHKNIGHT
    // [TIME_619t4] Boon of Longevity - COST:0
    // - Set: TIME_TRAVEL
    // --------------------------------------------------------
    // Text: Give Bwonsamdi <b>Lifesteal</b> <i>permanently</i>.
    //       Minions summoned by his <b>Deathrattle</b> cost (2) more.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, [[maybe_unused]] Entity* source,
           [[maybe_unused]] Playable* target) {
            player->SetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1,
                               player->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1) + 1);
            player->SetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_2,
                               player->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_2) | 2);
        }));
    cardDef.power.AddPowerTask(
        std::make_shared<IncludeTask>(EntityType::MINIONS_HAND_DECK_FIELD));
    cardDef.power.AddPowerTask(std::make_shared<FilterStackTask>(SelfCondList{
        std::make_shared<SelfCondition>(SelfCondition::IsCardID("TIME_619t")) }));
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("TIME_619e5", EntityType::STACK));
    cards.emplace("TIME_619t4", cardDef);
    // ------------------------------------ SPELL - DEATHKNIGHT
    // [TIME_619t5] Boon of Speed - COST:0
    // - Set: TIME_TRAVEL
    // --------------------------------------------------------
    // Text: Give Bwonsamdi <b>Rush</b> <i>permanently</i>.
    //       Minions summoned by his <b>Deathrattle</b> cost (2) more.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, [[maybe_unused]] Entity* source,
           [[maybe_unused]] Playable* target) {
            player->SetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1,
                               player->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1) + 1);
            player->SetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_2,
                               player->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_2) | 4);
        }));
    cardDef.power.AddPowerTask(
        std::make_shared<IncludeTask>(EntityType::MINIONS_HAND_DECK_FIELD));
    cardDef.power.AddPowerTask(std::make_shared<FilterStackTask>(SelfCondList{
        std::make_shared<SelfCondition>(SelfCondition::IsCardID("TIME_619t")) }));
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("TIME_619e6", EntityType::STACK));
    cards.emplace("TIME_619t5", cardDef);

    // ------------------------------ ENCHANTMENT - DEATHKNIGHT
    // [TIME_619e4] Pact for Power - COST:0
    // --------------------------------------------------------
    // Text: <b>Taunt</b>.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("TIME_619e4"));
    cards.emplace("TIME_619e4", cardDef);

    // ------------------------------ ENCHANTMENT - DEATHKNIGHT
    // [TIME_619e5] Pact for Longevity - COST:0
    // --------------------------------------------------------
    // Text: <b>Lifesteal</b>.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("TIME_619e5"));
    cards.emplace("TIME_619e5", cardDef);

    // ------------------------------ ENCHANTMENT - DEATHKNIGHT
    // [TIME_619e6] Pact for Speed - COST:0
    // --------------------------------------------------------
    // Text: <b>Rush</b>.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("TIME_619e6"));
    cards.emplace("TIME_619e6", cardDef);
}

void TimeTravelCardsGen::AddRogue(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------------ SPELL - ROGUE
    // [TIME_001] Chrono Daggers - COST:3
    // - Set: TIME_TRAVEL, Rarity: Rare
    // --------------------------------------------------------
    // Text: <b>Rewind</b> Throw 3 knives at random enemies
    //       that deal $2 damage each.
    // --------------------------------------------------------
    // GameTag:
    // - REWIND = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    for (int i = 0; i < 3; ++i)
    {
        cardDef.power.AddPowerTask(
            std::make_shared<RandomTask>(EntityType::ENEMIES, 1));
        cardDef.power.AddPowerTask(
            std::make_shared<DamageTask>(EntityType::STACK, 2, true));
    }
    cards.emplace("TIME_001", cardDef);
}

void TimeTravelCardsGen::AddRogueNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddShaman(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------------- SPELL - SHAMAN
    // [TIME_014] Instant Multiverse - COST:6
    // - Set: TIME_TRAVEL, Rarity: Epic
    // --------------------------------------------------------
    // Text: <b>Rewind</b> Summon 12 Mana worth of random minions.
    //       <b>Overload:</b> (3)
    // --------------------------------------------------------
    // GameTag:
    // - REWIND = 1
    // - OVERLOAD = 3
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            std::vector<Card*> pool;
            for (Card* card : Cards::GetAllCards())
            {
                if (card->IsCollectible() &&
                    card->GetCardType() == CardType::MINION &&
                    card->GetCost() >= 1)
                {
                    pool.emplace_back(card);
                }
            }
            int left = 12;
            while (left > 0 && !player->GetFieldZone()->IsFull())
            {
                std::vector<Card*> fit;
                for (Card* card : pool)
                {
                    if (card->GetCost() <= left)
                    {
                        fit.emplace_back(card);
                    }
                }
                if (fit.empty())
                {
                    break;
                }
                Card* pick = fit[Random::get<std::size_t>(0, fit.size() - 1)];
                left -= pick->GetCost();
                Generic::Summon(dynamic_cast<Minion*>(
                                    Entity::GetFromCard(player, pick)),
                                -1, source);
            }
        }));
    cards.emplace("TIME_014", cardDef);
}

void TimeTravelCardsGen::AddShamanNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddWarlock(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddWarlockNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddWarrior(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // --------------------------------------- MINION - WARRIOR
    // [TIME_034] Stadium Announcer - COST:4 [ATK:3/HP:3]
    // - Race: Dragon, Set: TIME_TRAVEL, Rarity: Rare
    // --------------------------------------------------------
    // Text: <b>Rewind</b> <b>Battlecry:</b> Both players equip
    //       a random weapon. Give yours +1/+1.
    // --------------------------------------------------------
    // GameTag:
    // - BATTLECRY = 1
    // - REWIND = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, [[maybe_unused]] Entity* source,
           [[maybe_unused]] Playable* target) {
            std::vector<Card*> pool;
            for (Card* card : Cards::GetAllCards())
            {
                if (card->IsCollectible() &&
                    card->GetCardType() == CardType::WEAPON)
                {
                    pool.emplace_back(card);
                }
            }
            for (Player* p : { player, player->opponent })
            {
                Card* pick = pool[Random::get<std::size_t>(0, pool.size() - 1)];
                const auto weapon =
                    dynamic_cast<Weapon*>(Entity::GetFromCard(p, pick));
                p->GetHero()->AddWeapon(*weapon);
                if (p == player)
                {
                    weapon->SetAttack(weapon->GetAttack() + 1);
                    weapon->SetDurability(weapon->GetDurability() + 1);
                }
            }
        }));
    cards.emplace("TIME_034", cardDef);
    // ---------------------------------------- SPELL - WARRIOR
    // [TIME_715] For Glory! - COST:5
    // - Set: TIME_TRAVEL, Rarity: Common
    // --------------------------------------------------------
    // Text: Draw 2 cards.
    //       Costs (1) less for each minion your opponent controls.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddAura(
        std::make_shared<AdaptiveCostEffect>([](const Playable* playable) {
            return playable->player->opponent->GetFieldZone()->GetCount();
        }));
    cardDef.power.AddPowerTask(std::make_shared<DrawTask>(2));
    cards.emplace("TIME_715", cardDef);
}

void TimeTravelCardsGen::AddWarriorNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddDemonHunter(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddDemonHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddNeutral(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------- SPELL - DEATHKNIGHT
    // [TIME_610] Shadows of Yesterday - COST:6
    // - Set: TIME_TRAVEL, Rarity: Rare
    // - Spell School: Shadow
    // --------------------------------------------------------
    // Text: <b>Rewind</b> Summon four 3/2 Shades.
    //       They each gain two random <b>Bonus Effects</b>.
    // --------------------------------------------------------
    // GameTag:
    // - REWIND = 1
    // --------------------------------------------------------
    // PARTIAL: "Bonus Effects" is a keyword with no engine support; the
    // Shades are summoned plain. Registered here because the generator has
    // no Death Knight section.
    cardDef.ClearData();
    cardDef.power.AddPowerTask(
        std::make_shared<SummonTask>("TIME_610t2", 4, SummonSide::SPELL));
    cards.emplace("TIME_610", cardDef);
    // --------------------------------------- MINION - NEUTRAL
    // [END_036] Morchie - COST:4 [ATK:3/HP:6]
    // - Set: TIME_TRAVEL, Rarity: Legendary
    // --------------------------------------------------------
    // Text: Your <b>Rewinds</b> keep BOTH potential outcomes.
    //       <b>Battlecry:</b> <b>Discover</b> a <b>Rewind</b> card
    //       from any class.
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - BATTLECRY = 1
    // - DISCOVER = 1
    // - AURA = 1
    // --------------------------------------------------------
    // RefTag:
    // - REWIND = 1
    // --------------------------------------------------------
    // NOTE: Rewind has no rules text in CardDefs.xml and no engine support;
    //       only the Discover is scripted. REWIND is XML tag 3934 (hsTags).
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, Playable* target) {
            std::vector<Card*> pool;
            for (Card* card : Cards::GetAllCards())
            {
                if (card->IsCollectible() && card->hsTags.contains(3934))
                {
                    pool.emplace_back(card);
                }
            }
            auto task = std::make_shared<DiscoverTask>(pool);
            task->SetPlayer(player);
            task->SetSource(source);
            task->SetTarget(target);
            task->Run();
        }));
    cards.emplace("END_036", cardDef);
}

void TimeTravelCardsGen::AddNeutralNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void TimeTravelCardsGen::AddAll(std::map<std::string, CardDef>& cards)
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
