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
        bool TSort(int, int&, Stack<Tv>*); //(连通域）基于 DFS 的拓扑排序算法
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
        Stack<Tv>* tSort(int); //基于 DFS 的拓扑排序算法
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
