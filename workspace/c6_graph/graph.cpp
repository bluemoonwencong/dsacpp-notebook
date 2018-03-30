#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include "../chap4/stack.cpp"

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

int main() {
    cout << "hello";
}
