#include "merchandise.h"



merch_t* merch_create(char* name, char* desc, int price)
{
    merch_t* new_merch = calloc(1, sizeof(merch_t));
    new_merch->name = name;
    new_merch->description = desc;
    new_merch->price = price;
    new_merch->location = ioopm_linked_list_create();
    return new_merch;
}


bool merch_lookup(ioopm_hash_table_t* merchandise_ht, char* name)
{
    elem_t unused;
    return ioopm_hash_table_lookup(merchandise_ht, str_elem(name), &unused);
}


merch_t* merch_get(ioopm_hash_table_t* merchandise_ht, char* name)
{
    elem_t merch;
    ioopm_hash_table_lookup(merchandise_ht, str_elem(name), &merch);
    merch_t* merch_pointer = merch.p;
    return merch_pointer;
}


bool merch_in_stock(ioopm_hash_table_t* merchandise_ht, char* name)
{
    if(!ioopm_hash_table_is_empty(merchandise_ht))  // No merch at all
    {
        elem_t merch_struct;
        if(ioopm_hash_table_lookup(merchandise_ht, str_elem(name), &merch_struct))  // Merch exists
        {
            merch_t* merch_struct_pointer = merch_struct.p;
            return !ioopm_linked_list_is_empty(merch_struct_pointer->location);
        }
    }
    return false;
}

char* merch_input(ioopm_hash_table_t* merchandise_ht, char* message, bool exists)
{
    char* name = capitalize(ask_question_string(message));

    if(exists)
    {
        while(!merch_lookup(merchandise_ht, name))
        {
            puts("No such merch!\n");
            merch_print_all(merchandise_ht);
            free(name);
            name = capitalize(ask_question_string(message));
        }
    }
    else
    {
        while(merch_lookup(merchandise_ht, name))
        {
            puts("That item already exists!\n");
            merch_print_all(merchandise_ht);
            free(name);
            name = capitalize(ask_question_string(message));
        }
    }
    return name;
}


void merch_destroy(merch_t* merch)
{
    if (merch == NULL)
    {
        return; 
    }
    free(merch->name);
    free(merch->description);
    
    if(!ioopm_linked_list_is_empty(merch->location)) {
        ioopm_list_iterator_t* iter = ioopm_list_iterator(merch->location);
        elem_t unused;
        while(ioopm_iterator_has_next(iter))
        {
            elem_t current = ioopm_iterator_current(iter);
            shelf_t* current_shelf = current.p;
            shelf_destroy(current_shelf);
            ioopm_iterator_next(iter, &unused);
        }
        elem_t current = ioopm_iterator_current(iter);
        shelf_t* current_shelf = current.p;
        shelf_destroy(current_shelf);

        ioopm_iterator_destroy(iter);
    }
    ioopm_linked_list_destroy(merch->location);
    free(merch);
}


ioopm_list_t* merch_locations(ioopm_hash_table_t* merchandise_ht, char* item_name)
{
    elem_t item;
    ioopm_hash_table_lookup(merchandise_ht, str_elem(item_name), &item);
    merch_t *pointer_to_item = item.p;
    return pointer_to_item->location;
}


int merch_total_quantity(ioopm_hash_table_t* merchandise_ht, char* item_name)
{
    int sum = 0;
    ioopm_list_t* locations = merch_locations(merchandise_ht, item_name);

    for(int i = 0; i < ioopm_linked_list_size(locations); i++) 
    {
        elem_t current;
        ioopm_linked_list_get(locations, i, &current);
        shelf_t* shelf_pointer = current.p;
        sum += shelf_pointer->quantity;
    }
    return sum;
}


void merch_decrease_stock(ioopm_hash_table_t* merchandise_ht, ioopm_hash_table_t* storage_ht, char* item_name, int decrease)
{
    ioopm_list_t* locations = merch_locations(merchandise_ht, item_name);
    
    for(int i = ioopm_linked_list_size(locations) - 1; i >= 0 ; i--)
    {
        elem_t current;
        ioopm_linked_list_get(locations, i, &current);
        shelf_t* shelf_pointer = current.p;
        
        if(decrease > shelf_pointer->quantity)  // Remove
        {
            decrease -= shelf_pointer->quantity;
            elem_t unused;
            shelf_remove(storage_ht, str_elem(shelf_pointer->shelf), &unused);
            ioopm_linked_list_remove(locations, i, &unused);
            shelf_destroy(unused.p);
        }
        else if(decrease == shelf_pointer->quantity)  // Remove and terminate
        {
            decrease -= shelf_pointer->quantity;
            elem_t unused;
            shelf_remove(storage_ht, str_elem(shelf_pointer->shelf), &unused);
            ioopm_linked_list_remove(locations, i, &unused);
            shelf_destroy(unused.p);
            break;
        }
        else  // Decrease
        {
            shelf_pointer->quantity -= decrease;
            break;
        } 
    }
}


void merchandise_free_all(ioopm_hash_table_t* merchandise_ht) 
{
    if(!ioopm_hash_table_is_empty(merchandise_ht))
    {   ioopm_list_t* merch_list = ioopm_hash_table_values(merchandise_ht);
        ioopm_list_iterator_t* merch_iter = ioopm_list_iterator(merch_list);

        while(ioopm_iterator_has_next(merch_iter))
        {
            elem_t current = ioopm_iterator_current(merch_iter);
            merch_destroy(current.p);
            elem_t unused;
            ioopm_iterator_next(merch_iter, &unused);
        }
        elem_t current = ioopm_iterator_current(merch_iter);
        merch_destroy(current.p);
        ioopm_iterator_destroy(merch_iter);
        ioopm_linked_list_destroy(merch_list);
    }
    ioopm_hash_table_destroy(merchandise_ht);
}


merch_t* merch_copy(merch_t* old_merch)
{
    merch_t* new_merch = merch_create(strdup(old_merch->name), strdup(old_merch->description), old_merch->price);
    ioopm_list_t* old_merch_locations = old_merch->location;
    for(int i = 0; i < ioopm_linked_list_size(old_merch_locations); i++)
    {
        elem_t shelf_elem;
        ioopm_linked_list_get(old_merch_locations, i, &shelf_elem);
        shelf_t* shelf_pointer = shelf_elem.p;

        shelf_t* copy = shelf_copy(shelf_pointer);
        shelf_append_to_merch(new_merch, copy);
    }
    return new_merch;
}


void merch_print_all(ioopm_hash_table_t* merchandise_ht)
{
    if(!ioopm_hash_table_is_empty(merchandise_ht))
    {   
        printf("---------- Available merchandise ----------\n");
        ioopm_list_t* merch_names = ioopm_hash_table_keys(merchandise_ht);
        for(int i = 0; i < ioopm_linked_list_size(merch_names); i++)
        {
            elem_t current;
            ioopm_linked_list_get(merch_names, i, &current);
            printf("%d.  %s\n", i + 1, current.s);
        }
        printf("-------------------------------------------\n");
        ioopm_linked_list_destroy(merch_names);
    }
}