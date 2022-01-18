# Implementing a simple calculator language
This is the coding homework for the following 3 classes:
* Overview of languages and compilers
* Language constructs under the hood
* Runtime features and bytecode interpreters

## Scanner, Parser, and Evaluator
These three functions support the following tokens:

* Integers of arbitrary length
  * Eg: 0, 1, 234, 9833
* Operators `+, -, *, /`.
  * Eg: `234+90`, `120/2+3`
* Parentheses for precedence
  * Eg: `120/(2+3)`
* Some math functions that are built-in into the calculator language
  * Eg: `2 * pi`, `cos(0)`, `sqrt(5)`, `exp(0) * 10`, `fact(5) + 3`
* Single-line user-defined functions that use tokens supported above
  * Eg: `def cube(x) sqrt(x) * x`

### Scanner
* `scan/1` takes a string of characters, turns them into tokens, and adds them to a Python generator

### Parser
* `parse/1` takes a generator of tokens and parses them into an AST in the form of a tuple

#### Grammar
* statement = function_definition | add_sub
* function_definition = def name '(' name ')' add_sub
* add_sub = mul_div ZERO_OR_MORE( ('+' | '-') mul_div )
* mul_div = operand ZERO_OR_MORE(('*' | '/') operand)
* operand = number | '(' add_sub ')' | name(add_sub)

Defining the grammar in this order allows us to support the usual mathematical precedence.
  * Parentheses take precedence over multiply and divide operators.
  * Multiply and divide operators take precedence over addition and subtraction operators.

All functions for the grammar are in the format `parse_grammar_name/1`
  * Eg: `statement` parsing is implemented in `parse_statement/1`

### Evaluator
* `eval/1` takes an AST in the form of a tuple and evaluates it

---

## Compile-to-bytecode and stack-based virtual machine
These two functions support the following tokens:
* Integers of arbitrary length
  * Eg: 0, 1, 234, 9833
* Operators `+, -, *, /`.
  * Eg: `234+90`, `120/2+3`

### Compile-to-bytecode
* `compile_to_bytecode/1`takes an AST from `parse/1` in `compiler.py` and returns a list of 'bytecode' that the virtual machine can process
  one instruction at a time using a stack

### Stack-based virtual machine
* `run/1` takes a list of instructions and evaluates it

---

## Setup
* Requires at least Python 3.10 due to the `match` operator

## Repl
* Run `python filename.py` to enter an interactive repl

## Testing
* Make sure `pytest` is installed
* `pytest` runs all test files
