typedef struct __nodedata__
{
    char *name; // 约定，在生成NodeData的时候，name的结尾必须'\0'也就是遵循C语言的字符串约定
    int count;
} NodeData;

void print_data(NodeData *);
char *get_name(NodeData *);
int get_count(NodeData *);
NodeData *new_nodedata(char *, int count);

void print_data(NodeData *data)
{
    printf("%d %s\n", data->count, data->name);
}
char *get_name(NodeData *data)
{
    return data->name;
}
int get_count(NodeData *data)
{
    return data->count;
}
NodeData *new_nodedata(char *name, int count)
{
    NodeData* temp = (NodeData*)malloc(sizeof(NodeData));
    temp->count=count;
    temp->name=name;
    return temp;
}