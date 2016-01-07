#!/usr/bin/env python
# encoding: utf-8
class LinkedStack:

    class _Node:
        __slot__ = '_element','_next'
        def __init__(self,element,next):
            self.element = element
            self.next = next

    def __init__(self):
        """ Create an empty stack"""
        self._head = None
        self._size = 0
    def __len__(self):
        return self._size
    def is_empty(self):
        return self._size == 0

    def top(self):
        if self.is_empty():
            raise Exception("Stack is empty")
        else:
            return self.head.element
    def push(self,e):
        newNode = self._Node(e,self.head)
        self.head = newNode
        #i can do self.head = self._Node(e,self.head)
        self.size +=1

    def pop(self):
        if self.is_empty():
            raise Exception("Stack is empty")
        tmp = self.head.element
        self.head = self.head.next
        self.size -=1
        return tmp

