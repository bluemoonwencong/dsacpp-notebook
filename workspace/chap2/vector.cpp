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

        void selectionSort(Rank lo, Rank hi); //选择排序算法

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

template <typename T> //元素类型
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) { //以数组区间 A[lo, hi) 为蓝本复制向量
    _elem = new T[_capacity = 2 * (hi-lo)]; //分配空间
    _size = 0; //规模清零
    while (lo < hi)
        _elem[_size++] = A[lo++];
}

template <typename T> Vector<T>& Vector<T>::operator=(Vector<T> const& V) { //重载赋值操作符
    if (_elem)
        delete [] _elem; //释放原有内容
    copyFrom(V._elem, 0, V.size()); //整体复制
    return *this; //返回当前对象的引用，以便链式赋值
}

template <typename T> void Vector<T>::expand() { //向量空间不足时扩容
    if (_size < _capacity)
        return; //尚未满员时，不必扩容

    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY; //不低于最小容量

    T* oldElem = _elem;
    _elem = new T[_capacity <<= 1]; //容量加倍
    for (int i=0; i<_size; i++)
        _elem[i] = oldElem[i]; //复制原向量内容，T 为基本类型，或已重载赋值操作符 '='

    delete [] oldElem; //释放原空间
}

template <typename T> void Vector<T>::shrink(){ //装填因子过小时压缩向量所占空间
    if (_capacity < DEFAULT_CAPACITY<<1) //不致收缩到 DEFAULT_CAPACITY 以下
        return;

    if (_size<<2 > _capacity) // 以 25% 为界，大于 25% 时不收缩
        return;

    T* oldElem = _elem;
    _elem = new T[_capacity >>= 1]; //容量减半
    for (int i=0; i<_size; i++)
        _elem[i] = oldElem[i];

    delete [] oldElem;
}

template <typename T> T& Vector<T>::operator[](Rank r) const { //重载下标操作符
    return _elem[r]; //assert: 0 <= r < _size
}

template <typename T> void permute(Vector<T>& V) { //随机置乱向量，使各元素等概率出现于每一位置
    for (int i=V.size(); i>0; i--) //自后向前
        swap(V[i-1], V[rand() % i]); //V[i-1] 与 V[0, i) 中某一随机元素交换，rand() 返回 0~MAX之间的整数
}

template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) { //等概率随机置乱向量区间[lo, hi)
    T* V = _elem + lo; //将子向量 _elem[lo, hi) 视作另一个向量 V[0, hi-lo)
    for (Rank i=hi-lo; i>0; i--) //自后向前
        swap(V[i-1], V[rand() % i]); //V[i-1] 与 V[0, i) 中某一随机元素交换，rand() 返回 0~MAX之间的整数
}

template <typename T> static bool lt(T* a, T* b) { //less than
    return lt(*a, *b);
}

template <typename T> static bool lt(T& a, T& b) { //less than
    return a < b;
}
template <typename T> static bool eq(T* a, T* b) { //equal
    return eq(*a, *b);
}
template <typename T> static bool eq(T& a, T& b) { //equal
    return a == b;
}

template <typename T> //无序向量的顺序查找，返回最后一个元素 e 的位置;失败时返回 lo-1
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {  //在 [lo, hi) 内查找
    //assert: 0 <= lo < hi <= _size
    while ((lo < hi--) && (e != _elem[hi]))
        ; // 自后向前，顺序查找
    return hi; //若 hi<lo, 则意味着失败; 否则 hi 即命中元素的秩
}

template <typename T> //将 e 作为秩为 r 的元素插入
Rank Vector<T>::insert(Rank r, T const& e) {
    //assert: 0 <= r <= size
    expand(); //若有必要， 扩容
    for (int i=_size; i>r; i--)
        _elem[i] = _elem[i-1]; //自后向前， 后继元素顺序后移一个单元

    _elem[r] = e; 
    _size++;
    return r;
}

template <typename T> int Vector<T>::remove(Rank lo, Rank hi) { //删除区间 [lo, hi)
    if (lo == hi) 
        return 0; //出于效率考虑，单独处理退化情况，比如 remove(0, 0)

    while (hi < _size)
        _elem[lo++] = _elem[hi++]; // [hi, _size] 顺次前移 hi-lo 个单元

    _size = lo; // 更新规模，直接丢弃尾部 [lo, _size=hi) 区间
    shrink(); //若有必要，则缩容
    return hi-lo; //返回被删除元素的数目
}

