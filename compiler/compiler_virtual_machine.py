import operator
from typing import List

### Compile_to_bytecode start

OPERATORS = {
  '+': operator.add,
  '-': operator.sub,
  '*': operator.mul,
  '/': operator.truediv,
}

def compile_to_bytecode(ast) -> List:
  """
  Takes an AST from `parse/1` in `compiler.py`.
  Returns a list of 'bytecode' that the virtual machine can process
  one instruction at a time using a stack
  """
  bytecode_list = []

  match ast:
    case str(_):
      bytecode_list.append(int(ast))
    case (left, operator, right):
      # use `extend`` instead of `append` cos compile always returns a list
      bytecode_list.extend(compile_to_bytecode(left))
      bytecode_list.extend(compile_to_bytecode(right))
      bytecode_list.append(operator) # eg: '+' is already a string

  return bytecode_list

### Virtual Machine Start

class Machine:
  def __init__(self) -> None:
    self.stack = []

  def run(self, program) -> int:
    for instruction in program:
      if isinstance(instruction, int):
        self.stack.append(instruction)
      elif instruction in OPERATORS:
        # this is so something like [5,3,'-'] is evaluated as
        # operator.sub(5,3) == 2 instead of the other way around
        second_argument = self.stack.pop()
        first_argument = self.stack.pop()
        result = OPERATORS[instruction](first_argument, second_argument)
        self.stack.append(result)
      else:
        raise ValueError(f"Unsupported instruction: {instruction}")

    # by the end, you should only have 1 item on the stack, which is the final result
    # pop that
    assert len(self.stack) == 1
    return self.stack.pop()

### Repl start
if __name__ == '__main__':
    from compiler_scan_parse_evaluate import scan, parse
    machine = Machine()
    while True:
        text = input('VM > ')
        parsed = parse(scan(text))
        print(f"AST from parser:\n{parsed}\n")
        bytecode_program = compile_to_bytecode(parsed)
        print(f"Bytecode program from compiler:\n{bytecode_program}\n")
        result = machine.run(bytecode_program)
        print(f"Result from stack virtual machine: {result}")
