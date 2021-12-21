#include "Vector.h"
#include "GraphNodeStruct.h"
#include "Tool.h"
#define Vector_Vertex Vector *
#define Vector_Vector_Edge Vector *
#define STRING_SIZE 20
typedef struct
{
    Vector_Vertex V;      // 这里为了能用一种更加简明的方式表示向量的元素
    Vector_Vector_Edge E; // 实际上表示的就是一个Edge的二维向量
    int n;                // 结点个数
    int e;                // 边的个数
} Graph;

Graph *make_graph(const char *name);
Vertex *get_vertex(Graph *, int);
Edge *get_edge(Graph *, int, int);
int next_nbr(Graph *g, int i, int k);
int first_nbr(Graph *g, int i);
void print_all_edge(Graph *, int );

static VData *new_v_data(char *, int);
static Vertex *new_vertex(VData *);
static EData *new_e_data(int, int);
static Edge *new_edge(EData *);

static void push_vertex(Graph *, Vertex *);
static void add_edge(Graph *, Edge *, int, int);

static void make_V(Graph *G, FILE *services);
static void make_E(Graph *G, FILE *services);
static void init_E(Graph *);

Vector *get_incidences(Graph *G, int i);
void set_incidences(Graph *G, int i, void *incidences);
int insert_vertex(Graph *G, void *vertex_data);
/**
 * @brief 取邻接矩阵中，与指定节点关联的边向量
 * 确切地讲，是对应节点的所有出边
 * 可能会返回NULL
 * @param G 图
 * @param i 节点index
 * @return Vector* 和对应节点相关的所有边组成的向量
 */
Vector *get_incidences(Graph *G, int i)
{
    // Vertex<Vertex<Edge>> E = G->E // Vertex*{Vertex*}
    return (Vector *)at_vector(G->E, i); //Vertex*{void*}
}

void set_incidences(Graph *G, int i, void *incidences)
{ // 更新结点边数组
    set_vector(G->E, i, incidences);
}

/**
 * @brief 初始化节点数据
 * @param name 节点姓名
 * @param len 姓名长度
 * @return VData* 节点数据结构体
 */
static VData *new_v_data(char *name, int len)
{
    VData *data = (VData *)malloc(sizeof(VData));
    data->len = len;
    data->name = name;
    printf("\b"); // 为啥打了一个不可见字符之后就能正常输出，是因为清空了缓存吗？
    return data;
}

/**
 * @brief 初始化节点
 * 初始化情况为：
 * typedef struct {
    VData *data = data;
    int in_degree = out_degree = 0; 节点初始情况下的入度出度都是0
    VState state = UNDISCOVERED; 节点初始的情况下是未发现状态
    int dTime = fTime = priority = __INT_MAX__; 发现时间，访问完成时间，优先级都是正无穷
    int parent = -1; 节点初始情况下父母为-1
} Vertex;
 * @param data VData直接作为成员使用
 * @return Vertex* 节点结构体
 */
static Vertex *new_vertex(VData *data)
{
    Vertex *vertex = (Vertex *)malloc(sizeof(Vertex));
    vertex->data = data;
    vertex->priority = vertex->dTime = vertex->fTime = __INT_MAX__;
    vertex->in_degree = 0;
    vertex->out_degree = 0;
    vertex->parent = -1;
    vertex->state = UNDISCOVERED;
    return vertex;
}

/**
 * @brief 新建边
 * @param distance 边的长度
 * @param fee 边的费用
 * @return EData* 边数据对象
 */
static EData *new_e_data(int distance, int fee)
{
    EData *data = (EData *)malloc(sizeof(EData));
    data->distance = distance;
    data->fee = fee;
    return data;
}

/**
 * @brief 新建边
 * 初始情况下边尚未被发现
 * 权重为0
 * @param data 边数据
 * @return Edge* 边
 */
static Edge *new_edge(EData *data)
{
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    edge->data = data;
    edge->type = UNDEFINED;
    edge->weight = 0;
    return edge;
}

Vertex *get_vertex(Graph *G, int i)
{
    return (Vertex *)at_vector(G->V, i);
}

Edge *get_edge(Graph *G, int i, int j)
{
    return (Edge *)at_vector((Vector *)at_vector(G->E, i), j);
}

int next_nbr(Graph *G, int i, int k)
{
    while (k > -1 && get_edge(G, i, --k) == NULL)
        ;
    return k;
}

int first_nbr(Graph *G, int i)
{
    return next_nbr(G, i, G->n);
}

static void push_vertex(Graph *G, Vertex *vertex)
{
    push_vector(G->E, vertex);
    G->n++;
}

/**
 * @brief 有向图插入边，from i to j
 * @param G 图
 * @param e 边对象
 * @param i 横坐标
 * @param j 纵坐标
 */
static void add_edge(Graph *G, Edge *e, int i, int j)
{
    if (get_edge(G, i, j))
        return;
    set_vector((Vector *)at_vector(G->E, i), j, e);
    Vertex *v_i = get_vertex(G, i);
    Vertex *v_j = get_vertex(G, j);
    v_j->in_degree++;
    v_i->out_degree++;
    G->e++;
}

