zorbaNodeKinds =
 c( anyNode        = 0,
    documentNode   = 1,
    elementNode    = 2,
    attributeNode  = 3,
    textNode       = 4,
    piNode         = 5,
    commentNode    = 6)

getNodeKind =
function(item, ...)
{
  ans = .Call("R_Item_getNodeKind", item)
  names(ans) = names(zorbaNodeKinds)[match(ans, zorbaNodeKinds)]
  ans
}

convertItemToR =
function(item, ...)
{
  .Call("R_Item_convertItemToR", item)
}


if(!isGeneric("getStringValue"))
   setGeneric("getStringValue", function(x, ...) standardGeneric("getStringValue"))

setMethod("getStringValue", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_getStringValue", x)
})


setAs("zorba::Item", "character",
       function(from)
           getStringValue(from))

if(!isGeneric("isAtomic"))
   setGeneric("isAtomic", function(x, ...) standardGeneric("isAtomic"))

setMethod("isAtomic", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_isAtomic", x)
})

if(!isGeneric("isNaN"))
   setGeneric("isNaN", function(x, ...) standardGeneric("isNaN"))

setMethod("isNaN", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_isNaN", x)
})

if(!isGeneric("isNull"))
   setGeneric("isNull", function(x, ...) standardGeneric("isNull"))

setMethod("isNull", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_isNull", x)
})

if(!isGeneric("isNode"))
   setGeneric("isNode", function(x, ...) standardGeneric("isNode"))

setMethod("isNode", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_isNode", x)
})


if(!isGeneric("getNodeName"))
   setGeneric("getNodeName", function(x, ...) standardGeneric("getNodeName"))

setMethod("getNodeName", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_getNodeName", x)
})

if(!isGeneric("getPrefix"))
   setGeneric("getPrefix", function(x, ...) standardGeneric("getPrefix"))

setMethod("getPrefix", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_getPrefix", x)
})


if(!isGeneric("getNamespace")) {
   setGeneric("getNamespace", function(name, ...) standardGeneric("getNamespace"))
   setMethod("getNamespace", "ANY",
              function(name, ...)
                  base::getNamespace(name))
}

setMethod("getNamespace", "zorba::Item",
function(name, ...)
{
  .Call("R_Item_getNamespace", name)
})


if(!isGeneric("getLocalName"))
   setGeneric("getLocalName", function(x, ...) standardGeneric("getLocalName"))

setMethod("getLocalName", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_getLocalName", x)
})

if(!isGeneric("getChildren"))
   setGeneric("getChildren", function(x, ...) standardGeneric("getChildren"))

setMethod("getChildren", "zorba::Item",
function(x, ...)
{
  .Call("R_Item_getChildren", x)
})




if(FALSE) {

setMethod("xmlAttrs", "zorba::Item",
function(node, addNamespace = FALSE, ...)
{
  .Call("R_Item_getAttributes", node, addNamespace)
})



setMethod("xmlRoot", "ZorbaXMLDocument",
           function(x, skip = TRUE, ...) {
             getChildren(x)[[1]]
           })


setMethod("xmlName", "ZorbaXMLElementNode",
           function(node, full = FALSE) {
               nm = getNodeName(node)
               if(full)
                 sprintf("%s:%s", getPrefix(node), nm)
               else
                 nm
           })
} else {
`xmlAttrs.zorba::Item` = xmlAttrs.ZorbaXMLElementNode =
function(node, addNamespace = FALSE, ...)
{
  .Call("R_Item_getAttributes", node, addNamespace)
}
xmlRoot.ZorbaXMLDocument =
function(x, skip = TRUE, ...) {
  getChildren(x)[[1]]
}

xmlName.ZorbaXMLElementNode =
function(node, full = FALSE) {
    nm = getNodeName(node)
    if(full)
      sprintf("%s:%s", getPrefix(node), nm)
    else
      nm
}
}



if(FALSE) 
setMethod("names", "ZorbaXMLElementNode",
           function(x, skip = TRUE, ...) {
              sapply(getChildren(x), xmlName)
           })



