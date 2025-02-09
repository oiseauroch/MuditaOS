// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

namespace service::name
{
    inline constexpr auto bluetooth = "ServiceBluetooth";
} // namespace service::name

namespace sys::bluetooth
{
    enum class BluetoothMode
    {
        ConnectedVoice,
        ConnectedAudio,
        ConnectedBoth,
        Disabled,
        Enabled
    };
}
