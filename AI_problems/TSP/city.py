#!/usr/bin/env python
# encoding: utf-8

import random as rd
import math
class City:
    def __init__(self,x_coord=None,y_coord=None):
        #generation of the city coordinates
        if not x_coord:
            self.__x =rd.randrange(200)
        else:
            self.__x = x_coord
        if not y_coord:
            self.__y = rd.randrange(200)
        else:
            self.__y = y_coord

    def getX(self):
        return self.__x
    def getY(self):
        return self.__y

    def distanceTo(self,city):
        xDistance = abs(self.getX()-city.getX())
        yDistance = abs(self.getY()-city.getY())

        distance = math.sqrt(xDistance**2+yDistance**2)
        return distance

    def __str__(self):
        return self.__x," , ",self.__y


