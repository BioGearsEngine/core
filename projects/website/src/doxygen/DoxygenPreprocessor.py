#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Created on Sat Jun 27 12:20:10 2020

@author: Shashank

"""

import os
import argparse
import logging

class DoxygenPreprocessor():

    @classmethod
    def main(cls,args):
        if len(args)!=2:
            logging.error("Command arguments are : <Directory of files to process> <Directory to place processed files>")
            return
        else:
            if not os.path.isdir(args[0]):
                logging.error("Cannot find Source Directory : {0}".format(args[0]))
                return
            sourceDir=args[0]
            destDir=args[1]
            if not os.path.exists(args[1]):
                try:
                    os.mkdir(destDir)
                except IOError as e:
                    logging.warning("Unable to destination directory {0}. I might not have file permissions".format(destDir))
            try:
                for files in os.listdir(args[0]):
                    if os.path.isdir(files) or not files.endswith(".md"):
                        continue
                    print("Processing file : " + files)
                    fileprocess=os.path.join(args[0],files)
                    fileoutput=os.path.join(args[1],files)
                    with open(fileprocess,'r',encoding="iso-8859-1") as fin, open(fileoutput,'w',encoding="iso-8859-1") as fout:
                        for line in fin:
                            #print(line)
                            if line.startswith("@insert"):
                                path_file_1=line.partition("@insert")[2].strip(" ").strip("\n")
                                #print(path_file_1)
                                if not "/" in path_file_1:
                                    path_file_1=os.path.join(args[0],path_file_1)
                                if os.path.isfile(path_file_1):
                                    with open(path_file_1,'r',encoding="iso-8859-1") as fstub:
                                        for lines in fstub:
                                            fout.write(lines)
                                            #fout.write("\n")
                                    fstub.close()
                                else:
                                    print("Could not find {0} to insert into this page searched [., {0}] ".format(path_file_1))
                                    fout.write("\n")
                                    fout.write("<img src=\"./images/MissingTable.jpg\"><center><i>Could not find " + os.path.basename(path_file_1) + "</i></center><br>")
                            else:
                                fout.write(line)
                                #fout.write("\n")
                    fin.close()
                    fout.close()
            except IOError as ex:
                logging.warning(ex)
                logging.warning("An Exception has occured with the filesystem. I might not have file permissions")




if __name__=="__main__":
    import sys
    parser = argparse.ArgumentParser(description='Creation of all markdown files for generation of Documentation')
    parser.add_argument('-p','--markdownfile',help='Path to markdown or methodology files',required=True)
    parser.add_argument('-f','--Destdir',help='Path where files are needed to be pasted',required=True)
    args=parser.parse_args()
    if args.markdownfile == None or args.Destdir == None:
        parser.print_help()
        sys.exit(0)
    else:
        args=[args.markdownfile,args.Destdir]
        DoxygenPreprocessor.main(args)

