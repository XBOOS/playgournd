#!/usr/bin/env python
# encoding: utf-8

class TourManager:
    destinationCities = []
    def __self__(self):
        return
    @staticmethod
    def addCity(city):
        TourManager.destinationCities.append(city)

    @staticmethod
    def getCity(index):
        if index<0 or index>=len(TourManager.destinationCities):
            raise Exception("Indes for destinationCities out of bound")
        return TourManager.destinationCities[index]

    @staticmethod
    def numberOfCities():
        return len(TourManager.destinationCities)
