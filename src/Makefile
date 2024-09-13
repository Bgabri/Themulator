
CC     = gcc
CFLAGS = -std=c99 -pedantic -Wall $(EXTRA)
BIN_DIR = bin
OUT_DIR = out
IN_DIR = in
EXTRA = 

.PHONY: clean debug

main: *.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ -g 


debug: main
		./$(BIN_DIR)/main -s -D Test

valgrind: main
	valgrind --leak-check=full -s ./$(BIN_DIR)/main -d -D Test
	mv vgcore.* $(BIN_DIR)



exit-codes: *.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/node $^ -g -D which_compile_error

pull-ensure :
	git clone https://github.com/bgabri/Ensure
	mv Ensure/ensure.c .
	mv Ensure/ensure.h .
	rm -r -f Ensure


$(BIN_DIR):
	mkdir -p $@
	@echo "made ./$@"


clean:
	rm -r -f $(OUT_DIR)
	rm -r -f $(BIN_DIR)
	rm -f vgcore.*


	