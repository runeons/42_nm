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
    # "/usr/bin/docker-proxy"
    "./bin/ft_nm"
    "./bin/easy32"
    "./bin/easy_test"
    "./bin/header"
    "./bin/header_and_prog_copy"
    "./bin/header_offset_error"
    "./bin/not_so_easy_test32.o"
    # "./bin/not_so_easy_test64_obj.o"
    # "./bin/unterminated_string"
    # "./bin/easy64"
    # "./bin/error_header"
    # "./bin/header_and_prog"
    # "./bin/header_cop"
    # "./bin/not_so_easy_test"
    # "./bin/not_so_easy_test32_obj.o"
    # "./bin/not_so_easy_test.owrong_arch"
)

# FILES+=(./bin/*)


if [ $# -eq 1 ]; then
    compare "$1"
else
    for FILE in "${FILES[@]}"; do
        compare "$FILE"
    done
fi