#include <stdbool.h>
#include <stdio.h>


int countOnes(unsigned int n) { // 统计整数 n 的二进制展开中数位 1 的总数： O(logn)
    int ones = 0;
    while (0 < n) { // 在 n 缩减至 0 之前，反复地
        ones += (1 & n); //检查最低位，若为 1 则计数
        n >>= 1;
    }
    return ones;
} // 等效于 glibc 的内置函数 int __builtin_popcount(unsigned int n)

int main() {
    unsigned int n = 441; // 110111001
    printf("Ones of 441: %d\n", countOnes(n));
}
