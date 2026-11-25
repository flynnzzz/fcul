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

$(BINDIR)/al_test_int: $(TESTDIR)/al_test_int.c $(SRCDIR)/arraylist.h
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $(BINDIR)/al_test_int $(TESTDIR)/al_test_int.c

# Clean, compile and run main
run-main: clean $(BINDIR)/main
	./$(BINDIR)/main

# Clean, compile and run test
run-test-int: clean $(BINDIR)/al_test_int
	./$(BINDIR)/al_test_int

# run-test with valgrind
val-test-int: clean $(BINDIR)/al_test_int
	valgrind ./$(BINDIR)/al_test_int
