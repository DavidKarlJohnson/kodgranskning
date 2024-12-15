------------------------- HOW TO RUN-------------------------
To run freq-count write the command, make compile_freq_count in the terminal. Now you can run the program with whatever textfile you want. Simply write ./a.out file1 file2 ... (where the files are txt file 1m-words.txt for instance). If you want to profile the program simply write gprof before ./a.out(no files) To gprof new textfiles you need to do the ./a.out file command again.
To time your program, time ./a.out input

To run the test for linked_list and hash_table write make tests in the console.

To check coverage of linked_list and hash_table writ make coverage in the console.
-----------------------------------------------------------




---------------------- DOCUMENTATION ----------------------

HASH TABLE:

  * A hash table consists of the type 'entry' which holds a 'key' and a 'value' pair which can be any generic datatype.

  * The eq functions for keys and value used in ioopm_hash_table_create() are ment to simply check
    a.i == b.i (the i should be sswapped depending on what value you decide to use on you elements).

  * The number of buckets in a hashtable is static at 17 and can be changed with the macro NO_buckets.
    The table is NOT dynamically resized as the number of entries grow and shrink. See the macro No_Buckets in 'hash_table.h'.

  * 'ioopm_hash_table_destroy' will free the underlying buckets, aswell as the entries they hold.

  * Clearing a hash table will not remove its dummy elements, but will free the underlying entries.

  * 'find_previous_entry_for_key' will search the entire bucket until either the given entry is found or it reaches the end. Function will 
     not terminate prematurely if the compared key size is exceeded.

  * Errors are handled by the function in question returning a bool depening on if the function executed as expected or not.
  


LINKED LIST:

  * 'ioopm_linked_list_destroy()' will free the underlying links, aswell as the entries they hold.

  * The operations: Remove, Insert and Get, will adjust the given index if it's invalid. A value that's too low will adjust to index 0. A value that's too high will adjust to the last index of the list.

  * Errors are handled by the function in question returning a bool depening on if the function executed as expected or not.

-----------------------------------------------------------

----Coverage----
(used gcov)

----Hash Table----
File 'hash_table.c'
Lines executed:98.55% of 138
Branches executed:100.00% of 64
Taken at least once:92.19% of 64
Calls executed:100.00% of 24
Creating 'hash_table.c.gcov'

----Linked list-----
File 'linked_list.c'
Lines executed:97.86% of 140
Branches executed:100.00% of 46
Taken at least once:89.13% of 46
Calls executed:96.15% of 26
Creating 'linked_list.c.gcov'


---------------------- PROFILING ----------------------

#Initial Profiling Results


1)  For each input, what are the top 3 functions?
    * small.txt
      1. eq_str
      2. ioopm_linked_list_size
      3. aschii_str_sum

    * 1k-long-words.txt 
      1. eq_str
      2. aschii_str_sum
      3. find_previous_entry_for_key

    * 10k-words.txt 
      1. eq_str
      2. aschii_str_sum
      3. find_previous_entry_for_key

    * 16k-words.txt 
      1. ioopm_hash_table_has_key
      2. eq_str
      3. find_previous_link


2)  For each input, are the top 3 functions in your code (that you have written), or is it in library functions?
    The top three functions for all text-files are written by ourselves. However, the 'eq_str' function is practically a 
    library function, considering that it's mostly a call to the 'strcmp' funciton in the 'string.h' library.


3)  Are the top 3 functions in your code consistent across the inputs? Why? Why not?
    The functions are mostly the same across the inputs, the biggest "incosistency" is that in the 16-k words text file our function 
    ioopm_hash_table_has_key takes the nr one spot as the most time consuming function. This function was never top three before and 
    is called upon relativly few times compared to other function. But because in this text file we have 16k words which means 16 k
    entries in hash-table each call to the has_key function takes more and more time since it has to iterate trough more and more 
    entreis. Why this function takes so much longer than say our insert or lookup function, when they in theory kind of do the 
    same thing, is because our has_key function does not calculate which bucket it is supposed to find the key it's looking for, and
    thus have to search through every bucket until it finds the right key (even those buckets in which the key can not exist) 


4)  Is there some kind of trend? (Possibly several)
    eq_str is nr 1 until 16k then it becomes nr 2. Maybe becomes nr 1 again sometime after 1 mil since the difference in time became alot
    smaller. But maybe that is because the time was so low when useing the 16k file (0.03 and 0.01) so the percentage becomes bigger...

    The more entries the longer the hash_table functions take.


