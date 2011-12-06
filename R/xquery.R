
xquery =
#
#
#  We can get the functions from an option, e.g. getOption("xquery.externalFunctions") 
#  We can also compute the prefix from the functions based on the number of parameters (either required or not).
#  We might allow the caller to specify the number of parameters.
#  And the namespace declaration.
#

#function(doc, query, ..., funcs = list(...), prefix = character(), ns = "R", ctxt = NULL)

# The prefix is akin to the prolog in the XQuery specs.
#
#
# This can all be written in R rather than C++ by creating
# the query and context objects and populating them. 
# In progress. Will use the TU.
#
#
#
function(query, ..., funcs = list(...), 
         addQuery = sapply(funcs, inherits, "XQueryContextFunction"),
         prefix = character(), 
         ns = character(), # "R",
         baseURI = paste("file://", getwd(), "/", sep = ""),
         ctxt = NULL, module = createModule(, ctxt),
         .deterministic = rep(TRUE, length(funcs)),
         resultSize = 100, expandFactor = 2.,
         registerModule = length(funcs) > 0)

{
     # we want to allow the caller to specify functions with something like
     #  xquery("doc", "query", "xyz", get, rnorm, foo = function(n) rexp(n, 3))
     # and then we will compute the names as "xyz", "abc", "rnorm" and "foo"

   if(length(funcs) && (length(names(funcs)) ==  0 || any(names(funcs) == "")) ) 
      funcs = fixNames(funcs, match.call(), names(funcs), names(formals(xquery)))

   if(registerModule) {
     if(is.null(ctxt))
        ctxt = makeContext()

     registerFunctions(module, .funcs = funcs, .deterministic = .deterministic)
   }

   if(length(ns))
     ns = paste(sprintf('declare namespace %s = "%s";', names(ns), ns), collapse = "\n");

   query = paste(c(ns, prefix, query), collapse = "\n")

   tuneParams = as.numeric(c(resultSize, expandFactor))

   .Call("R_executeXQuery", as.character(query), ctxt, as.character(baseURI), tuneParams) # funcs, rep(TRUE, length(funcs)))
}


createPrefix = 
 #
 #  generate code declaring functions.
 #
function(funcs, nargs = NA, ns = c("R" = "www.r-project.org"))
{

  ans = mapply(function(id, numParams) {
                  p = paste("x", seq(length = numParams), sep = "", collapse = ", ")
                  sprintf('declare function %s:%s(%s) external', names(ns), id, p)
               }, funcs, nargs)

  if(length(ns))
     c(sprintf('declare namespace %s="urn:%s"', names(ns), ns), ans)
  else
     ans
}


fixNames = 
  # Get the names of the functions being registered from the call
  # for those that don't have explicit names.
  # We are trying to catch something like
   #  register( rnorm, "rpois")
  # and use rnorm as the name
function(funcs, call, ids, omit)
{
  types = sapply(call[-1], class)
  wh = seq(along = types)

  o = names(types) %in% omit
  types = types[!o]
  wh = wh[!o]

  syms = sapply(call[-1][wh], function(x) if(is.function(x) || is.call(x)) "" else as.character(x))

  if(length(names(funcs)) == 0)
    names(funcs) = syms
  else {
    m = names(funcs) == ""
    names(funcs)[m] = syms[m]
  }
  

  funcs
}


setGeneric("status", 
             function(x, ...)
                standardGeneric("status"))

setMethod("status", "zorba::XQuery",
          function(x, ...)
             structure(.Call("R_XQuery_status", x), names = c("closed", "debugging", "updating")))

setGeneric("setTimeout", 
             function(x, time, ...)
                standardGeneric("setTimeout"))

setMethod("setTimeout", "zorba::XQuery",
          function(x, time, ...)
             .Call("R_XQuery_setTimeout", x, as.numeric(time)))



getDocLoadingTime =
function(query, ...)
  .Call("R_XQuery_getDocLoadingTime", query)


printPlan =
function(xquery, parse = TRUE)
{
    if(!is(xquery, "zorba::XQuery"))
       stop("can only print the XQuery plan for a zorba::XQuery")
    
   ans = .Call("R_XQuery_printPlan", xquery)
   if(parse)
       xmlParse(ans, asText = TRUE)
   else
      ans
}
