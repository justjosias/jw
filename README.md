# ESerm

A micro-journaling tool written in C.

Many journaling programs are better-designed for writing long entries. Of course, they do not force you to do so, but they certainly make it preferable to do it that way.

This is different. This is a tool for writing out your thoughts Twitter-style, with a CLI for managing, writing, and editing posts.

Writing out quick thoughts in quick post-format is what this tool is about. I am writing this for myself to be able to keep track of my thoughts in the past, as it can be quite helpful in the future in so many ways. 

This is currently under development, so there are no promises to its completeless or stability at the moment.

Future features:

- Full text search
- Categories and hashtags
- Encryption

TODO:
- Use system text editor instead of `get_text`

As this is a general way to let out thoughts and organize them, I think it would be useful to expand this from micro-journaling to noting general thoughts. Maybe other modes for notes and such. And as it uses Markdown, it can be very flexible.

## Word meanings

I chose specific meanings for different words used throughout this program.

First of all, the name. ESerm is short for Ego Sermo, which is a combination of the Latin words for "self" and "conversation."

Next, we don't have tweets, toots, or even posts. Here thoughts you save are called "cogs", which is short for "cogitatio", "cogitamentum", or "cogitatus" (take your pick). It can also refer to "cogs" turning in your mind.

Everything else is in plain old English.

## Installation

```
$ git clone https://codeberg.org/josias/ESerm && cd ESerm
$ make
$ ./eserm help
```

## License

Copyright (C) 2020, Josias Allestad
You may use this software under the terms of the [BSD-2-Clause license](LICENSE).
