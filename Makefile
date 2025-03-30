# Executabil 
EXEC = a.out

CC = gcc
CFLAGS = -Wall

# Sursa
SRC = test.c

# Comanda 
all:
	make build run check 
	clear

build:
	$(CC) $(CFLAGS) $(SRC)

run:
	bash -c 'for i in $$(seq 1 20); do \
		echo "Rulăm testul $$i..."; \
		./$(EXEC) "in/data$$i.in" "out/data$$i.out"; \
	done'

check:
	./checker -i

add_files:
	git add test.c

clear:
	rm -f $(EXEC) out/*.out

