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
                pass



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

