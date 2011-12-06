\name{loadDocument}
\alias{loadDocument}
\title{Load one or more documents into a Collection via the Zorba XML manager}
\description{
  This function loads one or more documents into the XML manager
  as Zorba Item objects. These can then be assembled into
  a collection, for example, and used in a query.
}
\usage{
loadDocument(doc, asText = rep(NA, length(doc)), isURI = rep(NA, length(doc)), xmlManager = xmlDataManager())
}
\arguments{
  \item{xmlManager}{the XML manager instantace associated with the Zorba
  instance in which we want to use the documents.}
  \item{doc}{a character vector or list giving the documents.}
  \item{asText}{a logical vector that indicates whether each element in
    \code{doc} is the name of a file or document, or the contents of the
    document itself.
    We will probably enhance this to look for AsIs objects and treat
    them as content and regular strings as file/URL names}
  \item{isURI}{a logical vector indicating whether each element is a URI
    or a regular file name. This is combined with \code{asText}.}
}
\value{
  A zorba::ItemList in which each object is an Item object,
  a reference to an C++-level object.
}
\references{
XQuery \url{http://www.w3.org/TR/xquery/}

Zorba \url{http://www.zorba-xquery.com/}  
}
\author{
Duncan Temple Lang
}

\seealso{
  mgr = xmlDataManager()
    # only the first 9 senate bills.
  billsDir = system.file("sampleDocs", "bills", package = "RXQuery")
  filenames = list.files(billsDir, pattern = "s[1-9].*.xml", full.names = TRUE)  
  items = loadDocument(mgr, filenames)
  col = createCollection(mgr, "http://www.govtrack.org/bills")
  q = 'for $ti in collection("http://www.govtrack.org/bills")/*/introduced/@datetime
      return $ti;'

  e = xquery(q)  
}
\examples{

}
\keyword{IO}
\keyword{programming}

