#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../chap4/stack.cpp"
#include "../chap4/queue.cpp"

using namespace std;

//二叉树节点 BinNode 模板类
#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（与 “空树高度为 -1”的约定相统一）
typedef enum {RB_RED, RB_BLACK} RBColor; //节点颜色，红黑树

template <typename T> struct BinNode { //二叉树节点模板类
    //成员
    T data; //数值
    BinNodePosi(T) parent; //父节点
    BinNodePosi(T) lChild; //左、右节点
    BinNodePosi(T) rChild;
    int height; //高度(通用)
    int npl; //Null Path Length (左式推，也可直接用 height 代替)
    RBColor color; //颜色（红黑树）

    //构造函数
    BinNode(): parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED) {}

    BinNode(T e, BinNodePosi(T) p=NULL, BinNodePosi(T) lc=NULL, BinNodePosi(T) rc=NULL,
            int h=0, int l=1, RBColor c=RB_RED)
        : data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) {}

    //操作接口
    int size(); //统计当前节点后代总数，亦即以其为根的子树的规模

    BinNodePosi(T) insertAsLC(T const&); //作为当前节点的左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const&); //作为当前节点的右孩子插入新节点

    BinNodePosi(T) succ(); //（中序遍历意义下）取当前节点的直接后继

    template <typename VST> void travLevel(VST&); //子树层次遍历
    template <typename VST> void travPre(VST&); //子树先序遍历
    template <typename VST> void travIn(VST&); //子树中序遍历
    template <typename VST> void travPost(VST&); //子树后序遍历

    //比较器、判等器（其它用到再补充）
    bool operator<(BinNode const& bn){ //小于
        return data < bn.data;
    }

    bool operator==(BinNode const& bn) { //等于
        return data == bn.data;
    }

};


// BinNode 节点 x 状态与性质的判断
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x)==(x).parent->lChild))
#define IsRChild(x) (!IsRoot(x) && (&(x)==(x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //至少有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //有两个孩子
#define IsLeaf(x) (!HasChild(x))

// 与 BinNode 节点 x 或指针 p 的关系
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rChild : (p)->parent->lChild) //返回其兄弟节点指针值

#define uncle(p) (IsLChild(*((p)->parent)) ? (x)->parent->parent->rChild : (p)->parent->parent->lChild) //返回其叔叔节点指针值

#define FromParentTo(x) (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lChild : (x).parent->rChild)) //返回来自父节点对其的引用指针


template <typename T> //将 e 作为当前节点的左孩子插入二叉树
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) {
    return lChild = new BinNode(e, this);
}

template <typename T> //将 e 作为当前节点的右孩子插入二叉树
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) {
    return rChild = new BinNode(e, this);
}

template <typename T> template <typename VST>
void BinNode<T>::travIn(VST& visit) { //二叉树中序遍历算法统一入口
    switch(rand() % 5) {
        case 1: travIn_I1(this, visit); break; //迭代版本 #1
        case 2: travIn_I2(this, visit); break; //迭代版本 #2
        case 3: travIn_I3(this, visit); break; //迭代版本 #3
        case 4: travIn_I4(this, visit); break; //迭代版本 #4
        default: travIn_R(this, visit); break; //递归版本
    }
}


