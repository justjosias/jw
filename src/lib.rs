use chrono::{DateTime, Utc};
use dirs;
use std::fs;
use std::path::{Path, PathBuf};

mod utils;

pub const DIRNAME: &str = "cogs";

pub struct Cog<'a> {
    date: DateTime<Utc>,
    title: Option<&'a str>,
    categories: Option<Vec<&'a str>>,
    text: &'a str,
}

impl Cog<'static> {
    pub fn filename(self: &Self) -> String {
	// Gets the title but formatted with dashes and strips invalid characters
	// so "Hello world" becomes "hello-world"
	// TODO: find a better way to deal with two-or-less-word cogs
	let illegal_chars = "?!%\"\',.\\/*[]:;|";
	let to_replace = " ";
	let file_title = if let Some(title) = self.title {
            utils::strip_chars(title, illegal_chars)
		.to_lowercase()
		.split_whitespace()
		.collect::<Vec<&str>>()
		.join("-")
	} else {
            let text = self.text.split_whitespace().collect::<Vec<&str>>();
            if text.len() >= 3 {
		text[0..3].join("-")
            } else if text.len() >= 1 {
		text[0].to_string() //.split(" ").collect::<Vec<&str>>().join("-")
            } else {
		"".to_string()
            }
	};

	file_title
    }
}

#[test]
fn test_filename() {
    let mut test_cog: Cog = Cog {
        date: Utc::now(),
	categories: Some(vec!["tech", "notes"]),
        title: Some("a post Title"),
        text: "Hey there. How are you doing?",
    };
    assert_eq!(test_cog.filename(), "a-post-title");
}

pub fn get_path(title: &str) -> String {
    let now: DateTime<Utc> = Utc::now();
    now.format(&format!("%Y-%m/%d-{}.md", title)).to_string()
}

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
    let file_title = if let Some(title) = title {
        title
            .to_string()
            .split_whitespace()
            .collect::<Vec<&str>>()
            .join("-")
    } else {
        let text = text.split_whitespace().collect::<Vec<&str>>();
        if text.len() >= 3 {
            text[0..3].join("-")
        } else if text.len() > 0 {
            text[0].to_string()
        } else {
            "".to_string()
        }
    };

    // Get required paths and initialize files
    let path = get_full_dir();
    fs::create_dir_all(path)?;
    let filepath = get_full_path(&get_path(&file_title));

    // if filepath.exists() {
    //    filepath.push("(2)");
    // }

    // Get some metadata
    let mut metadata = String::new();
    metadata.push_str("---\n");
    if let Some(title) = title {
        metadata.push_str("title: ");
        metadata.push_str(title);
        metadata.push_str("\n");
    }
    metadata.push_str("date: ");
    let now: DateTime<Utc> = Utc::now();
    metadata.push_str(&now.to_string());
    metadata.push_str("\n---\n");

    let mut newtext = String::new();
    newtext.push_str(&metadata);
    newtext.push_str(text);

    fs::write(filepath, newtext)?;

    Ok(())
}

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
