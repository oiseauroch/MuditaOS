// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "application-settings/data/QuoteSwitchData.hpp"

#include <BoxLayout.hpp>
#include <Image.hpp>
#include <Label.hpp>
#include <ListItem.hpp>
#include <service-db/QuotesMessages.hpp>

namespace gui
{
    class CategoryWidget : public ListItem
    {
      public:
        CategoryWidget(const Quotes::CategoryRecord &categoryRecord,
                       std::function<void(bool)> enableCategory,
                       std::function<void(const UTF8 &text)> bottomBarTemporaryMode = nullptr,
                       std::function<void()> bottomBarRestoreFromTemporaryMode      = nullptr);

      private:
        gui::HBox *hBox              = nullptr;
        gui::Label *inputBoxLabel    = nullptr;
        gui::Label *descriptionLabel = nullptr;
        gui::Image *tickImage        = nullptr;

        Quotes::CategoryRecord category;
        std::function<void(bool)> enableCategory                     = nullptr;
        std::function<void(const UTF8 &text)> bottomBarTemporaryMode = nullptr;
        std::function<void()> bottomBarRestoreFromTemporaryMode      = nullptr;
    };

} /* namespace gui */