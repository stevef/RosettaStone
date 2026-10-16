// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// Cards from the CATACLYSM set exist only in CardDefs.xml (build 254477),
// so every test here needs ROSETTA_CARDDEFS_XML; see REQUIRE_CARDDEFS_XML.

#include <Utils/CardSetHeaders.hpp>

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
