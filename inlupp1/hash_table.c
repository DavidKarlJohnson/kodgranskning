#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>


#define Success(v)      (option_t) { .success = true, .value = v };
#define Failure()       (option_t) { .success = false };
#define Successful(o)   (o.success == true)
#define Unsuccessful(o) (o.success == false)



/// @brief Adjusts the input so that no negative values are returned
/// @param key The key to adjust
/// @return A positive integer in the range: [0, No_Buckets]
/// @example And input of -2 with a bucket size of 17 will return -2+17 = 15
static int valid_bucket(int key)
{
  return key < 0 ? key += No_Buckets : key;
}


/// @brief creates a new entry
/// @param key the key of the new entry
/// @param value the value of the new entry
/// @param next_entry the entry which this new entry points to
/// @return the new entry
static entry_t* entry_create(const elem_t key, elem_t value, entry_t* next_entry) {
  entry_t* new_entry = calloc(1, sizeof(entry_t));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next_entry;
  return new_entry;
}


ioopm_hash_table_t *ioopm_hash_table_create(hash_function* hash_function, ioopm_eq_function* key_func, ioopm_eq_function *value_func) {
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));

  for(int i = 0; i < No_Buckets; i++)  // Initialize every bucket with a dummy element
  {
    ht->buckets[i] = entry_create(int_elem(0), void_elem(NULL), NULL);
  }
  ht->size = 0;
  ht->hash_function = hash_function;
  ht->key_func = key_func;
  ht->value_func = value_func;

  return ht;
}

/// @brief finds the entry that comes before the entry looked for
/// @param ht The hashtable to search
/// @param entry the first entry in the bucket
/// @param key key of the entry being looked for
/// @return the entry before the one looked for
static entry_t* find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *entry, const elem_t key) {    
  entry_t *previous = entry;
  entry_t *current = previous->next;  // Skip bucket
  while(current != NULL)
  {
    if(ht->key_func(current->key, key))  // Current key is equal to argument key, return previous
    {
      return previous;
    }
    previous = current;
    current = current->next;
  }
  return previous;  // Never found, return last entry
}


void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  for(int i = 0; i < No_Buckets; i++)  
  {
    entry_t *current_entry = ht->buckets[i];
    entry_t *next_entry = current_entry->next;
    while(next_entry != NULL)
    {
      free(current_entry);
      current_entry = next_entry;
      next_entry = current_entry->next;
    }
    free(current_entry);       
  }
  free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
  int bucket = valid_bucket(ht->hash_function(key) % No_Buckets);

  entry_t *previous = find_previous_entry_for_key(ht, ht->buckets[bucket], key);
  entry_t *next = previous->next;

  if(next && ht->key_func(next->key, key))  // Update existing
  {
    next->value = value;
  } else {  // Insert next
    previous->next = entry_create(key, value, next);
    ht->size++;
  }
}


bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result) {
  int bucket = valid_bucket(ht->hash_function(key) % No_Buckets);

  entry_t *previous = find_previous_entry_for_key(ht, ht->buckets[bucket], key);
  entry_t *next = previous->next;

  if (next && ht->key_func(next->key, key))
  {
    *result = next->value;
    return true;
  } 
  return false;
}


bool ioopm_hash_table_remove(ioopm_hash_table_t* ht, elem_t key, elem_t *result) {
  int bucket = valid_bucket(ht->hash_function(key) % No_Buckets);

  entry_t *previous = find_previous_entry_for_key(ht, ht->buckets[bucket], key);
  entry_t *to_remove = previous->next;

  if (to_remove && ht->key_func(to_remove->key, key)) {
    *result = to_remove->value;
    previous->next = to_remove->next;
    free(to_remove);
    ht->size--;
    return true;
  }
  return false;
}


size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht) {
  return ht->size;
}


bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht) {
  return ht->size == 0;
}


void ioopm_hash_table_clear(ioopm_hash_table_t *ht) {
  for(int i = 0; i < No_Buckets; i++)  
  {
    entry_t *bucket_start = ht->buckets[i];
    entry_t *next_entry = bucket_start->next;
    entry_t * temp = next_entry;
    while(next_entry != NULL)
    {
      temp = next_entry->next;
      free(next_entry);
      next_entry = temp;
    }
    bucket_start->next = NULL; 
    ht->size = 0;
  }
}


ioopm_list_t *ioopm_hash_table_keys(const ioopm_hash_table_t *ht) {
  ioopm_list_t *all_keys = ioopm_linked_list_create();

  for(int i = 0; i < No_Buckets; i++)  
  {
    entry_t *current_entry = ht->buckets[i]->next;  // Skip dummy
    while(current_entry != NULL)
    {
      ioopm_linked_list_append(all_keys, current_entry->key);
      current_entry = current_entry->next;
    }
  }
  return all_keys;
}


ioopm_list_t *ioopm_hash_table_values(const ioopm_hash_table_t *ht) {
  ioopm_list_t* all_values = ioopm_linked_list_create();

  for(int i = 0; i < No_Buckets; i++)   
  {
      entry_t *current_entry = ht->buckets[i]->next;  // Skip dummy
      while(current_entry != NULL)
      {
        ioopm_linked_list_append(all_values, current_entry->value);
        current_entry = current_entry->next;
      }
  }
  return all_values;
}


bool ioopm_hash_table_all(const ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg) {
  for(int i = 0; i < No_Buckets; i++)
  {
    entry_t *current_entry = ht->buckets[i]->next;  // Skip dummy
    while(current_entry != NULL)
    {
      if(pred(current_entry->key, current_entry->value, arg) == false) {
        return false;
      }
      current_entry = current_entry->next;
    }
  }
  return true;
}


bool ioopm_hash_table_any(const ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg) {
  for(int i = 0; i < No_Buckets; i++)
  {
    entry_t *current_entry = ht->buckets[i]->next;  // Skip dummy
    while(current_entry != NULL) 
    {
      if(pred(current_entry->key, current_entry->value, arg) == true) {
        return true;
      }
      current_entry = current_entry->next;
    }
  }
  return false;
}


void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_fun, void *arg) {
  for(int i = 0; i < No_Buckets; i++)
  {
    entry_t *current_entry = ht->buckets[i]->next;  // Skip dummy
    while(current_entry != NULL)
    {
      apply_fun(current_entry->key, &current_entry->value, arg);  
      current_entry = current_entry->next;
    }
  }
}


bool ioopm_hash_table_has_key(const ioopm_hash_table_t *ht, elem_t key) {
  for(int i = 0; i < No_Buckets; i++)
  {
    entry_t *current_entry = ht->buckets[i]->next;  // Skip dummy
    while(current_entry != NULL)
    {
      elem_t current_key = current_entry->key;
      if(ht->key_func(current_key, key) == true) 
      {
        return true;
      }
      current_entry = current_entry->next;
    }
  }
  return false;
}


bool ioopm_hash_table_has_value(const ioopm_hash_table_t *ht, elem_t value) {
  for(int i = 0; i < No_Buckets; i++)
  {
    entry_t *current_entry = ht->buckets[i]->next;  // Skip dummy
    while(current_entry != NULL)
    {
      elem_t current_value = current_entry->value;
    
      if(ht->value_func(current_value, value) == true) {
        return true;
      }
      current_entry = current_entry->next;
    }
  }
  return false;
}