#pragma once
#include "../auxs/gui_widget_console.h"
#include "../auxs/gui_widget_text_editor.h"
#include "../ae/proto-studio/doc_cpp_model.h"
using namespace pix;
using gui::widget;

struct source
{
    str text;
    str kind;
};
struct sourcer:
widget<sourcer>
{
    gui::text::editor editor;
    gui::property<gui::time> timer;
    gui::time lapse;

    doc::cpp ::model cpp;
    doc::text::model txt;
    doc::text::model *model;

    void operator = (source source)
    {
        editor.model = model =
            source.kind == ".c" or
            source.kind == ".java" or
            source.kind == ".cpp" ?
            &cpp : &txt;

        double k = 2.0;
        auto& lines =
        editor.view.cell.lines;
        editor.text = source.text; do {
        editor.page.style = pix::text::style{
            sys::font{"Consolas", int(
            gui::metrics::text::height*k)},
            RGBA::white};
            k *= 0.9; }
            while (k > 0.9 and (
            lines.coord.now.h > coord.now.h or
            lines.coord.now.w > coord.now.w));

        RGBA white  = gui::palettes["gray"][1].first;
        RGBA black  = RGBA::black;
        RGBA blue   = gui::palettes["blue"][4].first;
        RGBA teal   = RGBA::teal;
        RGBA purple = RGBA::purple;
        RGBA navy   = gui::palettes["cyan"][5].first;
        RGBA maroon = gui::palettes["orange"][8].first;
        RGBA green  = RGBA::green;
        RGBA red    = RGBA::red;

        using pix::text::style_index;
        auto s = editor.view.style.now;
        auto& ss = doc::model::styles;
        s.color = white;   ss["name"     ] = style_index(s);
        s.color = blue;    ss["keyword"  ] = style_index(s);
        s.color = teal;    ss["keyname"  ] = style_index(s);
        s.color = blue;    ss["pragma"   ] = style_index(s);
        s.color = purple;  ss["macros"   ] = style_index(s);
        s.color = purple;  ss["module"   ] = style_index(s);
        s.color = navy;    ss["number"   ] = style_index(s);
        s.color = black;   ss["space"    ] = style_index(s); 
        s.color = navy;    ss["literal"  ] = style_index(s); 
        s.color = navy;    ss["char"     ] = style_index(s); 
        s.color = maroon;  ss["symbol"   ] = style_index(s);
        s.color = maroon;  ss["semicolon"] = style_index(s);
        s.color = green;   ss["comment"  ] = style_index(s);
        s.color = red;     ss["error"    ] = style_index(s);

        model->tokenize();
        editor.update_colors = true;
    }

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

            editor.coord = XYWH(0, h, W, H-h);
            editor.page.alignment = XY{ pix::left, pix::top };
            editor.scroll.x.mode = gui::scroll::mode::none;
            editor.virtual_space = true;
            editor.view.wordwrap = false;
            editor.read_only = true;
        }

        if (timer.now == gui::time())
            timer.go (gui::time::infinity,
                      gui::time::infinity);

        if (what == &timer)
        {
        }
    }
};
