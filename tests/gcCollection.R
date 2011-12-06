library(RXQuery)

zorba()

#mgr = xmlDataManager()

billsDir = system.file("sampleDocs", "bills", package = "RXQuery")
filenames = list.files(billsDir, pattern = "s.*.xml", full.names = TRUE)
items = loadDocument(filenames)

id = "http://www.govtrack.org/bills"
# once we create the collection, 
col = createCollection(id)[[1]]

deleteCollection(col)
rm(col)
gc()
