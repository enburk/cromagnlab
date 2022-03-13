#pragma once
#include <format>
#include "source.h"
using namespace std::literals::chrono_literals;

struct App:
widget<App>
{
    gui::canvas canvas;
    sourcer sourcers[2];
    int active = 0;

    gui::button play, stop;
    gui::button prev, next;
    gui::button slow, coef, fast;
    gui::button cons;

    gui::console console;

    array<source> sources;
    array<int> undo, redo;
    int current = -1;

    gui::property<gui::time> timer;
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
        button(cons, (char*)(u8"\u2637"));
        button(slow, "slower");
        button(fast, "faster");
        button(coef, "1.0");
        slow.repeat_lapse = 16ms;
        fast.repeat_lapse = 16ms;
        slow.repeating = true;
        fast.repeating = true;
        coef.enabled = false;
        console.hide();
        play.hide();
    }

    void on_change_state (gui::button& button)
    {
        //console << button.text.text;
        RGBA disabled = gui::palettes["gray"][7].first;
        RGBA touched = gui::palettes["amber"][9].first;
        RGBA hovered = gui::palettes["amber"][8].first;
        RGBA active = gui::palettes["amber"][7].first;
        RGBA normal = gui::palettes["gray"][3].first;
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

            canvas.color = RGBA::black;
            canvas.coord = XYWH(0, 0, W, H);

            console.coord = XYWH(0, 0, W/3, H-2*h);
            console.scroll.x.mode = gui::scroll::mode::none;
            console.scroll.y.mode = gui::scroll::mode::none;
            console.page.canvas.color = RGBA{};
            console.page.color = RGBA::gray;

            sourcers[0].coord = XYWH(W/2, 0, W/2, H);
            sourcers[1].coord = XYWH(W/2, 0, W/2, H);

            auto button = [this, H, h](gui::button& butt, int x, int w = 0)
            {
                if (w == 0) w = 2 * h;
                butt.coord = XYWH(x*2*h, H-2*h, w, 2*h);
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
        }

        if (what == &cons)
        {
            if (console.alpha.to == 0)
                console.show(); else
                console.hide();
        }
        if (what == &slow)
        {
            str s = coef.text.text;
            auto k = std::atof(s.c_str());
            if (k > 0.19) {
                k -= 0.1;
                coef.text.text =
                std::format("{:1.1f}", k);
            }
        }
        if (what == &fast)
        {
            str s = coef.text.text;
            auto k = std::atof(s.c_str());
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
            if (sources.empty()) scan();
            if (sources.empty()) return;

            if (lapse < gui::time::now
            and play.alpha.now == 0)
                play_next = true;
        }

        if (play_next)
        {
            if (current != -1)
                undo += current;

            if (redo.empty())
                redo += aux::random(0, sources.size() - 1);

            current = redo.back();
            redo.pop_back();

            auto source = sources[current];

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

    void scan (std::filesystem::path dir = {})
    {
        using namespace std::filesystem;

        if (dir == path{}) dir = current_path() / "src";

        for (directory_iterator next(dir), end; next != end; ++next)
        {
            path path = next->path();
            if (is_directory (path)) {
                str name = path.filename().string();
                if (name.starts_with(".")
                ||  name.starts_with("_")
                ||  name == "packages")
                continue;
                scan(path);
            }
            if (is_regular_file(path))
            {
                source source;
                source.kind = path.extension().string();
                str s;
                {
                    std::ifstream stream(path); s = std::string{(
                    std::istreambuf_iterator<char>(stream)),
                    std::istreambuf_iterator<char>()};
                }
                s.replace_all("\r\n", "\n");
                s.replace_all("\r", "\n");
                if (s.starts_with("\xEF" "\xBB" "\xBF"))
                    s.upto(3).erase(); // UTF-8 BOM

                auto ss = s.split_by("/* {{{ */");

                str title;
                if (ss.size() > 0) {
                    title = ss.front();
                    ss.erase(0); }

                for (str s : ss)
                {
                    str meta, text;
                    s.split_by("/* }}} */", meta, text);

                    text.strip("\n");
                    source.text = title;
                    source.text += text;
                    sources += source;
                }
            }
        }
    }
};

sys::app<App> app("cromagnlab");

#include "../auxs/windows_fonts.h"
#include "../auxs/windows_images.h"
#include "../auxs/windows_system.h"
#include "../auxs/windows_windows.h"
