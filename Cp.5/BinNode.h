#include "Stack.h"

typedef struct __BN__
{
    NodeData *data;
    int height; // 节点高度指的是其对应子树的高度(height(v)=height(subtree(v)));这和节点深度是不一样的
    struct __BN__ *parent;
    struct __BN__ *lc;
    struct __BN__ *rc;
} BinNode;

int size_BinNode(BinNode *);
BinNode *new_BinNode(NodeData *data, BinNode *parent);
BinNode *insertAsLC(BinNode *parent, NodeData *data);
BinNode *insertAsRC(BinNode *parent, NodeData *data);
BinNode *succ_BinNode(BinNode *);

BinNode *insertAsChlid(BinNode *parent, NodeData *data);

BinNode *new_BinNode(NodeData *data, BinNode *parent)
{
    BinNode *temp = (BinNode *)malloc(sizeof(BinNode));
    temp->data = data;
    temp->parent = parent;
    return temp;
}
// 前身是作为左孩子插入
BinNode *insertAsLC(BinNode *parent, NodeData *data)
{
    return parent->lc = new_BinNode(data, parent);
}

// 前身是作为右孩子插入
BinNode *insertAsRC(BinNode *parent, NodeData *data)
{
    return parent->rc = new_BinNode(data, parent);
}

/**
 * @brief 递归的检索该节点对应的所有子节点，直到触底返回
 * 递归基是这个节点是叶子节点，此时仅计入这个节点本身就行了
 * @param node this节点
 * @return int 以node节点为根节点的子树的规模
 */
int size_BinNode(BinNode *node)
{
    int size = 1;
    if (node->lc)
    {
        size += size_BinNode(node->lc);
    }
    if (node->rc)
    {
        size += size_BinNode(node->rc);
    }
    return size;
}

// 长子，兄弟表示意义下的插入节点方法，尽管还是使用的BinNode，但是意义不相同
BinNode *insertAsChlid(BinNode *parent, NodeData *data)
{
    if (!parent->lc)
    {
        BinNode *node = new_BinNode(data, parent);
        return parent->lc = node;
    }
    BinNode *first_born = parent->lc;
    while (first_born->rc)
        first_born = first_born->rc;
    BinNode *node = new_BinNode(data, first_born);
    return first_born->rc = node;
}
