#include "shelf.h"



bool shelf_lookup(ioopm_hash_table_t* storage_ht, char* id)
{
    elem_t unused;
    return ioopm_hash_table_lookup(storage_ht, str_elem(id), &unused);
}


shelf_t* shelf_get(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, char* shelf_id)
{
    elem_t item_name;
    ioopm_hash_table_lookup(storage_ht, str_elem(shelf_id), &item_name);

    if(merch_in_stock(merchandise_ht, item_name.s))
    {
        ioopm_list_t* locations_list = merch_locations(merchandise_ht, item_name.s);
        ioopm_list_iterator_t* locations_iter = ioopm_list_iterator(locations_list);

        while(ioopm_iterator_has_next(locations_iter))
        {
            elem_t current = ioopm_iterator_current(locations_iter);
            shelf_t* shelf_pointer = current.p;
            if(strcmp(shelf_pointer->shelf, shelf_id) == 0)
            {
                ioopm_iterator_destroy(locations_iter);
                return shelf_pointer;
            }
            ioopm_iterator_next(locations_iter, &current);
        }
        elem_t current = ioopm_iterator_current(locations_iter);
        shelf_t* shelf_pointer = current.p;
        if(strcmp(shelf_pointer->shelf, shelf_id) == 0)
        {
            ioopm_iterator_destroy(locations_iter);
            return shelf_pointer;
        }
    }
    return NULL;

}


char* shelf_input(char* message)
{
    char* id = ask_question_string(message);

    while((strlen(id) != 3) || (toupper(id[0]) < 'A' || toupper(id[0]) > 'Z') || ((id[1] - '0') < 0 || (id[1] - '0') > 9) || ((id[2] - '0') < 0 || (id[2] - '0') > 9))
    {
        free(id);
        puts("Not a valid id!\nAccepted format: One capital letter (A-Z) followed by two digits (0-9)\n");
        id = ask_question_string(message);
    }
    return id;
}


shelf_t* shelf_create(char* id, int quantity)
{
    shelf_t* new_self = calloc(1, sizeof(shelf_t));
    new_self->shelf = id;
    new_self->quantity = quantity;
    return new_self;
}


void shelf_append_to_merch(merch_t* merch, shelf_t* shelf)
{
    ioopm_linked_list_append(merch->location, void_elem(shelf));
}


void shelf_destroy(shelf_t* shelf)
{
    if (shelf == NULL)
    {
        return; 
    }
    free(shelf->shelf);
    free(shelf);
}


void shelf_free_all(ioopm_hash_table_t* storage_ht) 
{
  for(int i = 0; i < 17; i++)  
  {
    entry_t *current_entry = storage_ht->buckets[i];
    entry_t *next_entry = current_entry->next;
    while(next_entry != NULL)
    {
      free(current_entry->key.s);
      free(current_entry->value.s);  
      free(current_entry);
      current_entry = next_entry;
      next_entry = current_entry->next;
    }
    free(current_entry->key.s);
    free(current_entry->value.s);  
    free(current_entry);       
  }
  free(storage_ht);
}


shelf_t* shelf_copy(shelf_t* old_shelf)
{
    return shelf_create(strdup(old_shelf->shelf), old_shelf->quantity);
}


bool shelf_remove(ioopm_hash_table_t* storage_ht, elem_t key, elem_t *result) 
{
  int bucket = storage_ht->hash_function(key) % 17;
  if(bucket < 0) 
  {
    bucket += 17;
  }
  entry_t *previous = find_previous_entry_for_key(storage_ht, storage_ht->buckets[bucket], key);
  entry_t *to_remove = previous->next;

  if(to_remove && storage_ht->key_func(to_remove->key, key)) 
  {
    *result = to_remove->value;
    previous->next = to_remove->next;
    free(to_remove->key.s);
    free(to_remove->value.s);
    free(to_remove);
    storage_ht->size--;
    return true;
  }
  return false;
}