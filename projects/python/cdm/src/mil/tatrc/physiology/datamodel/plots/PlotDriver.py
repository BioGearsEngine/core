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

    def processConfigFile(self, configFile,basedir):
        self.name = configFile.split(".")[0]
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

