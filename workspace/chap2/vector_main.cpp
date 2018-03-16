#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "vector.cpp"

using namespace std;

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

    v.unsort();
    v.report("unsort");
    v.insertionSort();  //习题 [3-8]
    v.report("insertionSort"); //3,4,4,6,7,9


    v.unsort();
    v.report("unsort");
    v.selectionSort();  //习题 [3-9]
    v.report("selectionSort"); //3,4,4,6,7,9

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
