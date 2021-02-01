// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "CommandHandler.hpp"

#include "BtCommand.hpp"
#include "Device.hpp"

namespace bluetooth
{
    namespace
    {
        [[nodiscard]] auto toString(bluetooth::Error::Code code) -> std::string
        {
            return utils::enumToString(code);
        }
    } // namespace

    CommandHandler::CommandHandler(sys::Service *service,
                                   std::shared_ptr<bluetooth::SettingsHolder> settings,
                                   std::shared_ptr<bluetooth::Profile> currentProfile)
        : service{service}, settings{std::move(settings)}, currentProfile{std::move(currentProfile)}
    {}

    Error::Code CommandHandler::handle(Command command)
    {
        switch (command) {
        case bluetooth::PowerOn:
            return Error::Success;
        case bluetooth::StartScan:
            return scan();
        case bluetooth::StopScan:
            return stopScan();
        case bluetooth::StartPan:
            return startPan();
        case bluetooth::VisibilityOn:
            return setVisibility(true);
        case bluetooth::VisibilityOff:
            return setVisibility(false);
        case bluetooth::ConnectAudio:
            return establishAudioConnection();
        case bluetooth::DisconnectAudio:
            return disconnectAudioConnection();
        case bluetooth::PowerOff:
            return Error::Success;
        }
        return Error::LibraryError;
    }

    Error::Code CommandHandler::scan()
    {
        bluetooth::GAP::setOwnerService(service);
        if (const auto ret = bluetooth::GAP::scan(); ret.err != bluetooth::Error::Success) {
            LOG_ERROR("Cant start scan!: %s %" PRIu32 "", toString(ret.err).c_str(), ret.lib_code);
            return ret.err;
        }

        LOG_INFO("Scan started!");
        // open new scan window
        return Error::Success;
    }

    Error::Code CommandHandler::stopScan()
    {
        bluetooth::GAP::stop_scan();
        return Error::Success;
    }

    Error::Code CommandHandler::startPan()
    {
        bluetooth::PAN::bnep_setup();
        if (const auto err = bluetooth::PAN::bnep_start(); err.err != bluetooth::Error::Success) {
            LOG_ERROR("PAN setup error: %s %" PRIu32, toString(err.err).c_str(), err.lib_code);
            return err.err;
        }
        return bluetooth::Error::Success;
    }

    Error::Code CommandHandler::setVisibility(bool visibility)
    {
        const auto status = bluetooth::GAP::set_visibility(visibility);
        settings->setValue(bluetooth::Settings::Visibility, static_cast<int>(visibility));
        return status.err;
    }

    Error::Code CommandHandler::establishAudioConnection()
    {
        currentProfile->setOwnerService(service);
        if (const auto status = currentProfile->init(); status != bluetooth::Error::Success) {
            return status;
        }

        currentProfile->connect();
        return Error::Success;
    }

    Error::Code CommandHandler::disconnectAudioConnection()
    {
        currentProfile->disconnect();
        return Error::Success;
    }
} // namespace bluetooth