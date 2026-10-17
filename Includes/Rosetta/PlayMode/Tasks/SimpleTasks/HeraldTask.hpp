// RosettaStone fork (hs-winprob): the Herald keyword (CATACLYSM, patch 35.0).
// Rules source: Blizzard, "Deathwing Strikes Back in CATACLYSM"
// (hearthstone.blizzard.com/en-us/news/24245219): playing a Herald card summons
// a Soldier that has the same ability as your class Colossal's appendages; each
// Herald use upgrades both; Herald twice to double their power, four times to
// double it a final time; only Death Knight, Demon Hunter, Rogue, Shaman,
// Warlock and Warrior have a Soldier.

#ifndef ROSETTASTONE_PLAYMODE_HERALD_TASK_HPP
#define ROSETTASTONE_PLAYMODE_HERALD_TASK_HPP

#include <Rosetta/PlayMode/Tasks/ITask.hpp>

namespace RosettaStone::PlayMode::SimpleTasks
{
//! Herald: count the use on the player (HERALD_COLOSSAL_AMOUNT), summon the
//! player's class Soldier to the right of the source and leave it on the task
//! stack, so a following task can address "the Soldier" as EntityType::STACK.
class HeraldTask : public ITask
{
 public:
    //! The Soldier token summoned for \p player's class, or nullptr.
    static const char* SoldierCardID(const Player* player);

    //! The power multiplier of Soldiers and appendages: 1, 2 after two
    //! Heralds, 4 after four.
    static int Multiplier(const Player* player);

 private:
    TaskStatus Impl(Player* player) override;
    std::unique_ptr<ITask> CloneImpl() override;
};
}  // namespace RosettaStone::PlayMode::SimpleTasks

#endif  // ROSETTASTONE_PLAYMODE_HERALD_TASK_HPP
