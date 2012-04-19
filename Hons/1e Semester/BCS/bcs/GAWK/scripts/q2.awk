BEGIN {count = 0}
/FINAL/ {count = count + 1}
END {print count}
