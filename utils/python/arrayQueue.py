#!/usr/bin/env python
# encoding: utf-8

class Empty(Exception):
    pass

class ArrayQueue():
    # using an fixed capacity array
    DEFAULT_CAPACITY = 10

    def __init__(self):
        self.Q = [None]*ArrayQueue.DEFAULT_CAPACITY
        self.size = 0
        self.front = 0
        self.capacity = ArrayQueue.DEFAULT_CAPACITY

    def __len__(self):
        return len(self.Q)

    def is_empty(self):
        return len(self.Q) == 0

    def first(self):
        if self.Q.is_empty():
            raise Empty("Queue is empty")
        else:
            return self.Q[self.front]

    def enqueue(self,e):
        if len(self.Q) == self.capacity:
            #raise Exception("Run out of capacity")
            self.resize(2*self.capacity)
        else:
            self.Q[(self.front+len(self.Q)) % self.capacity] = e
            self.size +=1

    def dequeue(self):
        if self.is_empty():
            raise Empty("Queue is empty")
        else:
            self.front = (self.front+1)%self.capacity
            return self.Q[(self.front-1)% self.capacity]
            self.size -=1

    def resize(self,cap):
        oldQ = self.Q
        self.Q= [None]*cap
        walk  = self.front
        for k in range(self.size):
            self.Q[k] = oldQ[walk]
            walk = (1+walk)%self.capacity
        self.front = 0 # almost forget to reset the front
        self.capacity = cap

    def shrink(self) :
        """
        also the shrinking property, when the length of the array becomes one forth, shrink it by half
        """
        if 0<self.size< self.capacity//4:
            self._resize(self.capacity//2)
