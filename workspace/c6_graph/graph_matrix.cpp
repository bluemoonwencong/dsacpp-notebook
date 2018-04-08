#include "./graph.h" //引入图 ADT
//#include "../chap2/vector.cpp" //引入向量，在 ./graph.h 中的 #include stack.h 中已引入

template <typename Tv> struct Vertex { //顶点对象
    Tv data; //数据
    int inDegree, outDegree; //出入度
    VStatus status; //顶点状态
    int dTime, fTime; //时间标签
    int parent; int priority; //在遍历树中的父节点、优先级数
    
    Vertex( Tv const& d = (Tv) 0): //构造新顶点
        data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
        dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {} //暂不考虑权重溢出
};

template <typename Te> struct Edge { //边对象
    Te data; //数据
    int weight; //权重
    EType type; //边类型

    Edge(Te const& d, int w):
        data(d), weight(w), type(UNDETERMINED) {}
};

template <typename Tv, typename Te> //顶点类型、边类型
class GraphMatrix: public Graph<Tv, Te> { //基于向量，以邻接矩阵形式实现的图
    private:
        Vector< Vertex<Tv> > V; //顶点集（向量）
        Vector< Vector< Edge<Te>* > > E; //边集（邻接矩阵）

    public:
        GraphMatrix() { this.n = this.e = 0; } //构造
        ~GraphMatrix() { //析构
            int n=this.n;
            for( int j=0; j<n; j++) //所有动态创建的
                for (int k=0; k<n; k++) //边记录
                    delete E[j][k]; //逐条清除
        }

        //顶点的基本操作：查找第 i 个顶点(0 <= i < n)
        virtual Tv& vertex(int i) {
            return V[i].data;
        }
        
        virtual int inDegree(int i) {
            return V[i].inDegree;
        }
        
        virtual int outDegree(int i) {
            return V[i].outDegree;
        }

        virtual int firstNbr(int i) { //首个邻接顶点
            return nextNbr(i, this.n);
        }

        virtual int nextNbr(int i, int j) { //相对于顶点 j 的下一个邻接顶点（改用邻接表可提高效率）
            while ( (-1<j) && (!exists(i, --j)) ) //逆向线性试探
                    ;
            return j;
        }

        virtual VStatus& status(int i) { //状态
            return V[i].status;
        }

        virtual int& dTime (int i) {
            return V[i].dTime;
        }

        virtual int& fTime (int i) {
            return V[i].fTime;
        }

        virtual int& parent (int i) {
            return V[i].parent;
        }

        virtual int& priority (int i) {
            return V[i].priority;
        }

        //顶点的动态操作
        virtual int insert(Tv const& vertex) {//插入顶点，返回编号
            for (int j=0; j<this.n; j++) //各顶点预留一条潜在的关联边
                E[j].insert(NULL);
            this.n++;

            //创建新顶点对应的边向量（容量和规模都为 n）
            E.insert( Vector<Edge<Te>*> (this.n, this.n, (Edge<Te>*)NULL ) );

            return V.insert( Vertex<Tv> (vertex) ); //顶点向量增加一个顶点
        }

        virtual Tv remove(int i) { //删除第 i 个顶点及其关联边(0 <= i < n)
            
            //删除第 i 行数据
            for (int j=0; j<this.n; j++) //所有出边
                if (exists(i, j)) {
                    delete E[i][j];
                    V[j].inDegree --;
                }
            E.remove(i); this.n--; 

            //删除顶点 i
            Tv vBak = vertex(i);
            V.remove(i);

            //删除第 i 列数据
            for(int j=0; j<this.n; j++) //所有入边
                if (Edge<Te>* e = E[j].remove(i)) {
                    delete e;
                    V[j].outDegree--;
                }

            return vBak;
        }


        //边的确认操作
        virtual bool exists(int i, int j) { //边 (i,j) 是否存在
            int n = this.n;
            return (0<=i) && (i<n) && (0<=j) && (j<n) && E[i][j] != NULL;
        }

        //边的基本操作：查找第 i 与 j 之间的联边(0<=i, j<n 且 exists(i,j))
        virtual EType& type(int i, int j){ //边类型
            return E[i][j]->type;
        }

        virtual Te& edge(int i, int j){  //边数据
            return E[i][j]->data;
        }

        virtual int& weight(int i, int j){ //边权重
            return E[i][j]->weight;
        }


        // 边的动态操作
        virtual void insert( Te const& edge, int w, int i, int j) { //插入权重为 w 的边 e=(i,j)
            if (exists(i, j)) //确保该边尚不存在
                return;

            E[i][j] = new Edge<Te>(edge, w);
            this.e++;
            V[i].outDegree++; //更新边计数与关联顶点的度数
            V[j].inDegree++;
        }

        virtual Te remove(int i, int j) { //删除顶点 i, j 之间的联边 (exists(i,j))
            Te eBak = edge(i, j);
            delete E[i][j];
            E[i][j] = NULL;
            this.e--;
            V[i].outDegree--;
            V[j].inDegree--;
            return eBak;
        }
};

int main(){
    cout << "hello" << endl;
}
