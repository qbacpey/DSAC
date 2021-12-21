#include "BinNode.h"
#define FAKE_NODE(P) ((P) ? (P->height) : (-1))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define STRING_SIZE 20

typedef BinNode *(*VST)(BinNode *, char **); // 副作用函数指针

typedef struct __BinTree__
{
    int _size;
    BinNode *root;
} BinTree;

BinTree *init_BinTree(NodeData *root_data);
int remove_BinTree(BinTree *, BinNode *);
int removeAt_BinTree(BinTree *, BinNode *);

int updateHeight_BinTree(BinTree *, BinNode *);
void updateHeightAbove_BinTree(BinTree *, BinNode *);
int size_BinTree(BinTree *);
int empty_BinTree(BinTree *);
BinNode *root_BinTree(BinTree *);
BinNode **deductOrder_BinTree(BinTree *, BinNode *);

BinNode *insertAsLC_BinTree(BinTree *, BinNode *, NodeData *);
BinNode *insertAsRC_BinTree(BinTree *, BinNode *, NodeData *);

BinNode *attachAsLT_BinTree(BinTree *main_tree, BinNode *node, BinTree *sub_tree);
BinNode *attachAsRT_BinTree(BinTree *main_tree, BinNode *node, BinTree *sub_tree);

BinTree *secede(BinTree *, BinNode *);

BinNode *find(BinTree *, Stack *, VST, char **);
void get_left_most_leaf(Stack *);
BinTree *load_tree(const char *filename);
void print_tree(BinTree *);

void print_children(BinTree *, char *);
BinNode *find_child(BinTree *, char *, char *);


static BinNode *verify_node_name(BinNode *node, char **argv)
{
    if (!strcmp(node->data->name, *(argv)))
    {
        return node;
    }
    return NULL;
}

static BinNode *print_node(BinNode *node, char **argv)
{
    if (node)
        print_data(node->data);
    return NULL;
}

void print_children(BinTree *tree, char *name)
{
    Stack *stack = init_stack();
    BinNode *child = find(tree, stack, &verify_node_name, &name);
    clean_stack(stack);               // 刚刚的bug是因为将内存中的data给free掉了。所以最好分deep clean和简单clean
    if (child) // 在If里边能赋值但是不能声明变量？
    {
        if(!(child = child->lc)){
            printf("This node don't have any child!\n");
        }
        while (child)
        {
            print_data(child->data);
            child = child->rc;
        }
        return;
    }
}
BinNode *find_child(BinTree *tree, char *parent_name, char *child_name)
{
    Stack *stack = init_stack();
    BinTree *child_tree = (BinTree *)malloc(sizeof(BinTree));
    child_tree->root = find(tree, stack, &verify_node_name, &parent_name);
    if (!child_tree->root){
        free(child_tree);
        return NULL;
    }
    BinNode *child = find(child_tree, stack, &verify_node_name, &child_name);
    if (child && strcmp(child->data->name,child_tree->root->data->name))
    {
        free(child_tree);
        return child;
    }
    free(child_tree);
    return NULL;
}

BinTree *load_tree(const char *filename)
{
    char *parent_name = (char *)malloc(sizeof(char) * STRING_SIZE);
    int count;
    char *child_name = (char *)malloc(STRING_SIZE * sizeof(char));
    FILE *definition = fopen(filename, "r");
    BinTree *tree = init_BinTree(new_nodedata("hospital", 1));
    Stack *stack = init_stack();
    // bug出现的原因就是共享了内存地址
    while (fscanf(definition, "%s %d %s", parent_name, &count, child_name) != EOF)
    {
        reset_stack(stack);
        // 冲突来自于父节点设置错误，之前是将parent设置为真父亲，现在是将parent设置为左兄弟
        insertAsChlid(find(tree, stack, &verify_node_name, &parent_name),new_nodedata(child_name, count));
        child_name = (char *)malloc(STRING_SIZE * sizeof(char));
    }
    free(stack);
    fclose(definition);
    return tree;
}

void print_tree(BinTree *tree)
{
    Stack *stack = init_stack();
    find(tree, stack, &print_node, NULL);
}

void print_ListNode(ListNode *node)
{
    print_data(((BinNode *)(node->data))->data);
}

BinNode *find(BinTree *tree, Stack *stack, VST visit, char **args)
{
    push_stack(stack, tree->root);
    BinNode *node = (BinNode *)top_stack(stack);
    while (!empty_stack(stack))
    {
        if (node->parent != (BinNode *)top_stack(stack))
        {
            get_left_most_leaf(stack);
        }
        node = (BinNode *)pop_stack(stack);
        BinNode *temp = (*visit)(node, args);
        if (temp)
        {
            return temp;
        }
    }
    if (args)
    {
        printf("\nError,don't find any nodes called:\"%s\"\n", *args);
    }
    return NULL;
}
void get_left_most_leaf(Stack *stack)
{
    BinNode *node = (BinNode *)top_stack(stack);
    while (node->data->count != -1)
    {
        if (node->lc)
        {
            if (node->rc)
                push_stack(stack, node->rc);
            push_stack(stack, node->lc);
        }
        else if (node->rc)
        {
            push_stack(stack, node->rc);
        }
        else // void *对齐问题
        {
            push_stack(stack, new_BinNode(new_nodedata("NULL", -1), node));
        }
        node = (BinNode *)top_stack(stack);
    }
    pop_stack(stack);
}

