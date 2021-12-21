#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

struct tree_node
{
    char *word;
    struct tree_node *lf_child;
    struct tree_node *ri_child;
    int count;
};

void treeprint(struct tree_node *);
int getword(char *, int);
void add_tree(struct tree_node *p, char *w);
struct tree_node *alloc_node(void);
void print_tree(struct tree_node *p);

int main()
{
    struct tree_node *root = alloc_node();
}

void add_tree(struct tree_node *p, char *w)
{
    if (!p)
    { // c有没有指针判空来着？
        p = alloc_node();
        p->count = 0;
        p->lf_child = NULL;
        p->ri_child = NULL;
        p->word = w;
        return;
    }
    int flag = strcmp(w, p->word);
    if (flag > 0)
    {
        add_tree(p->ri_child, w);
    }
    else if (flag < 0)
    {
        add_tree(p->lf_child, w);
    }
    else
    {
        (p->count)++;
    }
}

struct tree_node *alloc_node(void)
{
    return (struct tree_node *)malloc(sizeof(struct tree_node));
}

void print_tree(struct tree_node *p){
    if(!p){
        return;
    }
    print_tree(p->lf_child);
    printf("%s : %d \n",p->word,p->count);
    print_tree(p->ri_child);
}