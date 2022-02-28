#! /usr/bin/env python3

# -*- coding: utf-8 -*-
"""
@author: mmcdaniel
@date:   2019-10-11
@brief:  A small plotter tool
"""
import matplotlib.pyplot as plt
import sys
import os
import re
import pandas as pd
import shutil
import zipfile
import pathlib

import argparse

"""
This script assumes you are running from the command line.
Arguments:
    ValidationDir -- Directory containing BioGears PK validation runs
    LitDir--Directory containing literature CSV files
    Outdir--Directory in which to store plots (default to validation)
    plotTime -- Plot in s/min/hr (default = s)
Example:  python bg-plotPK Scenarios\Validation ..\..\share\doc\Pharmacokinetics 

"""
_log_verbose = 0
LOG_LEVEL_0 = 0
LOG_LEVEL_1 = 1
LOG_LEVEL_2 = 2
LOG_LEVEL_3 = 3
LOG_LEVEL_4 = 4


_output_directory = None
_validation_directory   = None
_lit_directory = None
def main( args):
    global _log_verbose
    global _validation_directory
    global _output_directory
    global _lit_directory

    parser = argparse.ArgumentParser()
    parser.add_argument('-v', help="Adjust the log verbosity.", dest='verbosity', default=0, action='count' )
    parser.add_argument('validation',
        action="store",
        help="Directory containing BioGears PK validation results files. ")
    parser.add_argument('literature',
        action="store",
        help="Directory containing literature files to plot BG results against.")
    parser.add_argument('--drugs',
        nargs="+", 
        action = 'store', 
        help = 'Plot a subset of drugs, rather than all in directory')
    parser.add_argument('--outdir',
        dest='outdir', action="store", 
        help="Directory in which validation folder will be stored, default = validation directory")
    parser.add_argument('--plotTime',action="store",default="s",
        help="Time unit to use for plots (s,min,hr), default = s")
    parser.add_argument('-logScale',action="store_true", help="Plot data on log scale if flagged")

    args = parser.parse_args()
    _log_verbose = args.verbosity

    log("Log level set to {0}".format(_log_verbose), LOG_LEVEL_1)

    _validation_directory = args.validation
    _lit_directory = args.literature
    drugList = args.drugs if args.drugs else []
    outDirTemp = args.outdir if args.outdir else _validation_directory
    _output_directory = os.path.join(outDirTemp,"PharmacokineticValidation")
    if not os.path.exists(_output_directory):
        os.makedirs(_output_directory)
    
    log("Validation Dir = {}".format(_validation_directory),LOG_LEVEL_2)
    log("Literature Dir = {}".format(_lit_directory),LOG_LEVEL_2)
    log("Output = {}".format(_output_directory),LOG_LEVEL_2)
    log("Plot time units = {}".format(args.plotTime),LOG_LEVEL_2)

    run_PKplots(drugList,args.plotTime,args.logScale)
    
def run_PKplots(_drugs, plotTime,scale):
    global _log_verbose
    defaultDrugs = ["Albuterol","Desflurane","Ertapenem","Fentanyl","Furosemide","Ketamine","Midazolam","Morphine","Naloxone","Norepinephrine","OralMoxifloxacin",
        "Piperacillin","Pralidoxime","Prednisone","Propofol","Rocuronium","Sarin","Succinylcholine","Tazobactam","TransmucosalFentanyl","TranexamicAcid","Vasopressin"]
    drugList = _drugs if _drugs else defaultDrugs
    
    for drug in drugList:
        bgResultsName = drug+"ValidationResults.csv"
        bgResultsFile = os.path.normpath(os.path.join(_validation_directory, bgResultsName))
        litResultsName = drug+".csv"
        litResultsFile = os.path.normpath(os.path.join(_lit_directory,litResultsName))
        if(os.path.exists(bgResultsFile)):
            plot_PK(bgResultsFile, litResultsFile, plotTime,scale)
        else:
            err("{} does not exist. Skipping".format(bgResultsName),LOG_LEVEL_0)