template <typename T> class BinTree { //二叉树模板类
    protected:
        int _size; //规模
        BinNodePosi(T) _root; //根节点
        virtual int updateHeight(BinNodePosi(T) p); //更新节点 p 的高度，每个应用实例中的高度概念不同
        void updateHeightAbove(BinNodePosi(T) p); //更新节点 p 及其祖先的高度

    public:
        BinTree(): _size(0), _root(NULL) {}

        ~BinTree() {
            if (0 < _size)
                remove(_root);
        }

        int size() const { //规模
            return _size;
        }

        bool empty() const {
            return !_root;
        }

        BinNodePosi(T) root() const {
            return _root;
        }

        BinNodePosi(T) insertAsRoot(T const& e); //插入根节点

        BinNodePosi(T) insertAsLC(BinNodePosi(T) p, T const& e); // e 作为 p 的左孩子（原无）插入
        BinNodePosi(T) insertAsRC(BinNodePosi(T) p, T const& e); // e 作为 p 的右孩子（原无）插入

        BinNodePosi(T) attachAsLC(BinNodePosi(T) p, BinTree<T>* &s); // s 作为 p 的左子树接入
        BinNodePosi(T) attachAsRC(BinNodePosi(T) p, BinTree<T>* &s); // s 作为 p 的右子树接入

        int remove(BinNodePosi(T) p); //删除以位置 p 处节点为根的子树，返回该子树原先的规模
        BinTree<T>* secede(BinNodePosi(T) p); //将子树 p 从当前树中摘除，并将其转换为一棵独立子树

        template <typename VST>
        void travLevel(VST& visit) { //层次遍历
            if (_root)
                _root->travLevel(visit);
        }

        template <typename VST>
        void travPre(VST& visit) { //先序遍历
            if (_root)
                _root->travPre(visit);
        }

        template <typename VST>
        void travIn(VST& visit) { //中序遍历
            if (_root)
                _root->travIn(visit);
        }

        template <typename VST>
        void travPost(VST& visit) { //后序遍历
            if (_root)
                _root->travPost(visit);
        }

        bool operator<(BinTree<T> const& t){
            return _root && t._root && (*_root < *t._root);
        }

        bool operator==(BinTree<T> const& t){
            return _root && t._root && (*_root == *t._root);
        }
};

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) p) { //更新节点 p 高度, 高度具体规则因树不同而异
    return p->height = 1 + max(stature(p->lChild), stature(p->rChild));
}

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) p) { //更新节点 p 及其祖先的高度
    while (p) { 
        int preHeight = stature(p);
        if (preHeight == updateHeight(p)) //优化：一旦高度未变，即可终止
            break;
        p = p->parent; 
    }
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e){ //将 e 作为根节点插入到空的二叉树中
    _size = 1;
    return _root = new BinNode<T>(e);
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) p, T const& e){ //将 e 作为 p 的左孩子插入
    _size ++;
    p->insertAsLC(e);
    updateHeightAbove(p);
    return p->lChild;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) p, T const& e){ //将 e 作为 p 的右孩子插入
    _size ++;
    p->insertAsRC(e);
    updateHeightAbove(p);
    return p->rChild;
}

template <typename T> //二叉树子树接入算法：将 S 当作节点 p 的左子树接入，S 本身置空
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) p, BinTree<T>* &S){ //p->lChild == NULL
    if (p->lChild = S->_root) //判断接入后的子树是否是空树
        p->lChild->parent = p;  //接入

    _size += S->_size; 
    updateHeightAbove(p);

    S->_root = NULL;
    S->_size = 0;
    release(S);
    S = NULL; //释放原树
    return p; //返回接入位置
}

template <typename T> //二叉树子树接入算法：将 S 当作节点 p 的右子树接入，S 本身置空
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) p, BinTree<T>* &S){ //p->rChild == NULL
    if (p->rChild = S->_root) //判断接入后的子树是否是空树
        p->rChild->parent = p;  //接入

    _size += S->_size; 
    updateHeightAbove(p);

    S->_root = NULL;
    S->_size = 0;
    release(S);
    S = NULL; //释放原树
    return p; //返回接入位置
}

template <typename T> //删除二叉树中位置 p 处的节点及其后代，返回被删除节点的个数
int BinTree<T>::remove(BinNodePosi(T) p) { //assert: p 为二叉树中的合法位置
    FromParentTo(*p) = NULL; //切断来自父节点的指针引用关联
    updateHeightAbove(p->parent); //更新祖先高度
    int n = removeAt(p); //删除子树 p
    _size -= n; //更新规模
    return n; //返回删除的节点总数
}

template <typename T> //删除二叉树中位置 p 处的节点及其后代，返回被删除节点的个数
static int removeAt(BinNodePosi(T) p) { //assert: p 为二叉树中的合法位置
    if (!p) //递归基：空树
        return 0;

    int n = 1 + removeAt(p->lChild) + removeAt(p->rChild); //递归释放左右子树

    //release(p->data); 
    release(p); //释放节点

    return n;
}

template <typename T> //二叉树子树分离算法：将子树 p 从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) p) { //assert: p 为二叉树中的合法位置
    FromParentTo(*p) = NULL; //切断来自父节点的指针
    updateHeightAbove(p->parent); //更新原树中所有祖先的高度

    BinTree<T>* S = new BinTree<T>; //创建一个以 p 为根的新树
    S->_root = p;
    p->parent = NULL;
    S->_size = p->size();

    _size -= S->_size;
    return S;
}

