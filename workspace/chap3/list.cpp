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

template <typename T> void List<T>::init(){ //列表初始化，在创建列表对象时统一调用
    header = new ListNode<T>; //创建头哨兵节点
    trailer = new ListNode<T>; //创建尾哨兵节点
    header->succ = trailer; header->pred = NULL;
    trailer->pred = header; trailer->succ = NULL;
    _size = 0;
}


template <typename T> //重载下标操作符，以通过秩直接访问列表节点（虽方便，但效率低，慎用）
T& List<T>::operator[] (Rank r) const { //assert: 0 <= r < size
    ListNodePosi(T) p = first(); //从首节点出发
    while (0 < r--)
        p = p->succ; //顺数第 r 个节点即是
    return p->data; //目标节点
}

//无序区间查找，和向量版本类似，也是从后向前查找，
//从节点 p (可为 trailer) 的 n 个前驱中，找到等于 e 的最后者
template <typename T>
ListNodePosi(T) List<T>::find( T const& e, int n, ListNodePosi(T) p) const {
    while ( 0 < n-- ) // 对于 p 的最近的 n 个前驱，从右向左
        if ( e == (p=p->pred)->data )
            return p; //逐个比对

    return NULL; // p 越出左边界意味着区间内不包含 e, 查找失败
} //失败时返回 NULL

template <typename T> ListNodePosi(T) List<T>::insertAsFirst( T const& e) {
    ++_size;
    return header->insertAsSucc(e); //e 当作首节点插入
}

template <typename T> ListNodePosi(T) List<T>::insertAsLast( T const& e) {
    ++_size;
    return trailer->insertAsPred(e); //e 当作末节点插入
}

template <typename T> ListNodePosi(T) List<T>::insertA( ListNodePosi(T) p, T const& e) {
    ++_size;
    return p->insertAsSucc(e); //e 当作 p 的后继插入, After
}

template <typename T> ListNodePosi(T) List<T>::insertB( ListNodePosi(T) p, T const& e) {
    ++_size;
    return p->insertAsPred(e); //e 当作 p 的前驱插入, Before
}

template <typename T> //将 e 紧靠当前节点之前插入当前节点所属列表（设有哨兵头节点 header)
ListNodePosi(T) ListNode<T>::insertAsPred( T const& e ) {
    // 创建新节点，其前驱指向当前节点的前驱，后继指向当前节点
    ListNodePosi(T) x = new ListNode<T>(e, pred, this);

    pred->succ = x; pred = x; //设置正向链接
    return x; //返回新节点位置
}

template <typename T> //将 e 紧靠当前节点之后插入当前节点所属列表（设有哨兵头节点 trailer)
ListNodePosi(T) ListNode<T>::insertAsSucc( T const& e ) {
    // 创建新节点，其后继指向当前节点的后续，前驱指向当前节点
    ListNodePosi(T) x = new ListNode<T>(e, this, succ);

    succ->pred = x; succ = x; //设置正向链接
    return x; //返回新节点位置
}

template <typename T> //列表内部方法：将某列表中自位置 p 起的 n 项，逐一插入到当前列表的末尾
void List<T>::copyNodes( ListNodePosi(T) p, int n) { //p 合法，且至少有 n-1 个真后继节点
    init();
    while (n--) {
        insertAsLast(p->data);
        p = p->succ;
    }
}

template <typename T> //将某列表中自位置 p 起的 n 项，复制为一个新的列表
List<T>::List( ListNodePosi(T) p, int n) {
    copyNodes(p, n);
}


template <typename T> //整体复制列表 L
List<T>::List( List<T> const& L) {
    copyNodes(L.first(), L._size);
}

template <typename T> //将列表 L 中自第 r(秩）项起的 n 项，复制为一个新的列表, assert: r+n <= L._size
List<T>::List( List<T> const& L, int r, int n) {
    copyNodes( L[r], n);
}

template <typename T> T List<T>::remove (ListNodePosi(T) p) { //删除合法节点 p，返回其数值
    T e = p->data; //假定 T 类似可直接赋值
    p->pred->succ = p->succ; p->succ->pred = p->pred;
    delete p; _size--;
    return e;
}

template <typename T> List<T>::~List(){
    clear();
    delete header;
    delete trailer;
}

