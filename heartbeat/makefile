CC = gcc
SRC = *.c
OUT = heartbeat.out

.PHONY: all
all:
	$(CC) $(SRC) -o $(OUT)

.PHONY: run
run: all
	./$(OUT)

.PHONY: clean
clean:
	rm -f $(OUT)