template <typename T, typename VST>
void travPre_R(BinNodePosi(T) p, VST& visit) { //二叉树先序遍历算法（递归版本）
    if (!p)
        return;

    visit(p->data);
    travPre_R(p->lChild, visit);
    travPre_R(p->rChild, visit);
}

//习题 5-10 使用栈消除尾递归
template <typename T, typename VST>
void travPre_I1(BinNodePosi(T) p, VST& visit) { //二叉树先序遍历算法（迭代1：使用栈消除尾递归）
    Stack<BinNodePosi(T)> s; //辅助栈

    if (p) //根节点入栈
        s.push(p);

    while (!s.empty()) { //在栈变空之前反复循环
        p = s.pop();
        visit(q->data); //先访问

        if (HasRChild(*p))
            s.push(p->rChild); //要先压入右子树节点

        if (HasLChild(*p))
            s.push(p->lChild);
    }
}

// 查看先序遍历过程，是先沿 **最左侧通路(leftmmost path)** 自顶而下访问沿途节点，再自底而上依次遍历这些节点的右子树。
//因此，先序遍历可分解为两段：
// + 沿最左侧通路自顶而下访问各节点
// + 再自底而上遍历对应的右子树。
//从当前节点出发，自顶而下沿左分支不断深入，直到没有左分支的节点，沿途节点遇到后立即访问，
//引入辅助栈，存储对应节点的右子树
template <typename T, typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) p, VST& visit, Stack<BinNodePosi(T)>& S){
    while (p) {
        visit(p->data); //访问当前节点

        if (p->rChild) //右孩子入栈暂存（优化：通过判断，避免空的右孩子入栈）
            S.push(p->rChild);
        
        p = p->lChild; //沿左分支深入一层
    }
}

template <typename T, typename VST>
void travPre_I2(BinNodePosi(T) p, VST& visit) { //二叉树先序遍历算法（迭代2）
    Stack<BinNodePosi(T)> S; //辅助栈

    S.push(p);

    while(!S.empty())
        visitAlongLeftBranch( S.pop(), visit, S);
}

template <typename T, typename VST>
void travPost_R(BinNodePosi(T) p, VST& visit) { //二叉树后序遍历算法（递归版本）
    if (!p)
        return;

    travPre_R(p->lChild, visit);
    travPre_R(p->rChild, visit);
    visit(p->data);
}


template <typename T, typename VST>
void travIn_R(BinNodePosi(T) p, VST& visit) { //二叉树中序遍历算法（递归版本）
    if (!p)
        return;

    travPre_R(p->lChild, visit);
    visit(p->data);
    travPre_R(p->rChild, visit);
}


//考查中序遍历过程：
//1. 从根结点开始，先沿最左侧通路自顶而下，到达最左的节点（即没有左孩子的节点），将沿途的节点压入辅助栈
//2. 现在可以访问最左的节点了，因此从栈中弹出该节点，访问它，如果它有右孩子，则将右孩子压入栈中（此后在迭代中能完成该右孩子为根的子树的相同遍历过程）
//3. 从栈中弹出一个节点，再次迭代。
template <typename T> //从当前节点出发，沿左分支不断深入，直到没有左分支的节点
static void goAlongLeftBranch(BinNodePosi(T) p, Stack<BinNodePosi(T)>& S) {
    while (p){
        S.push(p);
        p = p->lChild;
    }
}

template <typename T, typename VST>
void travIn_I1(BinNodePosi(T) p, VST& visit) { //二叉树中序遍历算法，迭代版本 1
    Stack<BinNodePosi(T)> S; //辅助栈

    while( true ){
        goAlongLeftBranch(p, S); //从当前节点出发，逐批入栈
        if (S.empty()) //直到所有节点处理完毕
            break;
        p = S.pop(); visit(p->data); //弹出栈顶节点并访问
        p = p->rChild; //转向右子树
    }
}


//遍历能将半线性的二叉树转化为线性结构。于是指定遍历策略后，就能在节点间定义前驱和后继了。其中没有前驱（后继）的节点称作首（末）节点。
//定位中序遍历中的直接后继对二叉搜索树很重要。
template <typename T>
BinNodePosi(T) BinNode<T>::succ() { //定位节点 v 的直接后继
    BinNodePosi(T) s = this; //记录后继的临时变量

    if (rChild) { //若有右孩子，则直接后继必在右子树中，具体地就是
        s = rChild; //右子树中的
        while (HasLChild(*s)) //最靠左（最小）的节点
            s = s->lChild;
    } else { //否则，直接后继应是 “将当前节点包含于基左子树中的最低祖先”，具体地就是
        while (IsRChild(*s))
            s = s->parent; //逆向地沿右向分支，不断朝左上方移动

        s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
    }

    return s;
}

