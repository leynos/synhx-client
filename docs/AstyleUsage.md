# Formatting Source with astyle

This project uses [Artistic Style](https://astyle.sourceforge.net/) to keep the C
code consistently formatted. The configuration file `.astylerc` at the repository
root defines the style, which approximates LLVM guidelines using two-space
indents and the OTBS brace style.

### Checking Formatting
Run Artistic Style in *dry run* mode to see what changes would be made without
modifying any files:

```sh
astyle --options=.astylerc --dry-run <file1> [file2 ...]
```

### Reformatting Files
To apply the style to specific files, omit the `--dry-run` option:

```sh
astyle --options=.astylerc <file1> [file2 ...]
```

The configuration is designed so that comments, preprocessor directives and other
non-indentation aspects of the code remain untouched.
