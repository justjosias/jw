//! Some miscellaneous utility functions

pub fn strip_chars(original: &str, chars: &str) -> String {
    original.chars().filter(|&c| !chars.contains(c)).collect()
}

/*impl String {
    pub fn strip_chars(&self, chars: &str) -> String {
	original.chars().filter(|&c| !chars.contains(c)).collect()
    }
}*/

#[test]
fn test_strip_chars() {
    assert_eq!(strip_chars("Hello! How are you doing today", " ?"), "Hello!Howareyoudoingtoday");
}
