#!/bin/bash

compare()
{
    FILE="$1"
    echo -e "\e[32mTesting $FILE ...\e[0m"
    ./ft_nm "$FILE" > nm1
    nm      "$FILE" > nm2
    diff nm1 nm2
    echo ""
}

compare_a()
{
    FILE="$1"
    echo -e "\e[32mTesting $FILE -a ...\e[0m"
    ./ft_nm -a "$FILE" > nm1_a
    nm      -a "$FILE" > nm2_a
    diff nm1_a nm2_a
    echo ""
}

FILES=(
    # OK
    # "./bin/header"
    # "./bin/header_and_prog_copy"
    # "./bin/unterminated_string"
    # "./bin/header_and_prog"
    # "./bin/header_copy"
    # "./bin/non_existing"
    # "./bin/wrong_arch"
    
    "./bin/not_so_easy_test"
    "./bin/not_so_easy_test64_obj.o"
    "./bin/easy64"
    "./bin/easy32"
    "./bin/easy_test"
    "./bin/not_so_easy_test32_obj.o"
    "./bin/header_offset_error"
    "./bin/error_header"

    "/usr/bin/docker-proxy"
    "./ft_nm"
    "./a.out"
)

FILES+=(./bin/*)
FILES+=(./objs/*)


if [ $# -eq 1 ]; then
    compare "$1"
else
    for FILE in "${FILES[@]}"; do
        compare   "$FILE"
        compare_a "$FILE"
    done
fi