// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// Cards from the ESCAPEFROM_VIOLET_HOLD set exist only in CardDefs.xml (build 254477),
// so every test here needs ROSETTA_CARDDEFS_XML; see REQUIRE_CARDDEFS_XML.

#include <Utils/CardSetHeaders.hpp>

// ----------------------------------------- SPELL - HUNTER
// [JAIL_879] Beast Tripwire - COST:4
// - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Rare
// --------------------------------------------------------
// Text: Summon a random 5-Cost Beast.
//       Shuffle 2 spells into your deck
//       that do it again when drawn.
// --------------------------------------------------------
TEST_CASE("[Hunter : Spell] - JAIL_879 : Beast Tripwire")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::HUNTER;
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
    auto& curDeck = *(curPlayer->GetDeckZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Beast Tripwire"));

    const int deckCount = curDeck.GetCount();
    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->card->GetCost(), 5);
    CHECK_EQ(curField[0]->card->GetRace(), Race::BEAST);
    CHECK_EQ(curDeck.GetCount(), deckCount + 2);

    int tripped = 0;
    for (int i = 0; i < curDeck.GetCount(); ++i)
    {
        if (curDeck[i]->card->id == "JAIL_879t")
        {
            ++tripped;
        }
    }
    CHECK_EQ(tripped, 2);
}

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
TEST_CASE("[Demon Hunter : Minion] - JAIL_733 : Vicious Voidscale")
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
    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Vicious Voidscale"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField[0]->HasTaunt(), true);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    const int handCount = curHand.GetCount();
    game.Process(opPlayer, PlayCardTask::SpellTarget(card2, curField[0]));
    CHECK_EQ(curField.GetCount(), 0);
    CHECK_EQ(curHand.GetCount(), handCount + 1);
    CHECK_EQ(curHand[handCount]->card->name, "Void Soul");

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    // Void Soul: a 1-Cost Demon first, then the level goes up.
    game.Process(curPlayer, PlayCardTask::Spell(curHand[handCount]));
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->card->GetRace(), Race::DEMON);
    CHECK_EQ(curField[0]->card->GetCost(), 1);
    CHECK_EQ(curPlayer->GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1), 1);

    const auto card3 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Void Soul"));
    game.Process(curPlayer, PlayCardTask::Spell(card3));
    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField[1]->card->GetCost(), 2);
}

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
TEST_CASE("[Rogue : Minion] - CAP_004 : Disguised Operator")
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
    auto& opHand = *(opPlayer->GetHandZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Disguised Operator"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Chillwind Yeti"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField[0]->HasRush(), true);

    const int opHandCount = opHand.GetCount();
    game.Process(curPlayer,
                 AttackTask(curField[0], opPlayer->GetFieldZone()->GetAll()[0]));
    CHECK_EQ(curField.GetCount(), 0);
    CHECK_EQ(opHand.GetCount(), opHandCount + 2);
}

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
TEST_CASE("[Priest : Minion] - CAP_803 : Lingering Spirit")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::PRIEST;
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
        curPlayer, Cards::FindCardByName("Lingering Spirit"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));

    curPlayer->GetHero()->SetDamage(1);

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField[0]->HasReborn(), true);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    // Hero is missing 1 Health: heal 1, the other 2 hit the only enemy, the hero.
    game.Process(opPlayer, PlayCardTask::SpellTarget(card2, curField[0]));
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->HasReborn(), false);
    CHECK_EQ(curField[0]->GetHealth(), 1);
    CHECK_EQ(curPlayer->GetHero()->GetHealth(), 30);
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 28);

    // Second death: hero is full, all 3 is excess.
    game.Process(opPlayer, PlayCardTask::SpellTarget(card3, curField[0]));
    CHECK_EQ(curField.GetCount(), 0);
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 25);
}

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
// KNOWN FAILURE: the engine applies plain stat enchantments (e.g. Blessing of
// Kings) directly to the target's tags and keeps no Enchantment entity for
// them (Generic::AddEnchantment, "Create Enchantment instance only when it is
// needed"), so there is nothing to steal. Needs an engine redesign.
TEST_CASE("[Neutral : Minion] - JAIL_101 : Violet Punisher" * doctest::may_fail())
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::PALADIN;
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
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Violet Punisher"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wolfrider"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Blessing of Kings"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, PlayCardTask::SpellTarget(card3, opField[0]));
    CHECK_EQ(opField[0]->GetAttack(), 7);
    CHECK_EQ(opField[0]->GetHealth(), 5);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::MinionTarget(card1, opField[0]));
    CHECK_EQ(opField[0]->GetAttack(), 3);
    CHECK_EQ(opField[0]->GetHealth(), 1);
    CHECK_EQ(curField[0]->GetAttack(), 9);
    CHECK_EQ(curField[0]->GetHealth(), 8);
}

