# GSC Interpreter

This is an interpreter for a simple Turing-complete programming language (SC) with a syntax similar to C.
It was developed as part of the course Language and Compilers of the Computer Science degree at FAMAF, UNC.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Development Information](#development-information)
- [Examples](#examples)
- [Language Documentation](#language-documentation)
- [Next Steps](#next-steps)

## Installation

To use and run the interpreter, you only need to have g++ installed on your system.
The requirement is pretty simple, as g++ is the standard C++ compiler on most systems.

To work with GSC, you can clone the repository and compile it:

```bash
git clone https://github.com/helcsnewsxd/gsc-interpreter.git
cd gsc-interpreter
make build
```

## Usage

Before compiling, you have two options to use GSC: interactive or file mode.
For the first one, you can run the interpreter without any arguments:

```bash
./gsc
```

And for the second one, you can pass a file as an argument:

```bash
./gsc my_program.sc
```

## Development Information

There are some tests for each implemented module in the [`test`](./test) directory.
It was made with the use of Catch2, a C++ testing framework.
You can run them with the following command:

```bash
make test
```

Also, there're an automatic documentation generated with Doxygen for the project and it's published in the corresponding GitHub Pages site: [docs](https://helcsnewsxd.github.io/gsc-interpreter/).

## Examples

Some program examples in SC language are provided in the [`examples`](./examples) directory.
There are programs like fibonacci and MCD calculators, prime number checker, and more.

## Language Documentation

The grammar of SC is defined in the [`grammar`](./grammar.txt) file.
The idea of the language is similar to C but there're a little important things to say:

- The language is dynamically typed, so you don't need to declare the type of a variable. For now, there're only number (32 bit integer), boolean and string types.
- The division is integer division, so `5 / 2` is `2` and not `2.5`.
- Isn't necessary that a conditional statement or expression returns a boolean value, it can return any type. In SC, `nil`, `0`, `""` and `false` are considered false, while any other value is considered true.
- The logical operators are `and` and `or`, instead of `&&` and `||`. Also, it doesn't have type checking and doesn't guarantee that the result of a logical operation is a boolean value. For example, `"hi" or false` is `"hi"`.
- The dangling `else` it's solved with the "nearest" `if` statement (conventional in C-like languages).
- You can use `var` to re-define variables. Isn't a good practice, but it's allowed to do something like `var x = 2; var x = 3;`.

Now, the language is pretty simple but Turing-complete, so you can do anything with it.
There aren't functions or classes yet, like there aren't another value types as arrays or objects.

## Next Steps

Some features and things that will be nice to implement in the future are:

- Features support:
  - Functions
  - Lambda functions
  - Classes
  - Arrays
  - Numbers (large numbers made internally with strings that allows integers and floats)
  - User input
- Things to improve:
  - Possibility to create more complex strings as the f-strings in Python
  - Add a `println` operator to have the possibility to print with a newline and allow `print` to do without it
  - Error handling and user-friendly error messages to track better the places in the code where they occur
- Syntax sugar:
  - Simple assignments to add, substract, multiply and divide the variable with a value, like `x += 2`, `x -= 3`, `x *= 4` and `x /= 5`.
  - Add `else if` statements to have a more readable syntax.
  - Ternary operator for conditional expressions, like `x = (a > b) ? a : b`.
  - Add `do while` statement.
