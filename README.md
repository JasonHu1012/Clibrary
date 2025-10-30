## Clibrary

This is a library containing various sublibraries for C, ranging from basic data structures to high-level utilities.

## Directory Layout

- include: header files
- lib: static libraries
- test: test codes
- src: implementation
- obj: object files
- bin: test programs

## Usage

### Compile

To compile all libraries, run:

```bash
make all
```

To compile some library (**stack** for example), run:

```bash
make lib/libstack.a
```

### Test

To perform all tests, run:

```bash
make test
```

To perform test for some library (**stack** for example), run:

```bash
make stack.test
```

### Clean

To clean up all compiled files, run:

```bash
make clean
```

### Link

For most libraries (**stack** for example), you only need to:

```bash
gcc main.c lib/libstack.a
```

But some libraries (**vector** for example) have dependencies, and you have to append more flag:

```bash
gcc main.c lib/libvector.a -lm
```

For more details, check the makefile to see how the test programs are compiled.
