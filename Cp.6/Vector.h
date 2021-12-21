typedef struct
{
    int size;
    int capacity; // 与其说是容量，不如说是哨兵index
    void *data;   // data指向的对象是void*数组，一般来说是这样
} Vector;         // 句柄对象

typedef int (*VSV)(void *, void *);

Vector *init_vector(int init_capacity);
void push_vector(Vector *, void *elem);
void *at_vector(Vector *, int index);
void set_vector(Vector *, int index, void *elem);
void print_vector(Vector *, VSV);
void *find_vector_data(Vector *, VSV, void *data);
int find_vector_index(Vector *, VSV, void *data);
void enlarge_to(Vector *vector, int new_capacity);
int size_vector(Vector *);
int capacity_vector(Vector *);
void *clean_vector(Vector *);

static int enlarge(Vector *, int multiple);

Vector *init_vector(int init_capacity)
{
    Vector *vector = (Vector *)malloc(sizeof(Vector));
    vector->data = malloc(sizeof(void *) * init_capacity);
    vector->capacity = init_capacity;
    vector->size = 0;
    return vector;
}

void push_vector(Vector *vector, void *elem)
{
    if (vector->size == vector->capacity)
        enlarge(vector, 2);
    // 不是解引用指针之后再做加法，做完加法再解引用
    *((void **)vector->data + vector->size++) = (void *)elem;
}

/**
 * @brief 返回data数组中对应位置的地址
 * 需要在caller执行强制类型转换成对应对象指针之后解引用
 * @param vector 向量
 * @param index 下标
 * @return void* 指向指定元素的指针，没找到就返回
 */
void *at_vector(Vector *vector, int index)
{
    return index < vector->capacity ? *((void **)vector->data + index) : NULL;
}

void set_vector(Vector *vector, int index, void *elem)
{
    *((void **)vector->data + index) = elem;
    vector->size++;
}

/**
 * @brief 利用比较器根据key确定是否该节点
 * 没有的话就返回NULL
 * @param vector 向量
 * @param comparator 比较器。如果符合要求就会返回1
 * @param data 用于比较器使用的key
 * @return void* 指向对象的void*，如果没有符合对象的指针的话，就会返回NULL
 */
void *find_vector_data(Vector *vector, VSV comparator, void *data)
{
    int n = vector->size;
    void **beg = (void **)vector->data;
    void **tag = NULL;
    for (int i = 0; i < n; i++)
    {
        if ((tag = beg + i) && comparator(tag, data))
            return *tag;
    }
    return tag;
}

/**
 * @brief 根据节点姓名寻找节点在数组中的下标
 * @param vector 向量
 * @param comparator 比较器
 * @param data 节点姓名
 * @return int 节点下标，如果没有找到的话，就返回-1
 */
int find_vector_index(Vector *vector, VSV comparator, void *data)
{
    int n = vector->size;
    void **beg = (void **)vector->data;
    for (int i = 0; i < n; i++)
    {
        if (comparator(beg + i, data))
            return i;
    }
    return -1;
}

void print_vector(Vector *vector, VSV traverse)
{
    int n = vector->size;
    void **beg = vector->data;
    for (int i = 0; i < n; i++)
    {
        traverse(beg, NULL);
        beg += 1; // 注意，C语言中地址寻址的单位是Byte
    }
}
/**
 * @brief caller 应当确保new_capacity大于当前的capacity
 * 如果没有确保这一点，行为是未定义的
 * @param vector 向量
 * @param new_capacity 新容量
 */
void enlarge_to(Vector *vector, int new_capacity)
{
    vector->data = realloc(vector->data, new_capacity);
}

static int enlarge(Vector *vector, int multiple)
{
    vector->data = realloc(vector->data, (vector->capacity *= multiple) * sizeof(void *)); // 注意，单位是Byte所以要加上Sizeof
    return vector->capacity;
}