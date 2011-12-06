library(RXQuery)

zorba()

#mgr = xmlDataManager()

billsDir = system.file("sampleDocs", "bills", package = "RXQuery")
filenames = list.files(billsDir, pattern = "s.*.xml", full.names = TRUE)
items = loadDocument(filenames)

id = "http://www.govtrack.org/bills"
# once we create the collection, 
col = createCollection(id)[[1]]

addNodes(col, .nodes = items)


q = 'for $ti in collection()/*/introduced/@datetime
      return $ti;'

xq = compileXQuery(q)
ctxt = makeDynamicContext(xq, new("zorba::DynamicContext"))
#xquery(q, ctxt = ctxt)
setDefaultCollection(ctxt, id)

ans = getQueryResults(xq)

deleteCollection(col)
rm(col)
gc()  # need to force the deletion


