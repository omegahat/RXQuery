\name{xquery}
\alias{xquery}

\title{Evaluate an XQuery expression}

\description{
 This function provides an interface to the XQuery engine
 Zorba and allows the caller to evaluate an XQuery expression(s).
 This interface also allows the caller to provide R functions
 that act as XQuery functions which can be invoked within 
 the XQuery expressions.  In other words, the R functions
 become available as regular XQuery expressions.
}

\usage{
xquery(query, ..., funcs = list(...),
        addQuery = sapply(funcs, inherits, "XQueryContextFunction"),
         prefix = character(),  ns = character(),
          baseURI = paste("file://", getwd(), "/", sep = ""), 
           ctxt = NULL, module = createModule(, ctxt),
            .deterministic = rep(TRUE, length(funcs)),
             resultSize = 100, expandFactor = 2,
            registerModule = length(funcs) > 0) 
}

\arguments{
%  \item{doc}{the XML content in which the XQuery query is evaluated}
  \item{query}{character string giving the XQuery expression/code.}
  \item{\dots}{a collection of R function objects and/or names which specifies R functions
   that are exported to XQuery and which can be used within XQuery expressions.}
  \item{funcs}{a named list of R function names and/or objects that will be registered with and made available to
  the XPath engine so that they can be called with the XPath query.}
  \item{addQuery}{No longer used!  See registerFunctions.  This now corresponds to a non-deterministic 
   function.
   a logical vector with as many elements as \code{funcs}
  which indicates whether the function needs to be passed a reference to the
  XQuery context object. A function can use this to perform computations within the XQuery engine.}
  \item{prefix}{a character vector which is used as content to place at the 
 start of the XPath expression provided in \code{query}. This is 
  intended to allow the separation of function declarations and definitions
  that are used in multiple XPath queries and are not specific to a particular query.
}
 \item{ns}{a named character vector that is used to declare namespaces in the prolog of the 
     query.}
 \item{baseURI}{the URI used to resolve relative URLs.}
 \item{ctxt}{the static context in which to evaluate this query.}
 \item{module}{if there are any functions to be registered with Zorba as
 external functions (\code{funcs}), we use this ExternalModule object to
 house and register them.}
 \item{.deterministic}{passed on to \code{\link{registerFunctions}}}
 \item{resultSize}{a guess as to how many elements (zorba::Item objects) will be returned from the query.
   This is used to allocate the R object to hold the results. The closer this is to the actual number
   the more efficient R will be. However, the package will do the correct thing regardless of the initial guess.}
 \item{expandFactor}{a number greater than 1 that is used to grow the R
   object used to collect the results from the query when we need more
   space.}
 \item{registerModule}{a logical that controls whether to register the R
   external function module with the Zorba context. When we specify R
   functions to use in the query, this is automatically done. However,
   one can force the registration of the module without specifying any
   functions to explicitly register. In that case, when the functions
   are needed, we look in the R search path (including the module's
   environment) and find the functions directly.
    We currently assume all functions found in this way are
    "deterministic" (wrt the Zorba compiler).
    In the future, we will look at the class of the object.
 }
}
%\details{}
\value{
An R object that is the result of the XQuery expression.
}
\references{
XQuery \url{http://www.w3.org/TR/xquery/}

Zorba \url{http://www.zorba-xquery.com/}
}
\author{Duncan Temple Lang}


\seealso{
 \code{\link{compileXQuery}}  
}

\examples{

xquery( "1 + 2")


  # invoke XQuery functions. This checks the conversion.
  # Check the class of these objects are POSIXt, POSIXct.

 xquery( "fn:current-dateTime()")
 xquery( "fn:current-date()")
 xquery( "fn:current-time()")

  #

query = 'declare namespace R = "urn:R" ; 
         declare function R:nchar($x) external; 
         declare function R:rnorm($n) external ; 
         R:nchar("a string with 27 characters") + R:rnorm(1)'


     # call with two R functions.
 i = xquery( query, rnorm, "nchar")


     # Specify the functions as a list with explicit names.
 i = xquery(query, funcs = list(rnorm = rnorm, nchar = "nchar"))
}
\keyword{IO}

