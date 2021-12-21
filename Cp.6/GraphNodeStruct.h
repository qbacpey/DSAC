typedef enum
{                 // 在图算法中，结点的状态信息将大显身手
    UNDISCOVERED, // 结点尚未被发现
    DISCOVERED,   // 结点已被发现
    VISITED       // 结点已被访问完毕
} VState;

typedef struct
{
    char *name;
    int len;
} VData;

typedef struct
{
    VData *data;    // 数据
    int in_degree;  // 结点出度
    int out_degree; // 结点入度（若是无向图，上述两者相同）
    VState state;   // 结点状态，初始时所有的结点都是尚未被发现的状态
    int dTime;      // 结点被发现的时间
    int fTime;      // 结点被访问完毕的时间
    int priority;   // 结点的优先级（权重不一定储存在边中）
    int parent;     // 生成树中父结点的位置
} Vertex;

typedef enum
{              // 同样，在图算法中，这些东西才会大显身手
    UNDEFINED, // 初始状态，尚未定义
    TREE,      // 生成树中采纳了这条边
    CROSS,     // 跨边
    FORWARD,   // 前向边
    BACKWARD   // 后向边
} EType;

typedef struct
{
    int distance;
    int fee;
} EData;

typedef struct
{
    EData *data; // 是的，边本身也能承载数据
    int weight;  // 边的权重
    EType type;  // 在遍历树中，边所属的类型
} Edge;