5)  Do the results correspond with your expectations?
    Our expectations were that the functions that would be the most time consuming where the ones that traversed the hashtable. This is 
    becasue for one, these are called everytime for every word, secondly they have a worst case scenario of taking O(n) time for every 
    entery (where n is all the enteries in a bucket) (and in ioopm_hash_table_has_key case n is for all the enteries in the entire 
    hashtable). These different functions, were, as expected pretty time consuming. What surprised us was that the different 
    "simple" compare functions, like eq_str, where top 3 every time. Of course they are called a lot (a lot more than any other), but still
    each should call take (relativly speaking) no time at all.



6)  Based on these results, do you see a way to make your program go faster?
    An obvious answer would be that to make the functions more effectiv eg. ioopm_hash_table_hash_key. Other than that one way to make
    the program go fast would be to either make the hashtable have more buckets intialy and therefor have less entries in each bucket
    and thus save time when the program iterates through each bucket. Another solution in the same spirit would be to make the hashtable itself add more buckets progresivly as the program runs and it buckets start to fill up. 
    The 'find_previous_entry' function goes through the whole bucket if it can't find the matching key. An improvement would be to sort each entry in a bucket according to "key size" that way we could interrupt the search if we go "past" the key we are looking for.





*** DETAILED SUMMARY BELOW ***


-----------------------------   small.txt   ------------------------------------
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00      486     0.00     0.00  eq_str
  0.00      0.00     0.00      171     0.00     0.00  ioopm_linked_list_size
  0.00      0.00     0.00       56     0.00     0.00  aschii_str_sum
  0.00      0.00     0.00       56     0.00     0.00  find_previous_entry_for_key
  0.00      0.00     0.00       56     0.00     0.00  find_previous_link
  0.00      0.00     0.00       56     0.00     0.00  linked_list_adjust_index
  0.00      0.00     0.00       56     0.00     0.00  valid_bucket
  0.00      0.00     0.00       55     0.00     0.00  ioopm_iterator_has_next
  0.00      0.00     0.00       45     0.00     0.00  entry_create
  0.00      0.00     0.00       28     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.00     0.00       28     0.00     0.00  ioopm_hash_table_insert
  0.00      0.00     0.00       28     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.00     0.00       28     0.00     0.00  ioopm_iterator_current
  0.00      0.00     0.00       28     0.00     0.00  ioopm_linked_list_append
  0.00      0.00     0.00       28     0.00     0.00  ioopm_linked_list_insert
  0.00      0.00     0.00       28     0.00     0.00  ioopm_linked_list_remove
  0.00      0.00     0.00       28     0.00     0.00  link_create
  0.00      0.00     0.00       28     0.00     0.00  process_word
  0.00      0.00     0.00       27     0.00     0.00  ioopm_iterator_next
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_keys
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.00     0.00        1     0.00     0.00  list_to_str_array
  0.00      0.00     0.00        1     0.00     0.00  process_file
  0.00      0.00     0.00        1     0.00     0.00  sort_keys
--------------------------------------------------------------------------------
real: 0m0,004s, user: 0m0,001s, sys: 0m0,003s
real: 0m0,004s, user: 0m0,001s, sys: 0m0,003s
real: 0m0,004s, user: 0m0,001s, sys: 0m0,003s
real: 0m0,004s, user: 0m0,000s, sys: 0m0,004s
real: 0m0,004s, user: 0m0,000s, sys: 0m0,004s



