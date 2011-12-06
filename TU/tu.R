library(RGCCTranslationUnit)

tu = parseTU("zorba.cpp.001t.tu")

# Check namespace_decl is working on scpe field.
#  RGCCTranslationUnit:::getNodeName(tu[[1280]])

classes = getClassNodes(tu)

classes = classes[ grep("^zorba", names(classes)) ]

names(classes) = gsub("zorba::", "", names(classes))


#########################

XmlDataManager = resolveType(classes[["XmlDataManager"]], tu)
m = getClassMethods(classes[["XmlDataManager"]])
# Error in node[["INDEX"]]
mm = resolveType(m, tu)

v = resolveType(classes[["Version"]], tu)
vm = getClassMethods(classes[["Version"]])
rvm = resolveType(vm, tu)
sapply(rvm, class)  # All static methods.

names(v@fields)
sapply(v@fields, slot, "access")
####################


classDefs = lapply(classes, function(x) {
                     tmp = resolveType(x, tu)
                     #tmp@methods = resolveType(getClassMethods(x), tu)
                   })

Rprof("tu.prof")
methods = lapply(classes, function(x) {
	                     print(x)
                             resolveType(getClassMethods(x), tu)
                          })
Rprof(NULL)



##
bind = RGCCTranslationUnit:::createClassBindings(classes[["DynamicContext"]], tu, resolvedMethods = methods)
