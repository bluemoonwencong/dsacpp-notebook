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

// 习题 [1-22]
class Fib { // Fibonacci 数列类
    private:
        int f, g; // fib(k-1), fib(k), int 型很快会溢出
    public:
        Fib(int n) { //初始化为不小于 n 的最小 Fibonacci 项， 如 Fib(6)=8, O(logn)
            f = 0; g = 1; // fib(0), fib(1)
            while (g < n) {
                next();
            }
        }

        int get() { //获取当前 Fibonacci 项，如 Fib(8).get() = 8, O(1)
            return g;
        }

        int next() { //转到下一项，如 Fib(8).next() = 13, O(1)
            g += f;
            f = g - f;
            return g;
        }

        int prev() { //转到上一项，如 Fib(8).prev() = 5, O(1)
            f = g - f;
            g -= f;
            return g;
        }
};


//习题 [1-23] 汉若塔
// 将 n 个盘从 src 柱借助 tmp 柱，移到 dest 柱, O(2^n)
void Hanoi(char src, char dest, char tmp, int n) {
    if ( n > 0) {
        Hanoi(src, tmp, dest, n-1); //将 n-1 个盘从 src 柱借助 dest，先移到 tmp
        cout << src << "--->" << n << ">---" << dest << endl; //将第 n 个盘从 src -> dest
        Hanoi(tmp, dest, src, n-1); //将 n-1 个盘从 tmp 柱借助 src 移到 dest
    }
}

//习题 [1-24] n 阶棋盘用 L 型积木覆盖问题
// 参数：
//     n: 棋盘阶数, 共有 4^n 个格子 
//     x, y: 原点，正常 XY 坐标，X 向右增加，Y 向上增加
//     dx, dy: 表示缺口方向，如 (+1, +1) 表示右上，(-1, -1) 表示左下
// 入口函数为 chess_board_cover(n, 0, 0, 1, 1): 表示在坐标原点开始，缺口在右上
void chess_board_cover(int n, int x, int y, int dx, int dy) {
    int sub_count = 1 << (n-1); // 4 个子盘的长度

    //先将 L 放在中间点位置，方向与当前处理棋盘的缺口方向相同
    //place(x+dx*(sub_count-1), y+dy*(sub_count-1), dx, dy);
    cout << "place at (" << x+dx*(sub_count-1) << ", " <<  y+dy*(sub_count-1) << "),  direction (" << dx << ", " << dy << ")" << endl;

    /*
     *           |
     *       2   |  3
     *    -------|----------
     *       1   |  4
     *           |
     */
    if (n > 1) {
        chess_board_cover(n-1, x, y, dx, dy); // 覆盖第 1 个子盘
        chess_board_cover(n-1, x, y + dy*(2*sub_count-1), dx, -dy); // 覆盖第 2 个子盘
        chess_board_cover(n-1, x+dx*sub_count, y+dy*sub_count, dx, dy); // 覆盖第 3 个子盘
        chess_board_cover(n-1, x + dx *(2*sub_count-1), y, -dx, dy); //覆盖第 4 个子盘
    }
}

//习题 [1-25]
// 正整数 a, b 的 最大公约数
// 1. 欧几里德的辗转相除法：
//   举例：求 (319, 377) 的最大公约数：
//   先比较，大的放前面 (319,377) -> (377, 319)
//   当除不尽时，辗转相除，更新了 (除数, 余数）: 
//      (377, 319)->(319,58)->(58,29)
//   当能除尽时，返回的除数就是最大公约数： (58,29)->29
//
// 参数是否定义为 tint64?
tint64 greatest_common_divisor_euro(tint64 a, tint64 b){
    tint64 tmp;
    if (a < b){ // a b 值调换
        tmp = a;
        a = b;
        b = tmp;
    }

    while(a % b) { //除不尽时
        tmp = a % b;
        a = b;
        b = tmp;
    }

    return b;
}
// 2. 中华更相减损法：
//   step1: 给定任意 2 正整数，判断它们是否都是偶数，是则用 2 约简，不是则执行 step2
//   step2: 以较大数减较小数，得差，差为偶数时不断用 2 约简直到奇数; 将较小数与差进行辗转相减，直到较小数与差相等时，此时的较小数（或差）与之前约简了的 2 乘积回来就得结果。
// 举例： 求 （98, 63) 的最大公约数
//   由于不同时为偶数，将大数减去小数得到差，将较小数和差组织为一个组合: 
//      98-63=35, (98,63)->(63, 35)
//      进行辗转相减： (63,35)->(35,28)->(28,7)->(21,7)->(14,7)
//      此时较小数和差相同，则结果为 7
//
// 举例：求 (260, 104) 结果：
//   将偶数简约： (260,104)->(130,52)->(65,26), 简约了 2 次
//   65-26=39 (65,26)->(39,26)->(26,13), 结果为 13*4=52
//
tint64 abs(tint64 a, tint64 b){
    if (a >=b)
        return a - b;
    else
        return b - a;
}