template <typename T> T Vector<T>::remove(Rank r) { //删除向量中秩为 r 的元素， 0 <= r < size
    T e = _elem[r];
    remove(r, r+1); //调用区间删除算法，等效于对区间 [r, r+1) 的删除
    return e; //返回被删除元素
}

template <typename T> int Vector<T>::deduplicate(){ //删除无序向量中重复元素（高效版本）
    int oldSize = _size;
    Rank i = 1; //从 _elem[1] 开始
    while (i < _size) //自前向后逐一考查各元素 _elem[i]
        (find(_elem[i], 0, i) < 0) ? //在其前缀中寻找与之雷同者（至多一个）
            i++ : remove(i); //若无雷同则继续考查其后续，否则删除雷同者

    return oldSize - _size; //向量规模变化量，即被删除元素总数
}

template <typename T> void Vector<T>::traverse(void (*visit)(T&)){ //利用函数指针机制的遍历
    for (int i=0; i<_size; i++)
        visit(_elem[i]);
}

template <typename T> template <typename VST> //元素类型、操作器
void Vector<T>::traverse(VST& visit) { //利用函数对象机制的遍历
    for (int i=0; i<_size; i++)
        visit(_elem[i]);
}


//有序向量甄别算法
template <typename T> int Vector<T>::disordered() const { //返回向量中逆序相邻元素对的总数
    int n = 0; //计数器
    for (int i=1; i<_size; i++) //逐一检查 _size-1 对相邻元素
        if (_elem[i-1] > _elem[i])
            n++; //逆序则计数
    return n; //向量有序当且仅当 n=0
}


/*
//有序向量重复元素删除算法（低效版本）
template <typename T> int Vector<T>::uniquify(){
    int oldSize = _size;
    int i = 1;
    while (i<_size) //自前向后，逐一比对各对相邻元素
        _elem[i-1] == _elem[i] ? remove[i] : i++; //若雷同，则删除后者; 否则转到后一元素
    return oldSize-_size; //返回删除元素总数
}
*/

//有序向量重复元素删除算法（高效版本）
template <typename T> int Vector<T>::uniquify(){
    Rank i = 0, j = 0; //各对互异 "相邻“ 元素的秩
    while (++j < _size) //逐一扫描，直到末元素
        if (_elem[i] != _elem[j]) //跳过雷同者
            _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧

    _size = ++i; //直接截除尾部多余元素
    shrink();
    return j - i; //返回删除元素总数
}
//二分查找版本A：在有序向量的区间 [lo, hi) 内查找元素 e, 0 <= lo <= hi <= _size
template <typename T> static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {
    while (lo < hi) { //每步迭代可能要做两次比较判断，有三个分支
        Rank mi = (lo + hi) >> 1; //以中点为轴点
        if (e < A[mi]) 
            hi = mi; //深入前半段 [lo, mi)继续查找
        else if ( e > A[mi])
            lo = mi + 1; //深入后半段
        else
            return mi; //在 mi 处命中
    } //成功查找可以提前终止
    return -1; //查找失败
} //有多个命中元素时，不能保证返回秩最大者； 查找失败时，简单返回 -1， 而不能指示失败的位置

//二分查找版本B：在有序向量的区间 [lo, hi) 内查找元素 e, 0 <= lo <= hi <= _size
template <typename T> static Rank binSearch_VB(T* A, T const& e, Rank lo, Rank hi) {
    //循环在区间不足两个时中止
    while (1 < hi-lo) { //每步迭代仅做一次比较判断，有两个分支;成功查找不能提前终止
        Rank mi = (lo + hi) >> 1; //以中点为轴点
        (e < A[mi]) ? hi = mi : lo = mi; //经比较后确定深入 [lo, mi) 或 [mi, hi)
    } // 出口时 lo+1 == hi, 即区间中只剩下一个元素 A[lo]
    return (e == A[lo]) ? lo : -1; //查找成功时返回对应的秩，否则统一返回 -1 
} //有多个命中元素时，不能保证返回秩最大者； 查找失败时，简单返回 -1， 而不能指示失败的位置


