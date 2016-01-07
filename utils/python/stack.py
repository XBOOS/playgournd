#!/usr/bin/env python
# encoding: utf-8

class Empty(Exception):
    pass
class ArrayStack():
    def __init__(self):
        self.L = list()
    def __len__(self):
        return len(self.L)
    def is_empty(self):
        return len(self.L)==0
    def top(self):
        if self.is_empty():
            raise Empty("Stack is empty")
        else:
            return self.L[-1]

    def push(self,s):
        self.L.append(s)
    def pop(self):
        if self.is_empty():
            raise Empty("Stack is empty")
        else:
            return self.L.pop()
