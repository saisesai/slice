#include <stdio.h>
#include <slice.h>

int main(int argc, char const *argv[])
{
    int n1 = 1, n2 = 2, n3 = 3;
    slice s1 = slice_new(sizeof(int), 3);
    int i;
    for (i = 0; i < 3; i++)
    {
        s1 = slice_append(s1, &n1);
        printf("len:%d cap:%d\n", s1->length, s1->capacity);
    }
    printf("len:%d cap:%d\n", s1->length, s1->capacity);
    s1 = slice_insert(s1, 0, &n2);
    printf("len:%d cap:%d\n", s1->length, s1->capacity);
    s1 = slice_insert(s1, 0, &n3);
    printf("s1[0]:%d\n", *(int *)slice_query(s1, 0));
    s1 = slice_remove(s1, 0);
    printf("s1[0]:%d\n", *(int *)slice_query(s1, 0));
    s1 = slice_pop(s1);
    printf("s1[0]:%d\n", *(int *)slice_query(s1, 0));
    s1 = slice_replace(s1, 0, &n3);
    printf("s1[0]:%d\n", *(int *)slice_query(s1, 0));
    slice_free(s1);
    return 0;
}
