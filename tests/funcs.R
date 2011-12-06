library(RXQuery)

# Example of using an R function as an external XQuery function.
# This is called with 2 XQuery arguments and if we had access to the
# XQuery when registering the function, we would be passed this also.
#

myFun = function(i, j, ctxt = NULL) {
 print(ctxt)
 print(j)

 10*i + j
}


v = xquery('for $i in (1, 2, 3, 4) return R:myFun($i, 3)', myFun,
              addQuery = TRUE,
              prefix = ' declare namespace R="urn:R"; declare function R:myFun($i, $j) external;')


if(FALSE) {

v = xquery('for $i in (1, 2, 3, 4) return {R:myFun($i)}', myFun, addQuery = TRUE,
           prefix = ' declare namespace R="urn:www.r-project.org" declare function R:myFun($i) external')



v = xquery('for $i in (1, 2, 3, 4) return R:myFun($i)', myFun, addQuery = TRUE,
           prefix = ' declare namespace R="urn:www.r-project.org" declare function R:myFun($i) external')
}
