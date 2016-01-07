#!/usr/bin/env python
# encoding: utf-8
from stack import ArrayStack
if __name__ == "__main__":

    def reverse_file(filename):
            S = ArrayStack()
            original = open(filename)
            for line in original:
                S.push(line.rstrip("\n"))
            original.close()


            output = open(filename,'w')
            while not S.is_empty():
                output.write(S.pop()+"\n")
            output.close()

    reverse_file("test.txt")


