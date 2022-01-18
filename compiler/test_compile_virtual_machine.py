from compiler_virtual_machine import Machine
from compiler_virtual_machine import compile_to_bytecode

import pytest
@pytest.mark.parametrize('program,result', [
    ([1, 5, '+'], 6),
    ([5, 3, '-'], 2),
    ([10, 5, '+', 3, '-'], 12),
    ([2, 5, '-', 5, '*', 3, '/'], -5),
])

def test_addition(program, result):
    vm = Machine()
    assert vm.run(program) == result

@pytest.mark.parametrize('ast,bytecode_list', [
    ('189', [189]),
    (('189', '+', '54'), [189, 54, '+']),
    ((('90', '+', ('1', '*', '3')), '+', '7'), [90, 1, 3, '*', '+', 7, '+']),
])

def test_bytecode_compiler(ast, bytecode_list):
  assert compile_to_bytecode(ast) == bytecode_list
