use clap::Clap;
use std::io;
use std::io::prelude::*;

mod lib;

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

#[derive(Clap)]
struct Post {
    #[clap(about = "Title for the text to be posted (can be empty to use the first words of cog)")]
    title: Option<String>,
}

// Reads user input line by line until the line equals the escape string
fn line_by_line(escape: &str) -> Result<String, std::io::Error> {
    println!("Write here. When you're done, type {} and press return.\n", escape);

    let mut input: Vec<String> = Vec::new();

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
