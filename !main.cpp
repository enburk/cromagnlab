#pragma once
#include <format>
#include "content.h"

struct App:
widget<App>
{
    canvas canvas;
    sourcer sourcers[2];
    int active = 0;

    console console;
    content content;

    button play, stop;
    button prev, next;
    button slow, coef, fast;
    button cons, cont;

    array<int> undo, redo;
    int current = -1;

    property<gui::time> timer;
    gui::time lapse;

    App()
    {
        skin = "gray+";
        auto button = [this](gui::button& butt, const char* s)
        {
            butt.text.text = s;
            butt.on_change_state = [this, &butt](){
                on_change_state(butt); };
        };
        button(play, (char*)(u8"\u25B6"));
        button(stop, (char*)(u8"\u23F8"));
        button(prev, (char*)(u8"\u23EE"));
        button(next, (char*)(u8"\u23ED"));
        button(cons, (char*)(u8"\u2630"));
        button(cont, (char*)(u8"\u2637"));
        button(slow, "slower");
        button(fast, "faster");
        button(coef, "1.0");
        slow.repeat_delay = 0ms;
        fast.repeat_delay = 0ms;
        slow.repeat_lapse = 16ms;
        fast.repeat_lapse = 16ms;
        slow.repeating = true;
        fast.repeating = true;
        coef.enabled = false;
        cons.kind = gui::button::toggle;
        cont.kind = gui::button::toggle;
        content.hide();
        console.hide();
        play.hide();
    }

    void on_change_state (gui::button& button)
    {
        //console << button.text.text;
        rgba disabled = gui::palettes["gray"][7].first;
        rgba touched = gui::palettes["amber"][9].first;
        rgba hovered = gui::palettes["amber"][8].first;
        rgba active = gui::palettes["amber"][7].first;
        rgba normal = gui::palettes["gray"][3].first;
        auto color = // order important
        not button.enabled  .now? disabled:
        button.enter_pressed.now? touched:
        button.mouse_pressed.now? touched:
        button.mouse_hover  .now? hovered:
        button.on.now? active: normal;
        button.text.color = color;
    };

    void on_change (void* what) override
    {
        if (what == &coord and
            coord.was.size !=
            coord.now.size)
        {
            int W = coord.now.w; if (W <= 0) return;
            int H = coord.now.h; if (H <= 0) return;
            int w = gui::metrics::text::height*25;
            int h = gui::metrics::text::height*12/7;
            int d = gui::metrics::line::width * 6;

            canvas.color = rgba::black;
            canvas.coord = xywh(0, 0, W, H);

            content.coord = xywh(0, 0, W, H);
            console.coord = xywh(0, 0, W/3, H-2*h);
            console.scroll.x.mode = gui::scroll::mode::none;
            console.scroll.y.mode = gui::scroll::mode::none;
            console.page.canvas.color = gui::palettes["gray"][9].first;
            console.page.color = rgba::white;
            console.page.alpha = 192;

            sourcers[0].coord = xywh(W/2, 0, W/2, H);
            sourcers[1].coord = xywh(W/2, 0, W/2, H);

            auto button = [this, H, h](gui::button& butt, int x, int w = 0)
            {
                if (w == 0) w = 2 * h;
                butt.coord = xywh(x*2*h, H-2*h, w, 2*h);
                butt.text.coord = butt.coord.now.local();
                butt.text.font = pix::font{"", h*6/6};
            };
            button(play, 0);
            button(stop, 0);
            button(prev, 1);
            button(next, 2);
            button(slow, 4, h*4);
            button(coef, 6);
            button(fast, 7, h*4);
            button(cons, 12);
            button(cont, 13);
        }

        if (what == &cons) console.show(cons.on.now);
        if (what == &cont) content.show(cont.on.now);
        if (what == &slow)
        {
            str s = coef.text.text;
            double k = std::stof(s);
            if (k > 0.19) {
                k -= 0.1;
                coef.text.text =
                std::format("{:1.1f}", k);
            }
        }
        if (what == &fast)
        {
            str s = coef.text.text;
            double k = std::stof(s);
            if (k < 9.81) {
                k += 0.1;
                coef.text.text =
                std::format("{:1.1f}", k);
            }
        }
        if (what == &play)
        {
            play.hide();
            stop.show();
        }
        if (what == &stop)
        {
            stop.hide();
            play.show();
        }

        bool play_next = false;

        if (what == &prev and not undo.empty())
        {
            play_next = true;
            redo += undo.back();
            undo.pop_back();
            current = -1;
        }
        if (what == &next)
        {
            play_next = true;
        }

        if (timer.now == gui::time())
            timer.go (gui::time::infinity,
                      gui::time::infinity);

        if (what == &timer)
        {
            if (content.sources.empty()) content.scan();
            if (content.sources.empty()) return;

            if (lapse < gui::time::now
            and play.alpha.now == 0)
                play_next = true;
        }

        if (play_next)
        {
            if (current != -1)
                undo += current;

            if (redo.empty())
                redo += aux::random(0, content.sources.size() - 1);

            current = redo.back();
            redo.pop_back();

            auto source = content.sources[current];

            sourcers[active] = source;
            sourcers[active].show(1s); active = (active + 1) % 2;
            sourcers[active].hide(1s);

            str s = coef.text.text;
            auto k = std::atof(s.c_str());

            lapse = gui::time::now;
            lapse.ms += 1000 + int(20/k*
                source.text.size());
        }

        prev.enabled = not undo.empty();
    }
};

sys::app<App> app("cromagnlab");

#include "../auxs/windows_fonts.h"
#include "../auxs/windows_images.h"
#include "../auxs/windows_system.h"
#include "../auxs/windows_render.h"
#include "../auxs/windows_windows.h"
