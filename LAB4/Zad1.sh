awk '{
    # Rozdzielenie pierwszego pola (czas) na składowe
    split($1, time, "-")
    ss = time[1]
    mm = time[2]
    hh = time[3]
    
    # Konwersja do formatu 12-godzinnego z am/pm
    if (hh == 0) {
        hh_12 = 12
        period = "am"
    } else if (hh < 12) {
        hh_12 = hh
        period = "am"
    } else if (hh == 12) {
        hh_12 = 12
        period = "pm"
    } else {
        hh_12 = hh - 12
        period = "pm"
    }
    
    # Usunięcie wiodącego zera z godziny
    hh_12 = hh_12 + 0
    
    # Wypisanie przekonwertowanego czasu i reszty pól
    printf "%d:%s %s", hh_12, mm, period
    for (i = 2; i <= NF; i++) {
        printf " %s", $i
    }
    printf "\n"
}' P.txt