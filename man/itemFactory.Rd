\name{itemFactory}
\alias{itemFactory}
\alias{zorba::Item-class}
\alias{zorba::ItemFactory-class}
\alias{zorba::ItemList-class}

\alias{xmlRoot,ZorbaXMLDocument-method}
\alias{xmlName,ZorbaXMLElementNode-method}
\alias{xmlRoot,ZorbaXMLElementNode-method}
\alias{coerce,ZorbaXMLElementNode,XMLNode-method}
\alias{coerce,ZorbaXMLElementNode,XMLInternalElementNode-method}

\title{Create a Zorba item factory reference}
\description{
 This function retrieves a reference to a/the Item factory for the
 specified Zorba engine instance.
}
\usage{
itemFactory(eng = zorba())
}
\arguments{
 \item{eng}{the Zorba engine instance}
}
\value{
The \code{zorba::ItemFactory} object.
}
\references{
}
\author{
Duncan Temple Lang
}

\seealso{
  \code{\link{loadDocument}}
}
\examples{

}
\keyword{IO}
\keyword{programming}
