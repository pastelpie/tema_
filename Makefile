# Executabil 
EXEC = a.out

CC = gcc
CFLAGS = -Wall

# Sursa
SRC = test.c


# Comanda 
all: 
	make build run_t1 run_t2 check 
	clear

build:
	$(CC) $(CFLAGS) $(SRC)

run_t1:
	bash -c 'for i in $$(seq 1 20); do \
		echo "=== Rulăm Task 1 - testul $$i ==="; \
		./$(EXEC) "in/data$$i.in" "out/data$$i.out"; \
	done' 

run_t2:
	bash -c 'for i in $$(seq 1 20); do \
		cp "in/data$$i.in" "in/tmp$$i.in"; \
		sed -i "1s/.*/2/" "in/tmp$$i.in"; \
		echo "=== Rulăm Task 2 - testul $$i ==="; \
		./$(EXEC) "in/tmp$$i.in" "out/t2-data$$i.out"; \
		rm -f "in/tmp$$i.in"; \
	done'
	
run_t3:
	bash -c 'for i in $$(seq 1 20); do \
		cp "in/data$$i.in" "in/tmp$$i.in"; \
		sed -i "1s/.*/3/" "in/tmp$$i.in"; \
		echo "=== Rulăm Task 3 - testul $$i ==="; \
		./$(EXEC) "in/tmp$$i.in" "out/t3-data$$i.out"; \
		rm -f "in/tmp$$i.in"; \
	done'

check:
	./checker -i

add_files:
	git add test.c

clear:
	rm -f $(EXEC) out/*.out