//二分查找版本C：在有序向量的区间 [lo, hi) 内查找元素 e, 0 <= lo <= hi <= _size
template <typename T> static Rank binSearch_VC(T* A, T const& e, Rank lo, Rank hi) {
    while (lo < hi) { //每步迭代仅做一次比较判断，有两个分支
        Rank mi = (lo + hi) >> 1; //以中点为轴点
        (e < A[mi]) ? hi = mi : lo = mi + 1; //经比较后确定深入 [lo, mi) 或 (mi, hi)
    } //成功查找不能提前终止
    return --lo; //循环结束时，lo 为大于 e 的元素的最小秩，故 lo-1 即不大于 e  的元素的最大秩
} //有多个命中元素时，总能保证返回秩最大者;查找失败时，能够返回失败的位置


// 习题 [1-22]
class Fib { // Fibonacci 数列类
    private:
        int f, g; // fib(k-1), fib(k), int 型很快会溢出
    public:
        Fib(int n) { //初始化为不小于 n 的最小 Fibonacci 项， 如 Fib(6)=8, O(logn)
            f = 0; g = 1; // fib(0), fib(1)
            while (g < n) {
                next();
            }
        }

        int get() { //获取当前 Fibonacci 项，如 Fib(8).get() = 8, O(1)
            return g;
        }

        int next() { //转到下一项，如 Fib(8).next() = 13, O(1)
            g += f;
            f = g - f;
            return g;
        }

        int prev() { //转到上一项，如 Fib(8).prev() = 5, O(1)
            f = g - f;
            g -= f;
            return g;
        }
};

// Fibonacci 查找版本A：在有序向量的区间 [lo, hi) 内查找元素 e, 0 <= lo <= hi <= _size
template <typename T> static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi) {
    Fib fib(hi-lo); //用 O(log_phi(hi-lo) 时间创建 Fib 数列，值不大于 hi-lo
    while (lo < hi) { //每步迭代可能要做两次比较，有三个分支
        while (hi-lo < fib.get())
            fib.prev(); //通过向前顺序查找（分摊O(1))
        Rank mi = lo + fib.get() - 1; //确定形如 Fib(k)-1 的轴点
        if (e < A[mi])
            hi = mi;
        else if (A[mi] < e)
            lo = mi + 1;
        else
            return mi; //命中
    } //成功查找可以提前终止
    return -1; //查找失败
} //有多个命中元素时，不能保证返回秩最大者； 查找失败时，简单返回 -1， 而不能指示失败的位置


//有序向量的区间 [lo, hi) 内，确定不大于 e 的最后一个节点的秩
template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const { //assert: 0 <= lo < hi <= _size
    return binSearch_VC(_elem, e, lo, hi);
    /*
    return (rand() % 2) ? // 按各 50%的概率随机使用
        binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi); //二分查找或 Fibonacci 查找
    */
}


template <typename T> void Vector<T>::sort(Rank lo, Rank hi) { //向量区间 [lo, hi) 排序
    switch(rand() % 5) { // 随机选取排序算法
        case 1: bubbleSort(lo, hi); break;
        case 2: mergeSort(lo, hi); break;
        //case 3: selectionSort(lo, hi); break;
        //case 4: heapSort(lo, hi); break;
        //default : quickSort(lo, hi); break;
        default : bubbleSort(lo, hi); break;
    }
}


template <typename T> //向量的起泡排序
void Vector<T>::bubbleSort() //assert: 0 <= lo < hi <= size
{
    bubbleSort(0, size());
}

template <typename T> //向量的起泡排序
void Vector<T>::bubbleSort(Rank lo, Rank hi) //assert: 0 <= lo < hi <= size
{
    while(!bubble(lo, hi--)) //逐趟扫描交换，直到全序
        ; // pass
}

template <typename T> bool Vector<T>::bubble(Rank lo, Rank hi) { //一趟扫描交换
    bool sorted = true; //整体有序标志
    while (++lo < hi) //自左向右，逐一检查各对相邻元素
        if (_elem[lo-1] > _elem[lo]) { //若逆序，则
            sorted = false; //意味着尚末整体有序，并需要
            swap(_elem[lo-1], _elem[lo]);
        }
    return sorted;
}

