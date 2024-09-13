
CC     = gcc
CFLAGS = -std=c99 -pedantic -Wall $(EXTRA)
BIN_DIR = bin
OUT_DIR = out
IN_DIR = in
EXTRA = 

.PHONY: clean debug

main: *.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ -g 


debug: main | $(OUT_DIR)
	for file in $$(ls $(IN_DIR)/* | sort); do \
		echo -e "\x1b[36m- - - - - - - - - - - - - - - \x1b[1m$${file}\x1b[0m\x1b[36m - - - - - - - - - - - - - - -\x1b[0m" ; \
		cat $${file} | ./$(BIN_DIR)/main ; \
		echo -e "\x1b[36m- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\x1b[0m" ; \
	done

valgrind: main | $(OUT_DIR)
	for file in $$(ls $(IN_DIR)/* | sort); do \
		echo -e "\x1b[36m- - - - - - - - - - - - - - - \x1b[1m$${file}\x1b[0m\x1b[36m - - - - - - - - - - - - - - -\x1b[0m" ; \
		cat $${file} | valgrind --leak-check=full -s ./$(BIN_DIR)/main ; \
		echo -e "\x1b[36m- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\x1b[0m" ; \
	done
	mv vgcore.* $(BIN_DIR)



exit-codes: *.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/node $^ -g -D which_compile_error


pull-ensure :
	git clone https://github.com/bgabri/Ensure
	mv Ensure/ensure.c .
	mv Ensure/ensure.h .
	rm -r -f Ensure


$(BIN_DIR) $(OUT_DIR):
	mkdir -p $@
	@echo "made ./$@"


clean:
	rm -r -f $(OUT_DIR)
	rm -r -f $(BIN_DIR)
	rm -f vgcore.*


	