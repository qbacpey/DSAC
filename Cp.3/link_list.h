#include <stdio.h>
#include <stdlib.h>
typedef struct __node__
{
    int data;
    struct __node__ *prev;
    struct __node__ *succ;
} Node;

typedef struct
{
    Node *header;
    Node *trailer;
    int size;
} List;

List *init_list();
Node *push_front(List *, int elem);
Node *push_back(List *, int elem);
Node *insert_as_pred(List *, Node *, int elem);
Node *insert_as_succ(List *, Node *, int elem);
int del_node(List *, Node *);
void clean(List *);
List *unique(List *);
List *copy_to(List *);
Node *move_back(Node *, Node *);
Node *move_front(Node *, Node *);
List *merge_list(List *A, List *B);
Node *merge_sort(List *, Node *beg, int length);
Node *merge_sort_merge(Node *beg, int beg_len, List *list, Node *mid, int mid_len);

Node *first(List *);
Node *last(List *);
void print_list(List *);
int size(List *);

int data(Node *);
Node *pred(List *, Node *);
Node *succ(List *, Node *);

Node *find(List *, int elem);
Node *find_before(List *, int elem, int distance, Node *);
Node *find_before_all(List *, int elem, Node *);

List *init_list()
{
    List *link_list = (List *)malloc(sizeof(List));
    Node *head = (link_list->header = (Node *)malloc(sizeof(Node)));
    Node *trail = (link_list->trailer = (Node *)malloc(sizeof(Node)));
    head->prev = NULL;
    head->succ = trail;

    trail->succ = NULL;
    trail->prev = head;

    link_list->size = 0;
    return link_list;
}

Node *push_front(List *list, int elem)
{
    return insert_as_succ(list, list->header, elem);
}
Node *push_back(List *list, int elem)
{
    return insert_as_pred(list, list->trailer, elem);
}

Node *insert_as_pred(List *list, Node *this_node, int elem)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    Node *prev = this_node->prev;
    new_node->prev = prev;
    new_node->succ = this_node;
    new_node->data = elem;
    prev->succ = new_node;
    this_node->prev = new_node;
    list->size++;
    return new_node;
}

Node *insert_as_succ(List *list, Node *this_node, int elem)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    Node *succ = this_node->succ;
    new_node->prev = this_node;
    new_node->succ = succ;
    new_node->data = elem;
    succ->prev = new_node;
    this_node->succ = new_node;
    list->size++;
    return new_node;
}

int del_node(List *list, Node *this_node)
{
    Node *prev = this_node->prev;
    Node *succ = this_node->succ;
    int data = this_node->data;
    free(this_node);
    prev->succ = succ;
    succ->prev = prev;
    list->size--;
    return data;
}

void clean(List *list)
{
    for (Node *head = list->header, *trail = list->trailer; head->succ != trail;)
    {
        del_node(list, head->succ);
    }
    free((void *)(list->header));
    free((void *)(list->trailer));
    free((void *)list);
}

Node *first(List *list)
{
    return succ(list, list->header);
}

Node *last(List *list)
{
    return pred(list, list->trailer);
}

void print_list(List *list)
{
    for (Node *head = (list->header)->succ, *trail = list->trailer; head != trail;)
    {
        printf("%d ", head->data);
        head = head->succ;
    }
}

int size(List *list)
{
    return list->size;
}

int data(Node *this_node)
{
    return this_node->data;
}

Node *pred(List *list, Node *this_node)
{
    Node *prev = this_node->prev;
    return (prev == list->header) ? NULL : prev;
}

Node *succ(List *list, Node *this_node)
{
    Node *succ = this_node->succ;
    return (succ == list->trailer) ? NULL : succ;
}

Node *find(List *list, int elem)
{
    for (Node *head = (list->header)->succ, *trail = list->trailer; head != trail;)
    {
        if (head->data == elem)
            return head;
        head = head->succ;
    }
    return NULL;
}
Node *find_before(List *list, int elem, int distance, Node *node)
{
    for (Node *q = node->prev; q->prev != NULL && (distance--) > 0;)
    {
        if (q->data == elem)
            return q;
        q = q->prev;
    }
    return NULL;
}
Node *find_before_all(List *list, int elem, Node *node)
{
    for (Node *q = node->prev; q->prev != NULL;)
    {
        if (q->data == elem)
            return q;
        q = q->prev;
    }
    return NULL;
}
List *unique(List *list)
{
    Node *windows = list->header;
    while (windows = windows->succ) // 空指针判断
    {
        Node *old_node = find_before_all(list, windows->data, windows);
        if (old_node)
        {
            del_node(list, old_node);
        }
    }
    return list;
}
List *merge_list(List *A, List *B)
{
    List *C = copy_to(A);
    List *D = copy_to(B);
    last(C)->succ = first(D);
    first(D)->prev = last(C);
    free((void *)(C->trailer));
    C->trailer = D->trailer;
    C->size += D->size;
    free((void *)(D->header));
    free((void *)D);
    return C;
}

List *copy_to(List *A)
{
    List *C = init_list();
    Node *windows = A->header;
    while (windows = windows->succ)
    {
        push_back(C, data(windows));
    }
    return C;
}

/**
 * @brief 将节点B移动到节点A的前边
 * 移动之后应当呈现A->B->C
 * @param A 主语节点
 * @param B 谓语节点
 * @return 返回B节点原来的后继节点
 */
Node *move_back(Node *A, Node *B)
{
    (B->prev)->succ = B->succ;
    (B->succ)->prev = B->prev;
    B->prev = A;
    Node *C = B->succ;
    B->succ = A->succ;
    (A->succ)->prev = B;
    A->succ = B;

    return C;
}
Node *move_front(Node *A, Node *B)
{
    (B->prev)->succ = B->succ;
    Node *C = B->succ;
    B->prev = A->prev;
    B->succ = A;
    (A->prev)->succ = B;
    A->prev = B;
    return C;
}

/**
 * @brief 为什么用c实现链表的归并排序和用C++实现如此不同？
 * 本质上来说，是因为在排序两个子序列之后（注意这里使用的是复制指针）指针指向的对象确实没有发生改变
 * 它们所指向的对象可能就不是此子序列的首节点了
 * 在这种情形下，由于函数调用使用的方式是值传递，就可能会导致意义上的错误
 * 
 * 想要应对这种情况，第一种方式就是通过返回值将指针修正为正确的值
 * 另一种应对方式是在merge_sort中保持p节点的前一个节点而不是此节点本身来防止这种情况的发生
 * C++中这一变化可以通过使用引用而不是指针来维护
 * 
 * @param list 
 * @param p 
 * @param n 
 * @return Node* 
 */
Node * merge_sort(List *list, Node *p, int n)
{
    if (n < 2)
    {
        return p;
    }
    int m = n >> 1;
    p = merge_sort(list, p, m); // 这里的交换是关键！！！
    Node *q = p;
    for (int i = 0; i < m; i++)
    {
        q = q->succ;
    }
    q = merge_sort(list, q, n - m);
    return merge_sort_merge(p, m, list, q, n - m);

}

Node *merge_sort_merge(Node *beg, int beg_len, List *list, Node *mid, int mid_len)
{
    Node *p_prev = beg->prev;
    Node *cursor = beg->prev;
    while (beg_len != 0 && mid_len != 0)
    {
        if (data(mid) < data(beg))
        {
            mid = move_back(cursor, mid);
            mid_len--;
        }
        else
        {
            beg = move_back(cursor, beg);
            beg_len--;
        }
        cursor = cursor->succ;
    }
    return p_prev->succ;
}