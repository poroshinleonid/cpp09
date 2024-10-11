#!/bin/bash

# make
# ./PmergeMe `shuf -i 1-10000 -n 1000 | tr "\n" " "` > out.txt
# grep "Before: " out.txt | awk "" | python3 ./tester.py

# Step 1: Run `make`
make

# Step 2: Generate random numbers with `shuf`, format them, and pass them to `PmergeMe`
./PmergeMe $(shuf -i 1-10000 -n 1000 | tr "\n" " ") > out.txt

# Step 3: Use `grep` to extract the line starting with "After :"
sorted_line=$(grep -Eo "After :[0-9 ]+" out.txt)

# Step 4: Remove "After :" part from the line to keep just the numbers
numbers=$(echo "$sorted_line" | sed 's/After : //')

# Step 5: Pass the numbers to the Python script for sorting check
python3 tester.py <<< "$numbers"
