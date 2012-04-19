#! /bin/gawk -f
# This script searches for the occurences of the pattern GGTTAA in genomedata.txt
#BEGIN {RS = "ID"}
$1 == "DE" {VAL = $0}
NR > 8 {print "found"}


{print  VAL}
END {print NF;print NR}