//优化的起泡排序，适合于最右侧多数元素已经就位的情况
//每趟扫描后，记录最右侧的逆序对位置，
//从而下趟可直接忽略后面已经就序的元素
//自右向左扫描
//习题[2-25] b)
template <typename T>
void Vector<T>::bubbleSort_tuned_for_tail_in_order()
{
    bubbleSort_tuned_for_tail_in_order(0, size());
}

template <typename T>
void Vector<T>::bubbleSort_tuned_for_tail_in_order(Rank lo, Rank hi)
{
    while ((lo=bubble_tuned_for_tail_in_order(lo, hi)) < hi)
        ; //pass
}

template <typename T> Rank Vector<T>::bubble_tuned_for_tail_in_order(Rank lo, Rank hi) {
    Rank last = hi; //最左侧的逆序对初始化为 [hi-2, hi-1]
    hi = hi-1;
    while (--hi >= lo) //自右向左，逐一检查各对相邻元素
        if (_elem[hi] > _elem[hi+1]) { //若逆序，则
            last = hi; //更新最左侧逆序对位置
            swap(_elem[hi], _elem[hi+1]);
        }
    return last;
}

//优化的起泡排序，适合于最左侧多数元素已经就位的情况
//每趟扫描后，记录最左侧的逆序对位置，
//从而下趟可直接忽略后面已经就序的元素
//自左向右扫描
template <typename T>
void Vector<T>::bubbleSort_tuned_for_header_in_order()
{
    bubbleSort_tuned_for_header_in_order(0, size());
}

template <typename T>
void Vector<T>::bubbleSort_tuned_for_header_in_order(Rank lo, Rank hi)
{
    while (lo < (hi = bubble_tuned_for_header_in_order(lo, hi)))
        ; //pass
}

template <typename T> Rank Vector<T>::bubble_tuned_for_header_in_order(Rank lo, Rank hi) {
    Rank last = lo; //最右侧的逆序对初始化为 [lo-1, lo]
    while (++lo < hi) //自左向右，逐一检查各对相邻元素
        if (_elem[lo-1] > _elem[lo]) { //若逆序，则
            last = lo; //更新最右侧逆序对位置
            swap(_elem[lo-1], _elem[lo]);
        }
    return last;
}

//习题[2-25] c) 首部和末尾的连续元素都已就位，只有中间的元素没有就位, O(n)
template <typename T> 
void Vector<T>::bubbleSort_tuned_for_header_and_tail_in_order()
{
    Rank lo = bubble_tuned_for_tail_in_order(0, size());
    Rank hi = bubble_tuned_for_header_in_order(0, size());
    cout << "lo=" << lo <<endl;
    cout << "hi=" << hi <<endl;
    bubbleSort(lo, hi);
}

template <typename T> //向量归并排序
void Vector<T>::mergeSort(Rank lo, Rank hi) { // 0 <= lo < hi <= size
    if (hi-lo < 2) //单元素区间自然是有序
        return;

    int mi = (lo+hi) >> 1; //中点为界
    mergeSort(lo, mi); mergeSort(mi, hi); //分别对前后半段排序
    merge(lo, mi, hi); //归并
}


template <typename T> //向量归并排序
void Vector<T>::mergeSort() {
    mergeSort(0, size());
}

template <typename T> //有序向量的归并
void Vector<T>::merge(Rank lo, Rank mi, Rank hi){ //以 mi 为界，合并有序子向量 [lo, mi), [mi, hi)
    T* A = _elem + lo; //前子向量的首地址，合并后的结果地址也从这开始

    int first_len = mi-lo;
    T* B = new T[first_len]; //用于临时存放前子向量
    for (Rank i=0; i<first_len; B[i] = A[i++])
        ; //pass

    int second_len = hi-mi;
    T* C = _elem + mi; //后子向量的首地址

    for (Rank i=0, j=0, k=0; (j<first_len) || (k<second_len); ){ //将 B[j] 和 C[k] 中的小者续至 A 末尾

        // 前子向量还有元素未处理时，
        //   1. 如果后子向量已经处理完毕，或者
        //   2. 其第一个元素小于后子向量的第一个元素
        if ( (j<first_len) && ( !(k<second_len) || (B[j]<=C[k]) ) )
            A[i++] = B[j++];

        // 后子向量还有元素未处理时，
        //   1. 如果前子向量已经处理完毕，或者
        //   2. 其第一个元素小于前子向量的第一个元素
        if ( (k<second_len) && ( !(j<first_len) || (C[k] < B[j]) ) )
            A[i++] = C[k++];
    }

    delete [] B;
} //归并后得到完整的有序向量 [lo, hi)

