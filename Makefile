file_location = ~/College/DAA/data.txt

all: debug

run:
	@./target/debug/app $(file_location)

debug: dirs
	@gcc app.cpp -Wall -Wextra -Wpedantic -g -lstdc++ -lncurses -o ./target/debug/app

dirs:
	@mkdir -p ./target
	@mkdir -p ./target/debug
	@mkdir -p ./target/prod

clean:
	@rm -f ./target/debug/*
	@rm -f ./target/prod/*

# hello:
# 	@echo "Hello, World"
# 	@echo "This line will print if the file hello does not exist."