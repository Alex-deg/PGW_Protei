#!/bin/bash

cur_day=$(date +"%d")

file_cdr="../Data/CDR_journal.csv"
file_log="../Data/pgw.log"

cdr_last_day=$(tail -n 1 ${file_cdr} | awk -F',' '{print $1}' | awk '{print $3}')
log_last_day=$(tail -n 1 ${file_log} | grep -oP '\[\K\d{4}-\d{2}-\d{2}' | awk -F'-' '{print $3}')

echo "log_last day = ${log_last_day}"
echo "cdr_last day = ${cdr_last_day}"

if ((cur_day != log_last_day));
then 
    echo > ${file_log}
fi

if ((cur_day != cdr_last_day))
then
    first_string=$(head -n 1 ${file_cdr})
    echo "first string in cdr = ${first_string}"
    echo ${first_string} > ${file_cdr}
fi