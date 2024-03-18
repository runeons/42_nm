#!/bin/bash

compare()
{
    FILE="$1"
    echo -e "\e[32mTesting $FILE ...\e[0m"
    ./ft_nm "$FILE" > nm1
    nm "$FILE" > nm2
    diff nm1 nm2
}

FILES=(
    "./bin/easy_test"
    "./bin/error_header"
    "./bin/header"
)

if [ $# -eq 1 ]; then
    compare "$1"
else
    for FILE in "${FILES[@]}"; do
        compare "$FILE"
    done
fi