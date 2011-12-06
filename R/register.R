createModule =
function(urn = "urn:R", ctxt = makeContext(zorba()), e = new.env(),
          obj = new("RExternalModule"), register = !missing(ctxt))
{
  ans = .Call("R_createRModule", as.character(urn), e, NULL, obj)
  if(register) 
     .Call("R_registerZorbaModule", ctxt, ans)
  
  ans
}



#
# Need an item factory. 
#

itemFactory =
function(eng = zorba())
{
  .Call("R_getItemFactory", eng, PACKAGE = "RXQuery")
}

registerFunctions =
function(module, factory = itemFactory(), ..., .funcs = list(...),
          .deterministic = rep(TRUE, length(.funcs)), fix = length(names(.funcs)) == 0 || any(names(.funcs) == ""))
{
   force(module)
  
    #XXX See fixNames in xquery.R
 if(length(.funcs) > 0) {
  if(fix)
    .funcs = fixNames(.funcs, match.call(), names(.funcs), c("module", "factory", ".funcs", ".deterministic", ".ids"))

  e = getEnvironment(module)
  
  types = rep("RExternalFunction", length(.funcs))
  types[sapply(.funcs, is, "function")] = "RClosureExternalFunction"
  
  mapply(function(id, func, deterministic, type) {
           obj = new(type)

           f = .Call("R_createExternalFunction", id, func, deterministic, factory, obj)
           assign(id, f, e)
           f
         }, names(.funcs), .funcs, .deterministic, types)
 }
}

if(!isGeneric("getEnvironment"))
  setGeneric("getEnvironment",
                 function(from, ...)
                     standardGeneric("getEnvironment"))

setMethod("getEnvironment", "RExternalModule",
           function(from, ...)
               .Call("R_RExternalModule_getEnvironment", from))