template <typename T>
void Vector<T>::report(string title){
    cout << "Vector(" << title << "): ";
    for (int i=0; i<_size; i++)
        cout << _elem[i] << ", ";
    cout << endl;
}


//习题 2-34 位图 Bitmap b)
class Bitmap {
    private:
        char* M; int N; //比特图所存放的空间 M[], 容量为 N * sizeof(char) * 8 比特。

    protected:
        void init(int n) {
            M = new char[N = (n+7)/8]; //申请能容纳 n 个比特的最少字节
            memset(M, 0, N);
        }

    public:
        Bitmap(int n=8) {
            init(n);
        }

        Bitmap(char* file, int n=8) { //从指定文件中读取比特图
            init(n);
            FILE* fp = fopen(file, "r");
            fread(M, sizeof(char), N, fp);
            fclose(fp);
        }

        ~Bitmap() {
            delete [] M;
            M = NULL;
        }

        void set(int k){ //将第 k 位置设置为 true
            expand(k);

            /*
             * k>>3 确定该位在哪个字节
             * k&0x07 确定字节中的位置
             * （0x80 >> (k & 0x07)) 将字节中的该位置 1
             */
            M[k >> 3] |= (0x80 >> (k & 0x07) );
        }

        void clear(int k){ //将第 k 位置设置为 false
            expand(k);

            /*
             * k>>3 确定该位在哪个字节
             * k&0x07 确定字节中的位置
             * (0x80 >> (k & 0x07)) 将字节中的该位置 1
             * ~(0x80 >> (k & 0x07)) 将字节中的该位置 0
             */
            M[k >> 3] &= ~(0x80 >> (k & 0x07));
        }

        bool test(int k){ //测试第 k 位是否为 true
            expand(k);

            /*
             * k>>3 确定该位在哪个字节
             * k&0x07 确定字节中的位置
             * （0x80 >> (k & 0x07)) 将字节中的该位置 1
             */
            return M[k >> 3] & (0x80 >> (k & 0x07) );
        }

        void dump(char* file) { //将位图整体导出至指定的文件，以便以后的新位图批量初始化
            FILE* fp = fopen(file, "w");
            fwrite(M, sizeof(char), N, fp);
            fclose(fp);
        }

        char* bits2string(int n) { //将前 n 位转换为字符串
            expand(n-1); //此时可能被访问的最高位为 bit[n-1]
            char* s = new char[n+1];
            s[n] = '\0'; //字符串所占空间，由上层调用者负责释放
            for (int i=0; i<n; i++)
                s[i] = test(i) ? '1' : '0';
            return s;
        }

        void expand(int k) { //若被访问的 Bitmap[k] 已出界，则需扩容
            if (k < 8*N)
                return;
            int oldN = N;
            char* oldM = M;
            init(2*k); //与向量类似，加倍策略
            memcpy(M, oldM, oldN);
            delete [] oldM;
        }
};

//习题 2-34 c)
//创建 Bitmap 对象时，如何节省下为初始化所有元素所需的时间？
//设位置只需提供 test() 和 set() 接口，暂时不需要 clear() 接口，
class Bitmap_without_init { //以空间换时间，仅允许插入，不支持删除
    private:
        Rank* F; Rank N; //规模为 N 的向量 F，
        Rank* T; Rank top; //容量为 N 和栈

    protected:
        inline bool valid(Rank r){ return (0 <= r) && (r < top); }

    public:
        Bitmap_without_init(Rank n=8) {
            N = n;
            F = new Rank[N]; T = new Rank[N]; // 在 O(1) 内隐式地初始化
            top = 0; 
        }

        ~Bitmap_without_init(){ delete [] F; delete [] T; }

        //接口
        inline void set(Rank k) {
            if (test(k))
                return;
            //要设置的位置 k，对应的 F[k] 处将值设置为栈的栈顶指针，
            //同时在栈中将栈顶指针处将值设置为 k，建立校验环
            //从而当要 test k 位置时，取出对应的 F[k] 处的值，即为当时
            //保存的栈顶指针，再从栈中取出值，如果值和 k 相同，则
            // k 位有设置值。
            T[top] = k; F[k] = top; ++top; //建立校验环
        }

