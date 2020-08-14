# jw

Just Write.

A micro-journaling tool written in C.

Many journaling programs are better-designed for writing long entries. Of course, they do not force you to do so, but they certainly make it preferable to do it that way.

This is different. This is a tool for writing out your thoughts Twitter-style, with a CLI for managing, writing, and editing posts.

Writing out quick thoughts in quick post-format is what this tool is about. I am writing this for myself to be able to keep track of my thoughts in the past, as it can be quite helpful in the future in so many ways.

To outline the philosophy of the project, I wrote a [blog post](https://josias.dev/posts/justwrite/).

This is currently under development, so there are no promises to its completeless or stability at the moment.

Features:
- Distraction-free editor
- Full text search
- Notebooks (categories)

TODO:
- Use system text editor instead of `get_text`
- Automatically add line breaks (or remove one of duplicate line breaks)
- GTK GUI

As this is a general way to let out thoughts and organize them, I think it would be useful to expand this from micro-journaling to noting general thoughts. Maybe other modes for notes and such. And as it uses Markdown, it can be very flexible.

Principles:
- Make posting as simple as opening jw, writing, and then closing
- jw should do any other work for the user, such as metadata, organization, etc.

## Installation

```
$ git clone https://git.josias.dev/jw && cd jw
$ make
$ ./jw help
```

## Similar Software

- [ThotKeeper](https://github.com/cmpilato/thotkeeper)

## License

Copyright (C) 2020, Josias Allestad

You may use this software under the terms of the [BSD-2-Clause license](LICENSE).