BinTree *init_BinTree(NodeData *root_data)
{
    BinNode *root = new_BinNode(root_data, NULL);
    BinTree *tree = (BinTree *)malloc(sizeof(BinTree));
    tree->root = root;
    tree->_size = 1;
    return tree;
}

int size_BinTree(BinTree *tree)
{
    return tree->_size;
}
int empty_BinTree(BinTree *tree)
{
    return tree->root ? 0 : 1;
}
BinNode *root_BinTree(BinTree *tree)
{
    return tree->root;
}
/**
 * @brief 由于C没有引用的原因，如果想要修改指定节点的数据，就需要返回**（好像由于返回值复制的原因，会将对应指针复制过去）
 * 换句话来说，之所以这么混乱的原因是因为该类型本身就是一个指针
 * 需要通过指针的指针才能修改对应数据
 * 虽然直接让Callee判断也能解决问题
 * 总之，这个函数的作用是作为修改父节点对应引用
 * @param tree 树
 * @param node 需要判断是父节点的左还是右孩子的节点
 * @return BinNode** 指向父节点对应对应指针的指针
 */
BinNode **deductOrder_BinTree(BinTree *tree, BinNode *node)
{
    BinNode *parent = node->parent;
    return parent->lc == node ? &(parent->lc) : &(parent->rc);
}

/**
 * @brief 更新对应节点的高度，实际上，这个节点应当是从下往上调用的
 * 准确来说，这是一个应当被updateHeightAbove调用的接口，这样方能自下而上的确保发生变动的节点
 * 的所有祖先节点都同样的发生了变动，此外，这个方法得以成立的**默认前提**是这个点所有的子节点的高度都是正确的
 * @param tree 树
 * @param node 点
 * @return int 该节点的新高度
 */
int updateHeight_BinTree(BinTree *tree, BinNode *node)
{
    return node->height = 1 + MAX(FAKE_NODE(node->lc), FAKE_NODE(node->rc));
}

/**
 * @brief 鉴于所有节点的通向根节点的路径仅有一条，迭代的次数实际上等于这个节点的深度
 * @param tree 树
 * @param node 点
 */
void updateHeightAbove_BinTree(BinTree *tree, BinNode *node)
{
    while (node)
    {
        updateHeight_BinTree(tree, node);
        node = node->parent;
    }
}

/**
 * @brief 在给定节点左方插入节点之后更新高度信息
 * @param tree 
 * @param node 
 * @param data 
 * @return BinNode* 返回右子节点
 */
BinNode *insertAsLC_BinTree(BinTree *tree, BinNode *node, NodeData *data)
{
    insertAsLC(node, data);
    updateHeightAbove_BinTree(tree, node);
    tree->_size++;
    return node->rc;
}
/**
 * @brief 在给定节点右方插入节点之后更新高度信息
 * @param tree 
 * @param node 
 * @param data 
 * @return BinNode* 返回左子节点
 */
BinNode *insertAsRC_BinTree(BinTree *tree, BinNode *node, NodeData *data)
{
    insertAsRC(node, data);
    updateHeightAbove_BinTree(tree, node);
    tree->_size++;
    return node->lc;
}

/**
 * @brief 这里的实现实际上是使用node的左子节点代替了sun_tree的根节点
 * 因此正确的操作应当是将左子节点更新为根节点之后将sub_tree销毁
 * 但是千万不要free它的_root指针
 * @param main_tree 主树
 * @param node 需要将子树接入的节点
 * @param sub_tree 子树
 * @return BinNode* 返回被更新的节点本身
 */
BinNode *attachAsLT_BinTree(BinTree *main_tree, BinNode *node, BinTree *sub_tree)
{
    if (node->lc = sub_tree->root) // 考虑到传入的子树可能是空的，使用的是赋值语句
    {
        node->lc->parent = node;
    }
    updateHeightAbove_BinTree(main_tree, node);
    main_tree->_size += sub_tree->_size;
    free(sub_tree);
    return node;
}

BinNode *attachAsRT_BinTree(BinTree *main_tree, BinNode *node, BinTree *sub_tree)
{
    if (node->rc = sub_tree->root) // 这里这么写的原因是因为考虑到传入的子树可能是空的，于是实际上使用的是赋值语句
    {
        node->rc->parent = node;
    }
    updateHeightAbove_BinTree(main_tree, node);
    main_tree->_size += sub_tree->_size;
    free(sub_tree);
    return node;
}

int remove_BinTree(BinTree *tree, BinNode *node)
{
    *(deductOrder_BinTree(tree, node)) = NULL;
    updateHeightAbove_BinTree(tree, node->parent);
    int n = removeAt_BinTree(tree, node);
    tree->_size -= n;
    return n;
}

int removeAt_BinTree(BinTree *tree, BinNode *node)
{
    if (!node)
    {
        return 0;
    }
    int node_number = 1 + removeAt_BinTree(tree, node->lc) + removeAt_BinTree(tree, node->rc); // 简单的递归实现
    free(node);
    return node_number;
}

BinTree *secede(BinTree *tree, BinNode *node)
{
    *(deductOrder_BinTree(tree, node)) = NULL;
    updateHeightAbove_BinTree(tree, node->parent);
    node->parent = NULL;

    BinTree *new_tree = (BinTree *)malloc(sizeof(BinTree));
    new_tree->_size = size_BinNode(node);
    tree->_size -= new_tree->_size;
    new_tree->root = node;
    return new_tree;
}