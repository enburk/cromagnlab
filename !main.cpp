#pragma once
#include "../auxs/gui_widget_console.h"
#include "../auxs/gui_widget_text_editor.h"
using namespace std::literals::chrono_literals;
using namespace pix;
using gui::widget;

struct App:
widget<App>
{
    void on_change (void* what) override
    {
        if (what == &coord and
            coord.was.size !=
            coord.now.size)
        {
        }
    }
};

sys::app<App> app("cromagnlab");

#include "../auxs/windows_fonts.h"
#include "../auxs/windows_images.h"
#include "../auxs/windows_system.h"
#include "../auxs/windows_windows.h"
