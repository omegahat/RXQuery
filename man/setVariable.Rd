\name{setVariable}
\alias{setVariable}
\alias{setVariableAsDocument}
\alias{setVariable,DynamicContext-method}
\alias{DynamicContext-class}
\alias{zorba::DynamicContext-class}
\alias{setVariable,zorba::DynamicContext-method}

\title{Explicitly create a Zorba variable}
\description{
  These functions and methods allow us to programmatically
   create a Zorba variable within a Zorba context.
}
\usage{
setVariable(obj, id, value)
}
\arguments{
  \item{obj}{the context in which to set the variable}
  \item{id}{the name of the Zorba variable being created}
  \item{value}{the R object to use as the value of the Zorba variable.}
}
%\details{}
\value{
}
\references{}
\author{
Duncan Temple Lang
}

\seealso{
 \code{\link{compileXQuery}}
}
\examples{

}
\keyword{IO}
\keyword{programming}