def plot_PK(bgFile, litFile, plotTime,scale):
    if ( not os.path.exists(bgFile)):
        err ("plot called with non exist file. This should not occur! {0}".format(input_source),LOG_LEVEL_0 )    
    else:
        basename,extension = os.path.splitext(os.path.basename(bgFile))
        
        log ("Processing: {0}".format(basename),LOG_LEVEL_1)

        #Read in results file
        dataCSV = pd.read_csv(bgFile,sep=',', header=0)

        #Time = x-axis by default
        bg_xData = dataCSV['Time(s)'].values
        xLabel="Time (s)"
        if plotTime=="min":
            bg_xData = bg_xData/60.0
            xLabel='Time (min)'
        if plotTime=="hr":
            bg_xData = bg_xData / 3600.0
            xLabel = 'Time (hr)'
         
        #Don't know exact units used--look for PlasmaConcentration substring (also prevents having to pass exact drug name)
        plasmaSubstring = 'PlasmaConcentration'
        plasmaHeader = [col for col in list(dataCSV) if re.search(plasmaSubstring, col)]
        yLabel = plasmaHeader[0]    #Assuming we get back list of size 1
        unitIndex = yLabel.find('(')
        unit = yLabel[unitIndex:len(yLabel)]
        if basename.find('Desflurane')!=-1:
            yLabel = 'Desflurane-EndTidalFraction'   #Desflurane does not use plasma concentration for validation
            unit=''
        
        litFound=False
        #Grab the literature file, if it exists
        try:
            litCSV = pd.read_csv(litFile, sep=',',header=0)
            log("Found literature file",LOG_LEVEL_2)
            lit_xHeader = list(litCSV)[0]
            lit_xData = litCSV[lit_xHeader].values
            if plotTime=="min":
                lit_xData=lit_xData/60.0
            if plotTime=="hr":
                lit_xData=lit_xData/3600.0
            lit_yHeader = list(litCSV)[1]
            lit_yData = litCSV[lit_yHeader]
            litUnitIndex = lit_yHeader.find('(')
            litUnit = lit_yHeader[litUnitIndex:len(lit_yHeader)]
            litFound=True
        except FileNotFoundError:
            err("Error: Could not find literature file: {}".format(litFile),LOG_LEVEL_0)


        bg_yData = dataCSV[yLabel].values
        plotMin = 0.01
        try:
            plotMax = 1.25 * max(bg_yData)
        except TypeError:
            err('Non-numeric Ymax, setting plot max to 10', LOG_LEVEL_1)
            plotMax= 10

        plt.plot(bg_xData,bg_yData,'-k',label='BioGears')
        if litFound:
            plt.plot(lit_xData,lit_yData,'ob', label = 'Literature')
            if min(lit_yData) < plotMin and min(lit_yData)>0:
                plotMin = min(lit_yData)
            if max(lit_yData) > plotMax:
                plotMax = 1.25 * max(lit_yData)
            if unit != litUnit:
                log("Warning:  BioGears and literature data units are different for {}".format(basename),LOG_LEVEL_0)
        plt.title(basename)
        if scale:
            plt.yscale('log')
            plotMax = 10 * max(bg_yData)
        plt.ylim([plotMin,plotMax])
        plt.xlabel(xLabel)
        plt.ylabel(yLabel)
        plt.grid()
        plt.legend()
        figName = os.path.join(_output_directory,"{}.png".format(basename))
        log("Saving : {0}".format(figName), LOG_LEVEL_3)
        if os.path.exists(figName):
            os.remove(figName)
        plt.savefig(figName)
        plt.close()

def err(message, level):
   if _log_verbose >= level:
    print( "{}\n".format(message))# end="", file=sys.stderr)
def log(message, level):
   if _log_verbose >= level:
    print( "{}\n".format(message))#, end="")
    
if __name__ == "__main__":
    # execute only if run as a script
    main( sys.argv )
