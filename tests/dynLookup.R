library(RXQuery)

foo =
function(x)
{
   cat("In foo: called with ", x, "\n")
   3 * x
}

xquery(' 1 + R:rpois(1, 5) + R:foo(4) ', # rnorm,  # force the registration of the external module.
        prefix = 'declare namespace R = "urn:R";
                  declare function R:rpois($n, $lambda) external;
                  declare function R:foo($x) external;',
       registerModule = TRUE)
