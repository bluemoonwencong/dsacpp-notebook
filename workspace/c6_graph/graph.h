#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include "../chap4/stack.cpp"
#include "../chap4/queue.cpp"

using namespace std;

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType; //边在遍历树中所属的类型


template <typename Tv, typename Te> //顶点类型、边类型
class Graph { //图的模板类
    private:
        void reset() { //所有顶点、边的辅助信息复位
            for (int i=0; i<n; i++) {  //所有顶点的
                status(i) = UNDISCOVERED; //状态
                dTime(i) = fTime(i) = -1; //时间标签
                parent(i) = -1; //在遍历树中的父节点
                priority(i) = INT_MAX; //优先级数

                for (int j=0; j<n; j++) //所有边的
                    if (exists(i, j))
                        type(i, j) = UNDETERMINED; //类型
            }
        }

        void BFS(int, int&); //（连通域）广度优先搜索算法
        void DFS(int, int&); //（连通域）深度优先搜索算法
        void BCC(int, int&, Stack<int>&); //（连通域）基于 DFS 的双连通分量分解算法
        bool TSort_DFS(int, int&, Stack<Tv>*); //(连通域）基于 DFS 的拓扑排序算法
        template <typename PU> void PFS(int, PU); //（连通域）优先级搜索框架

    public:
        //顶点
        int n; //顶点总数
        virtual int insert( Tv const& ) = 0; //插入顶点，返回编号
        virtual Tv remove( int ) = 0; //删除顶点及其关联边，返回该顶点信息
        virtual Tv& vertex( int ) = 0; //顶点 v 的数据（该顶点的确存在）
        virtual int inDegree( int ) = 0; //顶点 v 的入度（该顶点的确存在）
        virtual int outDegree( int ) = 0; //顶点 v 的出度（该顶点的确存在）
        virtual int firstNbr( int ) = 0; //顶点 v 的首个邻接顶点
        virtual int nextNbr( int v, int u) = 0; //顶点 v 的（相对于顶点 u 的）下一邻接顶点
        virtual VStatus& status( int ) = 0; //顶点 v 的状态
        virtual int& dTime( int ) = 0; //顶点 v 的时间标签
        virtual int& fTime( int ) = 0; //顶点 v 的时间标签
        virtual int& parent( int ) = 0; //顶点 v 在遍历树中的父亲
        virtual int& priority( int ) = 0; //顶点 v 在遍历树中的优先级数


        //边：无向边统一转化为方向互逆的一对有向边，从而将无向图视作有向图的特例
        int e; //边总数
        virtual bool exists( int v, int u ) = 0; //边 (v,u) 是否存在
        virtual void insert( Te const& e, int v, int u, int w ) = 0; //在顶点 v 和 u 之间插入权重为 w 的边 e
        virtual Te remove( int v, int u ) = 0; //删除顶点 v 和 u 之间的边 e，返回该边信息
        virtual EType& type( int v, int u ) = 0; //边 (v, u) 的类型
        virtual Te& edge( int v, int u ) = 0; //边 (v, u) 的数据（该边的确存在）
        virtual int& weight( int v, int u ) = 0; //边 (v, u) 的权重

        //算法
        void bfs(int); //广度优先搜索算法
        void dfs(int); //深度优先搜索算法
        void bcc(int); //基于 DFS 的双连通分量分解算法
        Stack<Tv>* tSort_DFS(int); //基于 DFS 的拓扑排序算法
        void prim (int); //最小支撑树 prim 算法
        void dijkstra (int); //最短路径 Dijkstra 算法
        template <typename PU> void pdf(int, PU); //优先级搜索框架
};

template <typename Tv, typename Te> //广度优先搜索 BFS 算法（全图）
void Graph<Tv, Te>::bfs(int s) { //assert: 0 <= s < n
    //初始化
    reset();
    int clock = 0;
    int v = s; 

    do //逐一检查所有顶点
        if (UNDISCOVERED == status(v) ) //一旦遇到尚未发现的顶点
            BFS(v, clock); //即从该顶点出发启动一次 BFS
    while ( s != (v=(++v%n)) ); //按序号检查，故不漏不重
}

