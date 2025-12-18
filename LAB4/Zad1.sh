awk -F'#' '{
  if (match($1, /([0-9]{2})-([0-9]{2})-([0-9]{2})/, a)) {

    sec  = a[1]
    min  = a[2]
    hour = a[3]

     if (hour > 12) {
      hour -= 12
      suffix = "pm"
    } else {
      suffix = "am"
    }

    min = sprintf("%02d", min)
    if( length($1) == 8 && hour >= 0 && hour <= 23 && min >= 0 && min <= 59 && sec >= 0 && sec <= 59)
      {
      sub(a[0], hour ":" min suffix)
      echo "here"
      }
  }
  print
}' "$1"
