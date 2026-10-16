// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// Hearthstone++ is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/PlayMode/CardSets/CataclysmCardsGen.hpp>
#include <Rosetta/PlayMode/Cards/CardPowers.hpp>

namespace RosettaStone::PlayMode
{
void CataclysmCardsGen::AddHeroes(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddHeroPowers(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddDruid(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------------- MINION - DRUID
    // [CATA_140] Merithra of the Dream - COST:8 [ATK:4/HP:12]
    // - Race: Dragon, Set: CATACLYSM, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> Fill your hand with random Dragons.
    //       If you spent 25 Mana while holding this, they cost (1).
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - BATTLECRY = 1
    // - TAG_SCRIPT_DATA_NUM_1 = 25
    // --------------------------------------------------------
    // NOTE: Mana spent is counted as the Cost of each friendly card played
    //       while this is in hand (TAG_SCRIPT_DATA_NUM_2); Hero Power mana
    //       is not counted.
    cardDef.ClearData();
    cardDef.power.AddTrigger(std::make_shared<Trigger>(TriggerType::PLAY_CARD));
    cardDef.power.GetTrigger()->triggerActivation = TriggerActivation::HAND;
    cardDef.power.GetTrigger()->triggerSource = TriggerSource::FRIENDLY;
    cardDef.power.GetTrigger()->tasks = { std::make_shared<CustomTask>(
        [](const Player* player, Entity* source,
           [[maybe_unused]] Playable* target) {
            const auto& event = player->game->currentEventData;
            if (!event || !event->eventSource)
            {
                return;
            }
            source->SetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_2,
                               source->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_2) +
                                   event->eventSource->GetCost());
        }) };
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            std::vector<Card*> pool;
            for (Card* card : Cards::GetAllCards())
            {
                if (card->IsCollectible() &&
                    card->GetCardType() == CardType::MINION &&
                    card->GetRace() == Race::DRAGON)
                {
                    pool.emplace_back(card);
                }
            }
            const bool ready =
                source->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_2) >=
                source->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1);
            while (!pool.empty() && !player->GetHandZone()->IsFull())
            {
                const auto idx = Random::get<std::size_t>(0, pool.size() - 1);
                Playable* dragon = Entity::GetFromCard(player, pool[idx]);
                if (ready)
                {
                    dragon->SetCost(1);
                }
                Generic::AddCardToHand(player, dragon);
            }
        }));
    cards.emplace("CATA_140", cardDef);
}

void CataclysmCardsGen::AddDruidNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddHunter(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddMage(std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------------ MINION - MAGE
    // [CATA_458] Archmage Kalec - COST:4 [ATK:4/HP:4]
    // - Set: CATACLYSM, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> Give all spells in your hand
    //       and deck <b>Spell Damage +1.</b>
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - BATTLECRY = 1
    // --------------------------------------------------------
    // RefTag:
    // - SPELLPOWER = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<IncludeTask>(EntityType::HAND));
    cardDef.power.AddPowerTask(std::make_shared<FilterStackTask>(SelfCondList{
        std::make_shared<SelfCondition>(SelfCondition::IsSpell()) }));
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("CATA_458e", EntityType::STACK));
    cardDef.power.AddPowerTask(std::make_shared<IncludeTask>(EntityType::DECK));
    cardDef.power.AddPowerTask(std::make_shared<FilterStackTask>(SelfCondList{
        std::make_shared<SelfCondition>(SelfCondition::IsSpell()) }));
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("CATA_458e", EntityType::STACK));
    cards.emplace("CATA_458", cardDef);
}

void CataclysmCardsGen::AddMageNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------- ENCHANTMENT - MAGE
    // [CATA_458e] Kalec's Insight - COST:0
    // - Set: CATACLYSM
    // --------------------------------------------------------
    // Text: <b>Spell Damage +1</b>.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(std::make_shared<Enchant>(Effects::SpellPowerN(1)));
    cards.emplace("CATA_458e", cardDef);
}

