\name{compileQuery}
\alias{compileXQuery}
\alias{compileXQuery,character,missing-method}
\alias{compileXQuery,character,zorba::StaticContext-method}

\alias{printPlan}

\title{Compile an XQuery expression/script}
\description{
  %XXX
  These functions compile an XQuery expression into an object that
  can be evaluated and results obtained.
  \code{compileQueryCtx} allows the caller to specify the static context
  for the query.

  Neither of these is currently used in other functions in the package.    
  We tend to use \code{\link{xquery}} directly rather than
  explicitly compiling the queries and then evaluating them
  and getting the results as separate actions.

  \code{printPlan} returns a string containing the XML description of
  the evaluation plan for the query.
}
\usage{
compileXQuery(query, ctxt, engine = zorba(), ..., obj = new("zorba::XQuery"))
printPlan(xquery, parse = TRUE)
}
\arguments{
  \item{query}{a string giving the XQuery script}
  \item{engine}{the Zorba engine in which the query is to be compiled}
  \item{obj}{the object that is to be returned, allowing the caller to
    specify the class. The function fills in the \code{ref} slot.}
  \item{ctxt}{a zorba::StaticContext for the XQuery to use}
%  \item{doc}{}
\item{\dots}{additional parameters for methods}
\item{xquery}{the compiled query}
   \item{parse}{a logical value indicating whether to parse the
  resulting XML text from \code{printPlan} as an XML document}
%  \item{funcs}{}
%  \item{prefix}{}
%  \item{ns}{}
}
\value{
  The filled in \code{obj}, i.e. with the \code{ref} slot filled in.
  By default, this corresponds to a \code{zorba::XQuery} object.
}
\references{
XQuery \url{http://www.w3.org/TR/xquery/}

Zorba \url{http://www.zorba-xquery.com/}
}
\author{
Duncan Temple Lang.
}
\seealso{
 \code{\link{xquery}}
}
\examples{
zorba()
  xq = compileXQuery("1+2")
#  printPlan(xq)
  xq = compileXQuery("(1+2, 1-2)")
#  printPlan(xq)
}
\keyword{IO}

