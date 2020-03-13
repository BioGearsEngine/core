#! /usr/bin/env python3

# -*- coding: utf-8 -*-
"""
@author: mmcdaniel
@date:   2018-06-05
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
    Verification Folder (i.e. AnesthesiaMachine, Drug, Patient, etc)
    ScenarioName
    Optional:  Lines to skip for faster plotting (5 by default)

Example:  python ScenarioPlotter.py Patient BasicStandard 

"""
_log_verbose = 0
LOG_LEVEL_0 = 0
LOG_LEVEL_1 = 1
LOG_LEVEL_2 = 2
LOG_LEVEL_3 = 3
LOG_LEVEL_4 = 4


_output_directory = None
_root_directory   = None
_baseline_directory = None
_clean_directory  = False
def main( args):
    global _log_verbose
    global _root_directory
    global _baseline_directory
    global _output_directory
    global _clean_directory

    parser = argparse.ArgumentParser()
    parser.add_argument('-v', help="Adjust the log verbosity.", dest='verbosity', default=0, action='count' )
    parser.add_argument('--version', action='version', version='%(prog)s 1.1')
    parser.add_argument('-r', help="recursivly plot directories", dest='recurse', action='store_true')
    parser.add_argument('-c', '--clean' , help="remove output directories recursivly before plotting", dest='cleanup', action='store_true')
    parser.add_argument('--root',
        dest='root', action="store", 
        help="Set an alternative root dir", default="")
    parser.add_argument('--baselines',
        dest='baselines', action="store", 
        help="Set an alternative verification root dir", default="")
    parser.add_argument('--outdir',
        dest='outdir', action="store", 
        help="Optional Output directory instead of inlinline layout ")
    parser.add_argument('scenario',
        nargs="+", action="store",
        help="One or more scenario files or directories. ")
    parser.add_argument('--skip', 
        dest='skip_count', action="store", default=10,
        help="Number of lines to skip for faster plotting")
    parser.add_argument('--plotTime',action="store",default="s",
        help="Time unit to use for plots (s,min,hr), default = s")

    args = parser.parse_args()
    _log_verbose = args.verbosity

    log("Log level set to {0}".format(_log_verbose), LOG_LEVEL_1)

    _root_directory = args.root
    _output_directory = args.outdir if args.outdir else _root_directory
    _baseline_directory = args.baselines if args.baselines else _output_directory
    _clean_directory = args.cleanup
    
    log("INPUT = {}".format(_root_directory),LOG_LEVEL_2)
    log("OUTPUT = {}".format(_output_directory),LOG_LEVEL_2)
    log("Baselines = {}".format(_baseline_directory),LOG_LEVEL_2)
    log("Recursive Mode = {}".format(args.recurse),LOG_LEVEL_2)
    log("Plot time units = {}".format(args.plotTime),LOG_LEVEL_2)
    
    log ("Processing",LOG_LEVEL_1)
    run_plots( args.root, args.scenario, args.skip_count, args.recurse ,args.plotTime )
    

def run_plots(root_dir, sources, skip_count, recurse, plotTime):
    global _log_verbose
    log ("run_plots({0},{1},{2},{3})".format(root_dir, sources, skip_count, recurse),LOG_LEVEL_3)
    for scenario in sources:
        possible_scenario = os.path.normpath(os.path.join(root_dir,scenario))
        if( os.path.isdir(possible_scenario) ):
         if recurse:
            for file in os.listdir(possible_scenario):
                try:
                  run_plots( possible_scenario,[file],skip_count,recurse, plotTime)
                except:
                  err("Unable to plot resuts for {0}".format(file),LOG_LEVEL_0)
         else:
            err("{0} is a directory.\n\t Use -r for recursive mode.".format(possible_scenario),LOG_LEVEL_0)
        elif( os.path.exists(possible_scenario)):
            plot(root_dir,scenario,skip_count, plotTime)
        else:
            err("{0} does not exist. Unable to process".format(possible_scenario),LOG_LEVEL_0)

