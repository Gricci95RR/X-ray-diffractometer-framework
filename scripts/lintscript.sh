#!/bin/bash

: ' 
This is a Bash script that performs linting on C++ code using the cpplint tool.
The script navigates to the "modules" directory and identifies each subdirectory as a module.
It then generates an array of module names and creates an array of exclude 
options that correspond to each modules "samples" directory.
'

echo "-- CPP Lint --"
cd modules
arrVar=()
searchFolder="./modules/"

for i in $(ls -d */)
do 
    echo "---- Detected Module: " ${i}
    arrVar+=(${i})

    excludeFolderOption+=("--exclude=${searchFolder}${i}samples/* ")
done

cppLintOptions=(--filter=-build/include_order,-build/c++11 \
                --linelength=300 \
                --counting=detailed \
                --recursive \
                --extensions=cpp \
                --headers=hpp,h \
                "${excludeFolderOption[@]}")

echo "CPPLINT OPTIONS ---> " "${cppLintOptions[@]}"

echo "---- STARTING CPP CHECK ----"
echo "---- Start search in Folder: " "${searchFolder}"
echo "----------------------------"

cd ..

py ./scripts/cpplint.py "${cppLintOptions[@]}" "${searchFolder}"