void CataclysmCardsGen::AddPaladin(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddPaladinNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddPriest(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddPriestNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddRogue(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddRogueNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddShaman(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddShamanNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddWarlock(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddWarlockNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void CataclysmCardsGen::AddWarrior(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ---------------------------------------- SPELL - WARRIOR
    // [CATA_582] Searing Fissure - COST:2
    // - Set: CATACLYSM, Rarity: Common
    // - Spell School: Fire
    // --------------------------------------------------------
    // Text: Deal $1 damage to all minions.
    //       Give your hero +3 Attack this turn.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(
        std::make_shared<DamageTask>(EntityType::ALL_MINIONS, 1, true));
    cardDef.power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("CATA_582e", EntityType::HERO));
    cards.emplace("CATA_582", cardDef);
}

void CataclysmCardsGen::AddWarriorNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ---------------------------------- ENCHANTMENT - WARRIOR
    // [CATA_582e] Flamebound - COST:0
    // - Set: CATACLYSM
    // --------------------------------------------------------
    // Text: +3 Attack this turn,
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(Enchants::GetEnchantFromText("CATA_582e"));
    cards.emplace("CATA_582e", cardDef);
}

void CataclysmCardsGen::AddDemonHunter(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ------------------------------------ SPELL - DEMONHUNTER
    // [CATA_528] Sigil of the Seas - COST:1
    // - Set: CATACLYSM, Rarity: Common
    // - Spell School: Fel
    // --------------------------------------------------------
    // Text: At the start of your next turn,
    //       summon a 3/3 Naga with <b>Taunt</b>.
    // --------------------------------------------------------
    // GameTag:
    // - SIGIL = 1
    // --------------------------------------------------------
    // RefTag:
    // - TAUNT = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddTrigger(
        std::make_shared<Trigger>(TriggerType::TURN_START));
    cardDef.power.GetTrigger()->tasks = { std::make_shared<SummonTask>(
        "CATA_528t", SummonSide::SPELL) };
    cardDef.power.GetTrigger()->removeAfterTriggered = true;
    cards.emplace("CATA_528", cardDef);
}

void CataclysmCardsGen::AddDemonHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------- MINION - DEMONHUNTER
    // [CATA_528t] Naga Monstrosity - COST:3 [ATK:3/HP:3]
    // - Race: Naga, Set: CATACLYSM
    // --------------------------------------------------------
    // Text: <b>Taunt</b>
    // --------------------------------------------------------
    // GameTag:
    // - TAUNT = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(nullptr);
    cards.emplace("CATA_528t", cardDef);
}

void CataclysmCardsGen::AddNeutral(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // --------------------------------------- MINION - NEUTRAL
    // [CATA_185] Faceless Replicator - COST:3 [ATK:3/HP:3]
    // - Set: CATACLYSM, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Elusive</b> <b>Deathrattle:</b> Transform the minion
    //       that killed this into a Faceless Replicator.
    // --------------------------------------------------------
    // GameTag:
    // - DEATHRATTLE = 1
    // - ELUSIVE = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddDeathrattleTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            const int killerID = source->GetGameTag(GameTag::LAST_AFFECTED_BY);
            const auto iter = player->game->entityList.find(killerID);
            if (iter == player->game->entityList.end())
            {
                return;
            }
            const auto killer = dynamic_cast<Minion*>(iter->second);
            if (!killer || killer->GetZoneType() != ZoneType::PLAY ||
                killer->isDestroyed)
            {
                return;
            }
            Generic::TransformMinion(killer->player, killer,
                                     Cards::FindCardByID("CATA_185"));
        }));
    cards.emplace("CATA_185", cardDef);

    // --------------------------------------- MINION - NEUTRAL
    // [CATA_497] Ultraxion - COST:6 [ATK:6/HP:7]
    // - Race: Dragon, Set: CATACLYSM, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> <b>Herald</b> {0}.
    //       Reduce Deathwing's Cost by ({1}).
    //       <i>(<b>Herald</b> to improve!)</i>
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - BATTLECRY = 1
    // - HERALD = 1
    // - TAG_SCRIPT_DATA_NUM_1 = 1
    // --------------------------------------------------------
    // NOTE: Herald has no rules text in CardDefs.xml and no engine support,
    //       so it is not scripted; {1} is unknown and taken as (1).
    cardDef.ClearData();
    // Deathwing, Worldbreaker (CATA_190h) is a Hero card, so look in hand
    // and deck by ID.
    for (const auto zone : { EntityType::HAND, EntityType::DECK })
    {
        cardDef.power.AddPowerTask(std::make_shared<IncludeTask>(zone));
        cardDef.power.AddPowerTask(
            std::make_shared<FilterStackTask>(SelfCondList{
                std::make_shared<SelfCondition>([](const Playable* playable) {
                    return playable->card->id == "CATA_190h";
                }) }));
        cardDef.power.AddPowerTask(std::make_shared<AddEnchantmentTask>(
            "CATA_497e", EntityType::STACK));
    }
    cards.emplace("CATA_497", cardDef);
}

void CataclysmCardsGen::AddNeutralNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ---------------------------------- ENCHANTMENT - NEUTRAL
    // [CATA_497e] Ultraxion Heralded - COST:0
    // - Set: CATACLYSM
    // --------------------------------------------------------
    // Text: Reduced Cost.
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddEnchant(std::make_shared<Enchant>(Effects::ReduceCost(1)));
    cards.emplace("CATA_497e", cardDef);
}

void CataclysmCardsGen::AddAll(std::map<std::string, CardDef>& cards)
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
