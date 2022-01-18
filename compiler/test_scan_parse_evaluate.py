from compiler_scan_parse_evaluate import Token, FunctionDefinition, VariableName
from compiler_scan_parse_evaluate import NAMESPACE
from compiler_scan_parse_evaluate import evaluate, parse, scan

import pytest

@pytest.mark.parametrize('text,tokens', [
  ('189', [Token('NUMBER', '189')]),
  ('8 + 19', [Token('NUMBER', '8'), Token('OPERATOR', '+'), Token('NUMBER', '19')]),
  ('(1 + 2)', [Token('LEFT_PAREN', '('), Token('NUMBER', '1'), Token('OPERATOR', '+'), Token('NUMBER', '2'), Token('RIGHT_PAREN', ')')]),
])
def test_scan(text, tokens):
    assert list(scan(text)) == tokens

@pytest.mark.parametrize('text,ast', [
  ('189', '189'),
  ('189 + 54', ('189', '+', '54')),
  ('1 + 1 * 3 +7', (('1', '+', ('1', '*', '3')), '+', '7')),
  ('189 + 54', ('189', '+', '54')),
  ('def square(x) x * x', FunctionDefinition('square', 'x', (VariableName('x'), '*', VariableName('x')))),
])
def test_parse(text, ast):
  assert parse(scan(text)) == ast

@pytest.mark.parametrize('text,value', [
    ('1234', 1234),
    ('1 + 7', 8),
    ('9 / 3', 3),
    ('1 * 2 * 3 * 4', 24),
    ('sin(0)', 0),
    ('fact(5)', 120),
    ('cos(sin(0)) + 18', 19)
])
def test_eval(text, value):
  assert evaluate(parse(scan(text))) == value

def test_function_definition():
  evaluate(parse(scan("def square(x) x * x")))
  print(f"NAMESPACE['square']: {NAMESPACE['square']}")
  print(f"Parsed AST after defining 'square' in earlier call: {parse(scan('square(5)'))}")
  assert evaluate(parse(scan("square(5)"))) == 25
