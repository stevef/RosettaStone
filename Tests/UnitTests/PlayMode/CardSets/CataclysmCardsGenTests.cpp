// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// Cards from the CATACLYSM set exist only in CardDefs.xml (build 254477),
// so every test here needs ROSETTA_CARDDEFS_XML; see REQUIRE_CARDDEFS_XML.

#include <Utils/CardSetHeaders.hpp>
#include <Rosetta/PlayMode/Tasks/SimpleTasks/HeraldTask.hpp>
#include <Rosetta/PlayMode/Zones/GraveyardZone.hpp>

// ---------------------------------------- SPELL - WARRIOR
// [CATA_582] Searing Fissure - COST:2
// - Set: CATACLYSM, Rarity: Common
// - Spell School: Fire
// --------------------------------------------------------
// Text: Deal $1 damage to all minions.
//       Give your hero +3 Attack this turn.
// --------------------------------------------------------
TEST_CASE("[Warrior : Spell] - CATA_582 : Searing Fissure")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Searing Fissure"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Wolfrider"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Chillwind Yeti"));

    game.Process(curPlayer, PlayCardTask::Minion(card2));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card3));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curField.GetCount(), 0);
    CHECK_EQ(opField[0]->GetHealth(), 4);
    CHECK_EQ(curPlayer->GetHero()->GetAttack(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curPlayer->GetHero()->GetAttack(), 0);
}

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
TEST_CASE("[Demon Hunter : Spell] - CATA_528 : Sigil of the Seas")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::DEMONHUNTER;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Sigil of the Seas"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curField.GetCount(), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 0);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->card->name, "Naga Monstrosity");
    CHECK_EQ(curField[0]->GetAttack(), 3);
    CHECK_EQ(curField[0]->GetHealth(), 3);
    CHECK_EQ(curField[0]->HasTaunt(), true);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 1);
}

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
TEST_CASE("[Neutral : Minion] - CATA_185 : Faceless Replicator")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Faceless Replicator"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Chillwind Yeti"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField[0]->GetGameTag(GameTag::CANT_BE_TARGETED_BY_SPELLS), 1);
    CHECK_EQ(curField[0]->GetGameTag(GameTag::CANT_BE_TARGETED_BY_HERO_POWERS), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, PlayCardTask::SpellTarget(card3, curField[0]));
    CHECK_EQ(curField.GetCount(), 1);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, AttackTask(opField[0], curField[0]));
    CHECK_EQ(curField.GetCount(), 0);
    CHECK_EQ(opField.GetCount(), 1);
    CHECK_EQ(opField[0]->card->name, "Faceless Replicator");
    CHECK_EQ(opField[0]->GetHealth(), 3);
}

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
TEST_CASE("[Mage : Minion] - CATA_458 : Archmage Kalec")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Archmage Kalec"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Fireball"));
    const auto card3 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Fireball"));

    game.Process(curPlayer, PlayCardTask::SpellTarget(card2, opPlayer->GetHero()));
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 24);

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(card3->GetGameTag(GameTag::SPELLPOWER), 1);

    curPlayer->SetUsedMana(0);
    game.Process(curPlayer, PlayCardTask::SpellTarget(card3, opPlayer->GetHero()));
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 17);

    int buffed = 0, spells = 0;
    for (auto& playable : curPlayer->GetDeckZone()->GetAll())
    {
        if (playable->card->GetCardType() == CardType::SPELL)
        {
            ++spells;
            buffed += playable->GetGameTag(GameTag::SPELLPOWER) == 1;
        }
    }
    CHECK_EQ(buffed, spells);
}

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
// --------------------------------------------------------
TEST_CASE("[Druid : Minion] - CATA_140 : Merithra of the Dream")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::DRUID;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Merithra of the Dream"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Pyroblast"));
    const auto card3 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Pyroblast"));
    const auto card4 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Fireball"));
    const auto card5 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Arcane Missiles"));

    game.Process(curPlayer, PlayCardTask::SpellTarget(card2, opPlayer->GetHero()));
    CHECK_EQ(card1->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_2), 10);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card3, opPlayer->GetHero()));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card4, opPlayer->GetHero()));
    game.Process(curPlayer, PlayCardTask::Spell(card5));
    CHECK_EQ(card1->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_2), 25);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    const int handBefore = curHand.GetCount();
    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curHand.GetCount(), 10);
    int dragons = 0, cheap = 0;
    for (auto& playable : curHand.GetAll())
    {
        if (playable->card->GetRace() == Race::DRAGON)
        {
            ++dragons;
            cheap += playable->GetCost() == 1;
        }
    }
    CHECK_GE(dragons, 10 - (handBefore - 1));
    CHECK_EQ(cheap, dragons);
}

// --------------------------------------- MINION - NEUTRAL
// [CATA_497] Ultraxion - COST:6 [ATK:6/HP:7]
// - Race: Dragon, Set: CATACLYSM, Rarity: Legendary
// --------------------------------------------------------
// Text: <b>Battlecry:</b> <b>Herald</b> {0}.
//       Reduce Deathwing's Cost by ({1}).
// --------------------------------------------------------
// GameTag:
// - ELITE = 1
// - BATTLECRY = 1
// - HERALD = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - CATA_497 : Ultraxion")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Ultraxion"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByID("CATA_190h"));
    CHECK_EQ(card2->GetCost(), 10);

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(card2->GetCost(), 9);
}

