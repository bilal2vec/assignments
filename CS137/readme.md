## gcc

- compile: `gcc main.c`
- make a .o file: `-c`
- generate debug symbols: `-g`
- warning messages: `-Wall`
- optimization level (ascending): `-01`
- link against a compiled file: `-o libone.o libtwo.o`

## gdb

- start: `gdb main.o`
- run with args: `(gdb) run 1 2 3`
- backtrace: `(gdb) backtrace`
- set breakpoint on line 3: `(gdb) break 3`

## clang

- compile: `clang main.c -o main.o`
- generate debug symbols: `-g`
- warning messages: `-Wall`
- optimization level (ascending): `-01`

## lldb

- start: `lldb main.o`
- run with args: `(lldb) run 1 2 3`
- set breakpoint on line 3: `(lldb) breakpoint set --line 3`
- see current variables: `(lldb) frame variable`


## make

- file name is `Makefile`
- format:
```Makefile
CC = gcc
CFLAGS = -g -Wall
main.o: main.c
    $(CC) $(CFLAGS) -c main.c
```
- call `make target` or just `make` to make all targets

## vscode

### tasks.json

```json
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "cppbuild",
			"label": "(clang) Compile",
			"command": "clang",
			"args": ["-g", "-Wall", "${file}", "-o", "${fileDirname}/${fileBasenameNoExtension}.o"],
			"options": {
				"cwd": "${workspaceFolder}"
			},
			"problemMatcher": ["$gcc"],
			"group": "build",
			"detail": "compiler: /usr/bin/clang"
		},
		{
			"type": "cppbuild",
			"label": "(gcc) Compile",
			"command": "gcc",
			"args": ["-g", "-Wall", "${file}", "-o", "${fileDirname}/${fileBasenameNoExtension}.o"],
			"options": {
				"cwd": "${workspaceFolder}"
			},
			"problemMatcher": ["$gcc"],
			"group": "build",
			"detail": "compiler: /usr/bin/gcc"
		}
	]
}
```

### launch.json

```json
{
	"version": "0.2.0",
	"configurations": [
		{
			"name": "(lldb) Launch",
			"type": "cppdbg",
			"request": "launch",
			"program": "${fileDirname}/${fileBasenameNoExtension}.o",
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false,
			"MIMode": "lldb",
			"preLaunchTask": "(clang) Compile"
		},
		{
			"name": "(gdb) Launch",
			"type": "cppdbg",
			"request": "launch",
			"program": "${fileDirname}/${fileBasenameNoExtension}.o",
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false,
			"MIMode": "gdb",
			"preLaunchTask": "(gdb) Compile"
		}
	]
}
```