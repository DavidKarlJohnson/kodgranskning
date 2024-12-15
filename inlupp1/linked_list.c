#include "linked_list.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct link link_t;
typedef struct list ioopm_list_t; 
typedef struct iter ioopm_list_iterator_t;


/// @brief Start at the head of the list and step forward right until the element before the given index
/// @param list The list to find the link in
/// @param index The index of the element we'd like to find the preceding link to
/// @return The link connected before the given index
/// @example A list with the elements [1, 2, 3, 4] will return link 2 (at index 1) if index 2 is the argument.
static link_t *find_previous_link(const ioopm_list_t *list, const int index) 
{
    link_t *current = list->head;

    for(int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current;
}


/// @brief Creates a link
/// @param element The value of the link
/// @param next The link which the link created should point to
/// @return The created link
static link_t *link_create(const elem_t element, link_t *next)
{
    link_t *link = calloc(1, sizeof(link_t));
    if(link)
    {
        link->element = element;
        link->next = next;
    }
    return link;
}


/// @brief Helper function for 'linked_list_insert'. 
///        * If an index smaller than 0 is entered, 0 will be returned.
///        * If an index greater than the list length is entered, the last index will be returned.
///        * If an index within bounds is entered, it's returned as is.
/// @param index The desired index entered by the user. Will be adjusted if not within bounds.
/// @param list The list to find the index for.
/// @return An index position that's within the bounds of the list
/// @example A list with the elements [1, 2, 3, 4] will return 3 if 6 is given as argument.
///          0 will be returned if -2 is given as argument. 1 will be returned if 1 is given as argument.
static size_t linked_list_adjust_index(int index, ioopm_list_t *list)
{
    if(index <= 0) {  // Too low
        return 0;
    } 
    else if(index > ioopm_linked_list_size(list)) {  // Too high
        return ioopm_linked_list_size(list) - 1;
    }
    return index;  // Correct
}


ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function* compare_function) 
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));

    if (list)  // Create a dummy element that both head and tail points to
    {
      list->head = list->tail = calloc(1, sizeof(struct link));
    }
    list->compare_function = compare_function;
    return list;
}


void ioopm_linked_list_destroy(ioopm_list_t *list) 
{
    elem_t useless;
    while((ioopm_linked_list_size(list)) > 0) {  // Free all elements except for dummy
        ioopm_linked_list_remove(list, 0, &useless);
    }
    free(list->head);  // Free dummy
    free(list);
}


void ioopm_linked_list_prepend(ioopm_list_t *list, const elem_t element) 
{
    ioopm_linked_list_insert(list, 0, element);
}


void ioopm_linked_list_append(ioopm_list_t *list, const elem_t element)
{
    ioopm_linked_list_insert(list, ioopm_linked_list_size(list), element);
}


void ioopm_linked_list_insert(ioopm_list_t *list, int index, const elem_t element)
{
    const size_t valid_index = linked_list_adjust_index(index, list);
    link_t *previous_link = find_previous_link(list, valid_index);
    link_t *new_link = link_create(element, previous_link->next);
    previous_link->next = new_link;

    if(valid_index == ioopm_linked_list_size(list)) {  // Update tail if inserted at the end
        list->tail = new_link;
    }

    list->size++;
}



bool ioopm_linked_list_remove(ioopm_list_t *list, int index, elem_t *result)
{
    if (list->size == 0){
        return false;
    }

    const size_t index_adjusted = linked_list_adjust_index(index, list);

    link_t *previous_link = find_previous_link(list, index_adjusted);
    link_t *to_remove = previous_link->next;
    const elem_t to_remove_element = to_remove->element;  // Store the value of the link before freeing

    if(index == ioopm_linked_list_size(list) - 1) {  // Update tail if last element is removed
        list->tail = previous_link;
    }

    previous_link->next = to_remove->next;  // Unlink
    free(to_remove);
    list->size--;
    *result = to_remove_element;
    return true;
}


bool ioopm_linked_list_get(ioopm_list_t *list, int index, elem_t *result)  
{
    if (list->size == 0)
    {
        return false;
    }
    const size_t index_adjusted = linked_list_adjust_index(index, list);
    link_t *previous_link = find_previous_link(list, index_adjusted);
    *result = previous_link->next->element;
    return true;
}


bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
    link_t *current = list->head;

    while(current != NULL){

        if(list->compare_function(current->element, element) == true){
            return true;
        }

        current = current->next;
    }

    return false;
}


size_t ioopm_linked_list_size(const ioopm_list_t *list)  
{   
    return list->size;
}


bool ioopm_linked_list_is_empty(const ioopm_list_t *list)
{
    return ioopm_linked_list_size(list) == 0;
}


void ioopm_linked_list_clear(ioopm_list_t *list){
    elem_t useless;
     while((ioopm_linked_list_size(list)) > 0) {
        ioopm_linked_list_remove(list, 0, &useless);
    }
}


bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_element_predicate *prop, void *extra)
{
    link_t *current = list->head;

    while(current != NULL){

        if(prop(current->element, extra) == false){
            return false;
        }

        current = current->next;
    }

    return true;
}


bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_element_predicate *prop, void *extra)
{
    link_t *current = list->head;

    while(current != NULL){

        if(prop(current->element, extra) == true){
            return true;
        }

        current = current->next;
    }

    return false;
}


void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_element_apply_function *fun, void *extra){
    link_t *current = list->head;

    while(current != NULL){
        fun(&current->element, extra);
        current = current->next;
    }

    return;
}


// ------ ITERATOR -------


ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *new_iterator = calloc(1, sizeof(ioopm_list_iterator_t));
  new_iterator->current = list->head;  
  new_iterator->list = list;  
  return new_iterator;
}


bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{   
    if(iter->current == NULL)
    {
        return false;
    }
    if (iter->current->next == NULL)
    {
        return false;
    }
    if (iter->current->next->next == NULL)
    {
        return false;
    }
    return true;
}


bool ioopm_iterator_next(ioopm_list_iterator_t *iter, elem_t *result){
    if(!(ioopm_iterator_has_next(iter)))
    {
        return false;
    }

    iter->current = iter->current->next;
    *result = iter->current->next->element;
    return true;
}


void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = iter->list->head;
}


elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    return iter->current->next->element;
}


void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

bool ioopm_iterator_remove(ioopm_list_iterator_t *iter, elem_t *result)
{
    if (!(ioopm_iterator_has_next(iter)))
    {
        return false;
    }
    link_t *to_remove = iter->current->next;
    *result = to_remove->element;
    iter->current->next = to_remove->next;
    iter->list->size--;
    free(to_remove);
    return true;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element){
    iter->current->next = link_create(element, iter->current->next);
    iter->list->size = iter->list->size + 1;
    return;
}

