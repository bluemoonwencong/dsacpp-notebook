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

        void bubbleSort(Rank lo, Rank hi); //起泡排序算法

        Rank max(Rank lo, Rank hi); //选取最大元素

        void selectionSort(Rank lo, Rank hi); //选择排序算法

        void merge(Rank lo, Rank mi, Rank hi); //归并算法

        void mergeSort(Rank lo, Rank hi); //归并排序算法

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

        void sort(Rank lo, Rank hi); //对 [lo, hi) 排序

        void sort() { //整体排序
            return sort(0, _size);
        }

        void unsort(Rank lo, Rank hi); //对 [lo, hi) 置乱

        void unsort() { //整体置乱
            unsort(0, _size);
        }

        int deduplicate(); //无序去重

        int uniquify(); //有序去重


        //遍历
        void traverse(void (*)(T&)); //遍历（使用函数指针，只读或局部性修改）

        template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
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
template <typename T> static bool lt(T& a, T& b) { //equal
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

template <typename T> int Vector<T>::remove(Rank r) { //删除向量中秩为 r 的元素， 0 <= r < size
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

template <typename T> template <template VST> //元素类型、操作器
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
template <template T> int Vector<T>::uniquify(){
    int oldSize = _size;
    int i = 1;
    while (i<_size) //自前向后，逐一比对各对相邻元素
        _elem[i-1] == _elem[i] ? remove[i] : i++; //若雷同，则删除后者; 否则转到后一元素
    return oldSize-_size; //返回删除元素总数
}
*/

//有序向量重复元素删除算法（高效版本）
template <template T> int Vector<T>::uniquify(){
    Rank i = 0, j = 0; //各对互异 "相邻“ 元素的秩
    while (++j < _size) //逐一扫描，直到末元素
        if (_elem[i] != _elem[j]) //跳过雷同者
            _elem[i++] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧

    _size = ++i; //直接截除尾部多余元素
    shrink();
    return j - i; //返回删除元素总数
}

//有序向量的区间 [lo, hi) 内，确定不大于 e 的最后一个节点的秩
template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const { //assert: 0 <= lo < hi <= _size
    return (rand() % 2) ? // 按各 50%的概率随机使用
        binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi); //二分查找或 Fibonacci 查找
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

int main() {

    cout << "hello" << endl;
}
