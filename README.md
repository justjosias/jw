# jw

Just Write.

A micro-journaling tool written in C.

Many journaling programs are better-designed for writing long entries. Of course, they do not force you to do so, but they certainly make it preferable to do it that way.

This is different. This is a tool for writing out your thoughts Twitter-style, with a CLI for managing, writing, and editing posts.

Writing out thoughts quickly and distraction-free is what this tool is about. I am writing this for myself to be able to keep track of my thoughts.

To outline the philosophy of the project, I wrote a [blog post](https://josias.dev/posts/justwrite/).

Features:
- Distraction-free editor
- Full text search
- Notebooks (categories)

TODO:
- GTK GUI

Since this is a general way to let out thoughts and organize them, I think it would be useful to expand this from micro-journaling to noting general thoughts. Maybe other modes for notes and such. Its use of Markdown also enables more flexbility.

Principles:
- Make posting as simple as opening, writing, and then closing
- The tool should do any other work for the user, such as metadata, organization, etc.

## Installation

```sh
$ git clone https://git.josias.dev/jw && cd jw
$ make
$ ./jw help
```

## Similar Software

- [ThotKeeper](https://github.com/cmpilato/thotkeeper)
- [jrnl](https://github.com/jrnl-org/jrnl/)
- [jrny](https://git.sr.ht/~detondev/jrny)

## License

Copyright (C) 2020, Josias Allestad

You may use this software under the terms of the [BSD-2-Clause license](LICENSE).