template <typename T> int List<T>::clear() { //清空列表
    int oldSize = _size;
    while( 0 < _size )
        remove( header->succ); //反复删除首节点，直到列表变空
    return oldSize;
}

template <typename T>
int List<T>::deduplicate(){ //剔除无序列表中的重复节点，从左向右进行，和向量版本类似
    if (_size < 2) //平凡列表自然无重复
        return 0;

    int oldSize = _size;
    ListNodePosi(T) p = header; Rank r = 0; 
    while ( (p=p->succ) != trailer ) { //从首节点开始，依次直到末节点
        ListNodePosi(T) q = find(p->data, r, p); //在 p 的 r 个真前驱中查找相同者
        q ? remove(q) : r++; //若的确存在，则删除;否则秩加一
    } //assert: 循环过程中的任意时刻， p 的所有前驱互不相同
    return oldSize - _size; //列表规模变化量，即被删除元素总数
} //O(n^2)


template <typename T> void List<T>::traverse( void (*visit)( T& ) ){ //借助函数指针机制遍历
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

template <typename T> template <typename VST> //元素类型、操作器
void List<T>::traverse( VST& vist){ //借助函数对象机制遍历
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}


// 有序列表的唯一化
template <typename T> int List<T>::uniquify(){
    if (_size < 2)
        return 0; //平凡列表自然无重复
    int oldSize = _size;

    ListNodePosi(T) p = first(); ListNodePosi(T) q; //p 为各区段起点，q 为其后继
    while ( (q = p->succ) != trailer ) //反复考查紧邻的节点对 (p, q)
        if (p->data != q->data)
            p = q;  //若互异，则转向下一区段
        else
            remove(q); //否则相同，删除后者
    return oldSize - _size; //返回被删除总数
} //O(n)

//有序列表的查找
//返回的位置应便于后续的（插入等）操作
template <typename T> //在有序列表内节点 p(可能是 trailer) 的 n 个真前驱中，找到 <= e 的最后者
ListNodePosi(T) List<T>::search( T const& e, int n , ListNodePosi(T) p ) const {
    // assert: 0 <= n <= rank(p) < _size
    while( 0 <= n--) //对于 p 的最近的 n 个前驱，从右到左逐个比较
        if ( ((p=p->pred)->data) <= e) //直到命中，数值越界或范围越界
            break;
    //assert: 至此位置 p 必符合输出语义约定--尽管此前最后一次关键码比较可能没有意义（等效于与 -inf比较)
    return p; //返回查找终止的位置
} //失败时，返回区间左边界的前驱（可能是 header) --调用者可通过 valid() 判断成功与否
//O(n)


template <typename T> void List<T>::sort( ListNodePosi(T) p, int n) { //列表区间排序
    switch( rand() % 3) {
        case 1: insertionSort(p, n); break; //插入排序
        case 2: selectionSort(p, n); break; //选择排序
        default: mergeSort(p, n); break; //归并排序

    }
}

//插入排序
//始终将整个序列视作并切分为两部分，有序的前缀 s[0, r) 和无序了后缀 S[r, n);通过迭代，反复地将后缀的首元素转移到前缀中。
//由此亦看出插入排序算法的不变性。
template <typename T> //列表的插入排序：对起始于位置 p 的 n 个元素排序
void List<T>::insertionSort( ListNodePosi(T) p, int n) { //valid(p) && rank(p) + n <= size
    for (int r=0; r<n; r++) { //逐一为各节点
        // search(e, r, p) 返回 p 的 r 个真前驱中不大于 e 的最后者位置
        insertA( search(p->data, r, p), p->data); 
        p = p->succ; //转向下一节点
        remove(p->pred);
    }
}

//选择排序
//将序列划分为无序的前缀 S[0, r) 及有序的后缀 S[r, n)，此后还要求前缀中的元素都不大于后缀中的元素。如此，每次只需从前缀中选出最大者，并作为最小元素转移至后缀中，即可使有序部分的范围不断扩张。
template <typename T> //列表的选择排序算法，对起始于位置 p 的 n 个元素排序
void List<T>::selectionSort ( ListNodePosi(T) p, int n) { //valid(p) && rank(p)+n <= size
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p;
    for (int i=0; i<n; i++)
        tail = tail->succ; //将 head 和 tail 指向排序区列表的 header 和 tailer

    while( 1<n ) { //在至少还剩下两个节点之前，在待排序区间内
        ListNodePosi(T) max = selectMax( head->succ, n); //找出最大者
        insertB( tail, remove(max) ); // 将无序前缀中的最大者移到有序后缀中作为首元素
        // swap(tail->pred->data, max->data); // 优化：可以不用按上面进行删除和插入操作，只需互换数值即可, 习题 3-13
        tail = tail->pred;
        n--;
    }
} //O(n^2)


template <typename T> //从起始于位置 p 的 n 个元素中选出最大者，相同的返回最后者
ListNodePosi(T) List<T>::selectMax( ListNodePosi(T) p, int n) {
    ListNodePosi(T) max = p; //最大者暂定为首节点 p
    for ( ListNodePosi(T) cur = p; 1 < n; n--) //从首节点 p 出发，将后续节点逐一与 max 比较
        if ((cur=cur->succ)->data >= max->data) //若当前元素 >= max, 则
            max = cur;
    return max; //返回最大节点位置
}

template <typename T> //有序列表的归并：当前列表中自 p 起的 n 个元素，与列表 L 中自 q 起的 m 个元素归并
void List<T>::merge( ListNodePosi(T) &p, int n, List<T>& L, ListNodePosi(T) q, int m) {
    //assert: this.valid(p) && rank(p)+n<=_size && this.sorted(p,n)
    //        L.valid(q) && rank(q)+m<=L._size && L.sorted(q,m)
    //注：在归并排序之类的场合，有可能 this==L && rank(p)+n=rank(q)
    //为方便归并排序，归并所得的有序列表依然起始于节点 p
    ListNodePosi(T) pp = p->pred; //方便之后能返回 p

    while ( 0 < m ) //在 q 尚未移出区间之前
        if ( (0<n) && (p->data <= q->data) ){ //若 p 仍在区间内且 v(p) <= v(q)
            if ( q == ( p=p->succ ) ) // 如果此时 p 部分已经处理完，则提前返回
                break;
            n--;  // p 归入合并的列表，并替换为其直接后继
        }
        else { //若 p 已超出右界或 v(q) < v(p) 则
            ListNodePosi(T) bb = insertB( p, L.remove( (q=q->succ)->pred )); //将 q 转移到 p 之前
            m--;
        }

    p = pp->succ; //确定归并后区间的起点
}


template <typename T> //列表的归并排序算法：对起始于位置 p 的 n 个元素排序
void List<T>::mergeSort( ListNodePosi(T) & p, int n) { //valid(p) && rank(p)+n <= _size
    if (n<2) 
        return;

    int m = n >> 1; //以中点为界
    ListNodePosi(T) q = p;
    for ( int i=0; i<m; i++) //均分列表
        q = q->succ; 

    mergeSort(p, m);
    mergeSort(q, n-m); //对前后子列表排序

    merge(p, m, *this, q, n-m); //归并
}//注意：排序后，p 依然指向归并后区间的起点

ListNodePosi(int) create_node(int data) {
    ListNodePosi(int) node = new ListNode<int>();
    node->data = data;
    return node;
}

//习题 3-18，共 3 种实现方式
template <typename T>
void List<T>::reverse() {  //适合 T 类型不复杂，能在常数时间内完成赋值操作的情况
    ListNodePosi(T) p = header;
    ListNodePosi(T) q = trailer;
    for (int i=0; i<_size; i+=2){ //从首末节点开始，由外而内，捉对地
        /*p = p->succ;              // 交换对称节点的数据项
        q = q->pred;
        swap(p->data, q->data);
        */
        swap( (p=p->succ)->data, (q=q->pred)->data );
    }
}


template <typename T>
void List<T>::reverse2() {  //适合 T 类型复杂，不能在常数时间内完成赋值操作的情况
    if (_size < 2)
        return;

    ListNodePosi(T) p; ListNodePosi(T) q;

    for ( p = header, q = p->succ; p != trailer; p = q, q = p->succ )
        p->pred = q; //自前向后，依次颠倒各节点的前驱指针

    for ( p = header, q = p->pred; p != trailer; p = q, q = p->pred )
        q->succ = p; //自前向后，依次颠倒各节点的后续指针

    // 准备互换头尾
    trailer->pred = NULL;
    header->succ = NULL;
    swap( header, trailer);
}

template <typename T>
void List<T>::reverse3() {  //适合 T 类型复杂，不能在常数时间内完成赋值操作的情况
    if (_size < 2)
        return;

    for ( ListNodePosi(T) p = header; p; p = p->pred ) //自前向后，依次
        swap(p->pred, p->succ);
    swap(header, trailer);
}

int main() {

    cout << "hello" << endl;

    // List test
    List<int> v = List<int>();
    v.report("Init"); 

    //ListNodePosi(int) node = create_node(9);
    v.insertAsFirst(4);
    v.insertAsLast(9);
    v.report("insertAsFirst(4), insertAsLast(0, 9)"); // 4, 9
    ListNodePosi(int) p = v.first();
    v.insertA(p, 5);
    v.report("insertA(first,5)"); //4,5,9
    p = v.first();
    v.insertB(p, 2);
    v.report("insertB(first,2)"); //2,4,5,9
    cout << "last=" << v.last()->data << endl; //9

    v.insertAsFirst(100);
    p = v.first();
    cout << "removed=" << v.remove(p) << endl; //100


    v.insertAsFirst(8);
    v.insertAsFirst(7);
    v.insertAsFirst(5);
    v.insertAsFirst(2);
    v.insertAsFirst(4);

    v.report("ori"); //4, 2, 5, 7, 8, 2, 4, 5, 9,

    List<int> *pv2 = new List<int>(v);
    pv2->sort();
    pv2->report("sorted"); // 2, 2, 4, 4, 5, 5, 7, 8, 9,

    delete pv2;
    pv2 = new List<int>(v);
    pv2->insertionSort();
    pv2->report("insertionSort"); // 2, 2, 4, 4, 5, 5, 7, 8, 9,

    delete pv2;
    pv2 = new List<int>(v);
    pv2->selectionSort();
    pv2->report("selectionSort"); // 2, 2, 4, 4, 5, 5, 7, 8, 9,


    List<int> *pv3 = new List<int>(*pv2);
    pv2->report("pv2:");
    pv3->report("pv3:");
    pv2->merge(*pv3);
    pv2->report("test merge pv2,pv3");

    delete pv2;
    pv2 = new List<int>(v);
    pv2->mergeSort();
    pv2->report("mergeSort"); // 2, 2, 4, 4, 5, 5, 7, 8, 9,
    cout << "find(1)=" << pv2->rank(pv2->find(1)) << endl; //-1
    cout << "find(2)=" << pv2->rank(pv2->find(2)) << endl; //1
    cout << "find(3)=" << pv2->rank(pv2->find(2)) << endl; //1

    pv2->uniquify();
    pv2->report("uniquify"); // 4, 2, 5, 7, 8, 9,


    delete pv2;
    pv2 = new List<int>(v);
    pv2->report("ori"); // 4, 2, 5, 7, 8, 2, 4, 5, 9,
    cout << "search(2)=" << pv2->rank(pv2->search(2)) << endl; //5
    cout << "search(1)=" << pv2->rank(pv2->search(1)) << endl; //-1
    cout << "search(9)=" << pv2->rank(pv2->search(9)) << endl; //8

    delete pv2;
    pv2 = new List<int>(v);
    pv2->report("ori"); // 4, 2, 5, 7, 8, 2, 4, 5, 9,
    pv2->deduplicate();
    pv2->report("deduplicate"); // 7, 8, 2, 4, 5 9,

    delete pv2;
    pv2 = new List<int>(v);
    pv2->report("ori"); // 4, 2, 5, 7, 8, 2, 4, 5, 9,
    pv2->reverse();
    pv2->report("reverse");

    delete pv2;
    pv2 = new List<int>(v);
    pv2->report("ori"); // 4, 2, 5, 7, 8, 2, 4, 5, 9,
    pv2->reverse2();
    pv2->report("reverse2");

    delete pv2;
    pv2 = new List<int>(v);
    pv2->report("ori"); // 4, 2, 5, 7, 8, 2, 4, 5, 9,
    pv2->reverse3();
    pv2->report("reverse3");
}
