library(RXQuery)

z = zorba()

ctxt = makeContext(z)
module = createModule(, ctxt)
e = getEnvironment(module)
objects(e)


registerFunctions(module, , "nchar", rnorm)
objects(e)


trace(nchar, quote(print(x)))
trace(rnorm, quote(print(n)))
query = 'declare namespace R = "urn:R" ; declare function R:nchar($x) external; declare function R:rnorm($n) external ; R:nchar("a string with 27 characters") + R:rnorm(1)'

i = xquery(query, ctxt)
print(i)
