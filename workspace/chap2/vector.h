#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

typedef int Rank; //秩
#define DEFAULT_CAPACITY 3 // 默认的初始容量（实际应用中可设置为更大）

template <typename T> class Vector { //向量模板类

    protected:

        Rank _size; int _capacity; T* _elem; //规模、容量、数据区

        void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间 A[lo, hi)

        void expand(); //空间不足时扩容

        void shrink(); //装填因子过小时压缩

        bool bubble(Rank lo, Rank hi); //扫描交换

        Rank bubble_tuned_for_tail_in_order(Rank lo, Rank hi);
        Rank bubble_tuned_for_header_in_order(Rank lo, Rank hi);

        Rank max(Rank lo, Rank hi); //选取最大元素

        void merge(Rank lo, Rank mi, Rank hi); //归并算法

        Rank partition(Rank lo, Rank hi); //轴点构造算法

        void quickSort(Rank lo, Rank hi); //快速排序算法

        void heapSort(Rank lo, Rank hi); //堆排序

    public:
        //构造函数
        Vector(int c=DEFAULT_CAPACITY, int s=0, T v=0) { //容量为 c, 规模为 s, 所有元素初始化为 v
            _elem = new T[_capacity = c];
            for (_size = 0; _size < s; _elem[_size++] = v) // s<=c
                ;
        }

        Vector(T const* A, Rank lo, Rank hi) { // 数组区间复制
            copyFrom(A, lo, hi);
        }

        Vector(T const* A, Rank n) { // 数组整体复制
            copyFrom(A, 0, n);
        }

        Vector(Vector<T> const& V, Rank lo, Rank hi) { //向量区间复制
            copyFrom(V._elem, lo, hi);
        }

        Vector(Vector<T> const& V) { //向量整体复制
            copyFrom(V._elem, 0, V._size);
        }

        //析构函数
        ~Vector() {
            delete [] _elem; //释放内存空间
        }

        // 只读访问接口
        Rank size() const { //规模
            return _size;
        }

        bool empty() const {
            return !_size;
        }

        int disordered() const; //判断向量是否已排序

        Rank find(T const& e) const { //无序向量整体查找
            return find(e, 0, _size);
        }

        Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找

        Rank search(T const& e) const { //有序向量整体查找
            return (_size <= 0) ? -1 : search(e, 0, _size);
        }

        Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找

        //可写访问接口
        T& operator[](Rank r) const; //重载下标操作符，可以类似于数组形式引用各元素

        Vector<T> & operator=(Vector<T> const&); //重载赋值操作符，以便直接克隆向量

        T remove(Rank r); //删除秩为 r 的元素

        int remove(Rank lo, Rank hi); //删除秩在区间 [lo, hi) 之间的元素

        Rank insert(Rank r, T const& e); //插入元素

        Rank insert(T const& e) { //默认作为末元素插入
            return insert(_size, e);
        }

        void put(Rank r, T const& e) {
            _elem[r] = e;
        }

        T get(Rank r) {
            return _elem[r];
        }

        void sort(Rank lo, Rank hi); //对 [lo, hi) 排序

        void sort() { //整体排序
            return sort(0, _size);
        }

        void bubbleSort(Rank lo, Rank hi); //起泡排序算法
        void bubbleSort(); //起泡排序算法
        void bubbleSort_tuned_for_tail_in_order(Rank lo, Rank hi); //起泡排序算法. 优化版本
        void bubbleSort_tuned_for_tail_in_order(); //起泡排序算法. 优化版本 ，适合末尾元素大部分就位的情况
        void bubbleSort_tuned_for_header_in_order(Rank lo, Rank hi); //起泡排序算法. 优化版本
        void bubbleSort_tuned_for_header_in_order(); //起泡排序算法. 优化版本 ，适合首部元素大部分就位的情况
        void bubbleSort_tuned_for_header_and_tail_in_order(); //起泡排序算法. 优化版本
        void mergeSort(Rank lo, Rank hi); //归并排序算法
        void mergeSort(); //归并排序算法

        //习题 3-8
        void insertionSort(Rank lo, Rank hi); //插入排序算法
        void insertionSort();

        //习题 3-9
        void selectionSort(Rank lo, Rank hi); //选择排序算法
        void selectionSort();


        void unsort(Rank lo, Rank hi); //对 [lo, hi) 置乱

        void unsort() { //整体置乱
            unsort(0, _size);
        }

        int deduplicate(); //无序去重

        int uniquify(); //有序去重


        //遍历
        void traverse(void (*)(T&)); //遍历（使用函数指针，只读或局部性修改）

        template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）

        //输出全部元素
        void report(string title);
}; //Vector
