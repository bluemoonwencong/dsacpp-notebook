#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "./stack.cpp"

using namespace std;

int main() {
    cout << "hello" << endl;
    Stack<int> v = Stack<int>();
    v.push(9);
    v.report("Init"); 
    cout << "top=" << v.top() << endl;
    cout << "pop=" << v.pop() << endl;
    v.report("Now"); 
}
