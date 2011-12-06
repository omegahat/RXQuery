\name{getNodeKind}
\alias{getNodeKind}
\alias{getNodeKind}
\alias{convertItemToR}
\alias{getStringValue}
\alias{isNaN}
\alias{isNull}
\alias{getChildren}
\alias{getNodeName}
\alias{getNamespace}
\alias{getLocalName}
\alias{isAtomic}
\alias{isNode}

\alias{getStringValue,zorba::Item-method}
\alias{isNaN,zorba::Item-method}
\alias{isNull,zorba::Item-method}
\alias{getChildren,zorba::Item-method}
\alias{getNodeName,zorba::Item-method}
\alias{getNamespace,zorba::Item-method}
\alias{getLocalName,zorba::Item-method}
\alias{isAtomic,zorba::Item-method}
\alias{isNode,zorba::Item-method}

\alias{getNamespace,ANY-method}

\alias{xmlAttrs,zorba::Item-method}

\alias{coerce,zorba::Item,character-method}

\alias{getPrefix}
\alias{getPrefix,zorba::Item-method}

\title{Functions for working with zorba::Item objects}
\description{
  These functions provide the R-level access to the
  C++-level methods in an zorba::Item object.
}
\usage{
getNodeKind(item, ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{item}{the zorba::Item object which is a reference to the
    C++-level object.}
  \item{\dots}{additional parameters for methods}
}
\value{

}
\references{
\url{http://www.zorba-xquery.com/doc/zorba-latest/cxx/html/classzorba_1_1Item.html}
}
\author{
Duncan Temple Lang
}
\seealso{
  \url{http://www.zorba-xquery.com/doc/zorba-latest/cxx/html/classzorba_1_1Item.html}
}
\examples{

library(RXQuery)
doc = loadDocument(system.file("sampleDocs", "bills", "s11.xml", package = "RXQuery"))
getNodeKind(doc[[1]])
isNode(doc[[1]])
top = getChildren(doc[[1]])[[1]]
getNodeKind(top)
getNodeName(top)
isAtomic(top)
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{IO}
\keyword{programming}
