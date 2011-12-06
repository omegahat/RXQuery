library(RXQuery)
zorba()

billsDir = system.file("sampleDocs", "bills", package = "RXQuery")

 #XXX this doesn't make sense. It is a doc
if(FALSE) {
qry = 'for $doc in collection("file:///Users/duncan/Data/GovTrack/bills/h1.xml")
       let $x := 1
        return
         <intro>{$doc/introduced/@datetime}</intro>'

a = xquery(qry)


###########

qry = sprintf('for $ti in doc("file://%s/h1.xml")//title
        return
         <intro>{$ti/text()}</intro>', billsDir)
xquery(qry)

##################

 # Just the first 9 bills
f = list.files(billsDir, pattern = "h[1-9].xml",  full.names = TRUE)
library(XML)
top = newXMLNode("catalog")
invisible(sapply(paste("file:", f, sep = "//"), function(f) newXMLNode("doc", attrs = c(href = f), parent = top)))
saveXML(top, "catalog.xml")

qry = 'import module namespace coll="http://www.zorba-xquery.com/zorba/collection-functions";
        for $doc in coll:import-catalog("catalog.xml")
          return
          <intro>{$doc/introduced/@datetime}</intro>'

print(xquery(qry))


