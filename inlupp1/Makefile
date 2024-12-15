C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit
COV		   = --coverage

test_hash: hash_table.c hash_table_test.c linked_list.c
	$(C_COMPILER) $(C_OPTIONS) $^ $(CUNIT_LINK) -o $@

test_linked: linked_list.c linked_list_test.c
	$(C_COMPILER) $(C_OPTIONS) $^ $(CUNIT_LINK) -o $@

tests: test_linked test_hash
	valgrind --leak-check=full ./test_linked
	@echo "\nLinked list test executed with valgrind\n"	
	valgrind --leak-check=full ./test_hash
	@echo "\nHash table test executed with valgrind"	
	
compile_freq_count: freq-count.c
	gcc -g freq-count.c hash_table.c linked_list.c -pg

linkedlist_coverage: linked_list.c linked_list_test.c 
	$(C_COMPILER) $(COV) $(C_OPTIONS) $^ $(CUNIT_LINK) -o $@

hash_test_coverage: hash_table.c hash_table_test.c linked_list.c
	$(C_COMPILER) $(COV) $(C_OPTIONS) $^ $(CUNIT_LINK) -o $@

run_coverage_hash: hash_test_coverage 
	./hash_test_coverage	

run_coverage_linked: linkedlist_coverage 
	./linkedlist_coverage

coverage: run_coverage_hash run_coverage_linked
	gcov -abcfu hash_table.c
	@echo "\ncoverage for hash table\n"
	gcov -abcfu linked_list.c
	@echo "\ncoverage for linked_list"

clean:
	rm -f *.o \
    *.gcda \
    *.gcno \
    *.gcov \
    test_linked \
    test_hash \
    hash_test_coverage \
    linkedlist_coverage

