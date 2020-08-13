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
from matplotlib.offsetbox import TextArea, VPacker, AnnotationBbox

logging.basicConfig(level = logging.INFO)

logging = logging.getLogger("MultiPlotter")
#os.chdir("/opt/biogears/core/build/runtime/")

class MultiPlotter():
    def __init__(self):
        self.data = []
        self.timeData = []

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
        # fill PlotJob with needed data if it doesn't exist
        # Plotter will look for the files it needs relative to the work directory
        # If this fails it will fall back to a baseline location if one was 
        # Provided to cmake at the time this file was generated
        if job.dataPath == None :
            job.dataPath = "Scenarios/" + job.verificationDirectory + "/baselines/"
        
        if job.dataFile == None:
            job.dataFile = job.name + "Results.zip"
        
        if job.outputFilename==None:
            job.outputFilename=job.titleOverride+".jpg"
        
        if len(job.outputFilename.split("."))==1:
            job.outputFilename+=".jpg"
        
        if job.imageWidth==None and job.imageHeight==None:
            job.imageWidth=1600
            job.imageHeight=800
        
        if not os.path.exists(job.dataPath):
            job.dataPath = os.path.join(job.basedir,job.dataPath)
        
        if not os.path.isfile(os.path.join(job.dataPath,job.dataFile)):
            job.dataPath = os.path.join(job.basedir,job.dataPath)
        
        if not job.fontSize:
            job.fontSize=22
        
        if not os.path.exists(os.path.dirname(job.outputDir)):
            os.mkdir(os.path.dirname(job.outputDir))
 
        self.drawgraph(job,os.path.join(job.dataPath,job.dataFile),os.path.join(job.outputDir,job.outputFilename))
    
    def y_fmt(self,x, y):
        return '{:2.2e}'.replace("0","").format(x).replace('e', 'E').replace("+","").replace("0","")
        
    def drawgraph(self,job,input_zip,output_file):
        """
        draw_graph
        ________________
        
        For plotting the graph it calls the function which after
        processing whether it is csv or zip file is used to plot
        to the graph
        """
        my_dpi=96
        try:
            if input_zip.endswith(".csv"):
                df = pd.read_csv(input_zip,low_memory=False)
                try:
                    self.plotting(job,input_zip,output_file,df,my_dpi)
                except IOError:
                    logging.error("File Not found at:"+input_zip)
                except Exception:
                    logging.error("Headers not found at:"+input_zip)

            elif input_zip.endswith(".zip"):
                zf=zipfile.ZipFile(input_zip)
                for i in zf.filelist:
                    if i.filename.endswith(".csv"):
                        df = pd.read_csv(zf.open(i.filename),low_memory=False)
                        try:
                            self.plotting(job,input_zip,output_file,df,my_dpi)
                        except IOError:
                            logging.error("File Not found at:"+input_zip)
                        except Exception:
                            logging.error("Headers Not found in:"+input_zip)
        except IOError:
            logging.info("Zip file Not found at :"+input_zip)
        
    def plotting(self,job,input_zip,output_file,df,my_dpi):
        """
        plotting
        ________________
        Main function which is a driver which uses the matplotlib plotting ability
        to plot to the graph plotting is determined by the commands in config file
        each command with parameters determine which if-else statement to execute
        and then on the basis of those parameters executing blocks of if-else for
        plotting
        
        """
        if len(job.Y2headers)>0:
            X1=df.loc[:,job.X1header].values
            X2=df.loc[:,job.X2header].values
            Y1=df.loc[:,job.Y1headers].values
            if (job.name.startswith("Morphine") or job.name.startswith("Sarin")) and os.path.exists(job.experimentalData):
                df2=pd.read_csv(job.experimentalData)
                Y2=np.array(df2.loc[:,job.Y2headers].values)
                X1=np.array(df2.loc[:,job.X1header].values[:Y2.shape[0]])
                Y1=np.array(df.loc[:,job.Y1headers].values[:Y2.shape[0]])
                X2=np.array(df.loc[:,job.X2header].values[:Y2.shape[0]])
            else:
                Y2=df.loc[:,job.Y2headers].values
            fig,ax = plt.subplots()
            fig.set_size_inches(w=job.imageWidth/my_dpi+3.1,h=job.imageHeight/my_dpi+1)
            
            if not os.path.exists(job.outputDir):
                os.mkdir(job.outputDir)
            
            if job.X1LowerBound and job.X1UpperBound:
                ax.set_xlim(job.X1LowerBound,job.X1UpperBound)
            elif job.X1LowerBound:
                ax.set_xlim(left=job.X1LowerBound)
            elif job.X1UpperBound:
                ax.set_xlim(right=job.X1UpperBound)

            if job.Y1LowerBound and job.Y1UpperBound:
                ax.set_ylim(job.Y1LowerBound,job.Y1UpperBound)
            elif job.Y1LowerBound:
                ax.set_ylim(bottom=job.Y1LowerBound)
            elif job.Y1UpperBound:
                ax.set_ylim(top=job.Y1UpperBound)        
            
            ax.get_yaxis().set_major_locator(MaxNLocator(nbins=18,min_n_ticks=8))
            ax.get_xaxis().set_major_locator(MaxNLocator(nbins=10,min_n_ticks=5))
            ax.yaxis.set_major_formatter(tick.FuncFormatter(self.y_fmt))
            ax.set_xlabel(job.X1header,fontsize=job.fontSize)
            
            if job.Y1Label!=None:
                ax.set_ylabel(job.Y1Label,color="g",fontsize=job.fontSize)
            else:
                ax.set_ylabel(job.Y1headers[0],color="g",fontsize=job.fontSize)
            p1=ax.plot(X1,Y1,color="g")

            ax2=ax.twinx()
            
            if job.Y2LowerBound and job.Y2UpperBound:
                ax2.set_ylim(job.Y2LowerBound,job.Y2UpperBound)
            elif job.Y2LowerBound:
                ax2.set_ylim(bottom=job.Y2LowerBound)
            elif job.Y2UpperBound:
                ax2.set_ylim(top=job.Y2UpperBound)
            
            ax2.get_yaxis().set_major_locator(MaxNLocator(nbins=18,min_n_ticks=8))
            ax2.get_xaxis().set_major_locator(MaxNLocator(nbins=10,min_n_ticks=5))
            ax2.yaxis.set_major_formatter(tick.FuncFormatter(self.y_fmt))
            ax2.set_xlabel(job.X1header,fontsize=job.fontSize)
            
            if job.Y2Label!=None:
                ax2.set_ylabel(job.Y2Label,color="b",fontsize=job.fontSize)
            else:
                ax2.set_ylabel(job.Y2headers[0],color="b",fontsize=job.fontSize)
            
            p2=ax2.plot(X2,Y2,color="b")
            title=job.name+"_"
            title+=job.Y2headers[0]
            title = title + "vs_" + job.X1header
            
            if job.X2header != None and not job.X1header.lower() == job.X2header.lower():
                title = title + "_" + job.X2header
            # Override the constructed title if desired
            
            if job.titleOverride != None and not len(job.titleOverride)==0 and not job.titleOverride.lower() == "None".lower():
                title = job.titleOverride
            if job.log>=1:
                logging.info("Creating Graph:"+title)
            
            if job.titleOverride !="None":
                plt.title(title,fontsize=job.fontSize)
            
            if job.showGridLines:
                plt.grid(b=True, which='major', color='r', linestyle='--')
            
            if not job.removeAllLegends:
                p=p1+p2
                labs=[job.Y1headers[0],job.Y2headers[0]]
                ax.legend(p,labs)
            
            if "(" and ")" in job.outputFilename:
                job.outputFilename=job.outputFilename.split("(")[0]+".jpg"
            plt.savefig(os.path.join(job.outputDir,job.outputFilename),dpi=my_dpi)
            plt.close("all")

        elif len(job.Y2headers)==0:
            X1=df.loc[:,job.X1header]
            Y1=df.loc[:,job.Y1headers]
            fig,ax = plt.subplots()
            fig.set_size_inches(w=job.imageWidth/my_dpi+3.1,h=job.imageHeight/my_dpi+1)
            
            if not os.path.exists(job.outputDir):
                os.mkdir(job.outputDir)
            
            if job.X1LowerBound and job.X1UpperBound:
                ax.set_xlim(job.X1LowerBound,job.X1UpperBound)
            elif job.X1LowerBound:
                ax.set_xlim(left=job.X1LowerBound)
            elif job.X1UpperBound:
                ax.set_xlim(right=job.X1UpperBound)

            if job.Y1LowerBound and job.Y1UpperBound:
                ax.set_ylim(job.Y1LowerBound,job.Y1UpperBound)
            elif job.Y1LowerBound:
                ax.set_ylim(bottom=job.Y1LowerBound)
            elif job.Y1UpperBound:
                ax.set_ylim(top=job.Y1UpperBound)        
            
            ax.get_yaxis().set_major_locator(MaxNLocator(nbins=10,min_n_ticks=8))
            ax.get_xaxis().set_major_locator(MaxNLocator(nbins=10,min_n_ticks=5))
            ax.yaxis.set_major_formatter(tick.FuncFormatter(self.y_fmt))
            ax.set_xlabel(job.X1header,fontsize=job.fontSize)
            ax.set_ylabel(job.Y1headers[0],fontsize=job.fontSize)
            p=ax.plot(X1,Y1)
            title=job.name+"_"
            title = title + "vs_" + job.X1header
            
            if job.X2header != None and not job.X1header.lower() == job.X2header.lower():
                title = title + "_" + job.X2header
            
            # Override the constructed title if desired
            if job.titleOverride != None and not len(job.titleOverride)==0 and not job.titleOverride.lower() == "None".lower():
                title = job.titleOverride
            if job.log>=1:
                logging.info("Creating Graph:"+title)
            
            if job.titleOverride !="None":
                plt.title(title,fontsize=job.fontSize)
            
            if job.showGridLines:
                plt.grid(b=True, which='major', color='r', linestyle='--')
            
            if not job.removeAllLegends:
                labs=job.Y1headers
                plt.legend(p,labs)
            
            if "(" and ")" in job.outputFilename:
                job.outputFilename=job.outputFilename.split("(")[0]+".jpg"
            plt.savefig(os.path.join(job.outputDir,job.outputFilename),dpi=my_dpi)
            plt.close("all")
        
        else:
            X1=df.loc[:,job.X1header]
            Y1=df.loc[:,job.Y1headers]
            fig,ax = plt.subplots()
            fig.set_size_inches(w=job.imageWidth/my_dpi+3.1,h=job.imageHeight/my_dpi+1)
            
            if not os.path.exists(job.outputDir):
                os.mkdir(job.outputDir)
            
            if job.X1LowerBound and job.X1UpperBound:
                ax.set_xlim(job.X1LowerBound,job.X1UpperBound)
            elif job.X1LowerBound:
                ax.set_xlim(left=job.X1LowerBound)
            elif job.X1UpperBound:
                ax.set_xlim(right=job.X1UpperBound)

            if job.Y1LowerBound and job.Y1UpperBound:
                ax.set_ylim(job.Y1LowerBound,job.Y1UpperBound)
            elif job.Y1LowerBound:
                ax.set_ylim(bottom=job.Y1LowerBound)
            elif job.Y1UpperBound:
                ax.set_ylim(top=job.Y1UpperBound)        
            
            ax.get_yaxis().set_major_locator(MaxNLocator(nbins=10,min_n_ticks=8))
            ax.get_xaxis().set_major_locator(MaxNLocator(nbins=10,min_n_ticks=5))
            ax.yaxis.set_major_formatter(tick.FuncFormatter(self.y_fmt))
            ax.set_xlabel(job.X1header,fontsize=job.fontSize)
            ax.set_ylabel(job.Y1headers[0],fontsize=job.fontSize)
            p1=ax.plot(X1,Y1)
            title=job.name+"_"
            title = title + "vs_" + job.X1header
            
            if job.X2header != None and not job.X1header.lower() == job.X2header.lower():
                title = title + "_" + job.X2header
            
            # Override the constructed title if desired
            if job.titleOverride != None and not len(job.titleOverride)==0 and not job.titleOverride.lower() == "None".lower():
                title = job.titleOverride
            if job.log>=1:
                logging.info("Creating Graph:"+title)
            
            if job.titleOverride !="None":
                plt.title(title,fontsize=job.fontSize)
            
            if job.showGridLines:
                plt.grid(b=True, which='major', color='r', linestyle='--')
            
            if not job.removeAllLegends:
                labs=job.Y1headers
                ax.legend(p1, labs)
            
            if "(" and ")" in job.outputFilename:
                job.outputFilename=job.outputFilename.split("(")[0]+".jpg"
            plt.savefig(os.path.join(job.outputDir,job.outputFilename),dpi=my_dpi)
            plt.close("all")

