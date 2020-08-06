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
import matplotlib

matplotlib.use('Agg')
logging.basicConfig(level = logging.INFO)
logging = logging.getLogger("ActionEventPlotter")

#os.chdir("/opt/biogears/core/build_4/runtime")

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
	
        self.drawgraph(self.events,self.actions,job,os.path.join(job.dataPath,job.dataFile),os.path.join(job.outputDir,job.outputFilename))

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

    
    def y_fmt(self,x, y):
        return '{:2.2e}'.replace("0","").format(x).replace('e', 'E').replace("+","").replace("0","")

    def drawgraph(self,events,actions,job,input_zip,output_file):
        try:
            if input_zip.endswith(".zip"):
                zf=zipfile.ZipFile(input_zip)
                for i in zf.filelist:
                    if i.filename.endswith(".csv"):
                        px_to_in=0.010416666666819
                        my_dpi=96
                        col=["red","yellow","green","blue","orange","lime","magenta","violet","black","purple","0.1","0.2","0.75","0.8","0.9","pink"]
                        df = pd.read_csv(zf.open(i.filename))
                        X=df.iloc[:,0].values
                        y=df.loc[:,job.headers[0]].values
                        yRange = max(y)-min(y)
                        #For most scenarios, set plot min to 0 and set upper boundary 25% above max value
                        if yRange > 0:
                            if min(y) >=0:
                                plotMin = 0.0
                                plotMax = 1.25 * max(y)
                            else:
                                #We have negative values somewhere, so lets just take 25% above and below
                                plotMin = min(y)-0.25*abs(min(y))
                                plotMax = max(y) + 0.25*abs(max(y))
                        else:
                            #This means we're probably plotting a bunch of zeros...
                            plotMin = y[0] - 5
                            plotMax = y[0] + 5
                        if job.legendOnly:
                            if not os.path.exists(job.outputDir):
                                os.mkdir(job.outputDir)
                            fig, ax = plt.subplots()
                            fig.set_size_inches(w=job.imageWidth/my_dpi+1.5,h=job.imageHeight/my_dpi+2.5)
                            ax.get_xaxis().set_ticks([])
                            ax.get_yaxis().set_ticks([])
                            Texts=[]
                            for t,c in zip(self.data,col[0:len(self.data)]):
                                Texts.append(TextArea(t.replace("\t","    "),textprops=dict(color=c,fontsize=12,style='italic'),multilinebaseline=True))
                                texts_vbox = VPacker(children=Texts,pad=1,sep=2,align="center")
                                ann = AnnotationBbox(texts_vbox,(0.25,.02),xycoords=ax.transAxes,
                                                            box_alignment=(0,0),bboxprops = 
                                                            dict(facecolor='wheat',boxstyle='round',color='whitesmoke'))
                                ann.set_figure(fig)
                                fig.artists.append(ann)
                            logging.info("Creating Graph:"+job.outputFilename)
                            plt.savefig(os.path.join(job.outputDir,job.outputFilename),dpi=my_dpi, bbox_inches = 'tight',pad_inches = 0)
                            plt.close("all")

                        else:
                            fig,ax = plt.subplots()
                            fig.set_size_inches(w=job.imageWidth/my_dpi,h=job.imageHeight/my_dpi)
                            if not os.path.exists(job.outputDir):
                                os.mkdir(job.outputDir)
                            if job.logAxis:
                                ax.set_yscale("log")
                                ax.yaxis.set_major_formatter(tick.FuncFormatter(self.y_fmt))
                                ax.yaxis.set_ticks_position("both")
                                ax.yaxis.set_tick_params(labelright=True)
                                plt.xlabel("Time(s)",fontsize=job.fontSize)
                                plt.ylabel(job.headers[0],fontsize=job.fontSize)
                                if job.titleOverride==None:
                                    plt.title(job.headers[0]+"_vs_Time_Action_Event_Plot",fontsize=job.fontSize)
                                    logging.info("Creating Graph:"+job.headers[0]+"_vs_Time_Action_Event_Plot")
                                elif job.titleOverride=="None":
                                    logging.info("Creating Graph:"+job.outputFilename)
                                    pass
                                else:
                                    plt.title(job.titleOverride,fontsize=job.fontSize)
                                    logging.info("Creating Graph:"+job.titleOverride)
                                plt.xlim(0,max(X))
                                p=plt.plot(X,y)
                                for i in range(0,len(self.timeData)):
                                    plt.axvline(self.timeData[i],color=col[i])
                                if job.showGridLines:
                                    plt.grid(b=True, which='major', color='r', linestyle='--')
                                if not job.hideAELegend and not job.removeAllLegends:
                                    labs=job.headers
                                    plt.legend(p,labs)
                                if "(" and ")" in job.outputFilename:
                                    job.outputFilename=job.outputFilename.split("(")[0]+".jpg"
                                plt.savefig(os.path.join(job.outputDir,job.outputFilename),dpi=my_dpi)
                                plt.close("all")
                            else:
                                ax.get_yaxis().set_major_locator(MaxNLocator(nbins=18,min_n_ticks=8))
                                ax.get_xaxis().set_major_locator(MaxNLocator(nbins=20,min_n_ticks=10))
                                ax.yaxis.set_major_formatter(tick.FuncFormatter(self.y_fmt))
                                ax.yaxis.set_ticks_position("both")
                                ax.yaxis.set_tick_params(labelright=True)
                                plt.xlabel("Time(s)",fontsize=job.fontSize)
                                plt.ylabel(job.headers[0],fontsize=job.fontSize)
                                if job.titleOverride==None:
                                    plt.title(job.headers[0]+"_vs_Time_Action_Event_Plot",fontsize=job.fontSize)
                                    logging.info("Creating Graph:"+job.headers[0]+"_vs_Time_Action_Event_Plot")
                                elif job.titleOverride=="None":
                                    logging.info("Creating Graph:"+job.outputFilename)
                                else:
                                    logging.info("Creating Graph:"+job.titleOverride)
                                    plt.title(job.titleOverride,fontsize=job.fontSize)
                                plt.xlim(0,max(X))
                                p=plt.plot(X,y)
                                for i in range(0,len(self.timeData)):
                                    plt.axvline(self.timeData[i],color=col[i])
                                if job.showGridLines:
                                    plt.grid(b=True, which='major', color='r', linestyle='--')
                                if not job.hideAELegend and not job.removeAllLegends:
                                    labs=job.headers
                                    plt.legend(p,labs)
                                if "(" and ")" in job.outputFilename:
                                    job.outputFilename=job.outputFilename.split("(")[0]+".jpg"
                                plt.savefig(os.path.join(job.outputDir,job.outputFilename),dpi=my_dpi)
                                plt.close("all")
        except:
            logging.error("Plotter couldn't plot job " + job.name +" Due to insufficiet: " + job.headers[0] + " In file: "+ job.dataFile)
