// Copyright (c) 2026 hs-winprob contributors

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef ROSETTASTONE_PLAYMODE_CARD_DEFS_XML_LOADER_HPP
#define ROSETTASTONE_PLAYMODE_CARD_DEFS_XML_LOADER_HPP

#include <Rosetta/PlayMode/Cards/Card.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace RosettaStone::PlayMode
{
//!
//! \brief CardDefsXmlLoader class.
//!
//! Loads card data from HearthSim/hsdata CardDefs.xml instead of cards.json.
//! Every <Entity> becomes a Card: id, dbfID, enUS name and text, every <Tag>
//! the GameTag enum names (plus all tags raw in Card::hsTags) and the card
//! IDs referenced by type="Card" tags.
//!
class CardDefsXmlLoader
{
 public:
    //! Counts from the last Load() call.
    struct Stats
    {
        std::size_t entities = 0;         //!< <Entity> elements read
        std::size_t tags = 0;             //!< <Tag> elements read
        std::size_t tagsUnnamed = 0;      //!< tags with no GameTag enum name
        std::size_t enumValuesUnknown = 0;  //!< set/type/race/... IDs unmapped
        std::string build;                //!< CardDefs build attribute
    };

    //! Loads card data from \p path.
    //! \param cards Data storage to store added cards.
    //! \param path The path of CardDefs.xml.
    //! \return Counts for the load.
    static Stats Load(std::vector<Card*>& cards, const std::string& path);

    //! Returns the counts from the last Load() call.
    static const Stats& GetLastStats();
};
}  // namespace RosettaStone::PlayMode

#endif  // ROSETTASTONE_PLAYMODE_CARD_DEFS_XML_LOADER_HPP
