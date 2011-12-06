library(RXQuery)

xquery(sprintf('doc("%s")//book', system.file("sampleDocs", "books.xml", package = "RXQuery")), nchar)

xquery('<items>{for $i in (1,2,3,4) return <item>{$i}</item>}</items>')

xquery('<items>{for $i in 1 to 10 return <item>{$i}</item>}</items>')