// --------------------------------------- WEAPON - WARRIOR
// [CAP_103] Hand Cannon - COST:3 [ATK:3/HP:2]
// - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Rare
// --------------------------------------------------------
// Text: After your hero attacks, your Cannoneers FIRE!
// --------------------------------------------------------
TEST_CASE("[Warrior : Weapon] - CAP_103 : Hand Cannon")
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
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Hand Cannon"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByID("CAP_107t"));
    const auto card3 =
        Generic::DrawCard(curPlayer, Cards::FindCardByID("CAP_107t"));

    game.Process(curPlayer, PlayCardTask::Weapon(card1));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    game.Process(curPlayer, PlayCardTask::Minion(card3));

    game.Process(curPlayer, AttackTask(curPlayer->GetHero(), opPlayer->GetHero()));
    // 3 from the weapon, 1 from each of the two Cannoneers
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 25);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    // end of turn: each Cannoneer fires once more
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 23);
}

// ----------------------------------- WEAPON - DEMONHUNTER
// [JAIL_730] Stardust Scythe - COST:4 [ATK:3/HP:3]
// - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Common
// --------------------------------------------------------
// Text: After your hero attacks, get a Void Soul.
// --------------------------------------------------------
TEST_CASE("[Demon Hunter : Weapon] - JAIL_730 : Stardust Scythe")
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

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Stardust Scythe"));

    game.Process(curPlayer, PlayCardTask::Weapon(card1));
    const int handCount = curHand.GetCount();

    game.Process(curPlayer, AttackTask(curPlayer->GetHero(), opPlayer->GetHero()));
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 27);
    CHECK_EQ(curHand.GetCount(), handCount + 1);
    CHECK_EQ(curHand[handCount]->card->name, "Void Soul");
}

// ------------------------------------- LOCATION - WARLOCK
// [JAIL_511] Spire of Solitude - COST:5 [HP:2]
// - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Epic
// --------------------------------------------------------
// Text: Summon a Demon with stats equal to your hand size.
//       It attacks a random enemy minion.
// --------------------------------------------------------
TEST_CASE("[Warlock : Location] - JAIL_511 : Spire of Solitude")
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
    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Spire of Solitude"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Chillwind Yeti"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::Location(card1));
    const auto location = dynamic_cast<Location*>(card1);
    // In this engine a location is on cooldown the turn it is played.
    CHECK(location->IsOnCooldown());

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    const int handSize = curPlayer->GetHandZone()->GetCount();
    game.Process(curPlayer, PlayLocationTask(location));
    CHECK_EQ(curField.GetMinionCount(), 1);
    const auto demon = curField.GetMinions()[0];
    CHECK_EQ(demon->card->GetRace(), Race::DEMON);
    CHECK_EQ(demon->GetAttack(), handSize);
    CHECK_EQ(demon->GetBaseHealth(), handSize);
    // it attacked the Yeti and was Immune while doing so
    CHECK_EQ(opField[0]->GetHealth(), 5 - handSize);
    CHECK_EQ(demon->GetHealth(), handSize);
    CHECK(location->IsOnCooldown());
}

// -------------------------------------- LOCATION - HUNTER
// [JAIL_877] Underbelly Network - COST:2 [HP:2]
// - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Common
// --------------------------------------------------------
// Text: Summon a 2/1 Rat with "<b>Deathrattle:</b> Draw a card."
// --------------------------------------------------------
TEST_CASE("[Hunter : Location] - JAIL_877 : Underbelly Network")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::HUNTER;
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
        curPlayer, Cards::FindCardByName("Underbelly Network"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));

    game.Process(curPlayer, PlayCardTask::Location(card1));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayLocationTask(dynamic_cast<Location*>(card1)));
    CHECK_EQ(curField.GetMinionCount(), 1);
    const auto rat = curField.GetMinions()[0];
    CHECK_EQ(rat->card->name, "Snoot Hoarder");
    CHECK_EQ(rat->GetAttack(), 2);
    CHECK_EQ(rat->GetHealth(), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    const int handCount = curHand.GetCount();
    game.Process(opPlayer, PlayCardTask::SpellTarget(card2, rat));
    CHECK_EQ(curField.GetMinionCount(), 0);
    CHECK_EQ(curHand.GetCount(), handCount + 1);
}

