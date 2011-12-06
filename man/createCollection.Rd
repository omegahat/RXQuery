\name{createCollection}
\alias{createCollection}
\alias{zorba::Collection-class}
\alias{size}
\alias{size,zorba::Collection-method}
\alias{getName}
\alias{getName,zorba::Collection-method}
\alias{addNodes}
\alias{addNodes,zorba::Collection,zorba::ItemList-method}
\alias{length,zorba::Collection-method}
\alias{deleteCollection}
\alias{getCollection}

\title{Work with a Zorba collection}
\description{
  A Zorba Collection object contains zero or more
  Item objects. These Item objects can "be" (i.e. contain) strings, numbers,
  URIs, XML nodes/documents, and so on. 
}
\usage{
createCollection(name, ..., .nodes = list(...), xmlManager = xmlDataManager())
deleteCollection(..., .ids = list(...), xmlManager = xmlDataManager())
getCollection(colNames, mgr = xmlDataManager())
}
\arguments{
  \item{xmlManager}{}
  \item{name}{}
  \item{\dots}{not used yet in createCollection but will allow a series of zorba::Item
    objects to be added to the sole collection being constructed.
    For deleteCollection, these are the names or collection objects
    (returned from \code{createCollection})
    identifying the collection to be deleted.
   }
   \item{.nodes}{}
   \item{.ids}{an alternative way to specify the identitifiers of the
     collections to be deleted which allows an existing list to be used
     rather than the "loose" collection of elements via \dots.}
   \item{colNames}{the names of the collection objects to retrieve from
     the data manager}
   \item{mgr}{the XML data manager}
}
\value{

}
\references{

}
\author{
Duncan Temple Lang
}



\seealso{

}
\examples{

}
\keyword{IO}
\keyword{programming}
