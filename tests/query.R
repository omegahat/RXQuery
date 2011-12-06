library(RXQuery)
q = "(1+2, 1-2)"
xq = compileXQuery(q)
cat(.Call("R_XQuery_printPlan", xq))
getQueryResults(xq)
getDocLoadingTime(xq)
status(xq)

