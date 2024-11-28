# rcc
A C Compiler Helper for Linux

## Usage

Using `rcc` just like `gcc` to compile your single C file

```bash
rcc example.c
```

By default, The rcc will compile your C file and create the executable file, then run the executable file and delete it.

Obviously, you can also change your settings in `~/.rccrc`  which is the config file.

## Settings

The RCC have 2 configure options:
- Compile parameters
- Whether keep the executable file

You can change the above options with two variables:
- `GCC_OPTIONS=`
- `KEEP_OUTPUT=`

For example: 
```bash
  GCC_OPTIONS=-Wall -Wextra -Ofast
  KEEP_OUTPUT=1
```

The executable file will be kept when the `KEEP_OUTPUT` value is `1`.

> The rcc usage only recommanded when you compile single C file, such as ACM/ICPC Contests or thinking tests.