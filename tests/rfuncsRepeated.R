library(RXQuery)

z = zorba()

ctxt = makeContext(z)
module = createModule(, ctxt)

registerFunctions(module, , bob = function(a) a + 10L)

query = 'declare namespace R = "urn:R" ; declare function R:bob($x) external ; R:bob(1)'
i = xquery(query, ctxt = ctxt)
print(i)

####

ctxt = makeContext(z)
module = createModule(, ctxt)
registerFunctions(module, , jane = function(a) a + 10L)
query = 'declare namespace R = "urn:R" ; declare function R:jane($x) external ;R:jane(1)'

i = xquery(query, ctxt = ctxt)
print(i)

