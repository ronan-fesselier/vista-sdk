#pragma once

#include <dnv/VistaSDK.h>

#include <optional>

namespace dnv::vista::sdk::c
{
    /**
     * @brief Selects the metadata tag slot for `name` on any type exposing the standard
     *        quantity()/content()/calculation()/state()/command()/type()/position()/detail()
     *        accessors (LocalIdBuilder, LocalId, mqtt::LocalId) - avoids repeating the
     *        same switch in each wrapper
     */
    template <typename T>
    inline const std::optional<MetadataTag>* selectMetadataTag(const T& obj, CodebookName name)
    {
        switch (name)
        {
            case CodebookName::Quantity:
                return &obj.quantity();
            case CodebookName::Content:
                return &obj.content();
            case CodebookName::Calculation:
                return &obj.calculation();
            case CodebookName::State:
                return &obj.state();
            case CodebookName::Command:
                return &obj.command();
            case CodebookName::Type:
                return &obj.type();
            case CodebookName::Position:
                return &obj.position();
            case CodebookName::Detail:
                return &obj.detail();
            default:
                return nullptr;
        }
    }
} // namespace dnv::vista::sdk::c
