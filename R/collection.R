setGeneric("addNodes",
            function(collection,  ..., .nodes = list(...))
                standardGeneric("addNodes"))

setMethod("addNodes", c("zorba::Collection", .nodes = "zorba::ItemList"),
            function(collection,  ..., .nodes = list(...))
{
  invisible(.Call("R_Collection_addNodes", collection, .nodes))
})

setGeneric("getName",
            function(x, ...)
             standardGeneric("getName"))

setMethod("getName", "zorba::Collection",
            function(x, ...)
               .Call("R_Collection_getName", x))



setGeneric("size",
            function(x, ...)
             standardGeneric("size"))

setMethod("size", "zorba::Collection",
            function(x, ...)
               .Call("R_Collection_size", x))

setMethod("length", "zorba::Collection",
            function(x)
               size(x))


getCollection =
function(colNames, mgr = xmlDataManager())
{
  if(!is(mgr, "zorba::XMLDataManager"))
     stop("need an XML data manager")
     
  .Call("R_Manager_getCollections", mgr, as.character(colNames))
}
