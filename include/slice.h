/*
packge:
    slice
sketch:
    This package implements slice type and its related operations.
components:
    slice.h
    slice.c
dependce:
    stdlib.h
    string.h
    stdio.h
*/

#ifndef SLICE_H
#define SLICE_H

/*
sketch:
    The slice structure.
*/
typedef struct
{
    int unit_size;  // The size of each unit.
    int length;     // The number of unit in the slice.
    int capacity;   // The capacity of slice.
    char* data;     // The data area.
} _slice, *slice;

/*
sketch:
    Make context in array align to the left.
param1:
    The left (start) address (pointer).
param2:
    The data start address (pointer).
param3:
    The data size (Byte).
*/
static void copy_to_front (void* start, void* first_data_ptr, int size);

/*
sketch:
    Make context in array move to the right.
param1:
    The target start address (pointer).
param2:
    The data start address (pointer).
param3:
    The data size (Byte).
attention:
    Pay attention to array RANGE!!!
*/
static void copy_to_back (void* start, void* first_data_ptr, int size);

/*
sketch:
    Make the capacity of slice twice bigger.
param1:
    The slice you want to operate on.
return:
    The silce after operaton.
attention:
    This function may have malloc fail error.
*/
static slice slice_get_bigger(slice ori_slice);

/*
sketch:
    Create a new slice.
param1:
    The size of each unit in slice.
param2:
    The capacity of slice.
return:
    The newly created slice.
attention:
    If malloc failed, this func will retrun input slice.
*/
slice slice_new (int unit_size, int capacity);

/*
sketch:
    Replace the data in a unit of slice.
param1:
    The slice you want to operate on.
param2:
    The index of the original unit.
param3:
    The pointer of data you want to replace with.
return:
    The silce after operaton.
attention:
    replace_index < ori_slice->lenght (or return input slice).
*/
slice slice_replace (slice ori_slice, int replace_index, void* replace_data_ptr);

/*
sketch:
    Insert a unit in slice.
param1:
    The slice you want to operate on.
param2:
    The index of the insert unit.
param3:
    The pointer of data you want to insert in.
return:
    The silce after operaton.
attention:
    insert_index <= ori_slice->lenght (or return input slice).
*/
slice slice_insert (slice ori_slice, int insert_index, void* insert_data_ptr);

/*
sketch:
    Append a unit to slice.
param1:
    The slice you intend to append to.
param2:
    The pointer of the unit.
return:
    The silce after operaton.
*/
slice slice_append (slice ori_slice, void* app_data_ptr);

/*
sketch:
    Remove a unit from slice.
param1:
    The slice you want to operate on.
param2:
    The index of the unit you want to move.
return:
    The silce after operaton.
attention:
    remove_index < ori_slice->length (or return input slice).
*/
slice slice_remove (slice ori_slice, int remove_index);

/*
sketch:
    Pop up a unit of slice.
param1:
    The slice you want to operate on.
return:
    The silce after operaton.
attention:
    ori_slice->length > 0 (or return input slice).
*/
slice slice_pop (slice ori_slice);

/*
sketch:
    Query the data in the slice.
param1:
    The slice you want to operate on.
param2:
    The index you want to query.
return:
    The pointer of the data queried.
attention:
    ori_slice->length > query_index.
*/
void* slice_query (slice ori_slice, int query_index);

/*
sketch:
    Judge if the slice is empty.
param1:
    The slice you want to operate on.
retrun:
    if empty return 1, else return 0;
*/
int slice_empty(slice ori_slice);

/*
sketch:
    Free the slice.
param1:
    The slice you want to free.
*/
void slice_free (slice s);

#endif // SLICE_H
