#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Created on Tue July 21 10:50 2020

@author: Shashank

"""

import zipfile
from matplotlib.ticker import FormatStrFormatter
import matplotlib.ticker as tick
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import logging
import matplotlib.pyplot as plt
from matplotlib.ticker import LinearLocator,MaxNLocator
from matplotlib.offsetbox import TextArea, VPacker, AnnotationBbox,HPacker,PackerBase,PaddedBox

logging.basicConfig(level = logging.INFO)
logging = logging.getLogger("ActionEventPlotter")

class ActionEventPlotter():
    def __init__(self):
        self.events = []
        self.data = []
        self.timeData = []
        self.actions = []
        
    def plot(self, listener):
        # fill PlotJob with needed data if it doesn't exist
        job = listener
        # Plotter will look for the files it needs relative to the work directory
        # If this fails it will fall back to a baseline location if one was 
        # Provided to cmake at the time this file was generated
        if job.dataPath == None :
            job.dataPath = "Scenarios/" + job.verificationDirectory + "/baselines/"
        if not os.path.exists(job.dataPath):
            job.dataPath = os.path.join(job.basedir,job.dataPath)
        if job.logPath == None:
            job.logPath = "Scenarios/" + job.verificationDirectory + "/baselines/"
        if not os.path.exists(job.logPath):
            job.logPath = os.path.join(job.basedir, job.logPath)
        if job.scenarioPath == None:
            job.scenarioPath = "Scenarios/" + job.verificationDirectory + "/"
        if job.dataFile == None:
            job.dataFile = job.name + "Results.zip"
        if not os.path.isfile(os.path.join(job.dataPath,job.dataFile)):
            job.dataPath = os.path.join(job.basedir,job.dataPath)
        if job.logFile == None:
            job.logFile = job.name + "Results.zip"
        if job.scenarioFile == None:
            job.scenarioFile = job.name + ".xml"
        if job.outputFilename==None:
            job.outputFilename=job.titleOverride+".jpg"
        if len(job.outputFilename.split("."))==1:
            job.outputFilename+=".jpg"
        #if not job.outputFilename==None:
        #    job.outputFilename.replace("jpg","png")
        if job.imageWidth==None and job.imageHeight==None:
            job.imageWidth=1600
            job.imageHeight=800
