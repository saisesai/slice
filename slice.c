#include "slice.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
sketch:
    Make context in array move to the left.
param1:
    The target start address (pointer).
param2:
    The data start address (pointer).
param3:
    The data size (Byte).
*/
static void copy_to_front (void* start, void* first_data_ptr, int size)
{
    char* cs = (char*)start;
    char* fcp = (char*)first_data_ptr;
    int i;
    for (i = 0; i < size; i++)
        cs[i] = fcp[i];
}

/*
sketch:
    Make context in array move to the right.
param1:
    The target start address (pointer).
param2:
    The data start address (pointer).
param3:
    The data size (Byte).
*/
static void copy_to_back (void* start, void* first_data_ptr, int size)
{
    char* cs = (char*)start;
    char* fcp = (char*)first_data_ptr;
    int i;
    for (i = size - 1; i >= 0; i--)
        cs[i] = fcp[i];
}

/*
sketch:
    Make the capacity of slice twice bigger.
param1:
    The slice you want to operate on.
return:
    The silce after operaton.
attention:
    If malloc failed, this func will retrun input slice.
*/
static slice slice_get_bigger (slice ori_slice)
{
    slice st = slice_new (ori_slice->unit_size, ori_slice->capacity * 2);
    if (st == NULL)
    {
        printf ("[SION][slice][ERROR][slice_get_bigger]:st malloc failed!\n");
        return ori_slice;
    }
    st->length = ori_slice->length;
    memcpy (st->data, ori_slice->data, ori_slice->unit_size * ori_slice->length);
    slice_free (ori_slice);
    return st;
}

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
    You need to call slice_free() to free the slice manualy.
    If malloc failed, this func will retrun NULL, and show some
    useful information, and free the memory malloced automatically.
*/
slice slice_new (int unit_size, int capacity)
{
    slice ns = (_slice*)malloc (sizeof (_slice));
    if (ns == NULL)
    {
        printf ("[SION][slice][ERROR][slice_new]:ns malloc failed!\n");
        return NULL;
    }
    /*
    typedef struct
    {
        int unit_size;  //1. The size of each unit.
        int length;     //2. The number of unit in the slice.
        int capacity;   //3. The capacity of slice.
        char* data;     //4. The data area.
    } _slice, *slice;
    */
    //1. The size of each unit.
    ns->unit_size = unit_size;
    //2. The number of unit in the slice.
    ns->length = 0;
    //3. The capacity of slice.
    ns->capacity = capacity;
    //4. The data area.
    ns->data = (char*)malloc (sizeof (char) * unit_size * capacity);
    if (ns == NULL)
    {
        printf ("[SION][slice][ERROR][slice_new]:ns->data malloc failed!\n");
        free (ns);
        return NULL;
    }
    return ns;
}

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
slice slice_replace (slice ori_slice, int replace_index, void* replace_data_ptr)
{
    if (replace_index >= ori_slice->length)
    {
        printf ("[SION][slice][ERROR][slice_replace]:replace_index out of range!\n");
        return ori_slice;
    }
    // Get the unit pointer.
    char* unit_ptr = & (ori_slice->data[0]) + replace_index * ori_slice->unit_size;
    // Copy the new data to the target unit.
    memcpy (unit_ptr, replace_data_ptr, ori_slice->unit_size);
    return ori_slice;
}

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
    insert_index <= ori_slice->length (or return input slice).
*/
slice slice_insert (slice ori_slice, int insert_index, void* insert_data_ptr)
{
    if (insert_index > ori_slice->length)
    {
        printf ("[SION][slice][ERROR][slice_insert]:insert_index out of range!\n");
        return ori_slice;
    }
    // If slice capacity is not enough, create a bigger one,
    // and free the older one.
    if (ori_slice->capacity == ori_slice->length)
        ori_slice = slice_get_bigger (ori_slice);
    // Move the original data a unit back.
    char* start_ptr = ori_slice->data + (insert_index + 1) * ori_slice->unit_size;
    char* first_data_ptr = ori_slice->data + insert_index * ori_slice->unit_size;
    int copy_data_size = (ori_slice->length - insert_index) * ori_slice->unit_size;
    copy_to_back (start_ptr, first_data_ptr, copy_data_size);
    // Copy the new data to the target unit.
    memcpy (first_data_ptr, insert_data_ptr, ori_slice->unit_size);
    // Add the length of slice.
    ori_slice->length++;
    return ori_slice;
}

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
slice slice_append (slice ori_slice, void* app_data_ptr)
{
    return slice_insert (ori_slice, ori_slice->length, app_data_ptr);
}

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
    0 <= remove_index < ori_slice->length (or return input slice).
*/
slice slice_remove (slice ori_slice, int remove_index)
{
    if (remove_index >= ori_slice->length || remove_index < 0)
    {
        printf ("[SION][slice][ERROR][slice_remove]:remove_index out of range!\n");
        return ori_slice;
    }
    char* target_pos = ori_slice->data + remove_index * ori_slice->unit_size;
    char* src_pos = target_pos + ori_slice->unit_size;
    int copy_data_size = (ori_slice->length - remove_index - 1) * ori_slice->unit_size;
    copy_to_front (target_pos, src_pos, copy_data_size);
    ori_slice->length--;
    return ori_slice;
}

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
slice slice_pop (slice ori_slice)
{
    if (ori_slice->length <= 0)
    {
        printf ("[SION][slice][ERROR][slice_pop]:ori_slice->length <= 0!\n");
        return ori_slice;
    }
    return slice_remove (ori_slice, 0);
}

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
void* slice_query (slice ori_slice, int query_index)
{
    if (ori_slice->length <= query_index)
    {
        printf ("[SION][slice][ERROR][slice_query]:ori_slice->length <= query_index!\n");
        return ori_slice;
    }
    return ori_slice->data + query_index * ori_slice->unit_size;
}

/*
sketch:
    Judge if the slice is empty.
param1:
    The slice you want to operate on.
retrun:
    if empty return 1, else return 0;
*/
int slice_empty (slice ori_slice)
{
    return (ori_slice->length > 0) ? 0 : 1;
}

/*
sketch:
    Free the slice.
param1:
    The slice you want to free.
*/
void slice_free (slice s)
{
    free (s->data);
    free (s);
}
