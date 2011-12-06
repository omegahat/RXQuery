library(RXQuery)

  # create the engine
z = zorba()

  # create a query
qry = compileXQuery("declare variable $var external; $var//book")

  # generate a context from the query
ctxt = makeDynamicContext(qry)

  # set the document for the query.
content = '<books><book id="foo" xxx="other">Book 1</book><book>Book 2</book><book><title>Catch 22</title><author>A.N. Other</author></book></books>'
setVariableAsDocument(ctxt, "var", "books.xml", content)

# setVariable(ctxt, "foo", 10)

a = getQueryResults(qry)


library(XML)
sapply(a, class)
sapply(a, xmlName)
sapply(a, xmlAttrs)

if(FALSE) {
   sapply(a, xmlValue) # fails. Needs text values to be XMLTextNode, XMLNode, RXMLAbstractNode, XMLAbstractNode
} else
   cat("Skipping test of xmlValue\n")


