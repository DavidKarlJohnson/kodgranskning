
#pragma once
#include <stdbool.h>
#include "common.h"
#include <stddef.h>

#define No_Buckets 17

/**
 * @file hash_table.h
 * @author Anton Backman och David Johnson
 * @date 17 okt 2024
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@..
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */


typedef struct link link_t;
typedef struct list ioopm_list_t; 
typedef struct iter ioopm_list_iterator_t;
typedef struct entry entry_t;
typedef struct option option_t;
typedef struct hash_table ioopm_hash_table_t;


typedef bool ioopm_predicate(elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);
typedef int hash_function(elem_t key);


struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t *buckets[No_Buckets];
  int size;
  hash_function *hash_function;
  ioopm_eq_function *key_func; 
  ioopm_eq_function *value_func; 
};


/// @brief Create a new hash table
/// @param hash_function The hashing function for deciding element placement
/// @param key_func The compare function of keys used on this ht (if table keys are string  a.s == b.s etc.)
/// @param value_func the compare function of values used on this ht (if table value are string  a.s == b.s etc.)
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(hash_function* hash_function, ioopm_eq_function* key_func, ioopm_eq_function *value_func);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert (non-negative)
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup (non-negative)
/// @param result param used to get access to the value stored in said key in the hashtable (called with &result)
/// @return a bool depending if there is an entry at that key (false = no, true = yes)
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove 
/// @param result param used to get acces to the value stored in the key to be removed (called with &result)
/// @return a bool depending if there is an entry at that key (false = no, true = yes)
bool ioopm_hash_table_remove(ioopm_hash_table_t* ht, elem_t key, elem_t *result);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true if size == 0, else false
bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return a linked list of keys from ht
ioopm_list_t *ioopm_hash_table_keys(const ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param ht hash table operated upon
/// @return a linked list of values from ht
ioopm_list_t *ioopm_hash_table_values(const ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought after
bool ioopm_hash_table_has_key(const ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought after
bool ioopm_hash_table_has_value(const ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate function
/// @param arg extra argument to pred
bool ioopm_hash_table_all(const ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate function
/// @param arg extra argument to pred
bool ioopm_hash_table_any(const ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_fun, void *arg);