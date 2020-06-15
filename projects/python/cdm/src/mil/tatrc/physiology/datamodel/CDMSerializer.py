from functools import wraps
from threading import RLock
import logging
def lock_for_object(obj, locks={}):
    return locks.setdefault(id(obj), RLock())

def synchronized(call):
    assert call.__code__.co_varnames[0] in ['self', 'cls']
    @wraps(call)
    def inner(*args, **kwds):
        with lock_for_object(args[0]):
            return call(*args, **kwds)
    return inner

class CDMSerializer(object):
    """ generated source for class CDMSerializer """
    def __init__(self):
        """ generated source for method __init__ """

    jaxb = None
    ATOMIC_PKGS = None
    ELEMENT_PKGS = None
    workingDir = "."

    @classmethod
    def main(cls, args):
        """ generated source for method main """
        if len(args):
            Log.error("Must provide at least one input xml file to read")
            System.exit(1)
        
        for file_ in args:
            obj = CDMSerializer.readFile(file_)
            if obj != None:
                logging.info(args[0] + " is a valid xml file for " + obj.__class__.__name__)



if __name__ == '__main__':
    import sys
    CDMSerializer.main(sys.argv)

