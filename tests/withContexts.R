library(RXQuery)

z = zorba()

ctxt = makeContext(z)
module = createModule(, ctxt)
e = getEnvironment(module)
objects(e)

wc = function(x, static, dynamic)
{
 print("in wc")
 print(x)
   10L
}

x = registerFunctions(module, , "nchar", rnorm, owc = wc, .deterministic = c(TRUE, TRUE, FALSE))
print(sapply(x, class))
objects(e)

#query = 'declare namespace R = "urn:R" ; declare function R:nchar($x) external; declare function R:rnorm($n) external ; declare function R:owc($x) external; R:nchar("a string with 27 characters") + R:rnorm(1) + R:owc(100)'

query = 'declare namespace R = "urn:R" ; declare function R:owc($x) external; R:owc(100)'

i = xquery(query, ctxt = ctxt)
print(i)
