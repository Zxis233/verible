---
---

# Verible

The Verible project's main mission is to parse SystemVerilog (IEEE 1800-2017)
for a wide variety of applications.

It was born out of a need to parse *un-preprocessed* source files, which is
suitable for single-file applications like style-linting and formatting. In
doing so, it can be adapted to parse *preprocessed* source files, which is what
real compilers and toolchains require.

The spirit of the project is that no-one should ever have to develop a
SystemVerilog parser for their own application, because developing a
standard-compliant parser is an enormous task due to the syntactic complexity of
the language.

A lesser (but notable) objective is that the language-agnostic components of
Verible be usable for rapidly developing language support tools for other
languages.

If you'd like to contribute, check out the [contributing](./CONTRIBUTING.md)
guide and the [development resources](./doc/development.md).


See the [README file for further information.](README.md)

## Tools

 * [verible-verilog-lint Info](verilog_lint.md)
 * [verible-verilog-format Info](verilog_format.md)
 * [verible-verilog-syntax Info](verilog_syntax.md)

## Information

 * [Code - https://github.com/google/verible](https://github.com/google/verible)
 * [Binaries - https://github.com/google/verible/releases](https://github.com/google/verible/releases)
 * [Bug Reports - https://github.com/google/verible/issues/new](https://github.com/google/verible/issues/new)
 * [Lint Rules](lint.md)
 * [Further Information](README.md)

## Authors

 * [Contributing](CONTRIBUTING.md)
 * [Authors](AUTHORS.md)
 * [License Information](license.md)

## Version

Generated on 2020-09-21 09:44:52 -0700 from [v0.0-614-gd3661b5](https://github.com/google/verible/commit/d3661b5a979a9da113b5ace533e25dffb9cd9a0e)