// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <catch2/catch.hpp>

#include "WorkerController.hpp"
#include "interface/BluetoothDriver.hpp"

#include <iostream>

using namespace bluetooth;

class DriverMock : public AbstractDriver
{
  public:
    Error::Code init() override
    {
        return initReturnCode;
    }
    Error::Code run() override
    {
        return runReturnCode;
    }
    Error::Code stop() override
    {
        return stopReturnCode;
    }
    void registerErrorCallback(const ErrorCallback &) override
    {}

    Error::Code initReturnCode = Error::Success;
    Error::Code runReturnCode  = Error::Success;
    Error::Code stopReturnCode = Error::Success;
};

auto InitializerMock = []() { return Error::Success; };

class HandlerMock : public AbstractCommandHandler
{
  public:
    Error::Code handle(Command command) override
    {
        return returnCode;
    }

    Error::Code returnCode = Error::Success;
};

TEST_CASE("Given StatefulController when turn on then turned on")
{
    auto driver    = std::make_unique<DriverMock>();
    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.turnOn();
    REQUIRE(controller.isOn());
}

TEST_CASE("Given StatefulController when error during device registration then turned off")
{
    auto driver    = std::make_unique<DriverMock>();
    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), []() { return Error::SystemError; }};
    controller.turnOn();
    REQUIRE(!controller.isOn());
}

TEST_CASE("Given StatefulController when error during driver init then turned off")
{
    auto driver            = std::make_unique<DriverMock>();
    driver->initReturnCode = Error::SystemError;

    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.turnOn();
    REQUIRE(!controller.isOn());
}

TEST_CASE("Given StatefulController when error during driver run then turned off")
{
    auto driver           = std::make_unique<DriverMock>();
    driver->runReturnCode = Error::SystemError;

    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.turnOn();
    REQUIRE(!controller.isOn());
}

TEST_CASE("Given StatefulController when restart then don't init twice")
{
    auto driver    = std::make_shared<DriverMock>();
    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{driver, std::move(processor), InitializerMock};
    controller.turnOn();
    REQUIRE(controller.isOn());

    controller.turnOff();
    REQUIRE(!controller.isOn());

    driver->initReturnCode = Error::SystemError;
    controller.turnOn();
    REQUIRE(controller.isOn());
}

TEST_CASE("Given StatefulController when turn off in off state then turned off")
{
    auto driver    = std::make_unique<DriverMock>();
    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.turnOff();
    REQUIRE(!controller.isOn());
}

TEST_CASE("Given StatefulController when turn off in on state then turned off")
{
    auto driver    = std::make_unique<DriverMock>();
    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.turnOn();
    REQUIRE(controller.isOn());

    controller.turnOff();
    REQUIRE(!controller.isOn());
}

TEST_CASE("Given StatefulController when shutdown in off state then terminated")
{
    auto driver    = std::make_unique<DriverMock>();
    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.shutdown();
    REQUIRE(controller.isTerminated());
}

TEST_CASE("Given StatefulController when shutdown in on state then terminated")
{
    auto driver    = std::make_unique<DriverMock>();
    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.turnOn();
    REQUIRE(controller.isOn());

    controller.shutdown();
    REQUIRE(controller.isTerminated());
}

TEST_CASE("Given StatefulController when process command successfully then turned on")
{
    auto driver    = std::make_unique<DriverMock>();
    auto processor = std::make_unique<HandlerMock>();
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.turnOn();
    REQUIRE(controller.isOn());

    controller.processCommand(Command::PowerOn);
    REQUIRE(controller.isOn());
}

TEST_CASE("Given StatefulController when processing command failed then restarted and turned on")
{
    auto driver           = std::make_unique<DriverMock>();
    auto processor        = std::make_unique<HandlerMock>();
    processor->returnCode = Error::SystemError;
    StatefulController controller{std::move(driver), std::move(processor), InitializerMock};
    controller.turnOn();
    REQUIRE(controller.isOn());

    controller.processCommand(Command::PowerOn);
    controller.processCommand(Command::PowerOn);
    REQUIRE(controller.isOn());
}