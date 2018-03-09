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
        void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int); //有序列表区间归并
        void mergeSort(ListNodePosi(T) p, int); //对从 p 开始的 n 个节点归并排序
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

        bool empty() const {
            return _size <= 0;
        }

        T& operator[](Rank r) const; //重载，支持循秩访问（效率低）

}

int main() {

    cout << "hello" << endl;
}
