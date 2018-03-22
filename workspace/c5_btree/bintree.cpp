#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../chap4/stack.cpp"

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

int main() {
    cout << "hello" << endl;
}
