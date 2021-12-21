int print_vertex(void *tag, void *useless);
int print_edge(void *tag, void *vertexes);
int compare_vertex_by_name(void *tag, void *data);

int print_vertex(void *tag, void *useless)
{
    if (!tag)
        return -1;
    Vertex *v = *((Vertex **)tag);
    printf("\n<======Info of vertex======>\n");
    printf("VData name:%s\n", v->data->name);
    printf("dTime:%d\n", v->dTime);
    printf("fTime:%d\n", v->fTime);
    printf("inDegree:%d\n", v->in_degree);
    printf("outDegree:%d\n", v->out_degree);
    printf("parent:%d\n", v->parent);
    printf("priority:%d\n", v->priority);
    switch (v->state)
    {
    case 0:
        printf("state:UNDISCOVERED\n");
        break;
    case 1:
        printf("state:DISCOVERED\n");
        break;
    case 2:
        printf("state:VISITED\n");
        break;
    default:
        break;
    }
    printf("<======Info of vertex======>\n\n");
}

int print_edge(void *tag, void *vertexes)
{
    if (!tag)
        return -1;
    char **names = (char **)vertexes;
    printf("\n<======Info of edge======>\n");
    printf("Edge from \"%s\" to \"%s\":\n", *names, *(names + 1));
    Edge *e = (Edge *)tag;
    printf("Fee:%d\n", e->data->fee);
    printf("Distance:%d\n", e->data->distance);
    printf("State:");
    switch (e->type)
    {
    case 0:
        printf("UNDEFINED\n");
        break;
    case 1:
        printf("TREE\n");
        break;
    case 2:
        printf("CROSS\n");
        break;
    case 3:
        printf("FORWARD\n");
        break;
    case 4:
        printf("BACKWORD\n");
        break;
    default:
        printf("ERROR\n");
        abort;
        break;
    }
    printf("<======Info of edge======>\n\n");
}

/**
 * @brief 比较节点姓名是否相等
 * @param tag 等待比对的vertex
 * @param data key姓名
 * @return int 如果是这个节点就是1，不是这个节点就是0
 */
int compare_vertex_by_name(void *tag, void *data)
{
    return !strcmp((*((Vertex **)tag))->data->name, data);
}