-----------------------------   1k-long-words.txt   --------------------------------
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00    17092     0.00     0.00  eq_str
  0.00      0.00     0.00     2002     0.00     0.00  aschii_str_sum
  0.00      0.00     0.00     2002     0.00     0.00  find_previous_entry_for_key
  0.00      0.00     0.00     2002     0.00     0.00  valid_bucket
  0.00      0.00     0.00     1001     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.00     0.00     1001     0.00     0.00  ioopm_hash_table_insert
  0.00      0.00     0.00     1001     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.00     0.00     1001     0.00     0.00  process_word
  0.00      0.00     0.00      147     0.00     0.00  ioopm_linked_list_size
  0.00      0.00     0.00       48     0.00     0.00  find_previous_link
  0.00      0.00     0.00       48     0.00     0.00  linked_list_adjust_index
  0.00      0.00     0.00       47     0.00     0.00  ioopm_iterator_has_next
  0.00      0.00     0.00       41     0.00     0.00  entry_create
  0.00      0.00     0.00       24     0.00     0.00  ioopm_iterator_current
  0.00      0.00     0.00       24     0.00     0.00  ioopm_linked_list_append
  0.00      0.00     0.00       24     0.00     0.00  ioopm_linked_list_insert
  0.00      0.00     0.00       24     0.00     0.00  ioopm_linked_list_remove
  0.00      0.00     0.00       24     0.00     0.00  link_create
  0.00      0.00     0.00       23     0.00     0.00  ioopm_iterator_next
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_keys
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.00     0.00        1     0.00     0.00  list_to_str_array
  0.00      0.00     0.00        1     0.00     0.00  process_file
  0.00      0.00     0.00        1     0.00     0.00  sort_keys
  --------------------------------------------------------------------------------
real: 0m0,007s, user: 0m0,004s, sys: 0m0,004s
real: 0m0,008s, user: 0m0,004s, sys: 0m0,004s
real: 0m0,005s, user: 0m0,004s, sys: 0m0,001s
real: 0m0,005s: user: 0m0,002s, sys: 0m0,002s
real: 0m0,006s, user: 0m0,005s, sys: 0m0,002s



-----------------------------   10k-words.txt   --------------------------------
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00  1160117     0.00     0.00  eq_str
  0.00      0.00     0.00    20002     0.00     0.00  aschii_str_sum
  0.00      0.00     0.00    20002     0.00     0.00  find_previous_entry_for_key
  0.00      0.00     0.00    20002     0.00     0.00  valid_bucket
  0.00      0.00     0.00    10001     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.00     0.00    10001     0.00     0.00  ioopm_hash_table_insert
  0.00      0.00     0.00    10001     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.00     0.00    10001     0.00     0.00  process_word
  0.00      0.00     0.00     1203     0.00     0.00  ioopm_linked_list_size
  0.00      0.00     0.00      400     0.00     0.00  find_previous_link
  0.00      0.00     0.00      400     0.00     0.00  linked_list_adjust_index
  0.00      0.00     0.00      399     0.00     0.00  ioopm_iterator_has_next
  0.00      0.00     0.00      217     0.00     0.00  entry_create
  0.00      0.00     0.00      200     0.00     0.00  ioopm_iterator_current
  0.00      0.00     0.00      200     0.00     0.00  ioopm_linked_list_append
  0.00      0.00     0.00      200     0.00     0.00  ioopm_linked_list_insert
  0.00      0.00     0.00      200     0.00     0.00  ioopm_linked_list_remove
  0.00      0.00     0.00      200     0.00     0.00  link_create
  0.00      0.00     0.00      199     0.00     0.00  ioopm_iterator_next
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_keys
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.00     0.00        1     0.00     0.00  list_to_str_array
  0.00      0.00     0.00        1     0.00     0.00  process_file
  0.00      0.00     0.00        1     0.00     0.00  sort_keys
--------------------------------------------------------------------------------
real: 0m0,031s, user: 0m0,027s, sys: 0m0,004s
real: 0m0,041s, user: 0m0,038s, sys: 0m0,002s
real: 0m0,018s, user: 0m0,015s, sys: 0m0,003s
real: 0m0,018s, user: 0m0,016s, sys: 0m0,003s
real: 0m0,021s, user: 0m0,019s, sys: 0m0,002s



