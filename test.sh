#!/bin/bash

FILE="a.out"

if [ $# -eq 1 ]; then
    FILE="$1"
fi

./ft_nm "$FILE" > nm1
nm      "$FILE" > nm2

awk '{print $2, $3}' nm1 > nm1_clean
awk '{print $2, $3}' nm2 > nm2_clean

diff nm1_clean nm2_clean
rm nm1_clean nm2_clean