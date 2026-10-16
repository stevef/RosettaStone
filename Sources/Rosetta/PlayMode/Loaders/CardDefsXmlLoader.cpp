// Copyright (c) 2026 hs-winprob contributors

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Rosetta/Common/Utils.hpp>
#include <Rosetta/PlayMode/Loaders/CardDefsXmlLoader.hpp>
#include <Rosetta/PlayMode/Loaders/CardLoader.hpp>
#include <Rosetta/PlayMode/Loaders/HsEnumNames.hpp>

#include <libxml/xmlreader.h>

#include <memory>
#include <regex>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace RosettaStone::PlayMode
{
namespace
{
CardDefsXmlLoader::Stats g_lastStats;

// Blizzard's numeric tag IDs for the tags whose value is itself an enum.
constexpr int HS_CARD_SET = 183, HS_CARDTYPE = 202, HS_CLASS = 199,
              HS_CARDRACE = 200, HS_RARITY = 203, HS_SPELL_SCHOOL = 1635,
              HS_MULTI_CLASS_GROUP = 480, HS_FACTION = 201, HS_CARDNAME = 185,
              HS_CARDTEXT = 184;

std::string Attr(xmlTextReaderPtr reader, const char* name)
{
    xmlChar* value = xmlTextReaderGetAttribute(reader, BAD_CAST name);
    std::string result = value ? reinterpret_cast<const char*>(value) : "";
    xmlFree(value);
    return result;
}

// Blizzard numeric ID -> HearthSim name -> RosettaStone positional enum.
template <typename Enum>
int MapEnum(const std::unordered_map<int, std::string_view>& names, int value,
            Enum fallback, CardDefsXmlLoader::Stats& stats)
{
    const auto iter = names.find(value);
    if (iter == names.end())
    {
        ++stats.enumValuesUnknown;
        return static_cast<int>(fallback);
    }

    const Enum result = StrToEnum<Enum>(iter->second);
    if (EnumToStr(result) != iter->second)
    {
        ++stats.enumValuesUnknown;
        return static_cast<int>(fallback);
    }

    return static_cast<int>(result);
}

std::string CleanText(std::string text)
{
    // Same cleanup as CardLoader (cards.json)
    RemoveSubstrs(text, std::string{ "\n" });
    RemoveSubstrs(text, std::string{ "[x]" });
    text.erase(0, text.find_first_not_of(' '));
    static const std::regex spaces("[' ']{2,}");
    return std::regex_replace(text, spaces, " ");
}
}  // namespace

CardDefsXmlLoader::Stats CardDefsXmlLoader::Load(std::vector<Card*>& cards,
                                                 const std::string& path)
{
    Stats stats;

    std::unordered_map<std::string_view, GameTag> tagByName;
    for (std::size_t i = 0;
         i < sizeof(GAME_TAG_STR) / sizeof(GAME_TAG_STR[0]); ++i)
    {
        tagByName.emplace(GAME_TAG_STR[i], static_cast<GameTag>(i));
    }
    // CardDefs.xml spellings that differ from the GameTag enum (cards.json)
    tagByName.emplace("HONORABLE_KILL", GameTag::HONORABLEKILL);
    tagByName.emplace("CASTS_WHEN_DRAWN", GameTag::CASTSWHENDRAWN);
    tagByName.emplace("CORRUPTED_CARD", GameTag::CORRUPTEDCARD);
    tagByName.emplace("SIDE_QUEST", GameTag::SIDEQUEST);
    tagByName.emplace("AUTO_ATTACK", GameTag::AUTOATTACK);

    const std::unique_ptr<xmlTextReader, decltype(&xmlFreeTextReader)> reader(
        xmlReaderForFile(path.c_str(), nullptr, XML_PARSE_NOBLANKS),
        &xmlFreeTextReader);
    if (!reader)
    {
        throw std::runtime_error("Can't open CardDefs.xml - Path: " + path);
    }

    std::unique_ptr<Card> card;
    int tagEnumID = 0;  // the <Tag> we are inside, for LocString children

    int ret;
    while ((ret = xmlTextReaderRead(reader.get())) == 1)
    {
        const int nodeType = xmlTextReaderNodeType(reader.get());
        const std::string_view name =
            reinterpret_cast<const char*>(xmlTextReaderConstName(reader.get()));

        if (nodeType == XML_READER_TYPE_ELEMENT)
        {
            if (name == "CardDefs")
            {
                stats.build = Attr(reader.get(), "build");
            }
            else if (name == "Entity")
            {
                card = std::make_unique<Card>();
                card->id = Attr(reader.get(), "CardID");
                card->dbfID = std::stoi(Attr(reader.get(), "ID"));
                // Tags cards.json always writes; Card getters use map::at()
                for (const GameTag tag :
                     { GameTag::ATK, GameTag::CARDRACE, GameTag::CARD_SET,
                       GameTag::CARDTYPE, GameTag::CLASS,
                       GameTag::MULTI_CLASS_GROUP, GameTag::COLLECTIBLE,
                       GameTag::COST, GameTag::DAMAGE, GameTag::DURABILITY,
                       GameTag::FACTION, GameTag::ARMOR, GameTag::HEALTH,
                       GameTag::RARITY, GameTag::SPELL_SCHOOL })
                {
                    card->gameTags[tag] = 0;
                }
                ++stats.entities;
            }
            else if (name == "Tag" && card)
            {
                ++stats.tags;
                tagEnumID = std::stoi(Attr(reader.get(), "enumID"));
                const std::string type = Attr(reader.get(), "type");
                const std::string tagName = Attr(reader.get(), "name");

                if (type == "Card")
                {
                    card->referencedCardIDs.emplace_back(
                        Attr(reader.get(), "cardID"));
                }
                if (type != "Int" && type != "Card")
                {
                    continue;  // LocString handled below, String skipped
                }

                int value = std::stoi(Attr(reader.get(), "value"));
                card->hsTags[tagEnumID] = value;

                switch (tagEnumID)
                {
                    case HS_CARD_SET:
                        value = MapEnum(HsEnumNames::CardSet, value,
                                        CardSet::INVALID, stats);
                        break;
                    case HS_CARDTYPE:
                        value = MapEnum(HsEnumNames::CardType, value,
                                        CardType::INVALID, stats);
                        break;
                    case HS_CLASS:
                        value = MapEnum(HsEnumNames::CardClass, value,
                                        CardClass::INVALID, stats);
                        break;
                    case HS_CARDRACE:
                        value = MapEnum(HsEnumNames::Race, value,
                                        Race::INVALID, stats);
                        break;
                    case HS_RARITY:
                        value = MapEnum(HsEnumNames::Rarity, value,
                                        Rarity::INVALID, stats);
                        break;
                    case HS_SPELL_SCHOOL:
                        value = MapEnum(HsEnumNames::SpellSchool, value,
                                        SpellSchool::NONE, stats);
                        break;
                    case HS_MULTI_CLASS_GROUP:
                        value = MapEnum(HsEnumNames::MultiClassGroup, value,
                                        MultiClassGroup::INVALID, stats);
                        break;
                    case HS_FACTION:
                        value = MapEnum(HsEnumNames::Faction, value,
                                        Faction::INVALID, stats);
                        break;
                    default:
                        break;
                }

                const auto iter = tagByName.find(tagName);
                if (iter == tagByName.end())
                {
                    ++stats.tagsUnnamed;
                    continue;
                }
                card->gameTags[iter->second] = value;
            }
            else if (name == "enUS" && card &&
                     (tagEnumID == HS_CARDNAME || tagEnumID == HS_CARDTEXT))
            {
                xmlChar* text = xmlTextReaderReadString(reader.get());
                std::string value =
                    text ? reinterpret_cast<const char*>(text) : "";
                xmlFree(text);

                if (tagEnumID == HS_CARDNAME)
                {
                    card->name = std::move(value);
                }
                else
                {
                    card->text = CleanText(std::move(value));
                }
            }
        }
        else if (nodeType == XML_READER_TYPE_END_ELEMENT)
        {
            if (name == "Entity" && card)
            {
                // Weapons keep durability in HEALTH; cards.json (and the
                // engine) call it DURABILITY
                if (card->GetCardType() == CardType::WEAPON &&
                    card->gameTags[GameTag::DURABILITY] == 0)
                {
                    card->gameTags[GameTag::DURABILITY] =
                        card->gameTags[GameTag::HEALTH];
                    card->gameTags[GameTag::HEALTH] = 0;
                }
                CardLoader::ApplyTextTags(card.get());
                cards.emplace_back(card.release());
            }
            else if (name == "Tag")
            {
                tagEnumID = 0;
            }
        }
    }

    if (ret != 0)
    {
        throw std::runtime_error("Failed to parse CardDefs.xml - Path: " +
                                 path);
    }

    g_lastStats = stats;
    return stats;
}

const CardDefsXmlLoader::Stats& CardDefsXmlLoader::GetLastStats()
{
    return g_lastStats;
}
}  // namespace RosettaStone::PlayMode
