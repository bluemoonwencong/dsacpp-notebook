#include <stdlib.h>
#include <iostream>
using namespace std;

typedef int Rank; //秩

#define ListNodePosi(T) ListNode<T>* //列表节点位置

// 列表节点
template <typename T> struct ListNode { //列表节点模板类，以双向链表形式实现
    //成员
    T data; //数值
    ListNodePosi(T) pred; //前驱
    ListNodePosi(T) succ; //后继

    //构造函数
    ListNode() {} //针对 header 和 trailer 的构造
    ListNode( T e, ListNodePosi(T) p=NULL, ListNodePosi(T) s = NULL)
        : data(e), pred(p), succ(s) {} //默认构造器

    //操作接口
    //紧靠当前节点之前插入新节点
    ListNodePosi(T) insertAsPred(T const& e);

    //紧靠当前节点之后插入新节点
    ListNodePosi(T) insertAsSucc(T const& e);
};


//List 模板类
template <typename T> class List{
    private:
        int _size; //规模
        ListNodePosi(T) header; //头哨兵
        ListNodePosi(T) trailer; //尾哨兵

    protected:
        void init(); //列表创建时的初始化
        int clear(); //清除所有节点
        void copyNodes(ListNodePosi(T) p, int n); //复制列表中自位置 p 起的 n 项
        void mergeSort(ListNodePosi(T) &p, int); //对从 p 开始的 n 个节点归并排序
        void merge(ListNodePosi(T) &, int, List<T>&, ListNodePosi(T), int); //有序列表区间归并
        void selectionSort(ListNodePosi(T) p, int); //对从 p 开始的 n 个节点选择排序
        void insertionSort(ListNodePosi(T) p, int); //对从 p 开始的 n 个节点插入排序

    public:
        //构造函数
        List(){ init(); } //默认
        List(List<T> const& L); //整体复制列表 L
        List(List<T> const& L, Rank r, int n); //复制列表 L 中自第 r 项起 n 项
        List(ListNodePosi(T) p, int n); //复制列表中自位置 p 起的 n 项

        //析构函数
        ~List(); //释放（包含头，尾哨兵在内的）所有节点

        //只读访问接口
        Rank size() const {
            return _size;
        }

        Rank rank( ListNodePosi(T) p) const {
            ListNodePosi(T) q = header;
            for( Rank r=0; q->succ != trailer; r++, q = q->succ ) {
                if (q->succ == p)
                    return r;
            }
            return -1;
        }

        bool empty() const {
            return _size <= 0;
        }

        T& operator[](Rank r) const; //重载，支持循秩访问（效率低）

        ListNodePosi(T) first() const { //首节点位置
            return header->succ;
        }

        ListNodePosi(T) last() const { //末节点位置
            return trailer->pred;
        }

        bool valid( ListNodePosi(T) p) { //判断位置 p 是否对外合法
            return p && (trailer != p) && (header != p); //将头、尾节点等同于 NULL
        }

        int disordered() const; //判断列表是否已排序

        ListNodePosi(T) find( T const& e) const { //无序列表查找
            return find(e, _size, trailer);
        }

        //无序区间查找，和向量版本类似，也是从后向前查找，
        //从节点 p (可为 trailer) 的 n 个前驱中，找到等于 e 的最后者
        ListNodePosi(T) find( T const& e, int n, ListNodePosi(T) p) const; 

        ListNodePosi(T) search( T const& e) const { //有序列表查找
            return search(e, _size, trailer);
        }

        ListNodePosi(T) search( T const& e, int n, ListNodePosi(T) p) const; //有序区间查找

        ListNodePosi(T) selectMax( ListNodePosi(T) p, int n); // 从 p 及其 n-1 个后继中选出最大者

        ListNodePosi(T) selectMax(){ //整体最大者
            return selectMax(header->succ, _size);
        }

        //可写访问接口
        ListNodePosi(T) insertAsFirst( T const& e); //将 e 当作首节点插入

        ListNodePosi(T) insertAsLast( T const& e); //将 e 当作末节点插入

        ListNodePosi(T) insertA( ListNodePosi(T) p, T const& e); // insertAfter, 将 e 当作 p 的后继插入

        ListNodePosi(T) insertB( ListNodePosi(T) p, T const& e); // insertBefore, 将 e 当作 p 的前驱插入

        T remove( ListNodePosi(T) p); //删除合法位置 p 处的节点，返回被删除节点

        void merge(List<T>& L) { //全列表归并
            ListNodePosi(T) p = first();

            merge(p, _size, L, L.first(), L._size);
        }

        void sort( ListNodePosi(T) p, int n); //列表区间排序

        void sort() { //列表整体排序
            sort(first(), _size);
        }

        void mergeSort(){
            ListNodePosi(T) p = first();
            mergeSort(p, _size);
        }

        void selectionSort() {
            selectionSort(first(), _size);
        }

        void insertionSort(){
            insertionSort(first(), _size);
        }

        int deduplicate(); //无序去重

        int uniquify(); //有序去重

        void reverse(); //前后倒置
        void reverse2(); //前后倒置
        void reverse3(); //前后倒置

        //遍历
        void traverse( void (*)(T&)); //遍历，依次实施 visit 操作(函数指数，只读或局部性修改)

        template <typename VST> //操作器
        void traverse( VST &); //遍历，依次实施 visit 操作（函数对象，可全局性修改）

        void report(string title){
            cout << "List(" << title << "): ";
            ListNodePosi(T) p = first();
            while(valid(p)){
                cout << p->data << ", ";
                p = p->succ;
            }
            cout << endl;
        }


        void show( ListNodePosi(T) p, int n){//显示从 p 位置开始的 n 个元素值
            for (int i=0; i<n; i++, p=p->succ)
                cout << p->data << ", ";
        }

};
