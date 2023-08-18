# TODO

# REGEXES
.*m_(.*);
.*(m_.*);
---
CDM_PROPERTY_UNMARSHAL_HELPER(in,out,$1)
io::Property::Marshall(in.$1(), out.Get$1());
source.Get$1().SetValue( 1.0);
&& (($1&& rhs.$1) ? $1->operator==(*rhs.$1) : $1 == rhs.$1)