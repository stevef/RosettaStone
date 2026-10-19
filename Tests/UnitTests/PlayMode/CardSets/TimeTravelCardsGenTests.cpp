// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// Cards from the TIME_TRAVEL set exist only in CardDefs.xml (build 254477),
// so every test here needs ROSETTA_CARDDEFS_XML; see REQUIRE_CARDDEFS_XML.

#include <Utils/CardSetHeaders.hpp>
#include <Rosetta/PlayMode/Zones/GraveyardZone.hpp>

// ---------------------------------------- SPELL - WARRIOR
// [TIME_715] For Glory! - COST:5
// - Set: TIME_TRAVEL, Rarity: Common
// --------------------------------------------------------
// Text: Draw 2 cards.
//       Costs (1) less for each minion your opponent controls.
// --------------------------------------------------------
TEST_CASE("[Warrior : Spell] - TIME_715 : For Glory!")
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

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("For Glory!"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wolfrider"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wolfrider"));
    CHECK_EQ(card1->GetCost(), 5);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, PlayCardTask::Minion(card3));
    CHECK_EQ(card1->GetCost(), 3);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    const int handCount = curHand.GetCount();
    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curHand.GetCount(), handCount + 1);
    CHECK_EQ(curPlayer->GetRemainingMana(), 7);
}

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
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - END_036 : Morchie")
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

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Morchie"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curPlayer->choice->choices.size(), 3u);
    for (const int id : curPlayer->choice->choices)
    {
        CHECK_EQ(game.entityList[id]->card->hsTags.contains(3934), true);
    }

    const int handCount = curHand.GetCount();
    TestUtils::ChooseNthChoice(game, 1);
    CHECK_EQ(curHand.GetCount(), handCount + 1);
    CHECK_EQ(curHand[handCount]->card->hsTags.contains(3934), true);
}

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
TEST_CASE("[Death Knight : Minion] - TIME_619 : Talanji of the Graves")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::PRIEST;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    // Talanji starts in the deck so the Fabled bundle is added at game start.
    config.player1Deck[0] = Cards::FindCardByName("Talanji of the Graves");
    for (std::size_t i = 1; i < 30; ++i)
    {
        config.player1Deck[i] = Cards::FindCardByName("Wisp");
    }

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
    auto& curDeck = *(curPlayer->GetDeckZone());

    int bundle = 0;
    for (auto& playable : curDeck.GetAll())
    {
        bundle += playable->card->id == "TIME_619t" ||
                  playable->card->id == "TIME_619t2";
    }
    CHECK_EQ(bundle, 2);

    // Find Talanji wherever the mulligan put her.
    Playable* talanji = nullptr;
    for (auto& playable : curHand.GetAll())
    {
        if (playable->card->id == "TIME_619")
        {
            talanji = playable;
        }
    }
    if (!talanji)
    {
        for (auto& playable : curDeck.GetAll())
        {
            if (playable->card->id == "TIME_619")
            {
                talanji = Generic::DrawCard(curPlayer, playable->card);
                curDeck.Remove(playable);
            }
        }
    }
    REQUIRE(talanji);

    const int handCount = curHand.GetCount();
    game.Process(curPlayer, PlayCardTask::Minion(talanji));
    CHECK_EQ(curHand.GetCount(), handCount);  // -Talanji +Bwonsamdi
    CHECK_EQ(curHand[handCount - 1]->card->id, "TIME_619t");
    CHECK_EQ(curPlayer->choice->choices.size(), 3u);
    TestUtils::ChooseNthChoice(game, 1);  // Boon of Power: Taunt
    CHECK_EQ(curPlayer->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1), 1);

    const auto bwonsamdi = curHand[handCount - 1];
    game.Process(curPlayer, PlayCardTask::Minion(bwonsamdi));
    CHECK_EQ(curField[1]->card->id, "TIME_619t");
    CHECK_EQ(curField[1]->HasTaunt(), true);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    const auto card1 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));
    game.Process(opPlayer, PlayCardTask::SpellTarget(card1, curField[1]));
    // Deathrattle: a random (4 + 2 x 1 boon) = 6-Cost minion
    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField[1]->card->GetCost(), 6);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    // A second Talanji resurrects Bwonsamdi with his Boon.
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Talanji of the Graves"));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    TestUtils::ChooseNthChoice(game, 3);  // Boon of Speed: Rush
    CHECK_EQ(curField.GetCount(), 4);
    CHECK_EQ(curField[3]->card->id, "TIME_619t");
    CHECK_EQ(curField[3]->HasTaunt(), true);
    CHECK_EQ(curField[3]->HasRush(), true);
    CHECK_EQ(curPlayer->GetGameTag(GameTag::TAG_SCRIPT_DATA_ENT_1), 2);
}