def plot(root_dir, source, skip_count, plotTime):
    valid_regex = '[.](txt|csv)'
    input_source = os.path.join(root_dir,source)
    basename,extension     = os.path.splitext(input_source)
    output_path = os.path.join(_output_directory,os.path.relpath(basename,_root_directory))
    baseline_path = os.path.join(_baseline_directory,os.path.relpath(basename,_root_directory))

    log("{}{}".format(basename,extension),LOG_LEVEL_4)
    if ( not os.path.exists(input_source)):
        err ("plot called with non exist file. This should not occur! {0}".format(input_source),LOG_LEVEL_0 )    
    if ( not re.match( valid_regex, extension) ):
        log ("Skipping {0}".format(os.path.basename(input_source)),LOG_LEVEL_3 )
    else:
        log ("Source File: {0}".format(input_source),LOG_LEVEL_1)

        #Create directory for plots ()
        dirname   =  os.path.dirname(output_path) 
        basename, extension = os.path.splitext(os.path.basename(output_path))
        out_dir = os.path.join(dirname,basename)

        if os.path.isdir(output_path) and _clean_directory: 
          shutil.rmtree(output_path)
        if not os.path.exists(output_path):
          os.makedirs(output_path)

        #TODO: Validity Check on Log Length

        #Read in results file
        dataCSV = pd.read_csv(input_source,sep=',', header=0)

        #Extract data calls (specified in xml)
        colNames = list(dataCSV)
        #Time = x-axis by default
        xData = dataCSV['Time(s)'].values[::int(skip_count)]
        xLabel="Time (s)"
        if plotTime=="min":
            xData = xData/60.0
            xLabel='Time (min)'
        if plotTime=="hr":
            xData = xData / 3600.0
            xLabel = 'Time (hr)'
        
        baseFound=False
        #Grab the zip file with the baselines, if they exist
        try:
            baseline_source = os.path.join( os.path.dirname(baseline_path),'baselines',basename+'.zip')
            baseZip = zipfile.ZipFile(baseline_source)
            log("Trying to find baseline file: {}".format(baseline_source),LOG_LEVEL_3)
            baseCSV = pd.read_csv(baseZip.open(basename+'.csv'), sep=',',header=0)
            log("Found baseline file in {}".format(baseline_source),LOG_LEVEL_2)
            xBase = baseCSV['Time(s)'].values[::int(skip_count)]
            if plotTime=="min":
                xBase=xBase/60.0
            if plotTime=="hr":
                xBase=xBase/3600.0
            baseFound=True
        except FileNotFoundError:
            err("Error: Could not find baseline location: {}".format(baseline_source),LOG_LEVEL_0)


        #Plot each column of data 
        for col in colNames:
            if col=='Time(s)':
                continue
            
            #Get all of the values we need
            
            yData = dataCSV[col].values[::int(skip_count)]
            log("{0},{1}".format(skip_count, yData), LOG_LEVEL_3)
            if( not yData.size ):
                log("Error: Column is empty did you provide a non resonable skip_count?", LOG_LEVEL_0)
            else:
                try:
                    yRange = max(yData)-min(yData)
                    #For most scenarios, set plot min to 0 and set upper boundary 25% above max value
                    if yRange > 0:
                        if min(yData) >=0:
                            plotMin = 0.0
                            plotMax = 1.25 * max(yData)
                        else:
                            #We have negative values somewhere, so lets just take 25% above and below
                            plotMin = min(yData)-0.25*abs(min(yData))
                            plotMax = max(yData) + 0.25*abs(max(yData))
                    else:
                        #This means we're probably plotting a bunch of zeros...
                        plotMin = yData[0] - 5
                        plotMax = yData[0] + 5
                except TypeError:
                    err('Non-numeric data, setting range 0-100', LOG_LEVEL_1)
                    plotMin = 0
                    plotMax= 100
                #Extract unit--assuming they are specified in parentheses
                unitIndex = col.find('(')
                if(unitIndex == -1):
                    unit = ''
                    plotCol = col
                else:
                    unit = col[unitIndex:len(col)]
                    plotCol = col.replace(unit,'')
                
                pltTitle = re.sub("([a-z])([A-Z])","\g<1> \g<2>", plotCol)
                
                #Unit Test outputs sometimes put '/' in unit names, which throws off SaveFig function. Replace '/' with 'per'
                if plotCol.find('/')!=-1:
                    plotCol = plotCol.replace('/','Per')
                log("Plotting {}".format(col), LOG_LEVEL_2)

                try:   
                    plt.plot(xData,yData,'-k',label='Scenario')
                    if baseFound:
                        try:
                            yBase = baseCSV[col].values[::int(skip_count)]
                            plt.plot(xBase,yBase,'xkcd:apple green',linestyle=':',label='Baseline')
                        except KeyError:
                            err('No baseline information for {}'.format(plotCol),LOG_LEVEL_0)
                    plt.title(pltTitle)
                    plt.ylim([plotMin,plotMax])
                    plt.xlabel(xLabel)
                    plt.ylabel(pltTitle + " " + unit)
                    plt.grid()
                    plt.legend()
                    figName = os.path.join(out_dir,"{}.png".format(plotCol))
                    log("Saving : {0}".format(figName), LOG_LEVEL_3)
                    if os.path.exists(figName):
                        os.remove(figName)
                    plt.savefig(figName)
                    plt.close()
                except ValueError:
                    err("Non-numeric data or range, not plotting {}".format(col),LOG_LEVEL_0)

def err(message, level):
    print( "{}\n".format(message) if _log_verbose >= level else "" , end="", file=sys.stderr)
def log(message, level):
    print( "{}\n".format(message) if _log_verbose >= level else "" , end="")
    
if __name__ == "__main__":
    # execute only if run as a script
    main( sys.argv )
