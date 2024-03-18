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
    "./bin/not_so_easy_test64_obj.o"
    "./bin/easy64"
    "./bin/easy32"
    "./bin/easy_test"
    "./objs/check.o"
    "./objs/cmp_names.o"
    "./objs/cmp_types.o"
    "./objs/debug.o"
    "./objs/display.o"
    "./objs/exit.o"
    "./objs/init_options.o"
    "./objs/nm.o"
    "./objs/sort_syms.o"
    "./objs/store_sym.o"
    "./objs/type_sym.o"
    "./objs/utils_options.o"
    "./bin/not_so_easy_test32.o"
    "./bin/not_so_easy_test32_obj.o"

    # TO CLEAN
    # "./bin/header_offset_error"
    # "./bin/error_header"
    # "/usr/bin/docker-proxy"

    # TO FIX
)

# FILES+=(./bin/*)


if [ $# -eq 1 ]; then
    compare "$1"
else
    for FILE in "${FILES[@]}"; do
        compare "$FILE"
    done
fi