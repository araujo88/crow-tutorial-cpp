all:
	g++ main.cpp -o main -Wall -Wpedantic -Wextra -std=c++17

clean:
	rm -rf main