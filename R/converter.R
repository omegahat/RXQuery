XSDTimeFormats = c(dateTime = "%Y-%m-%dT%H:%M:%S",
                   date = "%Y-%m-%d",
                   time = "%H:%M:%S")

zorbaPOSIXct =
function(val, format = "dateTime", fmt = XSDTimeFormats[format])
{
  as.POSIXct(strptime(val, fmt))
}