template <typename T, typename VST>
void travIn_I2(BinNodePosi(T) p, VST& visit) { //二叉树中序遍历算法，迭代版本 2
    Stack<BinNodePosi(T)> S; //辅助栈

    while( true ){
        if (p) { //沿最左侧通路自顶而下，将节点压入栈
            S.push(x);
            p = p->lChild;
        }
        else if (!S.empty()) {
            p = S.pop(); //尚未访问的最低祖先节点
            visit(p->data);
            p = p->lChild; //遍历该节点的右子树
        }
        else 
            break;  //遍历完成
        p = S.pop(); visit(p->data); //弹出栈顶节点并访问
        p = p->rChild; //转向右子树
    }
}

template <typename T, typename VST>
void travIn_I3(BinNodePosi(T) p, VST& visit) { //二叉树中序遍历算法：版本版本 3, 无需辅助栈
    bool backtrack = false; //前一步是否刚从右子树回溯 -- 省去栈，仅 O(1) 辅助空间
                            //回溯回来的表示当前节点的左侧都已经访问过了

    while (true)
        if (!backtrack && HasLChild(*p)) //若有左子树且不是刚刚回溯，则
            p = p->lChild; //深入遍历左子树
        else { //否则--无左子树或刚刚回溯（左子树已访问完毕）
            visit(p->data); //访问该节点
            if (HasRChild(*p)) { //若有右子树，则
                p = p->rChild; //深入右子树继续遍历
                backtrack = false; //并关闭回溯标志
            } else { // 若右子树为空，则
                if (!(x=x->succ())) //后继为空，表示抵达了末节点
                    break;
                backtrack = true; //并设置回溯标志
            }
        }
}

//将树 T 画在二维平面上，从左侧水平向右看云，未被遮挡的最高叶节点 v（称作最高左侧可见叶节点 HLVFL），即为后序遍历首先访问的节点，该节点可能是左孩子，也可能是右孩子（故用垂直边表示）。
//沿着 v 与树根之间的通路自底而上，整个遍历也可分解为若干个片段。每一片段，分别起始于通路上的一个节点，并包括三步：访问当前节点，遍历以其右兄弟（若存在）为根的子树，最后向上回溯至其父节点（若存在）并转下下一片段
template <typename T> //在以栈 S 顶节点为根的子树中，找到最高左侧可见叶节点
static void gotoHLVFL(Stack<BinNodePosi(T)> & S) { //沿途所遇节点依次入栈
    while (BinNodePosi(T) p = S.top()) //自顶而下，反复检查当前节点（即栈顶）
        if (HasLChild(*p)) { //尽可能向左
            if (HasRChild(*P))
                S.push(p->rChild); //若有右孩子，优先入栈
            S.push(p->lChild); //然后才转至左孩子
        }
        else //实不得已
            S.push(p->rChild); //才向右

    S.pop();//返回之前，弹出栈顶的空节点
}

template <typename T, typename VST>
void travPost_I(BinNodePosi(T) p, VST& visit) { //二叉树的后序遍历（迭代版本）
    Stack<BinNodePosi(T)> S; //辅助栈

    if (p)
        S.push(p); //根入栈

    while (!S.empty()) {
        if (S.top() != p->parent) //若栈顶不是当前节点之父（则必为其右兄），
            gotoHLVFL(S); //则此时以其右兄为根的子树中，找到 HLVFL

        p = S.pop(); //弹出该前一节点之后继，并访问
        visit(x->data); 
    }
}


// 层次遍历
//即先上后下，先左后右，借助队列实现。
template <typename T, typename VST>
void BinNode<T>::travLevel(VST& visit) { //二叉树层次遍历
    Queue<BinNodePosi(T)> Q; //辅助队列
    Q.enqueue(this); //根入队

    while (!Q.empty()) {
        BinNodePosi(T) p = Q.dequeue(); visit(p->data); //取出队首节点并访问

        if (HasLChild(*p)
                Q.enqueue(p->lChild);

        if (HasRChild(*p)
                Q.enqueue(p->rChild);
    }
}

int main() {
    cout << "hello" << endl;
}
