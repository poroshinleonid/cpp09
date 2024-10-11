# numbers = map(int, input(). split())
# if sorted(numbers) == numbers:
#     print("sorted")
# else:
#     print("unsorted")
l = list(map(int, input(). split()))
for i in range(0, len(l) - 1):
    if l[i] >= l[i + 1]:
        print("unsorted: ", l[i], l[i + 1])
# print(all(l[i] <= l[i+1] for i in range(len(l) - 1)))