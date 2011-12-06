library(RXQuery)

z = zorba()

ctxt = makeContext(z)
module = createModule(, ctxt)

registerFunctions(module, , err = function(x) stop("error from within R"))

query = 'declare namespace R = "urn:R" ; declare function R:err($x) external; R:err(1)'

i = xquery(query, ctxt = ctxt)

