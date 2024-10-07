CPP = "c++"
CPPFLAGS = -Wall -Wextra -Werror -std=c++17
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