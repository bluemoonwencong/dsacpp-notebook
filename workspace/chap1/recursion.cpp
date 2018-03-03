#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


// 数组求和算法，线性递归版
int sum_linear_recursion(int A[], int n) {
    if (n < 1) // 平凡情况，递归基
        return 0;
    else //一般情况
        return sum_linear_recursion(A, n-1) + A[n-1]; //递归：前 n-1 项之和，再累计第 n-1 项
} //O(1)*递归深度 = O(1)*(n+1) = O(n)


// 数组求和算法，二分递归版本，入口为 sum_binary_recursion(A, 0, n-1)
int sum_binary_recursion(int A[], int lo, int hi) {
    if (lo == hi) //如遇递归基（区间长度已降至 1)，则
        return A[lo]; //直接返回该元素
    else { // 否则是一般情况 lo < hi, 则
        int mi = (lo+hi) >> 1; //以居中单元为界，将原区间一分为二
        return sum_binary_recursion(A, lo, mi) + sum_binary_recursion(A, mi+1, hi); //递归对各子数组求和，然后合计
    }
} //O(hi-lo+1)，线性正比于区间的长度


// 数组倒置，多递归基递归版本（尾递归)
void reverse_mult_base_case(int* A, int lo, int hi) {
    if (lo < hi) {
        //swap(A[lo], A[hi]);
        int tmp = A[lo];
        A[lo] = A[hi];
        A[hi] = tmp;

        reverse_mult_base_case(A, lo+1, hi-1); //递归倒置 A(lo, hi)
    } // else 隐含了两种递归基, lo==hi, lo>hi
} // O(hi-lo+1)


// 数组倒置，将尾递归优化为迭代版本
void reverse_loop_version(int* A, int lo, int hi){
    while (lo < hi) {
        //swap(A[lo++], A[hi--]); //交换 A[lo], A[hi], 收缩待倒置区间
        int tmp = A[lo];
        A[lo] = A[hi];
        A[hi] = tmp;
        lo++;
        hi--;
    }
} // O(hi-lo+1)


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


// 计算 Fibonacci 数列的第 n 项，二分递归版本，O(2^n)
tint64 fibonacci_binary_recursion(int n) {
    return (n<2) ? 
        (tint64) n // 若到达递归基，直接取值
        : fibonacci_binary_recursion(n-1) + fibonacci_binary_recursion(n-2);
}

// Fibonacci 线性递归版本，入口形式 fibonacci_linear_recursion(n, prev), O(n)
// 使用临时变量，避免重复递归计算
tint64 fibonacci_linear_recursion(int n, tint64& prev) {
    if (n == 0){ //若到达递归基，则
        prev = 1;
        return 0; // 直接取值： fib(-1) = 1, fib(0)=0
    } else {
        tint64 prevPrev; prev = fibonacci_linear_recursion(n-1, prevPrev); //递归计算前两项
        return prevPrev + prev; //其和即为正解
    }
} // 用辅助变量记录前一项，返回数列的当前项, O(n)

// Fibonacci 迭代版本： O(n)
// 采用动态规划策略，按规模自小而大求解各子问题
tint64 fibonacci_loop_version(int n) {
    tint64 f = 0, g = 1; // 初始化 fib(0)=0, fib(1)=1
    while (0 < n--) {
        g += f; f = g-f; //依原始定义，通过 n 次加法和减法计算 fib(n)
    }
    return f;
}

int main() {
    int A[] = { 1, 2, 3, 4, 5 };
    int n = 5;

    printf("Array Sum(Linear recursion): %d\n", sum_linear_recursion(A, n));
    printf("Array Sum(Binary recursion): %d\n", sum_binary_recursion(A, 0, n-1));

    int B[] = { 1, 2, 3, 4, 5 };
    printf("Original Array:");
    for (int i=0; i<n; i++) {
        printf("%d, ", B[i]);
    }
    printf("\nReversed Array(reverse_mult_base_case):");
    reverse_mult_base_case(B, 0, 4);
    for (int i=0; i<n; i++) {
        printf("%d, ", B[i]);
    }
    printf("\n");

    printf("Original Array:");
    for (int i=0; i<n; i++) {
        printf("%d, ", B[i]);
    }
    printf("\nReversed Array(reverse_loop_version):");
    reverse_loop_version(B, 0, 4);
    for (int i=0; i<n; i++) {
        printf("%d, ", B[i]);
    }
    printf("\n");

    printf("power2: 2^10=%lld\n", power2(10));
    printf("power2_loop: 2^10=%lld\n", power2_loop(10));
    printf("power_loop(3, 3): 3^3=%lld\n", power_loop(3, 3));

    printf("fibonacci_binary_recursion(10):%lld\n", fibonacci_binary_recursion(10));
    tint64 pre;
    printf("fibonacci_linear_recursion(10, pre):%lld\n", fibonacci_linear_recursion(10, pre));
    printf("fibonacci_loop_version(10):%lld\n", fibonacci_loop_version(10));
}
