typedef struct __list_node__
{
    void *data;
    struct __list_node__ *prev;
    struct __list_node__ *succ;
} ListNode;

typedef void (*PLN)(ListNode *);

typedef struct __link_list__
{
    ListNode *header;
    ListNode *trailer;
    int size;
} LinkList;

LinkList *init_list();
ListNode *push_front(LinkList *, void *data);
ListNode *push_back(LinkList *, void *data);
ListNode *insert_as_pred(LinkList *, ListNode *, void *data);
ListNode *insert_as_succ(LinkList *, ListNode *, void *data);
void *del_node(LinkList *, ListNode *);
void clean_list(LinkList *);
LinkList *unique_list(LinkList *);
LinkList *copy_list_to(LinkList *);
LinkList *merge_list(LinkList *A, LinkList *B);

ListNode *first_list(LinkList *);
ListNode *last_list(LinkList *);
void print_list(LinkList *, PLN);
int size_list(LinkList *);

void *data_of_node(ListNode *);
ListNode *pred_of_node(LinkList *, ListNode *);
ListNode *succ_of_node(LinkList *, ListNode *);

ListNode *find_node(LinkList *, char *name);
ListNode *find_before_distance(LinkList *, char *name, int distance, ListNode *);
ListNode *find_before_all(LinkList *, char *name, ListNode *);

LinkList *init_list()
{
    LinkList *link_list = (LinkList *)malloc(sizeof(LinkList));
    ListNode *head = (link_list->header = (ListNode *)malloc(sizeof(ListNode)));
    ListNode *trail = (link_list->trailer = (ListNode *)malloc(sizeof(ListNode)));
    head->prev = NULL;
    head->succ = trail;

    trail->succ = NULL;
    trail->prev = head;

    link_list->size = 0;
    return link_list;
}

ListNode *push_front(LinkList *list, void *elem)
{
    return insert_as_succ(list, list->header, elem);
}
ListNode *push_back(LinkList *list, void *elem)
{
    return insert_as_pred(list, list->trailer, elem);
}

ListNode *insert_as_pred(LinkList *list, ListNode *this_node, void *elem)
{
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    ListNode *prev = this_node->prev;
    new_node->prev = prev;
    new_node->succ = this_node;
    new_node->data = elem;
    prev->succ = new_node;
    this_node->prev = new_node;
    list->size++;
    return new_node;
}

ListNode *insert_as_succ(LinkList *list, ListNode *this_node, void *elem)
{
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    ListNode *succ = this_node->succ;
    new_node->prev = this_node;
    new_node->succ = succ;
    new_node->data = elem;
    succ->prev = new_node;
    this_node->succ = new_node;
    list->size++;
    return new_node;
}

void *del_node(LinkList *list, ListNode *this_node)
{
    ListNode *prev = this_node->prev;
    ListNode *succ = this_node->succ;
    void *data = this_node->data;
    free(this_node);
    prev->succ = succ;
    succ->prev = prev;
    list->size--;
    return data;
}

void clean_list(LinkList *list)
{
    for (ListNode *head = list->header, *trail = list->trailer; head->succ != trail;)
    {
        void *data = del_node(list, head->succ);
    }
    free((void *)(list->header));
    free((void *)(list->trailer));
    free((void *)list);
}

ListNode *first_list(LinkList *list)
{
    return succ_of_node(list, list->header);
}

ListNode *last_list(LinkList *list)
{
    return pred_of_node(list, list->trailer);
}

void print_list(LinkList *list, PLN print_function)
{
    for (ListNode *head = (list->header)->succ, *trail = list->trailer; head != trail;)
    {
        (*print_function)(head);
        head = head->succ;
    }
}

int size_list(LinkList *list)
{
    return list->size;
}

void *data_of_node(ListNode *this_node)
{
    return this_node->data;
}

ListNode *pred_of_node(LinkList *list, ListNode *this_node)
{
    ListNode *prev = this_node->prev;
    return (prev == list->header) ? NULL : prev;
}

ListNode *succ_of_node(LinkList *list, ListNode *this_node)
{
    ListNode *succ = this_node->succ;
    return (succ == list->trailer) ? NULL : succ;
}
// /**
//  * @brief 仅比对对应节点名字是否相同
//  * @param list 表
//  * @param name 节点数据
//  * @return Node* 在表中找到的对应节点
//  */
// ListNode *find_node(LinkList *list, char *name)
// {
//     for (ListNode *head = (list->header)->succ, *trail = list->trailer; head != trail;)
//     {
//         if (!strcmp(get_name(head->data), name))
//             return head;
//         head = head->succ;
//     }
//     return NULL;
// }
// ListNode *find_before_distance(LinkList *list, char *name, int distance, ListNode *node)
// {
//     for (ListNode *q = node->prev; q->prev != NULL && (distance--) > 0;)
//     {
//         if (!strcmp(get_name(q->data), name))
//             return q;
//         q = q->prev;
//     }
//     return NULL;
// }
// ListNode *find_before_all(LinkList *list, char *name, ListNode *node)
// {
//     for (ListNode *q = node->prev; q->prev != NULL;)
//     {
//         if (!strcmp(get_name(q->data), name))
//             return q;
//         q = q->prev;
//     }
//     return NULL;
// }
// 由于使用了void*，废弃了这个函数
// LinkList *unique_list(LinkList *list)
// {
//     ListNode *windows = list->header;
//     while (windows = windows->succ) // 空指针判断
//     {
//         ListNode *old_node = find_before_all(list, windows->data->name, windows); // ！！！！
//         if (old_node)
//         {
//             del_node(list, old_node);
//         }
//     }
//     return list;
// }
LinkList *merge_list(LinkList *A, LinkList *B)
{
    LinkList *C = copy_list_to(A);
    LinkList *D = copy_list_to(B);
    last_list(C)->succ = first_list(D);
    first_list(D)->prev = last_list(C);
    free((void *)(C->trailer));
    C->trailer = D->trailer;
    C->size += D->size;
    free((void *)(D->header));
    free((void *)D);
    return C;
}
LinkList *copy_list_to(LinkList *A)
{
    LinkList *C = init_list();
    ListNode *windows = A->header;
    while (windows = windows->succ)
    {
        push_back(C, data_of_node(windows));
    }
    return C;
}