// RosettaStone fork (hs-winprob): the Herald keyword. See the header.

#include <Rosetta/PlayMode/Models/Hero.hpp>
#include <Rosetta/PlayMode/Models/Minion.hpp>
#include <Rosetta/PlayMode/Games/Game.hpp>
#include <Rosetta/PlayMode/Models/Player.hpp>
#include <Rosetta/PlayMode/Tasks/SimpleTasks/HeraldTask.hpp>
#include <Rosetta/PlayMode/Tasks/SimpleTasks/SummonTask.hpp>

#include <cstdlib>
#include <cstring>

namespace RosettaStone::PlayMode::SimpleTasks
{
const char* HeraldTask::SoldierCardID(const Player* player)
{
    // Soldier tokens from CardDefs.xml (build 254477); each repeats the text of
    // its class Colossal's appendage. Classes not aligned with Deathwing have none.
    switch (player->GetHero()->card->GetCardClass())
    {
        case CardClass::DEATHKNIGHT:
            return "CATA_780t";  // Soldier of Onyxia
        case CardClass::DEMONHUNTER:
            return "CATA_525t";  // Soldier of Azshara
        case CardClass::ROGUE:
            return "CATA_158t";  // Soldier of Sinestra
        case CardClass::SHAMAN:
            return "CATA_565t";  // Soldier of Al'Akir
        case CardClass::WARLOCK:
            return "CATA_725t";  // Soldier of Cho'gall
        case CardClass::WARRIOR:
            return "CATA_580t";  // Soldier of Ragnaros
        default:
            return nullptr;
    }
}

int HeraldTask::Multiplier(const Player* player)
{
    const int amount = player->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT);
    return amount >= 4 ? 4 : amount >= 2 ? 2 : 1;
}

TaskStatus HeraldTask::Impl(Player* player)
{
    // Switch for the fidelity measurement: the keyword as a no-op.
    if (const char* off = std::getenv("ROSETTA_DISABLE_KEYWORDS");
        off && std::strstr(off, "HERALD"))
    {
        return TaskStatus::COMPLETE;
    }

    player->SetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT,
                       player->GetGameTag(GameTag::HERALD_COLOSSAL_AMOUNT) + 1);

    const char* soldier = SoldierCardID(player);
    if (!soldier)
    {
        player->game->taskStack.playables.clear();
        return TaskStatus::COMPLETE;
    }

    // To the right of a minion on the board; otherwise (spell, weapon,
    // location, deathrattle) the default rightmost slot.
    const auto minion = dynamic_cast<Minion*>(m_source);
    const SummonSide side =
        minion && minion->GetZoneType() == ZoneType::PLAY ? SummonSide::RIGHT
                                                           : SummonSide::DEFAULT;
    player->game->taskStack.playables.clear();
    SummonTask summon(soldier, side, true);
    summon.SetPlayer(player);
    summon.SetSource(m_source);
    summon.SetTarget(m_target);
    const TaskStatus status = summon.Run();

    // "When summoned" text on a Soldier is scripted as its POWER tasks; the
    // engine's AFTER_SUMMON trigger never fires for the summoned minion itself
    // (Trigger::Validate excludes the owner).
    auto& stack = player->game->taskStack.playables;
    if (status == TaskStatus::COMPLETE && !stack.empty() &&
        stack.back()->card->id == soldier)
    {
        Playable* summoned = stack.back();
        summoned->ActivateTask(PowerType::POWER);
        stack = { summoned };
    }
    return status;
}

std::unique_ptr<ITask> HeraldTask::CloneImpl()
{
    return std::make_unique<HeraldTask>();
}
}  // namespace RosettaStone::PlayMode::SimpleTasks