// --------------------------------------- MINION - WARLOCK
// [CATA_725] Shadowsworn Disciple - COST:2 [ATK:2/HP:1]
// - Set: CATACLYSM, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> <b>Herald</b> {0}.
//       <b>Deathrattle:</b> Restore #3 Health to your hero.
// --------------------------------------------------------
// Keyword test: Herald counts per player, summons the class Soldier to the
// right of the source, and the Soldier's power doubles after two Heralds.
TEST_CASE("[Warlock : Minion] - CATA_725 : Shadowsworn Disciple (Herald)")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("CATA_725"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("CATA_725"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curPlayer->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT), 1);
    CHECK_EQ(SimpleTasks::HeraldTask::Multiplier(curPlayer), 1);
    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField[0]->card->id, "CATA_725");
    CHECK_EQ(curField[1]->card->id, "CATA_725t");
    CHECK_EQ(curField[1]->GetAttack(), 1);
    CHECK_EQ(curField[1]->GetHealth(), 1);

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(curPlayer->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT), 2);
    CHECK_EQ(SimpleTasks::HeraldTask::Multiplier(curPlayer), 2);
    CHECK_EQ(opPlayer->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT), 0);
    CHECK_EQ(curField.GetCount(), 4);
    // board: Disciple, Soldier, Disciple, Soldier
    CHECK_EQ(curField[2]->card->id, "CATA_725");
    CHECK_EQ(curField[3]->card->id, "CATA_725t");

    // Soldier of Cho'gall: at the end of your turn, destroy the minion to the
    // right to gain +{0}/+{0}; {0} = 2, doubled after two Heralds.

    curPlayer->GetHero()->SetDamage(10);
    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    // the first Soldier ate the second Disciple (its deathrattle healed 3)
    CHECK_EQ(curField.GetCount(), 3);
    CHECK_EQ(curField[1]->card->id, "CATA_725t");
    CHECK_EQ(curField[1]->GetAttack(), 1 + 4);
    CHECK_EQ(curField[1]->GetHealth(), 1 + 4);
    CHECK_EQ(curPlayer->GetHero()->GetHealth(), 23);
}

// --------------------------------------- MINION - WARRIOR
// [CATA_160] Scorching Ravager - COST:4 [ATK:4/HP:3]
// - Race: Dragon, Set: CATACLYSM, Rarity: Epic
// --------------------------------------------------------
// Text: <b>Battlecry:</b> <b>Herald</b> {0}.
//       Give the Soldier <b>Rush</b>.
// --------------------------------------------------------
TEST_CASE("[Warrior : Minion] - CATA_160 : Scorching Ravager")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("CATA_160"));
    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField[1]->card->id, "CATA_580t");
    CHECK_EQ(curField[1]->HasRush(), true);
    CHECK_EQ(curField[1]->GetAttack(), 2);
    CHECK_EQ(curField[0]->HasRush(), false);

    // Soldier of Ragnaros: Deathrattle: deal {0} (= 2) damage to a random
    // enemy; the only enemy is the hero.
    curField[1]->Destroy();
    game.ProcessDestroyAndUpdateAura();
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 28);
}

// ------------------------------------------ SPELL - ROGUE
// [CATA_785] Rite of Twilight - COST:2
// - Set: CATACLYSM, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Herald</b> {0}. <b>Combo:</b> Deal $3 damage.
// --------------------------------------------------------
TEST_CASE("[Rogue : Spell] - CATA_785 : Rite of Twilight")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("CATA_785"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("CATA_785"));

    // no combo: Herald only
    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->card->id, "CATA_158t");
    CHECK_EQ(curPlayer->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT), 1);
    // Soldier of Sinestra: when summoned, get a random spell from another
    // class
    const int handNow = curHand.GetCount();
    CHECK_EQ(curHand[handNow - 1]->card->GetCardType(), CardType::SPELL);
    CHECK_NE(curHand[handNow - 1]->card->GetCardClass(), CardClass::ROGUE);
    CHECK_NE(curHand[handNow - 1]->card->GetCardClass(), CardClass::NEUTRAL);

    // combo: Herald, then 3 damage to the target
    game.Process(curPlayer,
                 PlayCardTask::SpellTarget(card2, opPlayer->GetHero()));
    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curPlayer->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT), 2);
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 27);
}

// ------------------------------------- LOCATION - WARLOCK
// [CATA_492] Shrine of Twilight - COST:4 [HP:2]
// - Set: CATACLYSM, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Herald</b> {0}. Draw a card.
// --------------------------------------------------------
TEST_CASE("[Warlock : Location] - CATA_492 : Shrine of Twilight")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("CATA_492"));
    game.Process(curPlayer, PlayCardTask::Location(card1));
    const auto location = dynamic_cast<Location*>(card1);
    CHECK_EQ(curField.GetCount(), 1);
    // the engine puts a new location on cooldown; use it next turn
    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    const int handBefore = curHand.GetCount();
    game.Process(curPlayer, PlayLocationTask(location));
    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField.GetMinions()[0]->card->id, "CATA_725t");
    CHECK_EQ(curHand.GetCount(), handBefore + 1);
    CHECK_EQ(curPlayer->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT), 1);
}

// Herald in a class with no Soldier: counts, summons nothing.
TEST_CASE("[Neutral : Minion] - CATA_497 : Ultraxion (Herald, Mage)")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("CATA_497"));
    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curPlayer->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT), 1);
    CHECK_EQ(SimpleTasks::HeraldTask::SoldierCardID(curPlayer), nullptr);
}
