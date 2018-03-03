#include <iostream>
using namespace std;

typedef long long tint64;

inline tint64 square(tint64 a) { return a*a; }

tint64 power2(int n) { //幂函数 2^n算法，优化递归版本, n>=0
    if (0==n) return 1; //递归基
    return (n&1) ? square(power2(n >> 1)) << 1 : square(power2(n >> 1)); //视 n 的 奇偶分别递归
} // O(logn) = O(r), r 为输入指数 n 的比特位数


//迭代版本
//习题 [1-14]
tint64 power2_loop(int n) {
    tint64 pow = 1; // 累积器初始化为 2^0
    tint64 p = 2; // 累乘项初始化为 2，对应最低位为 1 的情况
    while (n>0) { // 迭代 log(n) 轮
        if (n&1) // 根据当前比特位是否为 1, 决定
            pow *= p; // 将当前累乘项计入累积器
        n >>= 1; //指数减半
        p *= p; //累乘项自乘
    }
    return pow; 
} // O(logn) = O(r), r 为输入指数 n 的比特位数

//而一般性的 a^n 计算如下 
tint64 power_loop(tint64 a, int n) { // a^n 算法： n >= 0
    tint64 pow = 1; // 累积器初始化为 2^0
    tint64 p = a; // 累乘项初始化为 a，对应最低位为 1 的情况
    while (n>0) { // 迭代 log(n) 轮
        if (n&1) // 根据当前比特位是否为 1, 决定
            pow *= p; // 将当前累乘项计入累积器
        n >>= 1; //指数减半
        p *= p; //累乘项自乘
    }
    return pow; 
} // O(logn) = O(r), r 为输入指数 n 的比特位数


// 习题 [1-16]
//2-Subset 子集问题
//有 n 个项的集合，子集共有 $2^n$ 个（考虑每个项包含和不包含在子集中的情况），遍历 0 ~ $2^n$，每个值转成二进制，依次判断：
int subset(int A[], int n, int m) {
    int end = (1<<n)-1; //2^n - 1
    for (int mask=0; mask<=end; mask ++) {
        int sum = 0;
        for(int i=0; i<n; i++) {
            if (mask & (1<<i)) {
                sum += A[i];
            }
        }
        if (sum == m)
            return mask; //mask 中的各个位数 1 代表所含的项
    }
    return 0;
}


int main() {
    cout << "power2_loop: 2^10=" << power2_loop(10) << endl;
    cout << "power_loop(3, 3)=" << power_loop(3, 3) << endl;

    int A[6] = {1, 2, 3, 4, 2, 6}; //sum=18=2X9
    cout << "subset(A, 6, 9)=" << subset(A, 6, 9) << endl;
    return 0;
}