// 根据习题 [1-25] 中的算法写为 O(log(a+b))
tint64 greatest_common_divisor_china(tint64 a, tint64 b){
    tint64 p = 1;
    while ( (a&0x1) == 0 && (b&0x1) == 0) { // a, b 都为偶数
        p <= 1;
        a >= 1;
        b >= 1;
    }

    while (1) {
        tint64 t = abs(a, b);
        if  ( t == 0 ) {
            return a * p;
        }

        while ( (t&0x1) == 0 ) // t  为偶数
            t >>= 1; // t=t/2

        //此时 t 为奇数
        if (a >= b)
            a = t;
        else
            b = t;
    }

}

//习题 [1-26]
//整体循环左移 k 位，shift(int A[], int n, int k), O(n)
//例如： A[]={1,2,3,4,5,6}, shift(A, 6, 2) => {3,4,5,6,1,2}
//  利用 reverse() 算法，
//  假设原数组的前缀 A[0,k) 为 L, 后缀 A[k,n) 为 R，
//  则原数组为 L+R, 左移 k 位即为 R+L=(L'+R')'，' 为倒置操作，即 reverse 操作。
//  该算法使用 reverse() 进行互换操作，数据在物理上都是连续分布的，实际运行时会激活缓存机制，效率很高。 O(n)
void reverse(int * A, int lo, int hi){
    if(lo < hi){
        swap(A[lo], A[hi]);
        reverse(A, lo+1, hi-1);
    }
}
void reverse(int * A, int len){
    return reverse(A, 0, len-1);
}
void shift(int A[], int n, int k){
    k %= n; 
    reverse(A, k);
    reverse(A+k, n-k);
    reverse(A, n);
}

//习题 [1-27]
int ackermann(int m, int n) {
    if (m == 0)
        return n+1;
    if (m>0 && n == 0)
        return ackermann(m-1, 1);
    if (m>0 && n>0)
        return ackermann(m-1, ackermann(m, n-1));
}


// 习题 [1-29] Hailstone 的长度，非递归
int hailstone( int n) {
    int len = 1;
    while( n != 1) {
        len += 1;
        if (n & 1) //n 为奇数 , if else 可用 x ? y : z 代替
            n = 3*n + 1;
        else
            n >>= 1; // n = n/2
    }
    return len;
}

int main() {
    cout << "power2_loop: 2^10=" << power2_loop(10) << endl;
    cout << "power_loop(3, 3)=" << power_loop(3, 3) << endl;

    int A[6] = {1, 2, 3, 4, 2, 6}; //sum=18=2X9
    cout << "subset(A, 6, 9)=" << subset(A, 6, 9) << endl;

    cout << "class Fibonacci test:" << endl;
    cout << "Fib(1).get()=" << Fib(1).get() << endl;
    cout << "Fib(8).next()=" << Fib(8).next() << endl; // 13
    cout << "Fib(8).prev()=" << Fib(8).prev() << endl;  // 5

    cout << "Hanoi(3):" << endl;
    Hanoi('X', 'Y', 'Z', 3);

    cout << "chess_board_cover(3, 0, 0, 1, 1):" << endl;
    chess_board_cover(3, 0, 0, 1, 1);

    cout << "greatest_common_divisor_euro(319, 377) = " << greatest_common_divisor_euro(319, 377) << endl;
    cout << "greatest_common_divisor_china(319, 377) = " << greatest_common_divisor_china(319, 377) << endl;

    int AA[6] = { 1, 2, 3, 4, 5, 6 };
    cout << "Original AA[6]:";
    for(int i=0; i<6; i++) cout << AA[i] << " ";
    cout << "\nafter shift(AA, 6, 2):";
    shift(AA, 6, 2);
    for(int i=0; i<6; i++) cout << AA[i] << " ";
    cout << endl;

    cout << "ackermann(2, 3)=" << ackermann(2,3) << endl;


    //Hailstone(7) = { 7, 22, 11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1 }
    cout << "hailstone(7)=" << hailstone(7) << endl; //17

    return 0;
}
