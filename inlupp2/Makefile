C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit
CCOV           = --coverage -g -O0

all: user_interface

compile_all: user_interface.c
	$(C_COMPILER) $(C_OPTIONS) user_interface.c hash_table.c linked_list.c cart.c merchandise.c shelf.c utils.c -o user_interface $(C_LINK_OPTIONS) $(CUNIT_LINK)

compile_shelf_tests: shelf_tests.c
	$(C_COMPILER) $(C_OPTIONS) $(CCOV) shelf_tests.c shelf.c merchandise.c hash_table.c linked_list.c utils.c -o shelf_tests $(C_LINK_OPTIONS) $(CUNIT_LINK)

compile_cart_tests: cart_tests.c
	$(C_COMPILER) $(C_OPTIONS) $(CCOV) cart_tests.c cart.c merchandise.c shelf.c hash_table.c linked_list.c utils.c -o cart_tests $(C_LINK_OPTIONS) $(CUNIT_LINK)

compile_merch_tests: merchandise_tests.c
	$(C_COMPILER) $(C_OPTIONS) $(CCOV) merchandise_tests.c merchandise.c shelf.c hash_table.c linked_list.c utils.c -o merchandise_tests $(C_LINK_OPTIONS) $(CUNIT_LINK)

run: compile_all
	./user_interface

run_memtest: compile_all
	valgrind --leak-check=full ./user_interface

run_shelf_tests: compile_shelf_tests
	valgrind --leak-check=full ./shelf_tests

run_cart_tests: compile_cart_tests
	valgrind --leak-check=full ./cart_tests

run_merch_tests: compile_merch_tests
	valgrind --leak-check=full ./merchandise_tests

coverage_shelf_tests: compile_shelf_tests
	./shelf_tests
	gcovr

coverage_cart_tests: compile_cart_tests
	./cart_tests
	gcovr

coverage_merch_tests: compile_merch_tests
	./merchandise_tests
	gcovr
	
clean:
	rm -f user_interface shelf_tests cart_tests merchandise_tests *.gcno *.gcda *.gcov


