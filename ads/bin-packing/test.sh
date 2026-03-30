#!/bin/bash

# Build the program
make

# Loop over all text<n>.txt files in the current directory
for file in test*.txt; do
    # Check that it actually matches files
    [ -e "$file" ] || continue

    echo "---Running main.exe on $file---"
    ./main "$file" -mode fullSearch
done
