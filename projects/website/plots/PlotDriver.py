import sys

global working
working = True
try:
    from ActionEventPlotter import ActionEventPlotter
    from MultiPlotter import MultiPlotter
    import multiprocessing
    import os
    import signal
    import time
    import argparse
    import logging
    import concurrent.futures
    logging.basicConfig(level = logging.INFO)
     
    class PlotJob():
        global working
        def __init__(self):
            self.name = None
            self.headers = list()
            self.plotter = None
            self.logger=True
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
            self.log=None
            self.X2Label = None
            self.Y2Label = None
            self.PFTFile = None
            self.basedir= None

        def Reset(self):
            self.headers = None
            self.eventOmissions = None
            self.actionOmissions = None
            self.compareData = None
            self.Y1headers = None
            self.Y2headers = None

    class PlotDriver():
        """
        PlotDriver
        ___________________

        Driver function which is used for parsing the whole commands from config file
        and then after parsing the commands extracting each and every parameter from
        each of the command and then associating to the object of this class named as 
        job.
        """

        def __init__(self):
            self.name = None
            self.jobs = list()
            self.preload = False
            self.onlyPlotFailures = False
            self.computedFilePath = None
            self.expectedFilePath = None
            self.abbreviateContents = 0
            self.isScenario = False
            self.jobname=""
            self.quit=False
            self.counter=0
            self.verbosity=None

        @classmethod
        def main(cls,args):
            global working
            """
            main
            _______

            Function that is executed first from extracting of arguments and then 
            calling the particular class from the appropriate file on the basis of
            plotname
            """

            me=PlotDriver()
            # invalid input
            if len(args)<3:
                logging.error("No files specified")
                return
            elif len(args)>3:
                if args[2].split(".")[1]!="config":
                    logging.error("Value " + args[2] + " doesn't seem to be a valid config file")
                    return
                if not os.path.isfile(args[2]):
                    logging.error("ConfigFile " + args[2] + " not found")
                    return
                me.processConfigFile(args[2],args[1])
            me.verbosity=args[4]
            original_sigint_handler = signal.signal(signal.SIGINT, signal.SIG_IGN)
            pool = multiprocessing.Pool(int(args[3]))
            signal.signal(signal.SIGINT, original_sigint_handler)
            try:
                global working
                p = pool.map_async(me.execute,range(len(me.jobs)))
                p.get(600)
            except KeyboardInterrupt:
                print("\n Shutting down. One moment while I clean up remaining threads.")
                working = False
                pool.terminate()
            else:
               pool.close()
            pool.join()
            
        def processConfigFile(self, configFile,basedir):
    
            """
            processConfigFile
            __________________________

            Config file is processed in this function and all of the commands are
            extracted and associated with the particular object which can be used
            later for plotting
            """
            self.name = configFile.split(".")[0]
            currentGroup = self.name
            try:
                with open(configFile,"r") as fin:
                    for line in fin:
                        self.counter+=1
                        if len(line)<=2 or line.startswith("#"):
                            continue 
                        
                        if line.startswith("@group"):
                            currentGroup = line.partition("@group")[2].strip()
                        
                        if len(currentGroup)==0:
                            currentGroup = self.name
                            continue
                        
                        if not "=" in line:
                            print("Error in config file Name of plotter not specified, check line:",str(self.counter))
                            continue 
                        
                        line = line.strip()
                        key = line.split("=")[0]
                        value = line.split("=",1)[1].strip()
                        if key.lower() == "Plotter".lower():
                            continue
                        job = PlotJob()
                        
                        if key[0] == '-':
                            job.ignore = True
                            key = key[1:]
                        self.jobs.append(job)
                        job.name = key.strip()
                        directives = value.strip().split(" ")
                        for directive in directives:
                            if not "=" in directive:
                                job.basedir=basedir
                                if directive.lower() == "ActionEventPlotter".lower():
                                    job.plotname="ActionEvent"
                                    job.plotter=ActionEventPlotter()
                                    continue
                                elif directive.lower()=="MultiPlotter".lower():
                                    job.plotname="Multiplotter"
                                    job.plotter=MultiPlotter()
                                    continue
                                elif directive.lower()=="ConvexHullPlotter".lower():
                                    job.plotname="ConvexHullPlotter"
                                    job.plotter=MultiPlotter()
                                elif directive.lower()=="RespiratoryPFTPlotter".lower():
                                    job.plotname="PFTPlotter"
                                    job.plotter="PFTPlotter()"
                                    continue
                                elif directive.lower() == "NoGrid".lower():
                                    job.showGridLines = False
                                    continue 
                                elif directive.lower() == "NoActions".lower():
                                    job.skipAllActions = True
                                    continue 
                                elif directive.lower() == "NoEvents".lower():
                                    job.skipAllEvents = True
                                    continue 
                                elif directive.lower() == "LogAxis".lower():
                                    job.logAxis = True
                                    continue 
                                elif directive.lower() == "ZeroAxis".lower():
                                    job.forceZeroYAxisBound = True
                                    continue 
                                elif directive.lower() == "HideActionEventLegend".lower():
                                    job.hideAELegend = True
                                    continue 
                                elif directive.lower() == "RemoveLegends".lower():
                                    job.hideAELegend = True
                                    job.removeAllLegends = True
                                    continue 
                                elif directive.lower() == "LegendOnly".lower():
                                    job.legendOnly = True
                                    continue 
                            else:
                                
                                key_1 = directive.split("=")[0].strip()
                                value_1 = directive.split("=")[1].strip()
                                if key_1.lower() == "Header".lower():
                                    configHeaders = value_1.split(",")
                                    for h in configHeaders:
                                        job.headers.append(h)
                                    continue 
                                elif key_1.lower() == "Y1".lower():
                                    configHeaders = value_1.split(",")
                                    for h in configHeaders:
                                        job.headers.append(h)
                                        job.Y1headers.append(h)
                                    continue 
                                elif key_1.lower() == "Y2".lower():
                                    configHeaders = value_1.split(",")
                                    for h in configHeaders:
                                        job.headers.append(h)
                                        job.Y2headers.append(h)
                                    continue 
                                elif key_1.lower() == "OmitActionsWith".lower():
                                    allSkips = value_1.split(",")
                                    for h in allSkips:
                                        job.actionOmissions.append(h)
                                    continue 
                                elif key_1.lower() == "OmitEventsWith".lower():
                                    allSkips = value_1.split(",")
                                    for h in allSkips:
                                        job.eventOmissions.append(h)
                                    continue 
                                elif key_1.lower() == "ImageDimensions".lower():
                                    dims = value_1.split(",")
                                    job.imageWidth = int(dims[0])
                                    job.imageHeight = int(dims[1])
                                    continue 
                                elif key_1.lower() == "VerificationDir".lower():
                                    job.verificationDirectory = value_1
                                    continue 
                                elif key_1.lower() == "Title".lower():
                                    job.titleOverride = value_1
                                    continue 
                                elif key_1.lower() == "OutputFilename".lower():
                                    job.outputFilename = value_1
                                    continue 
                                elif key_1.lower() == "ReadSkip".lower():
                                    job.resultsSkipNum = int(value_1)
                                    continue 
                                elif key_1.lower() == "X1".lower():
                                    job.X1header = value_1
                                    job.headers.append(value_1)
                                    continue 
                                elif key_1.lower() == "X2".lower():
                                    job.X2header = value_1
                                    job.headers.append(value_1)
                                    continue 
                                elif key_1.lower() == "X1LowerBound".lower():
                                    job.X1LowerBound = float(value_1)
                                    continue 
                                elif key_1.lower() == "X2LowerBound".lower():
                                    job.X2LowerBound = float(value_1)
                                    continue 
                                elif key_1.lower() == "X1UpperBound".lower():
                                    job.X1UpperBound = float(value_1)
                                    continue 
                                elif key_1.lower() == "X2UpperBound".lower():
                                    job.X2UpperBound = float(value_1)
                                    continue 
                                elif key_1.lower() == "Y1LowerBound".lower():
                                    job.Y1LowerBound = float(value_1)
                                    continue 
                                elif key_1.lower() == "Y2LowerBound".lower():
                                    job.Y2LowerBound = float(value_1)
                                    continue 
                                elif key_1.lower() == "Y1UpperBound".lower():
                                    job.Y1UpperBound = float(value_1)
                                    continue 
                                elif key_1.lower() == "Y2UpperBound".lower():
                                    job.Y2UpperBound = float(value_1)
                                    continue 
                                elif key_1.lower() == "X1Label".lower():
                                    job.X1Label = value_1
                                    continue 
                                elif key_1.lower() == "Y1Label".lower():
                                    job.Y1Label = value_1
                                    continue 
                                elif key_1.lower() == "X2Label".lower():
                                    job.X2Label = value_1
                                    continue 
                                elif key_1.lower() == "Y2Label".lower():
                                    job.Y2Label = value_1
                                    continue 
                                elif key_1.lower() == "ExperimentalData".lower():
                                    job.experimentalData = value_1
                                    continue 
                                elif key_1.lower() == "DataFileOverride".lower():
                                    job.dataFile = value_1
                                    continue 
                                elif key_1.lower() == "DataPathOverride".lower():
                                    job.dataPath = value_1
                                    continue 
                                elif key_1.lower() == "PFTFile".lower():
                                    job.PFTFile = value_1
                                    continue 
                                elif key_1.lower() == "OutputOverride".lower():
                                    job.outputDir = value_1
                                    continue 
                                elif key_1.lower() == "FontSize".lower():
                                    job.fontSize = int(value_1)
                                    continue 
                                else:
                                    logging.warning("Unrecognized config directive: " + directive 
                                                    +" Check line:"+str(self.counter))   
                fin.close()
            except Exception as e:
                logging.error("Something went wrong with config file:"+str(self.counter))

        def execute(self,idx):
            global working
            """
            Execute
            __________________
            This function is used for calling the appropriate file class
            with job as an argument which is used for plotting
            """
            if working:
                job=self.jobs[idx]
                if not job.ignore:
                    if self.jobname==job.name:
                        job.logger=False
                    else:
                        job.logger=True
                    if job.plotname!="PFTPlotter":
                        job.log=self.verbosity

                        if self.quit==False:
                            try:
                              job.plotter.plot(job)
                            except KeyboardInterrupt:
                              working = False
                              KeyboardInterrupt()

                    self.jobname=job.name 
                job.Reset()
                pass


    if __name__ == '__main__':
        parser = argparse.ArgumentParser(description='Creation of Plots')
        parser.add_argument('-p','--configpath',help='Path to Config file',required=True)
        parser.add_argument('-b','--baseline',help='Baseline Dir path',required=True)
        parser.add_argument('-j','--threads',help='No. of Threads')
        parser.add_argument('-v', help="Adjust the log verbosity.", dest='verbosity', default=0, action='count' )
        args=parser.parse_args()
        if args.configpath==None and args.baseline==None:
            parser.print_help()
            sys.exit(0)
        else:
            argv=[sys.argv[0],args.baseline,args.configpath,args.threads,args.verbosity]
            PlotDriver.main(argv)

except KeyboardInterrupt:
    sys.exit(1)
