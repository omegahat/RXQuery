library(RXQuery)
z = zorba()
print(z)
#mgr = xmlDataManager(z)

txt = '<books><book><title>War and Peace</title><author>Tolstoy</author></book>
        <book><title>The Pet Person</title><author>Willis</author></book>
</books>'

loadDocument(txt, asText = TRUE) # , xmlManager = mgr)

ctxt = makeContext(z)
print(ctxt)

xq = compileXQuery("1 + 2", ctxt)
# This works
#   xq = compileXQuery("1 + 2") 
getQueryResults(xq)


