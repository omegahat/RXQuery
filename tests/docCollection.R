library(RXQuery)

zorba()

#mgr = xmlDataManager()

billsDir = system.file("sampleDocs", "bills", package = "RXQuery")
filenames = list.files(billsDir, pattern = "s.*.xml", full.names = TRUE)
items = loadDocument(filenames)

col = createCollection("http://www.govtrack.org/bills")
print(getName(col[[1]]))
print(size(col[[1]]))
addNodes(col[[1]], .nodes = items)
print(size(col[[1]]))

q = 'for $ti in collection("http://www.govtrack.org/bills")/*/introduced/@datetime
      return $ti;'

e = xquery(q)

deleteCollection(col[[1]])
col = NULL
gc()

