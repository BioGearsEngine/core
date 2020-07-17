#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Created on Wed July 15 10:30 2020

@author: Shashank

"""

import os
import argparse
import logging
logging.basicConfig(level = logging.INFO)

class PlotDriver():
    def __init__(self):
        self.name = None
        self.jobs = list()
        self.preload = False
        self.onlyPlotFailures = False
        self.computedFilePath = None
        self.expectedFilePath = None
        self.abbreviateContents = 0
        self.isScenario = False

    @classmethod
    def main(cls,args):
        me=PlotDriver()
        # invalid input
        if len(args)<3:
            logging.error("No files specified")
            return
        elif len(args)==3:
            if args[2].split(".")[1]!="config":
                logging.error("Value " + args[2] + " doesn't seem to be a valid config file")
                return
            if not os.path.isfile(args[2]):
                logging.error("ConfigFile " + args[2] + " not found")
                return
            me.processConfigFile(args[2],args[1])
        me.execute()

    class PlotJob():
        def __init__(self):
            self.name = None
            self.headers = list()
            self.plotter = None
            self.titleOverride = None
            self.ignore = False
            self.showGridLines = True
            self.logAxis = False
            self.forceZeroYAxisBound = False
            self.removeAllLegends = False
            self.resultsSkipNum = 0
            self.plotname=None
            self.dataFile = None
            self.dataPath = None
            self.logFile = None
            self.logPath = None
            self.scenarioPath = None
            self.scenarioFile = None
            self.verificationDirectory = "Patient"
            self.X1Label = None
            self.Y1Label = None
            self.experimentalData = None
            self.outputDir = "Plots/"
            self.imageWidth = None
            self.imageHeight = None
            self.fontSize = 22
            self.outputFilename = None
            self.skipAllActions = False
            self.skipAllEvents = False
            self.hideAELegend = False
            self.legendOnly = False
            self.eventOmissions = list()
            self.actionOmissions = list()
            self.isComparePlot = False
            self.computedDataPath = None
            self.computedDataFile = None
            self.Y1headers = list()
            self.Y2headers = list()
            self.X1header = None
            self.X2header = None
            self.X1LowerBound = None
            self.X2LowerBound = None
            self.X1UpperBound = None
            self.X2UpperBound = None
            self.Y1LowerBound = None
            self.Y2LowerBound = None
            self.Y1UpperBound = None
            self.Y2UpperBound = None
            self.X2Label = None
            self.Y2Label = None
            self.PFTFile = None
            self.basedir= None

        def Reset(self):
            """ generated source for method Reset """
            self.headers = None
            self.eventOmissions = None
            self.actionOmissions = None
            self.compareData = None
            self.Y1headers = None
            self.Y2headers = None

    def processConfigFile(self, configFile,basedir):
        self.name = configFile.split(".")[0]
        currentGroup = self.name
        try:
            with open(configFile,"r") as fin:
                for line in fin:
                    if len(line)==0 or line.startswith("#"):
                        continue 
                    if line.startswith("@group"):
                        currentGroup = line.partition("@group")[2].strip()
                    if len(currentGroup)==0:
                        currentGroup = self.name
                        continue
                    if not "=" in line:
                        continue 
                    line = line.strip()
                    key = line.split("=")[0]
                    value = line.split("=",1)[1].strip()
                    if key.lower() == "Plotter".lower():
                        continue
                    #print(value)
                    job = self.PlotJob()
                    if key[0] == '-':
                        job.ignore = True
                        key = key[1:]
                    self.jobs.append(job)
                    job.name = key.strip()
                    directives = value.strip().split(" ")
		    pass

    def execute(self):
        pass


if __name__ == '__main__':
    import sys
    parser = argparse.ArgumentParser(description='Creation of Plots')
    parser.add_argument('-p','--configpath',help='Path to Config file',required=True)
    parser.add_argument('-b','--baseline',help='Baseline Dir path',required=True)
    args=parser.parse_args()
    if args.configpath==None and args.baseline==None:
        parser.print_help()
        sys.exit(0)
    else:
        argv=["PlotDriver.py",args.baseline,args.configpath]
        PlotDriver.main(argv)

