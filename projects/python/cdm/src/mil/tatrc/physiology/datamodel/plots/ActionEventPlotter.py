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
        if not job.skipAllEvents:
            self.events = self.getEventsFromLog(os.path.join(job.logPath, job.logFile),job)
        if not job.skipAllActions:
            self.actions = self.getActionsFromLog(os.path.join(job.logPath,job.logFile),job)
        if len(self.events)>0 and len(self.actions)>0:
            for i in self.events:
                self.timeData.append(i["time"])
                self.data.append("Event:"+i["text"])
            for i in self.actions:
                self.timeData.append(i["time"])
                self.data.append("Actions:"+i["text"])
        elif len(self.events)>0 and not len(self.actions)>0:
            for i in self.events:
                self.timeData.append(i["time"])
                self.data.append("Event:"+i["text"])
        elif not len(self.events)>0 and len(self.actions)>0:
            for i in self.actions:
                self.timeData.append(i["time"])
                self.data.append("Actions:"+i["text"])
        if not os.path.exists(os.path.dirname(job.outputDir)):
            os.mkdir(os.path.dirname(job.outputDir))
        if not job.fontSize:
            job.fontSize=22
    
    
    def getEventsFromLog(self, file_,job):
        events = []
        try:
            if file_.endswith(".zip"):
                zf = zipfile.ZipFile(file_,'r')
                for i in zf.filelist:
                    if i.filename.endswith(".log"):
                        fin = zf.open(i.filename,'r')
                        break
                        #  We expect results zips to only contain 1 text file
            for line in fin:
                line=line.decode("utf-8")
                if len(line)==0:
                    continue
                if "[Event]" not in line.split():
                    continue
                else:
                    event = {}
                    eventText =line.split("[Event]",1)[1].strip()
                    endTimeIndex = eventText.find("(s)")
                    if endTimeIndex == -1:
                        endTimeIndex = eventText.find(",")
                    event["time"] = float(eventText[0:endTimeIndex].strip())
                    event["text"] = eventText[eventText.find(",") + 1:].strip()
                    if job.logger==True:
                        logging.info("Adding Event:" + event["text"])
                    events.append(event)
            fin.close()
        except IOError as e:
            logging.warning("ActionEventPlotter couldn't read the log file " + file_)
        except NumberFormatException as e:
            logging.error("Couldn't correctly parse log file time to double")
        except Exception as e:
            logging.error("Something went wrong parsing the log", e)
            return None
        return events

    def getActionsFromLog(self,file_,job):
        actions = []
        flag=0
        txt=""
        try:
            if file_.endswith(".zip"):
                zf = zipfile.ZipFile(file_,'r')
                for i in zf.filelist:
                    if i.filename.endswith(".log"):
                        fin = zf.open(i.filename,'r')
                        break
                        #  We expect results zips to only contain 1 text file
            for line in fin:
                line=line.decode("utf-8")
                if len(line)==0:
                    continue
                if "[Action]" in line.split():
                    Action = {}
                    ActionText =line.split("[Action]",1)[1].strip()
                    ActionTimeIndex = ActionText.find("(s)")
                    if ActionTimeIndex == -1:
                        ActionTimeIndex = ActionText.find(",")
                    Action["time"] = float(ActionText[0:ActionTimeIndex].strip())
                    Action["text"] = ActionText[ActionText.find(",") + 1:].strip()
                    flag=1
                    txt+=ActionText[ActionText.find(",") + 1:].strip()

                elif flag==1 and line.startswith("\t"):
                    txt+=line
                elif flag==1 and not line.startswith("\t"):
                    txt=txt.replace("\t","\n\t",1)
                    Action["text"]=txt
                    if job.logger==True:
                        logging.info("Adding Action:" + Action["text"])
                    actions.append(Action)
                    txt=""
                    flag=0
            fin.close()
        except IOError as e:
            logging.warning("ActionEventPlotter couldn't read the log file " + file_)
        except NumberFormatException as e:
            logging.error("Couldn't correctly parse log file time to double")
        except Exception as e:
            logging.error("Something went wrong parsing the log", e)
            return None
        return actions

