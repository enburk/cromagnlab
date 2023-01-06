// https://github.com/fsprojects/FSharp.Data/blob/main/src/Html/HtmlParser.fs

            let appendEndTag name =
                append "</"
                append name
                append ">