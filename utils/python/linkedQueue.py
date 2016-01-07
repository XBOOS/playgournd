#!/usr/bin/env python
# encoding: utf-8

class LinkedQueue:
    class _Node:
        __slot__ = '_element','_next'
        def __init__(self,element,next):
            self.element = element
            self.next = next
    def __init__(self):
        self.head = None
        self.tail = None
        self.size = 0
    def __len__(self):
        return self.size

    def is_empty(self):
        return self.size == 0

    def first(self):
        if self.is_empty():
            raise Exception("Queue is empty")
        else:
            return self.head.element
    def enqueue(self,e):
        newNode = self._Node(e,None)
        if self.is_empty():
            self.head = newNode
        else:
            self.tail.next = newNode
        self.tail = newNode
        self.size +=1

    def dequeue(self):
        if self.is_empty():
            raise Exception("Queue is empty")
        else:
            self.size -=1
            tmp = self.head.element
            self.head = self.head.next
        if self.is_empty():
            self.tail = None
        return tmp