-----------------------------   16k-words.txt   --------------------------------
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 60.00      0.03     0.03    16993     0.00     0.00  ioopm_hash_table_has_key
 20.00      0.04     0.01 29530356     0.00     0.00  eq_str
 20.00      0.05     0.01     8742     0.00     0.00  find_previous_link
  0.00      0.05     0.00    33986     0.00     0.00  aschii_str_sum
  0.00      0.05     0.00    33986     0.00     0.00  find_previous_entry_for_key
  0.00      0.05     0.00    33986     0.00     0.00  valid_bucket
  0.00      0.05     0.00    26229     0.00     0.00  ioopm_linked_list_size
  0.00      0.05     0.00    16993     0.00     0.00  ioopm_hash_table_insert
  0.00      0.05     0.00    16993     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.05     0.00    16993     0.00     0.00  process_word
  0.00      0.05     0.00     8742     0.00     0.00  linked_list_adjust_index
  0.00      0.05     0.00     8741     0.00     0.00  ioopm_iterator_has_next
  0.00      0.05     0.00     4388     0.00     0.00  entry_create
  0.00      0.05     0.00     4371     0.00     0.00  ioopm_iterator_current
  0.00      0.05     0.00     4371     0.00     0.00  ioopm_linked_list_append
  0.00      0.05     0.00     4371     0.00     0.00  ioopm_linked_list_insert
  0.00      0.05     0.00     4371     0.00     0.00  ioopm_linked_list_remove
  0.00      0.05     0.00     4371     0.00     0.00  link_create
  0.00      0.05     0.00     4370     0.00     0.00  ioopm_iterator_next
  0.00      0.05     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.05     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.05     0.00        1     0.00     5.00  ioopm_hash_table_keys
  0.00      0.05     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.05     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.05     0.00        1     0.00     5.00  ioopm_linked_list_destroy
  0.00      0.05     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.05     0.00        1     0.00     0.00  list_to_str_array
  0.00      0.05     0.00        1     0.00    39.89  process_file
  0.00      0.05     0.00        1     0.00     0.00  sort_keys
--------------------------------------------------------------------------------
real: 0m0,303s, user: 0m0,291s, sys: 0m0,005s
real: 0m0,331s, user: 0m0,327s, sys: 0m0,003s
real: 0m0,336s, user: 0m0,330s, sys: 0m0,006s
real: 0m0,275s, user: 0m0,271s, sys: 0m0,004s
real: 0m0,298s, user: 0m0,293s, sys: 0m0,005s



-----------------------------   1m-words.txt   --------------------------------
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 37.62      1.55     1.55  1189441     0.00     0.00  ioopm_hash_table_has_key
 31.55      2.85     1.30 2788822913     0.00     0.00  eq_str
 27.67      3.99     1.14                             _init
  2.18      4.08     0.09  2378882     0.00     0.00  find_previous_entry_for_key
  0.73      4.11     0.03  2378882     0.00     0.00  aschii_str_sum
  0.24      4.12     0.01     8742     0.00     0.00  find_previous_link
  0.00      4.12     0.00  2378882     0.00     0.00  valid_bucket
  0.00      4.12     0.00  1189441     0.00     0.00  ioopm_hash_table_insert
  0.00      4.12     0.00  1189441     0.00     0.00  ioopm_hash_table_lookup
  0.00      4.12     0.00  1189441     0.00     0.00  process_word
  0.00      4.12     0.00    26229     0.00     0.00  ioopm_linked_list_size
  0.00      4.12     0.00     8742     0.00     0.00  linked_list_adjust_index
  0.00      4.12     0.00     8741     0.00     0.00  ioopm_iterator_has_next
  0.00      4.12     0.00     4388     0.00     0.00  entry_create
  0.00      4.12     0.00     4371     0.00     0.00  ioopm_iterator_current
  0.00      4.12     0.00     4371     0.00     0.00  ioopm_linked_list_append
  0.00      4.12     0.00     4371     0.00     0.00  ioopm_linked_list_insert
  0.00      4.12     0.00     4371     0.00     0.00  ioopm_linked_list_remove
  0.00      4.12     0.00     4371     0.00     0.00  link_create
  0.00      4.12     0.00     4370     0.00     0.00  ioopm_iterator_next
  0.00      4.12     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      4.12     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      4.12     0.00        1     0.00     0.01  ioopm_hash_table_keys
  0.00      4.12     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      4.12     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      4.12     0.00        1     0.00     0.01  ioopm_linked_list_destroy
  0.00      4.12     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      4.12     0.00        1     0.00     0.00  list_to_str_array
  0.00      4.12     0.00        1     0.00     2.97  process_file
  0.00      4.12     0.00        1     0.00     0.00  sort_keys
--------------------------------------------------------------------------------
real: 0m23,667s, user: 0m23,632s, sys: 0m0,026s
real: 0m23,889s, user: 0m23,851s, sys: 0m0,037s
real: 0m23,519s, user: 0m23,480s, sys: 0m0,038s
real: 0m24,192s, user: 0m24,156s, sys: 0m0,028s
real: 0m24,251s, user: 0m24,224s, sys: 0m0,024s



-----------------------------------------------------------