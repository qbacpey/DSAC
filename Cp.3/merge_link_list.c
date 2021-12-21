
#include "link_list.h"

int main(){
    List* A=init_list();
    push_back(A,2);
    push_back(A,11);
    push_back(A,117);
    push_back(A,9);
    push_back(A,21);
    push_back(A,13);
    push_back(A,5);
    print_list(A);
    printf("\n");
    merge_sort(A,first(A),size(A));
    print_list(A);
}