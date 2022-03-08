#pragma once
#include "source.h"
using namespace std::literals::chrono_literals;

struct App:
widget<App>
{
    gui::canvas canvas;
    sourcer sourcers[2];
    int active = 0;

    array<source> sources;

    gui::property<gui::time> timer;
    gui::time lapse;

    App() { skin = "gray+"; }

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

            sourcers[0].coord = XYWH(W/2, 0, W/2, H);
            sourcers[1].coord = XYWH(W/2, 0, W/2, H);
        }

        if (timer.now == gui::time())
            timer.go (gui::time::infinity,
                      gui::time::infinity);

        if (what == &timer)
        {
            if (sources.empty()) scan();
            if (sources.empty()) return;

            if (lapse < gui::time::now)
            {
                auto source = sources[aux::random(0, sources.size() - 1)];

                sourcers[active] = source;
                sourcers[active].show(1s); active = (active + 1) % 2;
                sourcers[active].hide(1s);

                lapse = gui::time::now;
                lapse.ms += 1000 + 20*
                    source.text.size();
            }
        }
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
