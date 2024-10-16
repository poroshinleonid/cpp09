CPP = "c++"
CPPFLAGS = -Wall -Wextra -Werror -std=c++17 -g
NAME = PmergeMe
SRCS = main.cpp \
				PmergeMe.cpp

OBJS = $(SRCS:.cpp=.o)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

format:
	clang-format -Werror -i --style=LLVM $(SRCS)
	clang-format -Werror -i --style=LLVM ./*.hpp

leaks: all
	valgrind --leak-check=full ./$(NAME)

# #!/bin/sh
# ./PmergeMe `seq 1 10000 | sort -R | head -n 3000` | grep "After" | sed 's/^After : //' > out.txt;
# tr ' ' '\n' <out.txt >out_lines.txt;
# sed -i '' '/^$/d' out_lines.txt;
# sort -h out_lines.txt >out_sorted.txt;
# diff out_lines.txt out_sorted.txt