template <typename Tv, typename Te> //广度优先搜索 BFS 算法（单个连通域）
void Graph<Tv, Te>::BFS(int v, int& clock) { //assert: 0 <= v < n
    Queue<int> Q; //引入辅助队列

    status(v) = DISCOVERED; Q.enqueue(v); //初始化起点

    while(!Q.empty()) {
        int v = Q.dequeue();
        dTime(v) == ++clock; //取出队首顶点 v
        for (int u=firstNbr(v); -1<u; u=nextNbr(v, u)) //枚举 v 的所有邻居 u
            if (UNDISCOVERED == status(u)) { //若 u 尚未被发现，则
                status(u) == DISCOVERED; Q.enqueue(u); //发现该顶点
                type(v, u) = TREE; parent(u) = v; //引入树边拓展支撑树
            } else { //若 u 已被发现，若者甚至已访问完成，则
                type(v, u) = CROSS; //将 (v, u) 归类于跨边
            }

        status(v) = VISITED; //至此，当前顶点访问完毕
    }
}

template <typename Tv, typename Te> //深度优先搜索 DFS 算法（全图）
void Graph<Tv, Te>::dfs(int s) { //assert: 0 <= s < n
    //初始化
    reset();
    int clock = 0;
    int v = s; 

    do //逐一检查所有顶点
        if (UNDISCOVERED == status(v) ) //一旦遇到尚未发现的顶点
            DFS(v, clock); //即从该顶点出发启动一次 DFS
    while ( s != (v=(++v%n)) ); //按序号检查，故不漏不重
}

template <typename Tv, typename Te> //深度优先搜索 DFS 算法（单个连通域）
void Graph<Tv, Te>::DFS(int v, int& clock) { //assert: 0 <= v < n
    dTime(v) = ++clock; //发现时间
    status(v) = DISCOVERED; //发现当前顶点 v

    for (int u=firstNbr(v); -1<u; u=nextNbr(v, u)) //枚举 v 的所有邻居 u
        switch( status(u) ) { //并视其状态分别处理
            case UNDISCOVERED: //u 尚未发现，意味着支撑树可在此拓展
                type(v, u) = TREE;
                parent(u) = v;
                DFS(u, clock);
                break;
            case DISCOVERED: //u 已被发现但尚未访问完毕，此处有一个有向环路，应属被后代指向的祖先
                type(v, u) = BACKWARD;
                break;
            default: //u 已访问完毕(VISITED, 有向图)，则视承袭关系分为前向边或跨边
                // 比对活跃期，判定在 DFS 树中 v 是否为 u 祖先，若是，则边 (v,u) 是前向边，否则
                // 二者必然来自相互独立的两个分支，边 (v,u) 应归类为跨边 CROSS
                type(v, u) = ( dTime(v) < dTime(u) ) ? FORWARD : CROSS;
                break;
        }

    status(v) = VISITED; //至此，当前顶点 v 方告访问完毕
    fTime(v) = ++clock; // 完毕时间
}


/* 基于 DFS 的拓扑排序
在深度优先搜索过程中，首先因访问完成而转换到 VISITED 状态的顶点 m，必然是出度为 0 的顶点，该顶点在此后的搜索过程中也不再起任何作用，于是下一转换至 VISITED 状态的顶点也可等效地理解为是从图中剔除 m （及其边）之后的出度为 0 者，在拓扑排序中，该顶点应为顶点 m 的前驱。DFS 搜索过程中各顶点被标记为 VISITED 的次序，恰好（逆序）给出了原图的一个拓扑排序。此外，DFS 搜索中一旦发现有边为 BACKWARD，则表示有环路，从而可立即终止算法并报告 “因非 DAG 而无法拓扑排序。
*/
template <typename Tv, typename Te> //基于 DFS 的拓扑排序算法
Stack<Tv>* Graph<Tv, Te>::tSort_DFS(int s) { //assert: 0 <= s < n
    reset();
    int clock = 0;
    int v = s;
    Stack<Tv>* S = new Stack<Tv>; //用栈记录排序顶点

    do {
        if (UNDISCOVERED == status(v))
            if (!TSort_DFS(v, clock, S)) { // clock 并非必需
                while (!S->empty()) //任一连通域（亦即整图）非 DAG，则不必继续计算，直接返回
                    S->pop();
                break;
            }
    } while (s != (v=(++v % n)));

    return S; //若输入为 DAG，则 S 内各顶点自顶向底排序; 否则（不存在拓扑排序），为空
}