static void init_E(Graph *G)
{
    int n = G->n;
    Vector *edge_out = init_vector(n);
    for (int i = 0; i < n; i++)
        push_vector(edge_out, init_vector(n));
    G->e = 0;
    G->E = edge_out;
}

static void make_V(Graph *G, FILE *services)
{
    Vector *vertexes = init_vector(6);
    G->V = vertexes;
    char *name1 = (char *)malloc(STRING_SIZE * sizeof(char));
    char *name2 = (char *)malloc(STRING_SIZE * sizeof(char));
    while (fscanf(services, "%s %s %*d %*d", name1, name2) != EOF)
    {
        if (find_vector_index(vertexes, &compare_vertex_by_name, name1) == -1)
        {
            char *temp = (char *)malloc(STRING_SIZE * sizeof(char));
            strcpy(temp, name1);
            push_vector(vertexes, new_vertex(new_v_data(temp, (int)strlen(temp))));
        }
        if (find_vector_index(vertexes, &compare_vertex_by_name, name2) == -1)
        {
            char *temp = (char *)malloc(STRING_SIZE * sizeof(char));
            strcpy(temp, name2);
            push_vector(vertexes, new_vertex(new_v_data(temp, (int)strlen(temp))));
        }
    }
    G->n = G->V->size;
    free(name1);
    free(name2);
    fseek(services, 0, SEEK_SET);
}
static void make_E(Graph *G, FILE *services)
{
    init_E(G);
    int fee;
    int distance;
    char *name1 = (char *)malloc(STRING_SIZE * sizeof(char));
    char *name2 = (char *)malloc(STRING_SIZE * sizeof(char));
    while (fscanf(services, "%s %s %d %d", name1, name2, &fee, &distance) != EOF)
    {
        add_edge(G, new_edge(new_e_data(distance, fee)), find_vector_index(G->V, &compare_vertex_by_name, name1), find_vector_index(G->V, &compare_vertex_by_name, name2));
    }
    free(name1);
    free(name2);
    fseek(services, 0, SEEK_SET);
}
Graph *make_graph(const char *name)
{
    FILE *services = fopen(name, "r");
    // FILE *services = fopen("services.txt", "r");
    if (services == NULL)
        printf("Empty File!\n");
    Graph *G = (Graph *)malloc(sizeof(Graph));
    make_V(G, services);
    make_E(G, services);
    return G;
}

/**
 * @brief 打印指定节点的所有出边
 * @param G 图
 * @param j 节点下标
 */
void print_all_edge(Graph *G, int j)
{
    char *name[2];
    for (int i = first_nbr(G, j); i >= 0; i = next_nbr(G, j, i))
    {
        name[0] = get_vertex(G, j)->data->name;
        name[1] = get_vertex(G, i)->data->name;
        print_edge(get_edge(G, j, i), name);
    }
}

static void test()
{
    //1.初始化向量
    Vector *vertexes = init_vector(6);
    //2.向其中放入节点
    Graph *G = (Graph *)malloc(sizeof(Graph));
    G->V = vertexes;
    push_vector(vertexes, new_vertex(new_v_data("Name1", 9)));
    push_vector(vertexes, new_vertex(new_v_data("Name2", 6)));
    push_vector(vertexes, new_vertex(new_v_data("Name3", 6)));
    push_vector(vertexes, new_vertex(new_v_data("Name4", 6)));
    push_vector(vertexes, new_vertex(new_v_data("Name5", 6)));
    push_vector(vertexes, new_vertex(new_v_data("Name6", 6)));
    G->n = 6;
    //3.测试能否正确寻回
    // print_vector(vertexes,&print_vertex);
    //4.向其中放入边
    init_E(G);
    add_edge(G, new_edge(new_e_data(12, 14)), 1, 2); // 横坐标一样的话直接爆炸，不会在添加之后就给人家清除了吧
    add_edge(G, new_edge(new_e_data(72, 64)), 1, 4); // 原因好像是重复访问，但是这怎么也不应该吧
    add_edge(G, new_edge(new_e_data(12, 64)), 4, 2);
    add_edge(G, new_edge(new_e_data(19, 34)), 2, 3);
    add_edge(G, new_edge(new_e_data(20, 64)), 4, 2);
    add_edge(G, new_edge(new_e_data(974, 24)), 5, 0);
    add_edge(G, new_edge(new_e_data(2341, 324)), 3, 3);
    //5.寻回边信息
    // print_edge(get_edge(G, 1, 2), NULL);
    // print_edge(get_edge(G, 1, 4), NULL);
    // print_edge(get_edge(G, 4, 2), NULL);
    // print_edge(get_edge(G, 5, 0), NULL);
    //6.测试find函数，寻找节点
    printf("Find by name:%s\n", ((Vertex *)find_vector_data(G->V, &compare_vertex_by_name, "Name1"))->data->name);
    printf("Find index by name:%d\n", find_vector_index(G->V, &compare_vertex_by_name, "Name6"));
    //6.1.寻找节点
}

#undef Vector_Vertex
#undef Vector_Vector_Edge
#undef STRING_SIZE