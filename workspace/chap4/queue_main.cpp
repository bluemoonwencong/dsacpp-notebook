#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <math.h>
#include "./queue.cpp"

using namespace std;

//列队使用
//银行服务模拟
//
struct Customer{ //顾客类：
    int window; //所属窗口（队列）
    unsigned int time; //需要的服务时长
    int id;
};

int bestWindow( Queue<Customer> windows[], int nWin) { //为新到顾客确定最佳队列
    int minSize = windows[0].size(), optiWin = 0; //最优队列（窗口）
    for ( int i=1; i< nWin; i++)
        if (minSize > windows[i].size() ) { //挑选出队列最短者
            minSize = windows[i].size();
            optiWin = i;
        } 

    return optiWin;
}

//模拟在银行中接受服务的过程
void simulate( int nWin, int servTime) { //按指定窗口数，服务总时间模拟银行业务
    Queue<Customer>* windows = new Queue<Customer>[nWin]; //为每一窗口创建一个队列

    for (int now=0; now<servTime; now++) { //在下班前，每隔一个单位时间
        if (rand() % (1+nWin)) { //新顾客以 nWin/(nWin+1) 的概率到达
            Customer c;
            c.id = now;
            c.time = 1 + rand() % 98; //新顾客到达，服务时长随机确定
            c.window = bestWindow(windows, nWin); //找出最佳（最短）的服务窗口
            windows[c.window].enqueue(c); //新顾客入对应的队列
            cout << "Customer " << c.id << " enters Queue " << c.window << endl;
        }

        for (int i=0; i< nWin; i++) //分别检查
            if (!windows[i].empty()) //各非空队列
                if (--windows[i].front().time <= 0) { // 队首顾客的服务时长减少一个单位
                    Customer c = windows[i].dequeue(); //服务完毕的顾客出列，由后继顾客接替
                    cout << "Customer " << c.id << " leaves Queue " << c.window << endl;
                }
    } //for

    for (int i=0; i<nWin; i++){
        cout << "Queue " << i+1 << " Size:" << windows[i].size() << endl;
    }
    
    delete [] windows;
}

//习题 4-23 两个有序队列的归并
Queue<int> queue_merge(Queue<int> A, Queue<int> B){
    Queue<int> q = Queue<int>();
    while(!A.empty() && !B.empty()){
        if (A.front() < B.front())
            q.enqueue( A.dequeue() );
        else
            q.enqueue( B.dequeue() );
    }

    while (!A.empty())
        q.enqueue( A.dequeue() );

    while (!B.empty())
        q.enqueue( B.dequeue() );

    return q;
}

int main() {
    Queue<int> s = Queue<int>();

    s.report("Init"); 
    cout << "Is empty = " << s.empty() << endl;

    s.enqueue(5);
    s.report("enqueue(5)");  //5
    s.enqueue(3); 
    s.report("enqueue(3)"); //5,3
    cout << "dequeue()=" << s.dequeue() << endl;
    cout << "front()=" << s.front() << endl;
    s.report("Now"); 
    cout << "size()=" << s.size() << endl;

    cout << "test bank simulation:" << endl;
    simulate(10, 60*8);


    // 习题 4-23 队列的归并算法
    Queue<int> A = Queue<int>();
    A.enqueue(1);
    A.enqueue(3);
    A.enqueue(5);
    A.enqueue(7);
    Queue<int> B = Queue<int>();
    B.enqueue(2);
    B.enqueue(4);
    B.enqueue(6);
    B.enqueue(7);

    Queue<int>  C = queue_merge(A, B);
    A.report("Q1:");
    B.report("Q2:");
    C.report("Q1+Q2:");
}

