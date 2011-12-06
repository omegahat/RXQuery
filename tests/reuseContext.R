library(RXQuery)

wc = function(x, static, dynamic)
{
 print("in wc")
 print(x)
   10L
}


z = zorba()

ctxt = makeContext(z)
module = createModule(, ctxt)
x = registerFunctions(module, , "nchar", rnorm, owc = wc, .deterministic = c(TRUE, TRUE, FALSE))


query = 'declare namespace R = "urn:R" ; declare function R:owc($x) external; R:owc(100)'

i = xquery(query, ctxt = ctxt)
i = xquery(query, ctxt = ctxt)
i = xquery(query, ctxt = ctxt)
