#include <iostream>
using namespace std;


int countOnes(unsigned int n) { // 统计整数 n 的二进制展开中数位 1 的总数： O(logn)
    int ones = 0;
    while (0 < n) { // 在 n 缩减至 0 之前，反复地
        ones += (1 & n); //检查最低位，若为 1 则计数
        n >>= 1;
    }
    return ones;
} // 等效于 glibc 的内置函数 int __builtin_popcount(unsigned int n)


/* 对于任意整数 n，不妨设其最低（右）为数位 1 对应 2^k，则最低的 k+1 位必然是 "100...0"，即
 * 数位 1 后是 k 个 0。
 * 而 n-1 的二进制展开后，其第 k+1 位是 0, 而低的 k 位都是 1，从而 n & (n-1) 后低 k+1 位都为 0.
 * 改进版本：操作次数线性正比于 n 的二进制展开中数位 1 的实际数目。
 */

int countOnes1(unsigned int n) { // O(ones) 正比于数位 1 的总数
    int ones = 0;
    while (n>0) {
        ones ++; //计数，至少有一位为 1
        n &= n-1;
    }
    return ones;
}

int main() {
    unsigned int n = 441; // 110111001
    cout << "countOnes(441): " << countOnes(114) << endl;
    cout << "countOnes1(441): " << countOnes1(114) << endl;

    return 0;
}
