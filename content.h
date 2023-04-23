#pragma once
#include "source.h"

struct content:
widget<content>
{
    array<source> sources;

    canvas canvas;
    gui::text::view contents;
    sourcer preview;

    void on_change (void* what) override
    {
        if (what == &coord and
            coord.was.size !=
            coord.now.size)
        {
            int W = coord.now.w; if (W <= 0) return;
            int H = coord.now.h; if (H <= 0) return;

            canvas.color = rgba::black;
            canvas.coord = xywh(0, 0, W, H);

            contents.coord = xyxy(0,0,W*3/4,H);
            preview .coord = xyxy(W*3/4,0,W,H);
        }
    }

    void scan (std::filesystem::path dir = {}, int indent = 0)
    {
        str abc;
        abc += bold(white("Content"));

        using namespace std::filesystem;

        if (dir == path{}) dir = current_path() / "src";

        for (directory_iterator next(dir), end; next != end; ++next)
        {
            path path = next->path();
            if (is_directory (path)) {
                str name = path.filename().string();
                if (name.starts_with(".")
                or  name.starts_with("_"))
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

                array<str> ss = s.split_by("/* {{{ */");

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

        contents.html = abc;
    }
};

