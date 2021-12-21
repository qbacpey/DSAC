#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link_list.h"

List *computing_intersection(List *A, List *B);
List *computing_union(List *A, List *B);
List *computing_complementary(List *A, List *B);
List *computing_difference(List *A, List *B);

List *computing_intersection(List *A, List *B)
{
    Node *head = first(A);
    List *C = init_list();
    while (head)
    {
        if (find(B, data(head)))
        {
            push_back(C, data(head));
        }
        head = head->succ;
    }
    return C;
}

List *computing_union(List *A, List *B)
{
    List *C = merge_list(A, B);
    return unique(A);
}

/**
 * @brief 求A相对于B的绝对补集
 * @param A 全集的子集
 * @param B 全集
 * @return List* 如果A并非是B的绝对补集，将会返回NULL并打印错误信息
 */
List *computing_complementary(List *A, List *B)
{
    if (size(computing_intersection(A, B)) != size(A))
    {
        printf("Error,can't compute the complementary of A according to B!");
        return NULL;
    }
    return computing_difference(A, B);
}

List *computing_difference(List *A, List *B)
{
    Node *head = first(B);
    List *C = init_list();
    while (head->succ)
    {
        if (!find(A, data(head)))
        {
            push_back(C, data(head));
        }
        head = head->succ;
    }
    return C;
}

int main()
{
    List *A = init_list();
    List *B = init_list();
    List *C = init_list();
    for (int i = 0; i < 20; i++)
    {
        push_back(A, rand() % 20);
        push_back(B, rand() % 20);
    }
    printf("Set A:");
    A = unique(A);
    print_list(A);
    printf("\n");
    printf("Set B:");
    B = unique(B);
    print_list(B);
    printf("\n");

    Node *index = first(A);
    for (int i = 0; i < 5; i++)
    { // 构建子集
        push_back(C, data(index = index->succ));
    }
    printf("Set C:");
    print_list(C);
    printf("\n");

    //1.computing_intersection
    printf("A与B的交集:");
    print_list(computing_intersection(A, B));
    printf("\n");

    //2.computing_union
    printf("A与B的并集:");
    print_list(computing_union(A, B));
    printf("\n");

    //3.computing_complementary
    printf("C相对于A的绝对补集:");
    List *D = computing_complementary(C, A);
    if (D)
    {
        print_list(D);
    }
    printf("\n");

    //4.computing_difference
    printf("C相对于B的差集:");
    List *F = computing_difference(C, B);
    if (F)
    {
        print_list(F);
    }
    printf("\n");
}