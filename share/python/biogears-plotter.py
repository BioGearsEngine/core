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


_output_directory = None;
_root_directory   = None
def main( args):
    global _log_verbose
    global _root_directory
    global _output_directory
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', help="Adjust the log verbosity.", dest='verbosity', default=0, action='count' )
    parser.add_argument('--version', action='version', version='%(prog)s 1.0')
    parser.add_argument('-r', help="recursivly plot directories", dest='recurse', action='store_true')
    parser.add_argument('--root',
        dest='root', action="store", 
        help="Set an alternative root dir", default="")
    parser.add_argument('--outdir',
        dest='outdir', action="store", 
        help="Optional Output directory instead of inlinline layout ")
    parser.add_argument('scenario',
        nargs="+", action="store",
        help="One or more scenario files or directories. ")
    parser.add_argument('--skip', 
        dest='skip_count', action="store", default=5,
        help="Number of lines to skip for faster plotting")

    args = parser.parse_args()
    _log_verbose = args.verbosity

    log("Log level set to {0}".format(_log_verbose), LOG_LEVEL_1)

    _root_directory = args.root
    _output_directory = args.outdir if args.outdir else args.root
    log("INPUT = {}".format(_root_directory),LOG_LEVEL_2)
    log("OUTPUT = {}".format(_output_directory),LOG_LEVEL_2)
    log("Recursive Mode = {}".format(args.recurse),LOG_LEVEL_2)
    
    log ("Processing",LOG_LEVEL_1)
    run_plots( args.root, args.scenario, args.skip_count, args.recurse )
    

def run_plots(root_dir, sources, skip_count, recurse):
    global _log_verbose
    log ("run_plots({0},{1},{2},{3})".format(root_dir, sources, skip_count, recurse),LOG_LEVEL_3)
    for scenario in sources:
        possible_scenario = os.path.normpath(os.path.join(root_dir,scenario))
        if( os.path.isdir(possible_scenario) ):
         if recurse:
            for file in os.listdir(possible_scenario):
                    run_plots( possible_scenario,[file],skip_count,recurse)
         else:
            err("{0} is a directory.\n\t Use -r for recursive mode.".format(possible_scenario),LOG_LEVEL_0)
        elif( os.path.exists(possible_scenario)):
            plot(root_dir,scenario,skip_count)
        else:
            err("{0} does not exist. Unable to process".format(possible_scenario),LOG_LEVEL_0)

def plot(root_dir, source, skip_count):
    valid_regex = '[.](txt|csv)'
    resolved_path = os.path.join(root_dir,source)
    basename,extension     = os.path.splitext(resolved_path)
    log("{}{}".format(basename,extension),LOG_LEVEL_4)
    if ( not os.path.exists(resolved_path)):
        log ("plot called with non exist file. This should not occur! {0}".format(resolved_path),LOG_LEVEL_0 )    
    if ( not re.match( valid_regex, extension) ):
        log ("Skipping {0}".format(os.path.basename(resolved_path)),LOG_LEVEL_3 )
    else:
        log ("{0}".format(resolved_path),LOG_LEVEL_1)

        # verDirOut = r'D:\BioGears\verification\Plots' + '\\' + verDir
        # targetDir = verDirOut + '\\' + fileName
        #Create directory for plots ()
        dirname   =  os.path.dirname(resolved_path) 
        basename, extension = os.path.splitext(os.path.basename(resolved_path))
        outputdir = os.path.join(dirname,basename)

        #TODO: Make removal a flag always create
        if not os.path.exists(outputdir):
          os.makedirs(outputdir)
        #        os.path.isdir(outputdir):
        #    shutil.rmtree(outputdir)

        #TODO: Validity Check on Log Length

        #Read in results file
        dataCSV = pd.read_csv(resolved_path,sep=',', header=0)
        #Extract data calls (specified in xml)
        colNames = list(dataCSV)
        #Time = x-axis by default
        xData = dataCSV['Time(s)'].values[::skip_count]

        #Plot each column of data 
        for col in colNames:
            if col=='Time(s)':
                continue
            
            #Get all of the values we need
            
            yData = dataCSV[col].values[::skip_count]
            log("{0},{1}".format(skip_count, yData), LOG_LEVEL_3)
            if( not yData.size ):
                log("Error: Column is empty did you provide a non resonable skip_count?", LOG_LEVEL_0)
            else:
                yRange = max(yData)-min(yData)
                #Set plot range to +/- 10% on either side
                if yRange > 0:
                    plotMin = min(yData) - 0.1*yRange
                    plotMax = max(yData) + 0.1*yRange
                else:
                    #This means we're probably plotting a bunch of zeros...
                    plotMin = yData[0] - 5
                    plotMax = yData[0] + 5
                
                #Extract unit--assuming they are specified in parentheses
                unitIndex = col.find('(')
                if(unitIndex == -1):
                    unit = ''
                else:
                    unit = col[unitIndex:len(col)]
                    col = col.replace(unit,'')
                
                log("Plotting {}".format(col), LOG_LEVEL_2)
                
                plt.plot(xData,yData)
                plt.title(col)
                plt.ylim([plotMin,plotMax])
                plt.xlabel('Time (s)')
                plt.ylabel(col + unit)
                plt.grid()
                figName = os.path.join(outputdir,"{}.png".format(col))
                log("Saving : {0}".format(figName), LOG_LEVEL_3)
                plt.savefig(figName)
                plt.close()

def err(message, level):
    print( "{}\n".format(message) if _log_verbose >= level else "" , end="", file=sys.stderr)
def log(message, level):
    print( "{}\n".format(message) if _log_verbose >= level else "" , end="")
    
if __name__ == "__main__":
    # execute only if run as a script
    main( sys.argv )
