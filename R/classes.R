setClass("NativeReferenceObject", representation(ref = "externalptr"))

setClass("XQueryEngine", contains = "NativeReferenceObject")
setClass("Zorba", contains = "XQueryEngine")

setClass("ZorbaObject", representation(engine = "XQueryEngine"), contains = "NativeReferenceObject")

setClass("zorba::StaticContext", contains = "ZorbaObject")
setClass("zorba::DynamicContext", contains = "ZorbaObject")  # zorba::StaticContext")

#setClass("zorba::StaticContext", contains = "NativeReferenceObject")
#setClass("zorba::DynamicContext", contains = "NativeReferenceObject")


setClass("zorba::XMLDataManager", contains = "ZorbaObject")

setClass("zorba::XQuery", contains = "ZorbaObject")


setClass("RExternalFunction", contains = "NativeReferenceObject")
setClass("RNamedExternalFunction", contains = "RExternalFunction")
setClass("RNamedAltExternalFunction", contains = "RExternalFunction")
setClass("RClosureExternalFunction", contains = "RExternalFunction")

setClass("RExternalModule", contains = "NativeReferenceObject")

setClass("zorba::Item", contains = "NativeReferenceObject")
setClass("ZorbaXMLElementNode", contains = "zorba::Item")
setClass("ZorbaXMLTextNode", contains = "zorba::Item")
setClass("ZorbaXMLPINode", contains = "zorba::Item")
setClass("ZorbaXMLAttributeNode", contains = "zorba::Item")
setClass("ZorbaXMLCommentNode", contains = "zorba::Item")
setClass("ZorbaXMLDocument", contains = "zorba::Item")

setClass("zorba::ItemFactory", contains = "NativeReferenceObject")


setClass("zorba::Collection", contains = "NativeReferenceObject")

setClass("zorba::ItemList", contains = "list")


setAs("ZorbaXMLElementNode", "XMLNode",
       function(from)
           .Call("R_zorbaXMLNode_toXMLNode", from))

setAs("ZorbaXMLElementNode", "XMLInternalElementNode",
       function(from) {
            node = as(from, "XMLNode")
            xmlRoot(xmlParse(saveXML(node), asText = TRUE))
       })

