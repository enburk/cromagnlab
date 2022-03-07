#pragma once
#include "../auxs/gui_widget_console.h"
#include "../auxs/gui_widget_text_editor.h"
using namespace std::literals::chrono_literals;
using namespace pix;
using gui::widget;

struct App:
widget<App>
{
    struct topic
    {
        str text;
        str kind;
    };

    array<topic> topics;

    gui::console console;
    gui::property<gui::time> timer;
    gui::time lapse;

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

            console.coord = XYWH(W/2, 0, W/3, H);
            console.page.alignment = XY{ pix::left, pix::top };
            console.page.style = pix::text::style{
                sys::font{"Consolas",
                gui::metrics::text::height*2},
                RGBA::white};
        }
        if (what == &skin)
        {
        }

        if (timer.now == gui::time())
            timer.go (gui::time::infinity,
                      gui::time::infinity);

        if (what == &timer)
        {
            if (topics.empty())
                scan();

            if (topics.empty())
                return;

            if (lapse < gui::time::now)
            {
                auto topic = topics[aux::random(0, topics.size()-1)];

                console.clear();
                console << doc::html::encoded(topic.text);

                lapse = gui::time::now;
                lapse.ms += 1000 + 20*
                    topic.text.size();
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
                topic topic;
                topic.kind = path.extension().string();
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

                auto ss = s.split_by("/*#####*/");

                str title;
                if (ss.size() > 0) {
                    title = ss.front();
                    ss.erase(0); }

                for (str s : ss)
                {
                    topic.text = s;
                    topics += topic;
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