        inline bool test(Rank k) {
            return valid(F[k]) && ( k == T[ F[k] ] );
        }

        char* bits2string() { //将前 n 位转换为字符串
            char* s = new char[N+1];
            s[N] = '\0'; //字符串所占空间，由上层调用者负责释放
            for (int i=0; i<N; i++)
                s[i] = test(i) ? '1' : '0';

            return s;
        }

};

//习题 2-34 c)
//创建 Bitmap 对象时，如何节省下为初始化所有元素所需的时间？
//如果还要支持 clear() 接口，则必须有效辨别两种无标记的位：从末标记过的
//和曾经标记后又被清除的。
//下面的实现中将清除后的 k 位，其对应的栈中的值约定为 -1-k。
class Bitmap_without_init2 { //以空间换时间，仅允许插入，支持删除
    private:
        Rank* F; Rank N; //规模为 N 的向量 F，
        Rank* T; Rank top; //容量为 N 和栈

    protected:
        inline bool valid(Rank r){ return (0 <= r) && (r < top); }
        inline bool erased(Rank k) {// 判断 [k] 是否曾经被标记过，后又被清除
            return valid (F[k]) &&
                (T[ F[k] ] == -1-k); //清除后的栈中值约定为 -1-k
        }

    public:
        Bitmap_without_init2(Rank n=8) {
            N = n;
            F = new Rank[N]; T = new Rank[N]; // 在 O(1) 内隐式地初始化
            top = 0; 
        }

        ~Bitmap_without_init2(){ delete [] F; delete [] T; }

        //接口
        inline void set(Rank k) {
            if (test(k))
                return;
            //要设置的位置 k，对应的 F[k] 处将值设置为栈的栈顶指针，
            //同时在栈中将栈顶指针处将值设置为 k，建立校验环
            //从而当要 test k 位置时，取出对应的 F[k] 处的值，即为当时
            //保存的栈顶指针，再从栈中取出值，如果值和 k 相同，则
            // k 位有设置值。
            //
            if (!erased(k)) //若初始标记，则创建新校验环，
                F[k] = top++; //
            T[ F[k] ] = k;  //若系曾经标记后被清除的，则恢复原校验环
        }

        inline void clear(Rank k) {
            if (test(k))
                T[ F[k] ] = -1-k;
        }

        inline bool test(Rank k) {
            return valid(F[k]) && ( k == T[ F[k] ] );
        }

        char* bits2string() { //将前 n 位转换为字符串
            char* s = new char[N+1];
            s[N] = '\0'; //字符串所占空间，由上层调用者负责释放
            for (int i=0; i<N; i++)
                s[i] = test(i) ? '1' : '0';

            return s;
        }
};

//习题 2-35
//利用 Bitmap 在 O(n) 内剔除 n 个 ASCII 字符中的重复字符，各字符仅保留一份
void uniquify_ascii(char* str, int len){
    Bitmap bm = Bitmap(128);
    cout << "aft: ";
    for (int i=0; i<len; i++){
        if (!bm.test(str[i])){
            cout << str[i];
            bm.set(str[i]);
        }
    }
    cout << endl;
}


//习题 2-36 利用 Bitmap 计算出不大于 10^8 的所有素数 Eratosthenes 筛法
// 素数（质数）为 > 1 的除 1 和自身外不能被其它整数整队的数，因此 0, 1 都不是质数。
// 筛法求素数：计算不大于 n 的所有素数
//   先排除 0, 1 两个非素数，从 2 到 n 迭代进行：
//     接下来的数 i 是一个素数，并将素数的整数倍 (i, 2i, ... ki) 都标识为非素数。
// 根据素数理论，不大于 N 的素数最多 N/ln(N) 个。
void Eratosthenes(int n, char* file) {
    Bitmap bm(n);
    bm.set(0); bm.set(1); //0 和 1 都不是素数
    for (int i=2; i<n; i++) //反复地从
        if (!bm.test(i))   //下一个可认定的素数 i 起
            for (int j= min(i, 46340)*min(i, 46340); j<n; j += i) //以 i 为间隔
                bm.set(j); //将下一个数标记为合数
    //B.dump(file); //将所有整数的筛法标记统一存入指定文件。
    cout << "Primary:";
    for (int i=0; i<n; i++){
        if (!bm.test(i)){
            cout << i << ", ";
        }
    }
    cout << endl;
}

