// CardDefsCheck: load CardDefs.xml, instantiate a list of card IDs, report.
//
//   CardDefsCheck <CardDefs.xml> <card_ids.txt>
//
// card_ids.txt: one card ID per line. Prints counts; exit code 1 if any ID
// is missing or fails to instantiate.

#include <Rosetta/PlayMode/Cards/CardDefs.hpp>
#include <Rosetta/PlayMode/Cards/Cards.hpp>
#include <Rosetta/PlayMode/Games/Game.hpp>
#include <Rosetta/PlayMode/Games/GameConfig.hpp>
#include <Rosetta/PlayMode/Loaders/CardDefsXmlLoader.hpp>
#include <Rosetta/PlayMode/Models/Entity.hpp>
#include <Rosetta/PlayMode/Models/Player.hpp>
#include <Rosetta/PlayMode/Zones/SetasideZone.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace RosettaStone;
using namespace PlayMode;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "usage: CardDefsCheck <CardDefs.xml> <card_ids.txt>\n";
        return 2;
    }

    std::vector<std::string> ids;
    {
        std::ifstream in(argv[2]);
        if (!in)
        {
            std::cerr << "can't open " << argv[2] << '\n';
            return 2;
        }
        for (std::string line; std::getline(in, line);)
        {
            if (!line.empty())
            {
                ids.emplace_back(line);
            }
        }
    }

    Cards::SetCardDefsXmlPath(argv[1]);

    const auto t0 = std::chrono::steady_clock::now();
    const auto& all = Cards::GetInstance().GetAllCards();
    const auto t1 = std::chrono::steady_clock::now();
    const auto loadMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    std::size_t scriptedAll = 0;
    for (const Card* card : all)
    {
        scriptedAll += card->IsScripted();
    }
    const auto& stats = CardDefsXmlLoader::GetLastStats();

    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;
    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);
    Player* player = game.GetCurrentPlayer();

    std::size_t found = 0, instantiated = 0, scripted = 0, vanilla = 0;
    std::vector<std::string> missing, failed;

    for (const auto& id : ids)
    {
        Card* card = Cards::FindCardByID(id);
        if (card->id != id)
        {
            missing.emplace_back(id);
            continue;
        }
        ++found;

        try
        {
            Playable* playable = Entity::GetFromCard(
                player, card, std::nullopt, player->GetSetasideZone());
            player->GetSetasideZone()->Add(playable);
            ++instantiated;
            (card->IsScripted() ? scripted : vanilla) += 1;
        }
        catch (const std::exception& e)
        {
            failed.emplace_back(id + " (" + e.what() + ")");
        }
    }

    std::cout << "CardDefs.xml build: " << stats.build << '\n'
              << "load time ms: " << loadMs << '\n'
              << "entities read: " << stats.entities << '\n'
              << "cards loaded: " << all.size() << '\n'
              << "cards with a script (CardDef): " << scriptedAll << " of "
              << CardDefs::Count() << " registered defs\n"
              << "tags read: " << stats.tags << ", not named in GameTag enum: "
              << stats.tagsUnnamed << ", enum values unmapped: "
              << stats.enumValuesUnknown << '\n'
              << "ids requested: " << ids.size() << '\n'
              << "ids found: " << found << '\n'
              << "ids instantiated: " << instantiated << '\n'
              << "  scripted: " << scripted << '\n'
              << "  vanilla (IsScripted()==false): " << vanilla << '\n';
    for (const auto& id : missing)
    {
        std::cout << "MISSING " << id << '\n';
    }
    for (const auto& id : failed)
    {
        std::cout << "FAILED " << id << '\n';
    }

    return (missing.empty() && failed.empty()) ? 0 : 1;
}
