# dksh

👴 A simple Linux shell.

Created by : Mr Dk.

2020 / 01 / 22 @Ningbo, Zhejiang, China

---

## About

It is a simple Linux shell program.

It supports:

* execute simple command like `ls`
* change the working directory by `cd`
* redirecting - `<` / `>`
* recursive piping - `|`

---

## Implementation Details

### Lexer

Trying to recognize the input and split them into tokens.

For unrecognized tokens, the lexer will make the shell ignoring the whole command.

* try to recognize the command character like `|` `<` `>`

* try to parse the whole string surrounded by `""` `''`

### Fork

The `exec()` system call will replace the whole process image to execute another program. In order to maintain the main process of the shell, before calling `exec()`, we need to use `fork()` system call to copy another process to execute the command.

However, for `cd` command, it changes the working directory in the _PCB_ of the main process by calling `chdir()`. So it is not necessary to fork another process before calling `chdir()`.

### Redirecting

For commands need to be redirected, the shell will open a file descriptor for the redirecting target. Then, the program override the file descriptor `0` (`STDIN`) or `1` (`STDOUT`) with the opening file descriptor, by calling `dup2()` system call. As a result, the input or output of the program will be redirected.

### Pipe && Process Chain

Pipe is used to pipe the output of one process to the input of another process. 

The command before `|` character should be executed in a forked process. Before calling `fork()`, a pipe should be created using `pipe()` system call first. Then, after calling `fork()`, the child process overrides the `STDOUT` with output file descriptor of the pipe; the origin process overrides the `STDIN` with input file descriptor of the pipe. As a result, the output of the child process will be piped as the input of the origin process.

For commands using multiple pipes, a process chain will be created. The program should be implemented in a recursive way. Specifically, the command is parsed in an reverse order: commands in front of the `|` will be executed by a newly forked process.

For example:

```shell
$ cat <input.txt | sort | uniq | cat >output.txt
```

The main process will fork a process _p1_ to execute `cat >output.txt`; _p1_ will fork a _p2_ to execute `uniq` and wating for the result from _p2_ . Then _p2_ will fork _p3_ for `sort`, and _p3_ will fork _p4_ to execute `cat <input.txt`.

---

## License

```
MIT License

Copyright (c) 2019 Mr Dk.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

