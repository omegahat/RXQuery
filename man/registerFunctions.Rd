\name{registerFunctions}
\alias{registerFunctions}
\alias{createModule}
\alias{getEnvironment}
\alias{getEnvironment,RExternalModule-method}
\title{Register R functions as external Zorba functions}
\description{
  We can use R functions  as part of Zorba queries by registering these
  R functions as external functions with the Zorba engine.
  We do this (in Zorba 1.0.0) by creating an external module
  and then associating it with a static context which
  is used in the evaluation of the Zorba query.
  The external module

  We could have an external module that looks up the
  R object by name along the R search path (or within
  a particular R environment) and that does not require
  explicitly registering the functions by name.
  This is similar to RObjectTables.
}
\usage{
registerFunctions(module, factory = itemFactory(), ...,
                  .funcs = list(...),
                   .deterministic = rep(TRUE, length(.funcs)),
                    fix = length(names(.funcs)) == 0 || any(names(.funcs) == ""))

createModule(urn = "urn:R", ctxt = makeContext(zorba()), e = new.env(), 
              obj = new("RExternalModule"), register = !missing(ctxt)) 
getEnvironment(from, ...) 
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{module}{the ExternalModule object in which these functions will
    be registered.}
  \item{factory}{the ItemFactory to use for the functions}
  \item{\dots}{the collection of functions specified individually in the
  call}
  \item{.funcs}{the collection of functions specified as a single list}
  \item{.deterministic}{a logical vector with as many elements as there
    are functions being registered. A value of \code{FALSE} indicates
    that the function should be called with two additional parameters -
    the static and dynamic Zorba context associated with the Zorba query
    being evaluated.}
  \item{fix}{a logical value indicating whether the functions need to be
    processed to determine their names.}
  \item{from}{the ExternalModule whose environment is being requested.}
  \item{urn}{the namespace for these functions}
  \item{ctxt}{the zorba::StaticContext in which to register these functions}
  \item{e}{the environment to use for the ExternalModule to house the R
     functions.}
   \item{obj}{the object that will be filled in when creating the
     ExternalModule.}
   \item{register}{whether to register the module with the specified context.}
}
\value{

}
\references{
XQuery \url{http://www.w3.org/TR/xquery/}

Zorba \url{http://www.zorba-xquery.com/}  
}
\author{Duncan Temple Lang}


\seealso{
 \code{\link{xquery}}
}
\examples{

}
\keyword{IO}

