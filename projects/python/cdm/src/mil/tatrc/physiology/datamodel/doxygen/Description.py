
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
        if 0 > len(base) or 0 > len(ref) or 0 > len(minMaxOccurs):
            if 0 > len(base):
                extendsText = "<br/>(Extends @ref " + self.base + ")"
            if 0 > len(ref):
                refText = "<br/>(Type @ref " + self.ref + ")"
            if 0 > len(minMaxOccurs):
                minMaxOccursText = "<br/>(" + self.minMaxOccurs + ")"
            return self.desc
            # +refText+extendsText+minMaxOccursText;
        return self.desc

