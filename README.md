# codam-42sh [![codecov](https://codecov.io/gh/OscarMulder/codam-42sh/branch/master/graph/badge.svg?token=hg6wEBSVIc)](https://codecov.io/gh/OscarMulder/codam-42sh)

## What is VelaSHell
Velashell is a bash-styled shell school project completely written in C. We
worked on it with 5 people for a total duration of around 6 months. The goal was
to write a complete and solid UNIX shell.

## The goal of the project
\<insert content\>

## Project General Instructions
1. The shell can not leak in any way.
2. In no way can the shell quit in an unexpected manner (e.g. segfault).
3. We may only use the following libc functions, anything else is
   **prohibited**:
   1. the whole `man 2` section
   2. `malloc`, `free`, `opendir`, `readdir`, `opendir`, `getcwd`, `signal`,
      `exit`, `isatty`, `ttyname`, `ttyslot`, `getenv`, `tcsetattr`,
      `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto` and
      `tputs`.
   3. `termcaps`
4. The terminal must be clean and not display gibberish.

## Functionality
1. Error monitoring without using `errno`
2. Prompt display
3. (Multiline) line edition
4. Redirection with the following operators:
   1. `>`
   2. `>>`
   3. `<`
   4. `<<`
   5. `>&`
   6. `<&`
5. Control operators:
   1. `|`
   2. `||`
   3. `&`
   4. `&&`
   5. `;`
6. Internal shell variable monitoring
8. Job control monitoring (WIP)
9. Globing (WIP)
10. Signal monitoring
11. Quoting through `"`, `'`, and `\`
12. Basic parameter expansion
13. Complete management of command history (WIP)
14. Alias management
15. Hash table
16. Autocomplete
17. Builtins:
    1.  `echo`
    2.  `cd`
    3.  `fc`
    4.  `env`
    5.  `set`
    6.  `unset`
    7.  `exit`
    8.  `export`
    9.  `history`
    10. `alias`
    11. `unalias`
    12. `type`
    13. `hash`
18. A manual page called `vsh.1`

## The manual
The VSH manual contains a thorough description of our shell. We have the following sections:</br></br>
**NAME**</br>
**SYNOPSIS**</br>
**DESCRIPTION**</br>
**OPTIONS**</br>
**ARGUMENTS**</br>
**INVOCATION**</br>
**DEFINITIONS**</br>
**INTRODUCTION**</br>
**RESERVED WORDS**</br>
**SHELL GRAMMAR**</br>
**COMMENTS**</br>
**QUOTING**</br>
**PARAMETERS**</br>
**EXPANSION**</br>
**REDIRECTION**</br>
**ALIAS**</br>
**SIMPLE COMMAND EXPANSION**</br>
**COMMAND EXECUTION**</br>
**SHELL EXECUTION ENVIRONMENT**</br>
**ENVIRONMENT**</br>
**EXIT STATUS**</br>
**SIGNALS**</br>
**JOB CONTROL**</br>
**HISTORY**</br>
**HISTORY EXPANSION**</br>
**SHELL BUILTIN COMMANDS**</br>
**KNOWN BUGS**</br>
**AUTHOR**</br>

## Github workflow
Because we want to make sure we have a good experience working on this project together, we setup a more advanced github workflow. Please keep the following things in mind while pushing to github.
- Every feature has its own branch, which will be merged via a pull request on Github.
- The master branch is the stable version.
- We want to test as much of the code as possible, so make sure to write tests for the code you commit.
- Have atleast 2 team members review your code before merging to master.
- Commit messages have to be in English, they should also make sense. Especially while merging.

## Authors
[Jorn Brinksma](https://github.com/jbrinksma),
[Marijn van Heesewijk](https://github.com/marijnvanh),
[Oscar Mulder](https://github.com/OscarMulder),
[Rob Kuijper](https://github.com/robkuijper),
[Thijs de Jong](https://github.com/thijsdejong)