#!/usr/bin/env python
# encoding: utf-8

from tourManager import TourManager
class Tour:

    def __init__(self,tour=None):
        if not tour:
            self.tour = tour

        self.tour = []
        self.distance = 0
        for i in range(TourManager.numberOfCities()):
            self.tour.append(None)


        def getTour(self):
            return self.tour
        def generateIndividual(self):




