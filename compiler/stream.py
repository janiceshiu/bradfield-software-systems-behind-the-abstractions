class Stream:
  def __init__(self, data):
    self.data = list(data)
    self.position = 0

  def take(self):
    result = self.data[self.position]
    self.position += 1
    return result

  def peek(self):
    return self.data[self.position]

  def at_end(self):
    return self.position >= len(self.data)

  def backward(self):
    self.position -= 1
