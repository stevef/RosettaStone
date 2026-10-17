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
    CardDef cardDef;

    // ------------------------------------------ SPELL - ROGUE
    // [CATA_785] Rite of Twilight - COST:2
    // - Set: CATACLYSM, Rarity: Rare
    // - Spell School: Shadow
    // --------------------------------------------------------
    // Text: <b>Herald</b> {0}. <b>Combo:</b> Deal $3 damage.
    // --------------------------------------------------------
    // GameTag:
    // - HERALD = 1
    // - COMBO = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<HeraldTask>());
    cardDef.power.AddComboTask(std::make_shared<HeraldTask>());
    cardDef.power.AddComboTask(
        std::make_shared<DamageTask>(EntityType::TARGET, 3, true));
    cardDef.property.playReqs = PlayReqs{ { PlayReq::REQ_TARGET_FOR_COMBO, 0 } };
    cards.emplace("CATA_785", cardDef);
}

void CataclysmCardsGen::AddRogueNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // ----------------------------------------- MINION - ROGUE
    // [CATA_158t] Soldier of Sinestra - COST:1 [ATK:1/HP:1]
    // - Race: Dragon, Set: CATACLYSM
    // --------------------------------------------------------
    // Text: When summoned, get a random spell from another class.
    //       It costs ({0}) less. <i>Herald twice to upgrade.</i>
    // --------------------------------------------------------
    // GameTag:
    // - TAG_SCRIPT_DATA_NUM_1 = 1
    // --------------------------------------------------------
    // "When summoned" = POWER tasks, run by HeraldTask after the summon.
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            const CardClass own = player->GetHero()->card->GetCardClass();
            std::vector<Card*> pool;
            for (Card* card : Cards::GetAllCards())
            {
                if (card->IsCollectible() &&
                    card->GetCardType() == CardType::SPELL &&
                    card->GetCardClass() != own &&
                    card->GetCardClass() != CardClass::NEUTRAL)
                {
                    pool.emplace_back(card);
                }
            }
            if (pool.empty())
            {
                return;
            }
            Card* pick = pool[Random::get<std::size_t>(0, pool.size() - 1)];
            Playable* spell = Entity::GetFromCard(player, pick);
            if (!Generic::AddCardToHand(player, spell))
            {
                return;
            }
            const int less = source->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1) *
                             HeraldTask::Multiplier(player);
            spell->SetCost(std::max(0, spell->GetCost() - less));
        }));
    cards.emplace("CATA_158t", cardDef);
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
    CardDef cardDef;

    // --------------------------------------- MINION - WARLOCK
    // [CATA_725] Shadowsworn Disciple - COST:2 [ATK:2/HP:1]
    // - Set: CATACLYSM, Rarity: Common
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> <b>Herald</b> {0}.
    //       <b>Deathrattle:</b> Restore #3 Health to your hero.
    // --------------------------------------------------------
    // GameTag:
    // - BATTLECRY = 1
    // - DEATHRATTLE = 1
    // - HERALD = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<HeraldTask>());
    cardDef.power.AddDeathrattleTask(
        std::make_shared<HealTask>(EntityType::HERO, 3));
    cards.emplace("CATA_725", cardDef);

    // ------------------------------------- LOCATION - WARLOCK
    // [CATA_492] Shrine of Twilight - COST:4 [HP:2]
    // - Set: CATACLYSM, Rarity: Rare
    // --------------------------------------------------------
    // Text: <b>Herald</b> {0}. Draw a card.
    // --------------------------------------------------------
    // GameTag:
    // - HERALD = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<HeraldTask>());
    cardDef.power.AddPowerTask(std::make_shared<DrawTask>(1));
    cards.emplace("CATA_492", cardDef);
}

