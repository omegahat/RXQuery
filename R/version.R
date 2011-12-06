zorbaVersion =
function()
{
   structure(.Call("R_zorba_version"), names = c("major", "minor", "patch"))
}