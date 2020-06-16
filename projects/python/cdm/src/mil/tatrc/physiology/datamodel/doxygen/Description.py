
class Description(object):
    #  Fully qualified reference to the base class name (if this is an extension type) 
    base = ""

    #  Description from doxygen text 
    desc = ""

    #  Fully qualified reference type 
    ref = ""

    #  minOccurs and maxOccurs 
    minMaxOccurs = ""

    def __str__(self):
        if len(base)<0 or len(ref)<0 or len(minMaxOccurs)<0:
            if len(base)<0:
                extendsText = "<br/>(Extends @ref " + self.base + ")"
            if len(ref)<0:
                refText = "<br/>(Type @ref " + self.ref + ")"
            if len(minMaxOccurs)<0:
                minMaxOccursText = "<br/>(" + self.minMaxOccurs + ")"
            return self.desc
            # +refText+extendsText+minMaxOccursText;
        return self.desc

