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

#os.chdir("/opt/biogears/core/build/runtime/")

class ActionEventPlotter():
    def __init__(self):
        self.events = []
        self.data = []
        self.timeData = []
        self.actions = []
        
    def plot(self, job):
        """
        Plot
        _________________
        
        This function takes in job object as an argument and
        tries to fill datapath,logpath with their respective 
        path which needed to be searched for first search takes
        place in Runtime directory for csv files. Then after that
        in Scenario folder in Runtime directory then finally
        if not found search is done in Baseline directory
        """
        
        if job.dataPath == None :
            job.dataPath = os.path.join("Scenarios",job.verificationDirectory,"baselines")
        
        if job.logPath == None:
            job.logPath = os.path.join("Scenarios",job.verificationDirectory,"baselines")
        
        if job.scenarioPath == None:
            job.scenarioPath = os.path.join("Scenarios",job.verificationDirectory)
            
        if job.dataFile == None:
            job.dataFile=job.name+"Results.csv"
        
        if job.logFile == None:
            job.logFile = job.name + "Results.zip"
            
        if job.scenarioFile == None:
            job.scenarioFile = job.name + ".xml"
            
        if job.outputFilename==None:
            job.outputFilename=job.titleOverride+".jpg"
            
        if len(job.outputFilename.split("."))==1:
            job.outputFilename+=".jpg"

        if job.imageWidth==None and job.imageHeight==None:
            job.imageWidth=1600
            job.imageHeight=800
            
        if not os.path.exists(job.dataPath):
            job.dataPath = os.path.join(job.basedir,job.dataPath)
            
        if not os.path.exists(job.logPath):
            job.logPath = os.path.join(job.basedir, job.logPath)
            
        if not os.path.isfile(os.path.join(job.dataPath,job.dataFile)):
            job.dataFile = job.name + "Results.zip"
            
        if not os.path.isfile(os.path.join(job.dataPath,job.dataFile)):
            job.dataPath = os.path.join(job.basedir,job.dataPath) 
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
        
        if job.log>=2:
            logging.info("Name of Plot" +job.name)
            logging.info("Input File: "+ os.path.join(job.dataPath,job.dataFile))
            logging.info("Output File: "+ os.path.join(job.outputDir,job.outputFilename))
        self.drawgraph(self.events,self.actions,job,os.path.join(job.dataPath,job.dataFile),
                       os.path.join(job.outputDir,job.outputFilename))
        
    def getActionsFromLog(self,file_,job):
        """
        getActionFromLog
        _________________
        
        This function is used to extract all Actions and it's 
        respective text from the log files inside datapath and 
        then log those appropriate to the command line
        """
        fin=None
        actions = []
        flag=0
        txt=""
        try:
            if file_.endswith(".zip"):
                try:
                    zf = zipfile.ZipFile(file_,'r')
                    for i in zf.filelist:
                        if i.filename.endswith(".log"):
                            fin = zf.open(i.filename,'r')
                            break
                            #  We expect results zips to only contain 1 text file
                except IOError as e:
                    logging.error("ActionEventPlotter couldn't read the log file " + file_)
        except IOError as e:
            logging.error("Zip file not found " +file_)
        if not fin:
            return actions
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
                try:
                    Action["time"] = float(ActionText[0:ActionTimeIndex].strip())
                except NumberFormatException as e:
                    logging.error("Couldn't correctly parse log file time to double")
                Action["text"] = ActionText[ActionText.find(",") + 1:].strip()
                flag=1
                txt+=ActionText[ActionText.find(",") + 1:].strip()

            elif flag==1 and line.startswith("\t"):
                txt+=line
            elif flag==1 and not line.startswith("\t"):
                txt=txt.replace("\t","\n\t",1)
                Action["text"]=txt
                if job.logger==True and job.log>2:
                    logging.info("Adding Action:" + Action["text"])
                actions.append(Action)
                txt=""
                flag=0
        fin.close()
        return actions
    
    def getEventsFromLog(self, file_,job):
        """
        getEventsFromLog
        __________________
        
        This function is used to extract respective event
        text and time from the log file inside datapath
        and then which can be used for plotting to the graph
        """
        
        events = []
        fin=None
        try:
            if file_.endswith(".zip"):
                try:
                    zf = zipfile.ZipFile(file_,'r')
                    for i in zf.filelist:
                        if i.filename.endswith(".log"):
                            fin = zf.open(i.filename,'r')
                            break
                except IOError as e:
                    logging.error("ActionEventPlotter couldn't read the log file " + file_)
        except IOError as e:
            logging.error("Zip File not found " +file_)
        if not fin:
            return events
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
                try:
                    event["time"] = float(eventText[0:endTimeIndex].strip())
                except NumberFormatException as e:
                    logging.error("Couldn't correctly parse log file time to double")
                event["text"] = eventText[eventText.find(",") + 1:].strip()
                if job.logger==True and job.log>2:
                    logging.info("Adding Event:" + event["text"])
                events.append(event)
        fin.close()
        return events
    
    def y_fmt(self,x, y):
        """
        y_fmt
        ________
        formatting the text to plot into the graph
        """
        
        return '{:2.2e}'.replace("0","").format(x).replace('e', 'E').replace("+","").replace("0","")
                
    def drawgraph(self,events,actions,job,input_zip,output_file):
        """
        draw_graph
        ________________
        
        For plotting the graph it calls the function which after
        processing whether it is csv or zip file is used to plot
        to the graph
        """
        my_dpi=96
        col=["red","yellow","green","blue","orange","lime","magenta","violet"
             ,"black","purple","0.1","0.2","0.75","0.8","0.9","pink"]
        try:
            if input_zip.endswith(".csv"):
                df = pd.read_csv(input_zip,low_memory=False)
                try:
                    self.plotting(events,actions,job,input_zip,output_file,df,my_dpi,col)
                except IOError:
                    logging.error("File Not found at:"+input_zip)
                except Exception:
                    logging.error("Headers not found at:"+input_zip+" Headers missing are :"+job.headers[0])

            elif input_zip.endswith(".zip"):
                zf=zipfile.ZipFile(input_zip)
                for i in zf.filelist:
                    if i.filename.endswith(".csv"):
                        df = pd.read_csv(zf.open(i.filename),low_memory=False)
                        try:
                            self.plotting(events,actions,job,input_zip,output_file,df,my_dpi,col)
                        except IOError:
                            logging.error("File Not found at:"+input_zip)
                        except Exception:
                            logging.error("Headers not found at:"+input_zip+" Headers missing are :"+job.headers[0])
        except IOError:
            logging.error("Zip file Not found at :"+input_zip)
        
    def plotting(self,events,actions,job,input_zip,output_file,df,my_dpi,col):
        """
        plotting
        ________________
        Main function which is a driver which uses the matplotlib plotting ability
        to plot to the graph plotting is determined by the commands in config file
        each command with parameters determine which if-else statement to execute
        and then on the basis of those parameters executing blocks of if-else for
        plotting
        
        """
        X=df.iloc[:,0].values[::20]
        y=df.loc[:,job.headers[0]].values[::20]
        if job.legendOnly:
            if not os.path.exists(job.outputDir):
                os.mkdir(job.outputDir)
            colors =["red","yellow","green","blue","orange","lime","magenta",
                     "violet","black","purple","0.1","0.2","0.75","0.8","0.9","pink"]
            f = lambda m,c: plt.plot([],[],marker=m, color=c, ls="none")[0]
            handles = [f("_", colors[i]) for i in range(0,len(self.data))]
            labels = [i.replace("\t","    ") for i in self.data]
            legend = plt.legend(handles, labels, loc=3, ncol=3, framealpha=1, frameon=False, fontsize=12)
            plt.axis('off')
            def export_legend(legend, filename=os.path.join(job.outputDir,job.outputFilename), expand=[-50,-50,50,50]):
                fig  = legend.figure
                fig.canvas.draw()
                bbox  = legend.get_window_extent()
                bbox = bbox.from_extents(*(bbox.extents + np.array(expand)))
                bbox = bbox.transformed(fig.dpi_scale_trans.inverted())
                fig.savefig(filename, dpi="figure", bbox_inches=bbox,pad_inches=0)
            
            export_legend(legend)
            if job.log>0:
                logging.info("Creating Graph:"+job.outputFilename.split(".")[0])
            plt.close("all")

        else:
            fig,ax = plt.subplots()
            fig.set_size_inches(w=job.imageWidth/my_dpi+1,h=job.imageHeight/my_dpi+1)
            
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
                    if job.log>0:
                        logging.info("Creating Graph:"+job.headers[0]+"_vs_Time_Action_Event_Plot")
                elif job.titleOverride=="None":
                    if job.log>0:
                        logging.info("Creating Graph:"+job.outputFilename.split(".")[0])
                else:
                    plt.title(job.titleOverride,fontsize=job.fontSize)
                    if job.log>0:
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
                ax.get_yaxis().set_major_locator(MaxNLocator(nbins=10,min_n_ticks=8))
                ax.get_xaxis().set_major_locator(MaxNLocator(nbins=15,min_n_ticks=10))
                ax.yaxis.set_major_formatter(tick.FuncFormatter(self.y_fmt))
                ax.yaxis.set_ticks_position("both")
                ax.yaxis.set_tick_params(labelright=True)
                plt.xlabel("Time(s)",fontsize=job.fontSize)
                plt.ylabel(job.headers[0],fontsize=job.fontSize)
                
                if job.titleOverride==None:
                    plt.title(job.headers[0]+"_vs_Time_Action_Event_Plot",fontsize=job.fontSize)
                    if job.log>0:
                        logging.info("Creating Graph:"+job.headers[0]+"_vs_Time_Action_Event_Plot")
                elif job.titleOverride=="None":
                    if job.log>0:
                        logging.info("Creating Graph:"+job.outputFilename.split(".")[0])
                else:
                    if job.log>0:
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
