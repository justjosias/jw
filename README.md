# jw

Just Write.

A micro-journaling tool written in C.

Many journaling programs are better-designed for writing long entries. Of course, they do not force you to do so, but they certainly make it preferable to do it that way.

This is different. This is a tool for writing out your thoughts Twitter-style, with a CLI for managing, writing, and editing posts.

Writing out thoughts quickly and distraction-free is what this tool is about. I am writing this for myself to be able to keep track of my thoughts.

To outline the philosophy of the project, I wrote a [blog post](https://josias.dev/posts/justwrite/).

Features:
- Uses your favorite editor
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

Depends on `libbsd`.

## Usage

First, you must make a new notebook. A notebook is a collection of posts and jw is built around this concept.

Here we make a notebook called "journal", but it can be named anything.

```
$ jw new journal
```

When you have a notebook, you can write a post with `jw post journal`. That opens up your default editor (or else vi) in which you can write a post. When you are done, save and quit. jw handles the rest.

You can run a full-text search on your posts by running `jw search journal <query>`. It currently returns a list of paths to posts where the query was found.

## Configuration

The program can be configured by modifying `~/.config/jw/config.yaml`.

```yaml
root: jw
editor: vi
```

The above example configuration sets the root directory (where all jw data is stored) to `~/jw`, and sets the default editor (overriding `$EDITOR`) to `vi`.

This is the default configuration if nothing is changed.

## Notebooks

"Notebooks" are collections of posts stored in the `root` directory (as defined in the global configuration). They can vary in purpose and are configured separately.

Most of the time making a new notebook is as simple as `jw new <notebook_name>`.

Each notebook can be configured with a `notebook.yaml` in the corresponding directory.

An example `notebook.yaml`:
```yaml
name: journal # Default: the name of the directory
description: General notes and random thoughts
file_path: %Y-%m-%d-%s.md # other examples: %Y-%m-%d-%s.md, or even posts.md, where all entries are stored in a single file. %s refers to the first letters of the post
metadata: false # put some metadata (like the timestamp) at the beginnning of each post
```

By default notebooks are given a basic configuration when made with `jw new`.

## Similar Software

- [ThotKeeper](https://github.com/cmpilato/thotkeeper)
- [jrnl](https://github.com/jrnl-org/jrnl/)
- [jrny](https://git.sr.ht/~detondev/jrny)
- [nb](https://github.com/xwmx/nb)

## License

Copyright (C) 2020, 2021, 2022 Josias Allestad

You may use this software under the terms of the [BSD-2-Clause license](LICENSE).
