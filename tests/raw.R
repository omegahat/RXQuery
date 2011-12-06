# R CMD SHLIB query.cpp marshal.cpp extFun.cpp


library(RXQuery)

if(FALSE) {
  .Call("R_executeXQuery", "1 + 2", character())
}

trace(nchar, quote(print(x)))
trace(rnorm, quote(print(n)))
query = 'declare namespace R = "urn:R" ; declare function R:nchar($x) external; declare function R:rnorm($n) external ; R:nchar("a string with 27 characters") + R:rnorm(1)'

i = xquery(query, rnorm, "nchar")
print(i)
