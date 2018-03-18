#include <stdlib.h>
#include <iostream>
#include "./list.cpp"
using namespace std;

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
