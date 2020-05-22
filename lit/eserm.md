---
title: ESerm
author: Josias Allestad
---

## Library

The library does the actual heavy-lifting, and allows its use in other programs (maybe web someday?).

``` {.rust file=src/lib.rs}

pub fn get_path() -> String {
    
}
```

## CLI

This is the basic CLI we need for putting input for the rest of the program.

``` {.rust file=src/main.rs}
<<includes>>
<<cli-config>>
<<main-function>>
```

First we include the [clap](https://github.com/clap-rs/clap) crate for managing the CLI arguments.

``` {.rust #includes}
use clap::Clap;
```

Now for the actual CLI configuration:

``` {.rust #cli-config}
#[derive(Clap)]
#[clap(version = "0.1", author = "Josias <justjosias@tutanota.com>")]
struct Opts {
    #[clap(subcommand)]
    subcmd: SubCommand,
}

#[derive(Clap)]
enum SubCommand{
    Post(Post),
}
```

The `Post` structure contains information for the `post` subcommand. `text` is an optional string that contains the CLI input of the user. For example, when `post test` is given, `post.text` will be `Some(text)`.

``` {.rust #cli-config}
#[derive(Clap)]
struct Post {
    #[clap(help = "Text to be posted (can be empty to open text editor)")]
    text: Option<String>,
}
```

Now we have the main function, which manages the arguments and passes them along to the various parts of the library.

``` {.rust #main-function}
fn main() {
    let opts: Opts = Opts::parse();

    match opts.subcmd {
        SubCommand::Post(p) => {
            match p.text {
                Some(text) => println!("{}", text),
                None => println!("No text!"),
            }
        }
    }
}
```