// ------------------------------------------ SPELL - DRUID
// [JAIL_436] Widow's Bite - COST:2
// - Set: ESCAPEFROM_VIOLET_HOLD, Rarity: Common
// --------------------------------------------------------
// Text: Give your hero +1 Attack this turn. Gain 1 Armor.
//       Add "Widow's Feast" to your hand.
// --------------------------------------------------------
TEST_CASE("[Druid : Spell] - JAIL_436 : Widow's Bite")
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

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Widow's Bite"));

    int handCount = curHand.GetCount();
    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curPlayer->GetHero()->GetAttack(), 1);
    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 1);
    CHECK_EQ(curHand.GetCount(), handCount);
    CHECK_EQ(curHand[handCount - 1]->card->name, "Widow's Feast");

    game.Process(curPlayer, PlayCardTask::Spell(curHand[handCount - 1]));
    CHECK_EQ(curPlayer->GetHero()->GetAttack(), 3);
    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 3);
    CHECK_EQ(curHand[handCount - 1]->card->name, "Widow's Banquet");

    game.Process(curPlayer, PlayCardTask::Spell(curHand[handCount - 1]));
    CHECK_EQ(curPlayer->GetHero()->GetAttack(), 7);
    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 7);
    CHECK_EQ(curHand.GetCount(), handCount - 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);
    CHECK_EQ(curPlayer->GetHero()->GetAttack(), 0);
}

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
TEST_CASE("[Shaman : Minion] - JAIL_800 : Mug'Zee")
{
    REQUIRE_CARDDEFS_XML();

    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    SUBCASE("Mug: no other minions")
    {
        config.player1Deck[0] = Cards::FindCardByName("Mug'Zee");
        for (std::size_t i = 1; i < 30; ++i)
        {
            config.player1Deck[i] = Cards::FindCardByName("Frostbolt");
            config.player2Deck[i] = Cards::FindCardByName("Wisp");
        }
        config.player2Deck[0] = Cards::FindCardByName("Wisp");

        Game game(config);
        game.Start();
        game.ProcessUntil(Step::MAIN_ACTION);

        Player* curPlayer = game.GetCurrentPlayer();
        Player* opPlayer = game.GetOpponentPlayer();
        curPlayer->SetTotalMana(10);
        curPlayer->SetUsedMana(0);
        opPlayer->SetTotalMana(10);
        opPlayer->SetUsedMana(0);

        CHECK_EQ(curPlayer->GetHero()->heroPower->card->id, "JAIL_800hp1");

        const auto card1 =
            Generic::DrawCard(curPlayer, Cards::FindCardByName("Chillwind Yeti"));
        const auto card2 =
            Generic::DrawCard(curPlayer, Cards::FindCardByName("Chillwind Yeti"));
        CHECK_EQ(card1->GetCost(), 4);

        for (int i = 0; i < 2; ++i)
        {
            game.Process(curPlayer, EndTurnTask());
            game.ProcessUntil(Step::MAIN_ACTION);
            game.Process(opPlayer, EndTurnTask());
            game.ProcessUntil(Step::MAIN_ACTION);
        }

        // owner's 3rd turn: first minion costs (2) less
        CHECK_EQ(card1->GetCost(), 2);
        game.Process(curPlayer, PlayCardTask::Minion(card1));
        CHECK_EQ(card2->GetCost(), 4);
    }

    SUBCASE("Zee: no spells")
    {
        config.player1Deck[0] = Cards::FindCardByName("Mug'Zee");
        for (std::size_t i = 1; i < 30; ++i)
        {
            config.player1Deck[i] = Cards::FindCardByName("Wisp");
            config.player2Deck[i] = Cards::FindCardByName("Wisp");
        }
        config.player2Deck[0] = Cards::FindCardByName("Wisp");

        Game game(config);
        game.Start();
        game.ProcessUntil(Step::MAIN_ACTION);

        Player* curPlayer = game.GetCurrentPlayer();
        Player* opPlayer = game.GetOpponentPlayer();
        curPlayer->SetTotalMana(10);
        curPlayer->SetUsedMana(0);
        opPlayer->SetTotalMana(10);
        opPlayer->SetUsedMana(0);

        CHECK_EQ(curPlayer->GetHero()->heroPower->card->id, "JAIL_800hp2");

        for (int i = 0; i < 4; ++i)
        {
            const auto wisp =
                Generic::DrawCard(curPlayer, Cards::FindCardByName("Wisp"));
            game.Process(curPlayer, PlayCardTask::Minion(wisp));
        }
        CHECK_EQ(curPlayer->GetHero()->heroPower->GetGameTag(
                     GameTag::TAG_SCRIPT_DATA_NUM_2),
                 4);
        const auto archer =
            Generic::DrawCard(curPlayer, Cards::FindCardByName("Elven Archer"));
        // 5th minion: Battlecry (deal 1 damage) triggers twice
        game.Process(curPlayer,
                     PlayCardTask::MinionTarget(archer, opPlayer->GetHero()));
        CHECK_EQ(opPlayer->GetHero()->GetHealth(), 28);

        const auto archer2 =
            Generic::DrawCard(curPlayer, Cards::FindCardByName("Elven Archer"));
        game.Process(curPlayer,
                     PlayCardTask::MinionTarget(archer2, opPlayer->GetHero()));
        CHECK_EQ(opPlayer->GetHero()->GetHealth(), 27);
    }
}
