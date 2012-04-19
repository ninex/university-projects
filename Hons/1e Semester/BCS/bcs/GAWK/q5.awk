#BEGIN {teks = "";teks2 = ""}
{n=0}
/START/ {teks = $0; n = 1}
/FINAL/ {teks = teks "\n" $0; n = 1}
n==0 {teks2 = teks2 "\n" $0;n=0}
END {print teks,teks2}
