#!/usr/bin/env python
# encoding: utf-8

from stack import ArrayStack

def isMatch(s):
    left = "{[("
    right = ")]}"
    S = ArrayStack()

    for c in s:
        if c in left:
            S.push(c)
        elif c in right:
            # what id didnt have before
            # should check empty first
            if S.is_empty():
                return False
            # if right.index(c) != left.index(S.pop())
            #      return False
            cc = S.pop()
            if not cc+c in ["{}","()","[]"]:
                return False
        else:
            raise Exception("Invalid string")
    if S.is_empty():
        return True
    else:
        return False
    #should simpty reutnr S.is_empty


if __name__=="__main__":
    print "Testing for matching strings"
    print isMatch("[{()}]") ,"== True"
    print isMatch("[[])){}"),"== False"

