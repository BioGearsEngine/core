# TODO

# REGEXES
.*m_(.*);

if \(in.Has(.*)\(\)\)\s+\n\s+UnMarshall.*
if \(in.Has(.*)\(\)\) \{\s+\n\s+io::Property::UnMarshall.*;\s+\n\s+}
if \(in.Has(.*)\(\)\) \{\s+\n\s+UnMarshall.*;\n\s+}
---
CDM_PROPERTY_UNMARSHAL_HELPER(in,out,$1)
io::Property::Marshall(in.$1(), out.Get$1());
source.Get$1().SetValue( 1.0);
&& ((m_$1&& rhs.m_$1) ? m_$1->operator==(*rhs.m_$1) : m_$1 == rhs.m_$1)


# Iterator Equivilance Check
  equivilant &= m_testSuite.size() == rhs.m_testSuite.size()
  if (equivilant) {
    for (auto i = 0; equivilant && i < m_Children.size(); ++i) {
      equivilant &= (m_Children[i] && rhs.m_Children[i])
        ? m_Children[i]->operator==(*rhs.m_Children[i])
        : m_Children[i] == rhs.m_Children[i];
    }
  }
# Override operator==

    auto ptr = dynamic_cast<decltype(this)>(&rhs);
    return (ptr) ? this->operator==(*ptr) : false;

# regular operator==

if (this == & rhs){
    return true;
}

return ...