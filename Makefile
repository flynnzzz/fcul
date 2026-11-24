CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-function -std=c99
SRCDIR = .
TESTDIR = test
BINDIR = bin

.PHONY: clean
clean:
	rm -rf $(BINDIR)/main $(BINDIR)/test

$(BINDIR)/main: $(SRCDIR)/main.c $(SRCDIR)/arraylist.h
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/main $(SRCDIR)/main.c

$(BINDIR)/al_test: $(TESTDIR)/al_test.c $(SRCDIR)/arraylist.h
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $(BINDIR)/al_test $(TESTDIR)/al_test.c

# Clean, compile and run main
run-main: clean $(BINDIR)/main
	./$(BINDIR)/main

# Clean, compile and run test
run-test: clean $(BINDIR)/al_test
	./$(BINDIR)/al_test

# run-test with valgrind
val-test: clean $(BINDIR)/al_test
	valgrind ./$(BINDIR)/al_test