// ------------------------------------------- SPELL - MAGE
// [TIME_000] Semi-Stable Portal - COST:2
// - Set: TIME_TRAVEL, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Rewind</b> Add a random minion to your hand.
//       It costs (3) less.
// --------------------------------------------------------
// Keyword test: after a Rewind card resolves the player is offered Keep
// Timeline / Rewind Timeline once; Rewind marks USED_REWIND and hands the
// restore to the caller (Game::pendingRewind).
TEST_CASE("[Mage : Spell] - TIME_000 : Semi-Stable Portal (Rewind)")
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

    auto& curHand = *(curPlayer->GetHandZone());
    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("TIME_000"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("TIME_000"));
    const int handBefore = curHand.GetCount();

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    // the effect: a random minion, 3 cheaper
    CHECK_EQ(curHand.GetCount(), handBefore);  // -1 played, +1 added
    const auto added = curHand[handBefore - 1];
    CHECK_EQ(added->card->GetCardType(), CardType::MINION);
    CHECK_EQ(added->GetCost(), std::max(0, added->card->GetCost() - 3));
    // the Rewind choice
    REQUIRE(curPlayer->choice);
    auto cards = TestUtils::GetChoiceCards(game);
    REQUIRE_EQ(cards.size(), 2u);
    CHECK_EQ(cards[0]->id, "TIME_000ta");
    CHECK_EQ(cards[1]->id, "TIME_000tb");
    // keep
    game.Process(curPlayer,
                 ChooseTask::Pick(curPlayer, curPlayer->choice->choices[0]));
    CHECK_FALSE(curPlayer->choice);
    CHECK_EQ(card1->GetGameTag(GameTag::USED_REWIND), 0);
    CHECK_EQ(game.pendingRewind, nullptr);

    game.Process(curPlayer, PlayCardTask::Spell(card2));
    REQUIRE(curPlayer->choice);
    const int graveBefore = curPlayer->GetGraveyardZone()->GetCount();
    // rewind
    game.Process(curPlayer,
                 ChooseTask::Pick(curPlayer, curPlayer->choice->choices[1]));
    CHECK_FALSE(curPlayer->choice);
    CHECK_EQ(card2->GetGameTag(GameTag::USED_REWIND), 1);
    CHECK_EQ(game.pendingRewind, card2);
    // both options and the cast "Rewind Timeline" copy went to the graveyard
    CHECK_EQ(curPlayer->GetGraveyardZone()->GetCount(), graveBefore + 3);
    // a used Rewind is not offered again
    game.pendingRewind = nullptr;
    Generic::OfferRewind(curPlayer, card2);
    CHECK_FALSE(curPlayer->choice);
}

// --------------------------------------- MINION - WARRIOR
// [TIME_034] Stadium Announcer - COST:4 [ATK:3/HP:3]
// - Race: Dragon, Set: TIME_TRAVEL, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Rewind</b> <b>Battlecry:</b> Both players equip
//       a random weapon. Give yours +1/+1.
// --------------------------------------------------------
TEST_CASE("[Warrior : Minion] - TIME_034 : Stadium Announcer")
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

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByID("TIME_034"));
    game.Process(curPlayer, PlayCardTask::Minion(card1));
    REQUIRE(curPlayer->GetHero()->HasWeapon());
    REQUIRE(opPlayer->GetHero()->HasWeapon());
    const auto mine = curPlayer->GetHero()->weapon;
    CHECK_EQ(mine->GetAttack(), mine->card->gameTags[GameTag::ATK] + 1);
    CHECK_EQ(mine->GetDurability(),
             mine->card->gameTags[GameTag::DURABILITY] + 1);
    const auto theirs = opPlayer->GetHero()->weapon;
    CHECK_EQ(theirs->GetAttack(), theirs->card->gameTags[GameTag::ATK]);
    // a Rewind minion offers the choice after its battlecry
    REQUIRE(curPlayer->choice);
    CHECK_EQ(TestUtils::GetChoiceCards(game)[1]->id, "TIME_000tb");
}