void CataclysmCardsGen::AddWarlockNonCollect(
    std::map<std::string, CardDef>& cards)
{
    CardDef cardDef;

    // --------------------------------------- MINION - WARLOCK
    // [CATA_725t] Soldier of Cho'gall - COST:1 [ATK:1/HP:1]
    // - Set: CATACLYSM
    // --------------------------------------------------------
    // Text: At the end of your turn, destroy the minion to the
    //       right to gain +{0}/+{0}. <i>Herald twice to upgrade.</i>
    // --------------------------------------------------------
    // GameTag:
    // - TAG_SCRIPT_DATA_NUM_1 = 2
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddTrigger(std::make_shared<Trigger>(TriggerType::TURN_END));
    cardDef.power.GetTrigger()->tasks = { std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            const auto self = dynamic_cast<Minion*>(source);
            if (!self || self->GetZoneType() != ZoneType::PLAY)
            {
                return;
            }
            const auto field = player->GetFieldZone();
            const int pos = self->GetZonePosition();
            if (pos + 1 >= field->GetCount())
            {
                return;
            }
            Minion* right = (*field)[pos + 1];
            const int gain = source->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1) *
                             HeraldTask::Multiplier(player);
            right->Destroy();
            player->game->ProcessDestroyAndUpdateAura();
            self->SetAttack(self->GetAttack() + gain);
            self->SetBaseHealth(self->GetBaseHealth() + gain);
        }) };
    cards.emplace("CATA_725t", cardDef);
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

    // --------------------------------------- WEAPON - WARRIOR
    // [CATA_580] Cataclysmic War Axe - COST:3 [ATK:3/HP:2]
    // - Set: CATACLYSM, Rarity: Common
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> <b>Herald</b> {0}.
    // --------------------------------------------------------
    // GameTag:
    // - BATTLECRY = 1
    // - HERALD = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<HeraldTask>());
    cards.emplace("CATA_580", cardDef);

    // --------------------------------------- MINION - WARRIOR
    // [CATA_160] Scorching Ravager - COST:4 [ATK:4/HP:3]
    // - Race: Dragon, Set: CATACLYSM, Rarity: Epic
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> <b>Herald</b> {0}.
    //       Give the Soldier <b>Rush</b>.
    // --------------------------------------------------------
    // GameTag:
    // - BATTLECRY = 1
    // - HERALD = 1
    // --------------------------------------------------------
    // RefTag:
    // - RUSH = 1
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<HeraldTask>());
    // CardDefs.xml has no enchantment entity for this; set the tag directly.
    cardDef.power.AddPowerTask(
        std::make_shared<SetGameTagTask>(EntityType::STACK, GameTag::RUSH, 1));
    cards.emplace("CATA_160", cardDef);
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

    // --------------------------------------- MINION - WARRIOR
    // [CATA_580t] Soldier of Ragnaros - COST:1 [ATK:2/HP:1]
    // - Race: Elemental, Set: CATACLYSM
    // --------------------------------------------------------
    // Text: <b>Deathrattle:</b> Deal {0} damage to a random enemy.
    //       <i>Herald twice to upgrade.</i>
    // --------------------------------------------------------
    // GameTag:
    // - DEATHRATTLE = 1
    // - TAG_SCRIPT_DATA_NUM_1 = 2
    // --------------------------------------------------------
    cardDef.ClearData();
    cardDef.power.AddDeathrattleTask(std::make_shared<CustomTask>(
        [](Player* player, Entity* source, [[maybe_unused]] Playable* target) {
            const int dmg = source->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1) *
                            HeraldTask::Multiplier(player);
            std::vector<Playable*> enemies;
            enemies.emplace_back(player->opponent->GetHero());
            for (auto& m : player->opponent->GetFieldZone()->GetAll())
            {
                enemies.emplace_back(m);
            }
            auto* pick = enemies[Random::get<std::size_t>(0, enemies.size() - 1)];
            Generic::TakeDamageToCharacter(dynamic_cast<Playable*>(source),
                                           dynamic_cast<Character*>(pick), dmg,
                                           false);
        }));
    cards.emplace("CATA_580t", cardDef);
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
    // NOTE: {1} is unknown and taken as (1). Herald: HeraldTask (keyword-rules).
    cardDef.ClearData();
    cardDef.power.AddPowerTask(std::make_shared<HeraldTask>());
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
