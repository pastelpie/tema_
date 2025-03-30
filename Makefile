# Executabil (implicit: a.out)
EXEC = a.out

# Compilator și opțiuni
CC = gcc
CFLAGS = -Wall

# Fișier sursă
SRC = test.c

# Comanda principală
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

