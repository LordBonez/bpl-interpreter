# BPL Interpreter

A C++ recursive-descent parser and interpreter for a Basic Perl-Like language.
Original coursework: NJIT CS280, Fall 2025. Parser/evaluator implementations by Anthony Vorontsov.

## Features

- Variables, assignment and conditional statements.
- Numeric and string expressions, operator precedence and typed runtime values.
- Line-numbered syntax/runtime diagnostics.
- 19 original input/expected-output fixture pairs and a repeatable comparison runner.

## Build and run

Requirements: CMake 3.16+, a C++17 compiler, and Python 3 for the fixture runner.

```sh
cmake -S . -B build
cmake --build build --config Release
```

On Windows with Visual Studio:

```sh
build/Release/bpl-interpreter.exe tests/fixtures/testprog10
python tests/run_fixtures.py build/Release/bpl-interpreter.exe
ctest --test-dir build -C Release --output-on-failure
```

With a single-configuration generator on Linux/macOS, use `build/bpl-interpreter` instead.
Some fixtures intentionally contain invalid programs; their expected diagnostics are part of the test.
The runner ignores blank lines and trailing spaces, but compares all other output lines.
One course reference output has a documented arithmetic inconsistency; its original file is retained,
and a separate verified expectation is used for that case. See [the erratum](tests/REFERENCE_ERRATUM.md).
The runner reports both project expectation matches and unmodified course reference matches.

## Attribution and maintenance

This repository preserves the original course source headers and provided lexer/driver/header/test
scaffolding. The parser and value-operation source files identify Anthony's implementation.
Repository packaging, CMake configuration, and the fixture runner were added in October 2026.
The same maintenance pass fixes integer-remainder evaluation and one diagnostic punctuation mismatch,
with eight targeted value-operation checks.
See VALIDATION.md for the current build/test result and known limitations.
No blanket license is applied to instructor-provided material.
