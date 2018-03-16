#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../chap2/vector.cpp"

using namespace std;

template <typename T> 
class Stack: public Vector<T> { //向量为基类，派生出栈模板类

    public: // size(), empty() 等都继承使用
        void push( T const& e) { //入栈：等效于将新元素作为向量的末元素插入
            this->insert( this->size(), e);
        }

        T pop() { //出栈：等效于删除向量的末元素
            return this->remove(this->size()-1);
        }

        T& top() { //取顶：直接返回向量的末元素
            return (*this)[this->size()-1];
        }
};