int main() {
    cout << "hello" << endl;

    // Vector test
    Vector<int> v = Vector<int>();
    v.report("Init"); 
    v.insert(0, 9);
    v.insert(0, 4);
    v.report("insert(0,4), insert(0, 9)"); // 4, 9
    v.insert(1, 5);
    v.report("insert(1,5)"); //4,5,9
    v.put(1, 2);
    v.report("put(1,2)"); //4,2,9
    cout << "get(2)=" << v.get(2) << endl; //9
    v.insert(3,6);
    v.report("insert(3,6)"); //4,2,9,6
    v.insert(1, 7);
    v.report("insert(1, 7)"); //4,7,2,9,6
    v.remove(2);
    v.report("remove(2)"); //4,7,9,6
    v.insert(1,3);
    v.report("insert(1,3)"); //4,3,7,9,6
    v.insert(3, 4);
    v.report("insert(3, 4)"); //4,3,7,4,9,6
    cout << "size()=" << v.size() << endl;

    cout << "disordered()=" << v.disordered() << endl; // 3
    cout << "find(9)=" << v.find(9) << endl; //4
    cout << "find(5)=" << v.find(5) << endl; //-1

    v.sort();
    v.report("sorted"); //3,4,4,6,7,9

    v.unsort();
    v.report("unsort");
    v.bubbleSort();
    v.report("bubbleSort"); //3,4,4,6,7,9

    v.unsort();
    v.report("unsort");
    v.bubbleSort_tuned_for_tail_in_order();
    v.report("bubbleSort_tuned_for_tail_in_order"); //3,4,4,6,7,9

    v.unsort();
    v.report("unsort");
    v.bubbleSort_tuned_for_header_in_order();
    v.report("bubbleSort_tuned_for_header_in_order"); //3,4,4,6,7,9

    v.unsort();
    v.report("unsort");
    v.bubbleSort_tuned_for_header_and_tail_in_order();
    v.report("bubbleSort_tuned_for_header_and_tail_in_order"); //3,4,4,6,7,9


    v.unsort();
    v.report("unsort");
    v.mergeSort();
    v.report("mergeSort"); //3,4,4,6,7,9

    cout << "disordered()=" << v.disordered() << endl; // 0

    cout << "search(1)=" << v.search(1) << endl; //-1
    cout << "search(4)=" << v.search(4) << endl; //2
    cout << "search(8)=" << v.search(8) << endl; //4
    cout << "search(9)=" << v.search(9) << endl; //5
    cout << "search(10)=" << v.search(10) << endl; //5

    v.uniquify();
    v.report("uniquified");

    cout << "search(9)=" << v.search(9) << endl; //4

    //习题 [2-34] Bitmap b) 测试
    cout << "Bitmap test:" << endl;
    Bitmap bitmap = Bitmap();
    bitmap.set(0);
    bitmap.set(1);
    bitmap.set(9);
    cout << "Bitmap:" << bitmap.bits2string(15) << endl; //110000000100000


    //习题 [2-34] 无需初始化时间的 Bitmap c) 测试
    cout << "Bitmap_without_init test:" << endl;
    Bitmap_without_init bitmap2 = Bitmap_without_init(10);
    bitmap2.set(0);
    bitmap2.set(1);
    bitmap2.set(9);
    cout << "Bitmap:" << bitmap2.bits2string() << endl; //1100000001

    //习题 [2-34] 无需初始化时间的 Bitmap c) 支持 clear()测试
    cout << "Bitmap_without_init2 test:" << endl;
    Bitmap_without_init2 bitmap3 = Bitmap_without_init2(10);
    bitmap3.set(0);
    bitmap3.set(1);
    bitmap3.set(9);
    cout << "Bitmap:" << bitmap3.bits2string() << endl; //1100000001
    bitmap3.clear(1);
    cout << "Bitmap:" << bitmap3.bits2string() << endl; //1000000001

    //习题 2-35 测试
    char* str = new char[101];
    for(int i=0; i<100; i++){
        str[i] = 'a' + rand()%26; //a-z
    }
    str[100] = '\0';

    cout <<"ori: " << str <<endl;
    uniquify_ascii(str, 100);

    Eratosthenes(200, NULL);
}
