#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../chap3/list.cpp"

using namespace std;

template <typename T> 
class Queue: public List<T> { //List 为基类，派生出队列模板类

    public: // size(), empty() 等都继承使用
        void enqueue( T const& e) { //入队：尾部插入
            this->insertAsLast(e);
        }

        T dequeue() { //出队：首部删除
            return this->remove(this->first());
        }

        T& front() { //队首
            return this->first()->data;
        }
};
