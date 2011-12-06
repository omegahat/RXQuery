
zorba = 
  # Create the engine
function(obj = new("Zorba"))
{
  .Call("R_initZorbaEngine", obj)
}


makeContext = 
  # create an evaluation context for performing queries.
function(eng = zorba(), obj = new("zorba::StaticContext"))
{
  ans = .Call("R_createStaticContext", eng, obj, is(obj, "zorba::DynamicContext"))
  ans@engine = eng
  ans
}

makeDynamicContext =
  # From an XQuery
function(query, obj = new("zorba::DynamicContext"))
{
  .Call("R_createDynamicContext", query, obj)
}

setVariableAsDocument =
function(ctxt, var, uri, value)
{
  .Call("R_setVariableAsDocument", ctxt, var, uri, value)
}

setDefaultCollection =
function(ctxt, uri)
{
  if(!is(ctxt, "zorba::DynamicContext"))
    stop("can only set the defaul collection on a zorba::DynamicContext")

  .Call("R_context_setDefaultCollection", ctxt, if(is(uri, "zorba::Item")) uri else as.character(uri))
}


getQueryResults = 
function(query, resultSize = 100, factor = 2.)
{
  .Call("R_getQueryResults", query, as.numeric(resultSize), as.numeric(factor))
}



setGeneric("setVariable",
            function(obj, id, value) {
               standardGeneric("setVariable")
            })


setMethod("setVariable", "zorba::DynamicContext",
            function(obj, id, value) {
               .Call("R_context_setVariable", obj, as.character(id), value)
            })


setGeneric("compileXQuery",
            function(query, ctxt, engine = zorba(), ..., obj = new("zorba::XQuery"))
                standardGeneric("compileXQuery"))

setMethod("compileXQuery", c("character", "missing"),
function(query, ctxt, engine = zorba(), ..., obj = new("zorba::XQuery"))
{
  ans = .Call("R_compileQuery", as.character(query), engine, obj)
  ans@engine = engine
  ans
})


setMethod("compileXQuery", c("character", "zorba::StaticContext"),
function(query, ctxt, engine = zorba(), doc = character(), ..., obj = new("zorba::XQuery")) #, ..., funcs = list(...), prefix = character(), ns = "R")
{
  .Call("R_context_compileQuery", ctxt, query) 
})



if(FALSE)
registerFunction = 
  # Want to be able to declare these functions as well as register them
  # and so not have to declare them across queries.
function(fun, id, ns = c(R = "www.r-project.org"))
{

}


xmlDataManager = 
function(eng = zorba(), obj = new("zorba::XMLDataManager"))
{
  ans = .Call("R_get_xmlDataManager", eng, obj)
  ans@engine = eng
  ans
}

loadDocument = 
function(doc, asText = rep(NA, length(doc)), isURI = rep(NA, length(doc)), xmlManager = xmlDataManager())
{
    # Allow for a mix of URIs, verbatim text, etc.
 
  if(any(i <- is.na(isURI)))
    isURI[i] = length(grep("^[[:space:]]+(http|ftp)",  doc[i])) > 0
  
  if(!all(isURI) && any(is.na(asText)) ) {
     i = !isURI & is.na(asText)
     asText[i] = !file.exists(doc[i])
  }
  
  doc = path.expand(doc)
  
  ans = .Call("R_xmlManager_loadDocuments",  xmlManager, doc, isURI, asText)
  new("zorba::ItemList", ans)
}

createCollection =
function(name, ..., .nodes = list(...), xmlManager = xmlDataManager())
{
  ans = .Call("R_xmlManager_createCollection",  xmlManager, as.character(name))
  names(ans) = name

#  if(length(name) == 1)

  ans
}


deleteCollection =
function(..., .ids = list(...), xmlManager = xmlDataManager())
{
   i = sapply(.ids, is, "zorba::Collection")
   if(any(i))
      .ids[i] = lapply(.ids[i], getName)
   
   .Call("R_xmlManager_deleteCollections", xmlManager, unlist(.ids))
}
