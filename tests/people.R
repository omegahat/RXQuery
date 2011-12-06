# Note sure what this is testing.
library(RXQuery)
z = zorba()
mgr = xmlDataManager()
if(file.exists("/Users/duncan/Data/GovTrack/people.xml.1"))
  o = loadDocument("/Users/duncan/Data/GovTrack/people.xml.1")

ctxt = makeContext(z)

xq = compileXQuery("1 + 2", ctxt)



