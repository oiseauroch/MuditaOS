// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <Style.hpp>
#include <utf8/UTF8.hpp>

namespace gui::option
{
    enum class Arrow : bool
    {
        Disabled,
        Enabled
    };

    enum class ContactOperation
    {
        Add,
        Details,
        Edit
    };

    enum class SettingRightItem
    {
        Disabled,
        ArrowBlack,
        ArrowWhite,
        On,
        Off,
        Bt,
        Checked,
    };

    namespace window
    {
        inline constexpr gui::Length option_left_margin  = 10;
        inline constexpr gui::Length option_right_margin = 10;
        inline constexpr uint32_t optionsListTopMargin   = 10U;
        inline constexpr uint32_t optionsListX           = style::window::default_left_margin;
        inline constexpr uint32_t optionsListY           = style::header::height + optionsListTopMargin;
        inline constexpr uint32_t optionsListW           = style::window::default_body_width;
        inline constexpr uint32_t optionsListH           = style::window::default_body_height;
    } // namespace window

} // namespace gui::option