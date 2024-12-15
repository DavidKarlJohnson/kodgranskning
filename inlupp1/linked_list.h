#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "common.h"

/**
 * @file linked_list.h
 * @author Anton Backman och David Johnson
 * @date 17 Oct 2024
 * @brief Simple implementation of linked lists.
*/


typedef struct link link_t;
typedef struct list ioopm_list_t; 
typedef struct iter ioopm_list_iterator_t;


typedef bool ioopm_element_predicate(elem_t element, void *extra);  
typedef void ioopm_element_apply_function(elem_t *element, void *extra);  


struct link
{
    elem_t element;
    link_t *next;
};

struct list
{
    link_t *head;
    link_t *tail;
    size_t size;
    ioopm_eq_function *compare_function;
};

struct iter 
{
  link_t *current;
  ioopm_list_t *list;  
};


/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create();

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param element the element to be appended
void ioopm_linked_list_append(ioopm_list_t *list, elem_t element);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param element the element to be prepended
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t element);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element. Automatically adjusts a non acceptable index
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param element the value to be inserted
void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t element);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list
/// @param index the position in the list
/// @param result Used to recive the value of the link at said index
/// @return Bool depending if te removal was successful (True = yes)
bool ioopm_linked_list_remove(ioopm_list_t *list, int index, elem_t *result);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param result Used to recive the value of the link at said index
/// @return bool if depending if something was at that index or not
bool ioopm_linked_list_get(ioopm_list_t *list, int index, elem_t *result); 

/// @brief Test if an element is in the list
/// @param list the linked list
/// @param element the element sought after
/// @return true if element is in the list, else false
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t ioopm_linked_list_size(const ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool ioopm_linked_list_is_empty(const ioopm_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list the linked list
void ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_element_predicate *prop, void *extra);

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_element_predicate *prop, void *extra);

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of fun
void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_element_apply_function *fun, void *extra);

///----- ITERATORS -----

/// @brief Create an iterator for a given list
/// @param list the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list);

/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if there is at least one more element 
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step
/// @param iter the iterator
/// @param result used to get the value of the next element
/// @return true if it successfully stepped to the next element
bool ioopm_iterator_next(ioopm_list_iterator_t *iter, elem_t *result);

/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @param result used to get the value of the removed element
/// @return true or false depending on if the value was removed
bool ioopm_iterator_remove(ioopm_list_iterator_t *iter, elem_t *result);

/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @return the current element
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);