\name{zorba}
\alias{zorba}
\alias{xmlDataManager}
\alias{zorba::XMLDataManager-class}
\alias{Zorba-class}
\title{Get zorba engine or Zorba XML data manager}
\description{
  These functions allow us to get access to an instance of the 
  Zorba engine or to the XML single data manager within that engine.
}
\usage{
zorba(obj = new("Zorba"))
xmlDataManager(eng = zorba(), obj = new("zorba::XMLDataManager"))
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{obj}{the R object that will be returned. This is a parameter so
    that the caller can specify the class of the result.  The C code
    fills in the \code{ref} slot of this object.}
  \item{eng}{the instance of the Zorba engine whose XML data manager is
    being requested.}
}
\value{
  The filled in \code{obj} object.
}
\references{
  \url{http://www.zorba-xquery.com}
}
\author{
Duncan Temple Lang
}

\seealso{

}
\examples{
  z = zorba()
  mgr = xmlDataManager(z)


  mgr = xmlDataManager(z)
}
\keyword{IO}


