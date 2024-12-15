#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static bool eq_int(elem_t a, elem_t b) { return a.i == b.i; }

static bool eq_str(elem_t a, elem_t b) { return strcmp(a.s, b.s) == 0; }


/// @brief *Hashing function* Sum each aschii-value 
/// @param key String of which each char will be added to a sum
/// @return The sum of all characters aschii-value
static int aschii_str_sum(elem_t key)
{
  int sum = 0;

  for(int i = 0; i < strlen(key.s); i++) {
    sum += key.s[i];
  }
  return sum;
}


static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}


/// @brief Turns a list of keys into a string array
/// @param list A list containing elem_t 
/// @return An array with the same string content as the argument list
static char** list_to_str_array(ioopm_list_t *list)
{
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  char **array = calloc (ioopm_linked_list_size(list) + 1, sizeof(char*)); 
  int index = 0;
  
  while(ioopm_iterator_has_next(iter))
  {
    elem_t unused;
    array[index] = ioopm_iterator_current(iter).s;
    index++;
    ioopm_iterator_next(iter, &unused);
  }
  array[index] = ioopm_iterator_current(iter).s;  

  ioopm_iterator_destroy(iter);
  return array;
}


/// @brief Sorts an array of strings in lexographical order
/// @param keys The array of strings
/// @param no_keys The size of the array
static void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}


/// @brief Checks if the argument word already exists in the hashtable. If it does, 
///        its value gets updated, else, insert the word.      
/// @param word The word to insert into the HT.
/// @param ht The table to insert into.
void process_word(char *word, ioopm_hash_table_t *ht)
{
  int counter = 1;

  if(ioopm_hash_table_has_key(ht, string_elem(word)))  // Exists in HT already, update value
  {
    elem_t element;
    ioopm_hash_table_lookup(ht, string_elem(word), &element);
    counter += element.i;
    ioopm_hash_table_insert(ht, string_elem(word), int_elem(counter));
    return;
  }

  ioopm_hash_table_insert(ht, string_elem(strdup(word)), int_elem(counter));
  return;
}


/// @brief Goes throgh a file and sends every word to process_word()
/// @param filename Name of file to process
/// @param ht The hashtable to insert the words into
void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  while (true)
  {
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);

    if (feof(f))
    {
      free(buf);
      break;
    }

    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters))
    {
      process_word(word, ht);
    }

    free(buf);
  }

  fclose(f);
}


int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(aschii_str_sum, eq_str, eq_int);

  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }

    ioopm_list_t *keys = ioopm_hash_table_keys(ht);  // Isolate the keys of the HT and put in a list
    char **keys_arr = list_to_str_array(keys);  // Turn list to an array for sorting
    sort_keys(keys_arr, ioopm_linked_list_size(keys));

    for(int i = 0; i < ioopm_linked_list_size(keys); i++)  // Look up the sorted keys in the HT (with the help of the array) and print them in order + their frequency
    {
      elem_t freq;
      ioopm_hash_table_lookup(ht, string_elem(keys_arr[i]), &freq);
      printf("%s: %d\n", keys_arr[i], freq.i);
    }
    
    int i = 0;

    while (keys_arr[i] != NULL) //freeing all strdups
    {
      free(keys_arr[i]);
      i++;
    }
    free(keys_arr);
    ioopm_linked_list_destroy(keys);
  }
  
  else
  {
    puts("Usage: file1_name.txt file2_name.txt...");
  }

  ioopm_hash_table_destroy(ht);
}