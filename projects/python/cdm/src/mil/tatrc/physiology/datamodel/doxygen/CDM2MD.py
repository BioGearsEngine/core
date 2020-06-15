import logging
import os

class CDM2MD(object):
    @classmethod
    def main(cls, args):
        try:
            destDir = "./doc/doxygen/processed_md/"
            os.mkdir(destDir)
            fout = open(destDir + "CDMTables.md","w" , "UTF-8")
            fout.write("CDM Tables {#CDMTables}\n")
            fout.write("=======================\n")
            skipProperties = List()
            # skipProperties.add("Comment");
            # skipProperties.add("ScenarioTime");
            sce = classes("binding")
            for c in sce:
                if c.startswith("Scalar") or c == "PropertyData":
                    continue
                WriteDoxyTable(c, "CDM", writer, skipProperties)
            fout.close()
        except Exception as e:
            logging.error("Could not create directory .markdown", e)
            return


if __name__ == '__main__':
    import sys

    CDM2MD.main(sys.argv)

