#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "./bintree.cpp"

using namespace std;

struct Visit {
    virtual void operator()(const char data) {
        cout << data << ", ";
    }
};


void re_init_btree(BinTree<char> &btree) {
    BinNodePosi(char) i = btree.insertAsRoot('i');

    BinNodePosi(char) d = btree.insertAsLC(i, 'd');
    BinNodePosi(char) l = btree.insertAsRC(i, 'l');

    BinNodePosi(char) c = btree.insertAsLC(d, 'c');
    BinNodePosi(char) h = btree.insertAsRC(d, 'h');
    BinNodePosi(char) k = btree.insertAsLC(l, 'k');
    BinNodePosi(char) n = btree.insertAsRC(l, 'n');

    BinNodePosi(char) a = btree.insertAsLC(c, 'a');
    BinNodePosi(char) f = btree.insertAsLC(h, 'f');
    BinNodePosi(char) j = btree.insertAsLC(k, 'j');
    BinNodePosi(char) m = btree.insertAsLC(n, 'm');
    BinNodePosi(char) p = btree.insertAsRC(n, 'p');

    BinNodePosi(char) b = btree.insertAsRC(a, 'b');
    BinNodePosi(char) e = btree.insertAsLC(f, 'e');
    BinNodePosi(char) g = btree.insertAsRC(f, 'g');
    BinNodePosi(char) o = btree.insertAsLC(p, 'o');
}

int main() {
    BinTree<char> btree;

    BinNodePosi(char) i = btree.insertAsRoot('i');

    BinNodePosi(char) d = btree.insertAsLC(i, 'd');
    BinNodePosi(char) l = btree.insertAsRC(i, 'l');

    BinNodePosi(char) c = btree.insertAsLC(d, 'c');
    BinNodePosi(char) h = btree.insertAsRC(d, 'h');
    BinNodePosi(char) k = btree.insertAsLC(l, 'k');
    BinNodePosi(char) n = btree.insertAsRC(l, 'n');

    BinNodePosi(char) a = btree.insertAsLC(c, 'a');
    BinNodePosi(char) f = btree.insertAsLC(h, 'f');
    BinNodePosi(char) j = btree.insertAsLC(k, 'j');
    BinNodePosi(char) m = btree.insertAsLC(n, 'm');
    BinNodePosi(char) p = btree.insertAsRC(n, 'p');

    BinNodePosi(char) b = btree.insertAsRC(a, 'b');
    BinNodePosi(char) e = btree.insertAsLC(f, 'e');
    BinNodePosi(char) g = btree.insertAsRC(f, 'g');
    BinNodePosi(char) o = btree.insertAsLC(p, 'o');

    Visit visit = Visit();

    cout << "travPre_R:" << endl;
    travPre_R<char, Visit> (btree.root(), visit);  // i, d, c, a, b, h, f, e, g, l, k, j, n, m, p, o, 
    cout << endl;
    cout << "travPre_I1:" << endl;
    travPre_I1<char, Visit> (btree.root(), visit);  // i, d, c, a, b, h, f, e, g, l, k, j, n, m, p, o, 
    cout << endl;
    cout << "travPre_I2:" << endl;
    travPre_I2<char, Visit> (btree.root(), visit);  // i, d, c, a, b, h, f, e, g, l, k, j, n, m, p, o, 
    cout << endl;
    cout << endl;

    cout << "travIn_R:" << endl;
    travIn_R<char, Visit> (btree.root(), visit); //a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, 
    cout << endl;
    cout << "travIn_I1:" << endl;
    travIn_I1<char, Visit> (btree.root(), visit); //a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, 
    cout << endl;
    cout << "travIn_I2:" << endl;
    travIn_I2<char, Visit> (btree.root(), visit); //a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, 
    cout << endl;
    cout << "travIn_I3:" << endl;
    travIn_I3<char, Visit> (btree.root(), visit); //a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, 
    cout << endl;
    cout << "travIn_I4:" << endl;
    travIn_I4<char, Visit> (btree.root(), visit); //a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, 
    cout << endl;
    cout << endl;

    cout << "travPost_R:" << endl;
    travPost_R<char, Visit> (btree.root(), visit);  //b, a, c, e, g, f, h, d, j, k, m, o, p, n, l, i
    cout << endl;
    cout << "travPost_I:" << endl;
    travPost_I<char, Visit> (btree.root(), visit); 
    cout << endl;
    cout << endl;

    cout << "travLevel:" << endl;
    travLevel<char, Visit> (btree.root(), visit); //i, d, l, c, h, k, n, a, f, j, m, p, b, e, g, o
    cout << endl;
    cout << endl;


    cout << "succ(i):" << i->succ()->data << endl; //j
    cout << "succ(h):" << h->succ()->data << endl; //i
    cout << endl;

    cout << "pred(j):" << j->pred()->data << endl; //i
    cout << "pred(i):" << i->pred()->data << endl; //h
    cout << endl;

    //习题 5-23
    cout << "Original travLevel:" << endl;
    travLevel<char, Visit> (btree.root(), visit); //i, d, l, c, h, k, n, a, f, j, m, p, b, e, g, o
    cout << endl;
    swap_pre_I1<char>(btree.root());
    cout << "After swap_pre_I1 travLevel:" << endl;
    travLevel<char, Visit> (btree.root(), visit); // i, l, d, n, k, h, c, p, m, j, f, a, o, g, e, b,
    cout << endl;
    swap_pre_I2<char>(btree.root());
    cout << "After swap_pre_I2 travLevel:" << endl;
    travLevel<char, Visit> (btree.root(), visit); //i, d, l, c, h, k, n, a, f, j, m, p, b, e, g, o
    cout << endl;
    swap_pre_R<char>(btree.root());
    cout << "After swap_pre_R travLevel:" << endl;
    travLevel<char, Visit> (btree.root(), visit); // i, l, d, n, k, h, c, p, m, j, f, a, o, g, e, b,
    cout << endl;
    cout << endl;

    //习题 5-25
    re_init_btree(btree);
    cout << "Original tree level trav:" << endl;
    travLevel<char, Visit> (btree.root(), visit); // i, d, l, c, h, k, n, a, f, j, m, p, b, e, g, o, 
    cout << endl;
    char max = replace_as_children_largest_post_R<char>(btree.root());
    cout << "replace_as_children_largest_post_R: max=" << max << endl;
    travLevel<char, Visit> (btree.root(), visit); // p, h, p, c, h, k, p, b, g, j, m, p, b, e, g, o
    cout << endl;
    cout << endl;

    re_init_btree(btree);
    cout << "Original tree level trav:" << endl;
    travLevel<char, Visit> (btree.root(), visit); // i, d, l, c, h, k, n, a, f, j, m, p, b, e, g, o, 
    cout << endl;
    replace_as_children_largest_post_I<char>(btree.root());
    cout << "replace_as_children_largest_post_I: " << endl;
    travLevel<char, Visit> (btree.root(), visit); // p, h, p, c, h, k, p, b, g, j, m, p, b, e, g, o
    cout << endl;
}
