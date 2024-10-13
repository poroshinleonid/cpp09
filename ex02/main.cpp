#include "PmergeMe.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Error: invalid arguments" << std::endl;
    std::cout << "usage: ./PmergeMe *numbers*" << std::endl;
    return 1;
  }
  PmergeMe pm;
  pm.sort(argc, argv);
  return 0;
}

/*
test.sh
#!/bin/bash
./PmergeMe `jot -r 300 1 100000 | tr '\n' ' '` > out.txt
if grep -q "Error" "out.txt"; then
    exit 0 
fi
unsorted_line=$(grep -Eo "Before:[0-9 ]+" out.txt)
sorted_line=$(grep -Eo "After :[0-9 ]+" out.txt)
unsorted_numbers=$(echo "$unsorted_line" | sed 's/Before: //')
sorted_numbers=$(echo "$sorted_line" | sed 's/After : //')
# echo "$unsorted_numbers" | wc -w
# echo "$sorted_numbers" | wc -w
python3 tester.py <<< "$sorted_numbers"

megatest.sh
#!/bin/bash
output_file="out4.txt"
> "$output_file"
for i in {1..1000}; do
    echo "$i"
    ./test.sh >> "$output_file"
done
count=$(grep -c "unsorted" "$output_file")
echo "Number of lines containing 'unsorted': $count"

tester.py
import sys
def is_sorted(numbers):
    return numbers == sorted(numbers)
if __name__ == "__main__":
    if len(sys.argv) > 1:
        print("Please provide numbers through input redirection or directly as command-line arguments.")
    else:
        try:
            content = sys.stdin.read().strip()
            numbers = [int(num) for num in content.split()]
            if is_sorted(numbers):
                print("The numbers are sorted.")
            else:
                print("The numbers are not sorted.")
        except ValueError:
            print("Please provide valid numbers.")

*/