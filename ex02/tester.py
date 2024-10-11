# numbers = map(int, input(). split())
# if sorted(numbers) == numbers:
#     print("sorted")
# else:
#     print("unsorted")
# l = list(map(int, input(). split()))
# for i in range(0, len(l) - 1):
#     if l[i] >= l[i + 1]:
#         print("unsorted: ", l[i], l[i + 1])
# print(all(l[i] <= l[i+1] for i in range(len(l) - 1)))
import sys

def is_sorted(numbers):
    return numbers == sorted(numbers)

if __name__ == "__main__":
    # Check if input is provided via command line arguments
    if len(sys.argv) > 1:
        print("Please provide numbers through input redirection or directly as command-line arguments.")
    else:
        try:
            # Read input from stdin (using redirection)
            content = sys.stdin.read().strip()
            
            # Split the numbers by spaces and convert them to a list of integers
            numbers = [int(num) for num in content.split()]
            
            # Check if the list is sorted
            if is_sorted(numbers):
                print("The numbers are sorted.")
            else:
                print("The numbers are not sorted.")
        except ValueError:
            print("Please provide valid numbers.")
    # print(numbers)