---
title: ESerm
author: Josias Allestad
---

The program is divided into several sections:
- Library (the backend that does pretty much everything)
  - Reading
  - Writing
  - Searching
- CLI

## Library

The library does the actual heavy-lifting, and allows its use in other programs (maybe web someday?).

``` {.rust file=src/lib.rs}
use chrono::{DateTime, Utc};
use dirs;
use std::fs;
use std::path::PathBuf;

/// DIRNAME contains the name of the cogs directory relative to the user's home directory
pub const DIRNAME: &str = "cogs";

/// get_path returns the path of a new cog in the form of 2020-01/01-[text].md
pub fn get_path(title: &str) -> String {
    let now: DateTime<Utc> = Utc::now();
    now.format(&format!("%Y-%m/%d-{}.md", title)).to_string()
}

/// get_full_path returns the full path of a cog by prepending the home directory and cog directory to a subpath
pub fn get_full_path(subpath: &str) -> PathBuf {
    let mut path = PathBuf::new();
    let home = dirs::home_dir().unwrap();
    path.push(home);
    path.push(DIRNAME);
    path.push(&subpath);

    path
}

/// get_full_dir returns the directory for a new cog
pub fn get_full_dir() -> PathBuf {
    let mut dir = get_dir();
    let now: DateTime<Utc> = Utc::now();
    dir.push(now.format("%Y-%m").to_string());
    PathBuf::from(dir)
}

/// get_dir returns the path of the cogs directory
pub fn get_dir() -> PathBuf {
    let mut path = PathBuf::new();
    let home = dirs::home_dir().unwrap(); // unwrap is okay here since we can't proceed if we don't have home

    path.push(home);
    path.push(DIRNAME);

    path
}

pub fn write_file(text: &str, title: Option<&str>) -> Result<(), std::io::Error> {
    // If the title is None, use the first three words of the text as a title
    let title: String = match title {
        Some(title) => title.to_string().split_whitespace().collect::<Vec<&str>>().join("-"),
        None => {
            let text = text.split_whitespace().collect::<Vec<&str>>();
            if text.len() > 3 {
                text[0..2].join("-")
            } else if text.len() > 0 {
                text[0].to_string()
            } else {
                return Ok(());
            }
        },
    };
    let path = get_full_dir();

    fs::create_dir_all(path)?;
    let mut filepath = get_full_path(&get_path(&title));

    //if filepath.exists() {
    //    filepath.push("(2)");
    //}

    fs::write(filepath, text)?;

    Ok(())
}

<<lib-tests>>
```

Now the tests:

``` {.rust #lib-tests}
#[cfg(test)]
mod tests {
    use super::*;

    use chrono::{DateTime, Utc};

    #[test]
    fn test_get_path() {
        let path = get_path("test");
        let now: DateTime<Utc> = Utc::now();
        assert_eq!(path, now.format("%Y-%m/%d-test.md").to_string());
    }

    #[test]
    fn test_get_full_path() {
        let path = get_full_path("2020/05/20-hi.md");
        println!("{:?}", path);
    }

    #[test]
    fn test_get_full_dir() {
        let dir = get_full_dir();
        println!("{:?}", dir);
    }

    #[test]
    fn test_write_file() {
        write_file(" ", Some("hi")).unwrap();
    }
}
```

## CLI

This is the basic CLI we need for putting input for the rest of the program.

``` {.rust file=src/main.rs}
<<cli-includes>>
<<cli-config>>
<<main-function>>
```

First we include the [clap](https://github.com/clap-rs/clap) crate for managing the CLI arguments.

``` {.rust #cli-includes}
use clap::Clap;
use std::io;
use std::io::prelude::*;
```

Other things we need in the program:

``` {.rust #cli-includes}
mod lib;
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
    #[clap(about = "Title for the text to be posted (can be empty to use the first words of cog)")]
    title: Option<String>,
}
```

Now we have the main function, which manages the arguments and passes them along to the various parts of the library.

``` {.rust #main-function}
// Reads user input line by line until the line equals the escape string
fn line_by_line(escape: &str) -> Result<String, std::io::Error> {
    println!("Write here. When you're done, type {} and press return.\n", escape);

    let mut input: Vec<String> = Vec::new();
    //let mut input = String::new();

    let stdin = io::stdin();
    for line in stdin.lock().lines() {
        let line = line?;
        if line == escape {
            break;
        }
        input.push(line);
    }

    input.push("".to_string()); // push this so that an extra newline is inserted

    Ok(input.join("\n"))
}

/*
#[test]
fn test_line_by_line() {
    let lines = line_by_line("DONE").unwrap();
    println!("{}", lines);
    assert_eq!(lines, "hi");
}*/

fn main() {
    let opts: Opts = Opts::parse();

    match opts.subcmd {
        SubCommand::Post(p) => {
            match p.title {
                Some(title) => lib::write_file(&line_by_line("DONE").unwrap(), Some(&title)),
                None => lib::write_file(&line_by_line("DONE").unwrap(), None),
            }.unwrap();
        }
    }
}
```
