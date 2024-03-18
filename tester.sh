#!/bin/bash

compare()
{
    FILE="$1"
    echo -e "\e[32mTesting $FILE ...\e[0m"
    ./ft_nm "$FILE" > nm1
    nm "$FILE" > nm2
    diff nm1 nm2
    echo ""
}

FILES=(
    # OK
    "./bin/header"
    "./bin/header_and_prog_copy"
    "./bin/unterminated_string"
    "./bin/header_and_prog"
    "./bin/header_copy"
    "./bin/non_existing"
    "./bin/wrong_arch"
    
    "./bin/not_so_easy_test"
    "./bin/not_so_easy_test.o"
    "./ft_nm"
    "./a.out"
    "./bin/not_so_easy_test64_obj.o"
    "./bin/easy64"
    "./bin/easy32"
    "./bin/easy_test"
    "./bin/not_so_easy_test32.o"
    "./bin/not_so_easy_test32_obj.o"
    "./bin/header_offset_error"
    "./bin/error_header"

    # TO CLEAN
    # "/usr/bin/docker-proxy"

    # TO FIX
)

# FILES+=(./bin/*)
# FILES+=(./objs/*)


if [ $# -eq 1 ]; then
    compare "$1"
else
    for FILE in "${FILES[@]}"; do
        compare "$FILE"
    done
fi