template <typename Tv, typename Te> //基于 DFS 的拓扑排序算法（单趟）
bool Graph<Tv, Te>::TSort_DFS(int v, int& clock, Stack<Tv>* S) { //assert: 0 <= v < n
    dTime = ++clock;
    status(v) = DISCOVERED; //发现顶点 v
    for (int u=firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举 v 所有邻居 u
        switch(status(u)) { //并视 u 的状态分别处理
            case UNDISCOVERED:
                parent(u) = v;
                status(v, u) = TREE;
                if (!TSort_DFS(u, clock, S)) //从顶点 u 处出发深入搜索
                    return false; //若 u 及其后代不能拓扑排序（则全图也如此）
                break;
            case DISCOVERED:
                status(v, u) = BACKWARD; //一旦发现后向边（非 DAG），则
                return false; //不必深入
            default: //VISITED (digraphs only)
                status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
                break;
        }

    status(v) = VISITED;
    S->push(vertex(v)); //顶点被标记为 VISITED; 并入栈
    return true; //v 及后代可拓扑排序
}


/*
双连通域分解算法
关节点删除后，图中的双连通域会增加。

+ 叶节点： 在 DFS 树中，叶节点绝不可能是原图中的关节点，此类顶点的删除即不影响 DFS 树的连通性，也不影响原图的连通性。
+ 根节点： 若 DFS 树的根节点有至少两个分支，则必为一个关节点。
+ 内部节点： 如下图中，若节点 C 的移除导致其某一真子树（如 D 为根的树）与其真祖先之间无法连通，则 C 必为关节点。反之，若 C 的所有真子树都能与 C 的某一真祖先连通，则 C 就不可能为关节点。在无向图的 DFS 树中， C 的真子树只能通过 **backward** 边与 C 的真祖先连通。因此，只要在 DFS 搜索中记录并更新各顶点 v 所能（经由 backward 边）连通的最高祖先(highest connected ancestor, HAC)，即可及时认定关节点。
*/
template <typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s) { //基于 DFS 的 BCC 分解算法
    reset();
    int clock = 0;
    int v = s;
    Stack<int> S; //栈 S 用于记录已访问的顶点

    do 
        if (UNDISCOVERED == status(v)) { //一旦找到有未发现的顶点（新连通分量）
            BCC( v, clock, S); //即从该顶点出发启动一次 BCC
            S.pop(); //遍历返回后，弹出栈中最后一个顶点---当前连通域的起点
        }
    while ( s != (v=(++v % n)) );
}

#define hca(x) (fTime(x)) //利用此处闲置的 fTime[] 充当 hca[]

template <typename Tv, typename Te>
void Graph<Tv, Te>::BCC(int v, int& clock, Stack<int>& S) {
    hca(v) = dTime(v) = ++clock;
    status(v) = DISCOVERED;
    S.push(v); // v 被发现并入栈
    for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举 v  的所有邻居 u
        switch( status(u) ) { //并视 u 的状态分别处理
            case UNDISCOVERED:
                parent(u) = v;
                type(v, u) = TREE;
                BCC(u, clock, S); //从顶点 u 处深入

                if (hca(u) < dTime(v)) //遍历返回后，若发现 u（通过后向边）可指向 v 的真祖先
                    hca(v) = min(hca(v), hca(u)); //则 v 亦必如此
                else { //否则，以 v 为关节点（u 以下即是一个 BCC，且其中顶点此时正集中于栈 S 的顶部）
                    while ( v != S.pop() ) //依次弹出当前 BCC 中的节点，亦可根据实际需求转存至其它结构
                        /*pass*/;
                    S.push(v); //最后一个顶点（关节点）重新入栈---总计至多两次
                }
                break;
            case DISCOVERED:
                type(v, u) = BACKWARD; //标记(v, u), 并按照 “越小越高” 的准则
                if (u != parent(v))
                    hca(v) = min(hca(v), dTime(u)); //更新 hca[v]
                break;
            default: //VISITED（有向图）
                type(v, u) = (dTime(v) < dTime(u)) ? FORWARD:CROSS;
                break;
        }
    status(v) = VISITED; //对 v 的访问结束
}

#undef hca
