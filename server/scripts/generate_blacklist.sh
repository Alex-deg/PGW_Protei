#!/bin/bash

num=21
count=$(($(od -An -N1 -i /dev/random)%$num))

#echo "Количество = ${count}"

cd "../Data/"

num=10

if ! (find ./ -maxdepth 1 -name "IMSI_blacklist.txt") 
then
    #echo "Создание файла"
    touch IMSI_blacklist.txt
fi

echo -n "" > IMSI_blacklist.txt

for (( i=1; i <= $count; i++ ))
do
    for(( j=1; j <= 15; j++))
    do
        echo -n $(($(od -An -N1 -i /dev/random)%$num)) >> IMSI_blacklist.txt
    done
    echo -e "" >> IMSI_blacklist.txt
done
