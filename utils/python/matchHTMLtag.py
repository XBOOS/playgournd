#!/usr/bin/env python
# encoding: utf-8
from stack import ArrayStack
def isMatchedHTML(raw):
    # find "<" then find next ">", after that,  get the tag if starting with / then it is the ending tag, pop.otherwise, it is starting tag,push into stack
    S = ArrayStack()
    i = raw.find("<")# this will find the first "<"

    while i != -1:
        j = raw.find(">",i+1)
        if j == -1:
            return False
        tag = raw[i+1:j]
        if not tag.startswith("/"):
            S.push(tag)
        else:
            if S.is_empty():
                return False
            else:
                if S.pop()!= tag[1:]:
                    return False
        i = raw.find("<",j+1)
    return S.is_empty()

if __name__ == "__main__":
    raw = "<html></html>"
    print isMatchedHTML(raw),"== True"
    raw = "<tag></gls>"
    print isMatchedHTML(raw),"== False"











