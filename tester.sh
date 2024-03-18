#!/bin/bash

FILE="a.out"

if [ $# -eq 1 ]; then
    FILE="$1"
fi

./ft_nm "$FILE" > nm1
nm      "$FILE" > nm2

diff nm1 nm2