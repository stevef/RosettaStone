// Copyright (c) 2017-2024 Chris Ohk

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Rosetta/PlayMode/Games/Game.hpp>
#include <Rosetta/PlayMode/Models/Enchantment.hpp>
#include <Rosetta/PlayMode/Models/Player.hpp>

#include <algorithm>
#include <utility>

namespace RosettaStone::PlayMode
{
Enchantment::Enchantment(Player* _player, Card* _card,
                         std::map<GameTag, int> tags, Playable* owner,
                         Entity* target, int id)
    : Playable(_player, _card, std::move(tags), id),
      m_owner(owner),
      m_target(target)
{
    // Do nothing
}

std::shared_ptr<Enchantment> Enchantment::GetInstance(Playable* owner,
                                                      Card* _card,
                                                      Entity* target, int num1,
                                                      int num2, int id)
{
    using enum GameTag;

    if (id < 0)
    {
        id = owner->player->game->GetNextID();
    }

    std::map<GameTag, int> tags;
    tags[ENTITY_ID] = id;
    tags[CONTROLLER] = owner->player->playerID;
    // Power.log: an enchantment is an entity in the PLAY zone while it is
    // attached, with CREATOR = the entity that applied it and ATTACHED = its
    // target. It goes to GRAVEYARD when it expires (see Remove()).
    tags[ZONE] = std::to_underlying(ZoneType::PLAY);
    tags[CREATOR] = owner->GetGameTag(ENTITY_ID);
    tags[ATTACHED] = target->GetGameTag(ENTITY_ID);

    if (num1 > 0)
    {
        tags[TAG_SCRIPT_DATA_NUM_1] = num1;

        if (num2 > 0)
        {
            tags[TAG_SCRIPT_DATA_NUM_2] = num2;
        }
    }

    auto instance = std::make_shared<Enchantment>(owner->player, _card, tags,
                                                  owner, target, id);

    target->appliedEnchantments.emplace_back(instance);
    owner->game->entityList.emplace(id, instance.get());

    if (_card->gameTags[TAG_ONE_TURN_EFFECT] == 1)
    {
        instance->m_isOneTurnActive = true;
        owner->game->oneTurnEffectEnchantments.emplace_back(instance);
    }

    instance->orderOfPlay = owner->game->GetNextOOP();

    if (!_card->power.GetDeathrattleTask().empty())
    {
        if (const auto playable = dynamic_cast<Playable*>(target); playable)
        {
            playable->SetGameTag(DEATHRATTLE, 1);
        }
    }

    return instance;
}

Playable* Enchantment::GetOwner() const
{
    return m_owner;
}

Entity* Enchantment::GetTarget() const
{
    return m_target;
}

int Enchantment::GetScriptTag1() const
{
    return GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_1);
}

int Enchantment::GetScriptTag2() const
{
    return GetGameTag(GameTag::TAG_SCRIPT_DATA_NUM_2);
}

bool Enchantment::IsOneTurnActive() const
{
    return m_isOneTurnActive;
}

void Enchantment::AddOneTurnEffect(const std::shared_ptr<IEffect>& effect)
{
    m_oneTurnEffects.emplace_back(effect);
}

const std::vector<std::weak_ptr<IEffect>>& Enchantment::GetOneTurnEffects()
    const
{
    return m_oneTurnEffects;
}

Card* Enchantment::GetCapturedCard() const
{
    return m_capturedCard;
}

void Enchantment::SetCapturedCard(Card* _card)
{
    m_capturedCard = _card;
}

void Enchantment::Remove()
{
    // Power.log: an enchantment that expires goes to GRAVEYARD (one whose
    // target died is sent to REMOVEDFROMGAME by GraveyardZone first).
    // m_target is not read here: it may already be destroyed.
    if (GetZoneType() == ZoneType::PLAY)
    {
        SetZoneType(ZoneType::GRAVEYARD);
    }

    if (const auto deathrattleTask = card->power.GetDeathrattleTask();
        !deathrattleTask.empty() &&
        m_target->zone->GetType() == ZoneType::GRAVEYARD)
    {
        for (const auto& power : card->power.GetDeathrattleTask())
        {
            std::unique_ptr<ITask> clonedPower = power->Clone();

            clonedPower->SetPlayer(m_target->player);
            clonedPower->SetSource(m_target);
            clonedPower->SetTarget(this);

            game->taskQueue.Enqueue(std::move(clonedPower));
        }
    }

    if (ongoingEffect)
    {
        ongoingEffect->Remove();
    }

    if (activatedTrigger)
    {
        activatedTrigger->Remove();
    }

    m_oneTurnEffects.clear();
}
}  // namespace RosettaStone::PlayMode
