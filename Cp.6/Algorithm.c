#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Graph.h"
#include "Stack.h"
#define MIN(X, Y) ((X) < (Y)) ? (X) : (Y)

typedef void (*PU)(Graph *, int, int);

void pfs(Graph *G, int s, int q, PU prioUpdater);
void dfs(Graph *, Stack *, int *clock, int v, int u, VSV);
void print_path(Graph *G, int s, int q);
static void pfs_update(Graph *G, int uk, int s);

void pfs(Graph *G, int g, int q, PU prioUpdater)
{
    Vertex *v = get_vertex(G, g);
    v->priority = 0;
    v->state = VISITED;
    v->parent = -1;
    int s = g;
    while (1)
    {
        for (int u = first_nbr(G, s); u > -1; u = next_nbr(G, s, u)) // 更新所有邻接节点优先级{
            prioUpdater(G, u, s);

        int shortest = __INT_MAX__;
        for (int w = G->n - 1; w >= 0; w--) // 寻找下一个优先级最高的节点
            if (UNDISCOVERED == get_vertex(G, w)->state && shortest > get_vertex(G, w)->priority)
            {
                shortest = MIN(shortest, get_vertex(G, w)->priority);
                s = w;
            }
        if (VISITED == get_vertex(G, s)->state || shortest == __INT_MAX__) // 整张图所有的节点都已经被发现;
        {                                                                  // 或者遍历完当前节点所属连通分量
            printf("Fail to find path from %s to %s.\n", v->data->name, get_vertex(G, q)->data->name);
            break;
        }
        else if (!strcmp(get_vertex(G, s)->data->name, get_vertex(G, q)->data->name)) // 成功找到该节点
        {
            printf("Success to find the path!\n\nNow going to print the path from \"%s\" to \"%s\":\n\n", get_vertex(G, g)->data->name, get_vertex(G, q)->data->name);
            get_vertex(G, s)->state = VISITED;
            get_edge(G, get_vertex(G, s)->parent, s)->type = TREE;
            print_path(G, g, q);
            break;
        }
        get_vertex(G, s)->state = VISITED;
        get_edge(G, get_vertex(G, s)->parent, s)->type = TREE;
    }
}

void print_path(Graph *G, int s, int q)
{
    Stack *stack = init_stack();
    for (int p = q; p != -1; p = get_vertex(G, p)->parent)
    {
        int *temp = (int *)malloc(sizeof(int)); // 又是内存分配!!! 退出循环的作用域就出问题了
        *temp = p;
        push_stack(stack, temp);
    }
    free(pop_stack(stack));
    int f = s;
    int b = 0;
    int total = 0;
    while (!empty_stack(stack))
    {
        int *temp = (int *)pop_stack(stack);
        b = *temp; // 函数未曾退出，没有问题
        free(temp);
        printf(" %20s ", get_vertex(G, f)->data->name);
        Edge *edge = get_edge(G, f, b);
        total += edge->data->fee;
        printf("=== %d ===>", edge->data->fee);
        printf(" %-20s \n", get_vertex(G, b)->data->name);
        f = b;
    }
    printf("\nTotal fee:%d\n",total);
}

/**
 * @brief 修改uk的优先级
 * @param G 图
 * @param uk s的邻居
 * @param s s
 */
static void pfs_update(Graph *G, int uk, int s)
{
    if (UNDISCOVERED != get_vertex(G, uk)->state)
        return;
    int maybe = get_vertex(G, s)->priority + get_edge(G, s, uk)->data->fee;
    if (get_vertex(G, uk)->priority > maybe){
        get_vertex(G, uk)->priority = maybe;
        get_vertex(G, uk)->parent = s;
    }
}

int main()
{
    Graph *G = make_graph("services.txt");
    char* place1 = (char*)malloc(sizeof(char)*20);
    char* place2 = (char*)malloc(sizeof(char)*20);
    printf("Pls input first place:");
    scanf("%s",place1);
    printf("\n");
    printf("Pls input second place:");
    scanf("%s",place2);
    printf("\n");
    pfs(G, find_vector_index(G->V, &compare_vertex_by_name, place1), find_vector_index(G->V, &compare_vertex_by_name, place2), &pfs_update);
}

#undef MIN