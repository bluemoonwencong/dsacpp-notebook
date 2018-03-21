#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <math.h>
#include "./stack.cpp"

using namespace std;

//栈的应用
// 逆序输出 1. 进制转换
void convert(Stack<char>& s, int n, int base) { //十进制数 n 到 base 进制的转换
    static char digit[] //0<n, 1<base<=16, 新进制下的数位符号，可视base取值适当扩充
        = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    while (n>0) { //由低到高，逐一计算出新进制下的各数位
        int remainder = (int) n % base; //余数即为当前位
        s.push(digit[remainder]);
        n /= base;
    }
}//新进制下由高到低的各数位，自顶而下保存于栈中

void convert_number(int n, int base) {
    Stack<char> s = Stack<char>();

    convert(s, 12345, 8);
    cout << "12345(10) --> ";
    while( !s.empty() ){
        cout << s.pop();
    }
    cout << "(8)" << endl;
}

//栈的应用 2. 括号匹配
bool parentheses_match(char* &exp, int lo, int hi) { //表达式括号匹配检查，可兼顾三种括号
    Stack<char> s = Stack<char>(); //使用栈记录已发现但尚未匹配的括号
    for( int i=lo; i<hi; i++) {
        switch( exp[i] ){
            case '(': case '[': case '{':  //左括号直接进栈
                s.push(exp[i]); 
                break;
            case ')':  //右括号若与栈顶失配，则表达式必不匹配
                if (s.empty() || s.pop() != '(') {
                    return false;
                }
                break;
            case ']':
                if (s.empty() || s.pop() != '[') {
                    return false;
                }
                break;
            case '}':
                if (s.empty() || s.pop() != '{') {
                    return false;
                }
                break;
        default: //非括号字符一律忽略
                break;
        }
    }
    return s.empty();
}


//习题 4-6
// 输出一个浮点数并压入栈中
/*
float readNumber(char* &S, Stack<float> & opnd) {
    opnd.push(0); //初始化为 0
    char c = *S;
    bool is_decimal = false; //是否处理的是小数部分
    float decimal_base = 0.1;
    while ( *S ) {
        if (isdigit(*S)){
            float val = opnd.pop();
            int digit = int(*S)-0x30;

            if (!is_decimal) //处理非小数部分， x = x*10 + y
                opnd.push( val * 10 + digit );
            else { //处理小数部分，x=x+y*decimal_base
                opnd.push( val + digit*decimal_base );
                decimal_base *= 0.1; // 当有小数时，计算下一个数位的基数
            }
            S++;
        }
        else if ( *S == '.' ) {
            is_decimal = true;
            decimal_base = 0.1;
            S++;
        }
        else break;
    }
    return opnd.top();
}
*/
float readNumber(char* &p, Stack<float>& stk) {//将起始于 p 的子串解析为数值，并存入操作数栈
    stk.push( (float) (*p-'0') ); //当前数位对应的值进栈
    while( isdigit(*(++p)) ) //只要后续还有紧邻的数字（即多位整数的情况），则
        stk.push( stk.pop()*10 + (*p-'0') ); //弹出原操作数并追加新数位后，新数值重新入栈

    if ('.' != *p) return stk.top(); //此后非小数点，则意味着当前操作数解析完成
    float fraction = 1; //否则，意味着还有小数部分
    while (isdigit(*(++p)))
        stk.push( stk.pop() + (*p-'0')*(fraction/=10) ); //小数部分

    return stk.top();
}

float calcu(char op, float opnd) { //实施一元计算
    if (op == '!'){
        float ret = 1;
        for (int i=(int)opnd; i>1; i--)
            ret *= i;
        return ret;
    }
    return -1;
}

float calcu( float pOpnd1, char op, float pOpnd2 ) {//实施二元计算，结果入栈
    switch(op){
        case '+':
            return pOpnd1 + pOpnd2; break;
        case '-':
            return pOpnd1 - pOpnd2; break;
        case '*':
            return pOpnd1 * pOpnd2; break;
        case '/':
            return pOpnd1 / pOpnd2; break;
        case '^':
            return pow(pOpnd1, pOpnd2); break;
    }
    return -1;
}

void append( string &RPN, char op){ //读入操作数，并将其接至 PRN 末尾
    RPN = RPN + op;
}

void append( string &RPN, float opnd){ //读入操作数，并将其接至 PRN 末尾
    ostringstream ss;
    ss << opnd;
    RPN  = RPN + ss.str();
}

void append( string &RPN, double opnd){ //读入操作数，并将其接至 PRN 末尾
    ostringstream ss;
    ss << opnd;
    RPN  = RPN + ss.str();
}

//栈的应用 3. 中缀表达式求值
#define N_OPTR 9 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
//加，减，乘，除，乘方，阶乘，左括号，右括号，起始符与终止符

const char pri[N_OPTR][N_OPTR] = { //运算符优先等级[栈顶][当前]
    /*          |------------当前运算符-----------| */
    /*          +     -     *     /     ^     !     (     )     \0 */
    /* -- + */ '>',  '>',  '<',  '<',  '<',  '<',  '<',  '>',  '>',    
    /* |  - */ '>',  '>',  '<',  '<',  '<',  '<',  '<',  '>',  '>',    
    /* 栈 * */ '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',    
    /* 顶 / */ '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',    
    /* 运 ^ */ '>',  '>',  '>',  '>',  '>',  '<',  '<',  '>',  '>',    
    /* 算 ! */ '>',  '>',  '>',  '>',  '>',  '>',  ' ',  '>',  '>',    
    /* 符 ( */ '<',  '<',  '<',  '<',  '<',  '<',  '<',  '=',  ' ',    
    /* |  ) */ ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',    
    /* --\0 */ '<',  '<',  '<',  '<',  '<',  '<',  '<',  ' ',  '='    
};

int op2index(char op) {
    switch(op){
        case '+': return ADD; break;
        case '-': return SUB; break;
        case '*': return MUL; break;
        case '/': return DIV; break;
        case '^': return POW; break;
        case '!': return FAC; break;
        case '(': return L_P; break;
        case ')': return R_P; break;
        case '\0': return EOE; break;
        default: return -1; break;
    }
}

char orderBetween(char op1, char op2) { //视其与栈顶运算符之间优先级高低分别处理
    int index1 = op2index(op1);
    int index2 = op2index(op2);
    return pri[index1][index2];
}

float evaluate( char* S, string & RPN) { //对已剔除空白符的表达式求值，并转换为逆波兰式 RPN
    Stack<float> opnd; //运算数栈
    Stack<char> optr; //运算符栈

    // 改进1： assert parentheses_match()
    optr.push('\0'); //尾哨兵 '\0' 也作为头哨兵首先入栈

    while (!optr.empty()) { //在运算符栈非空之前，逐个处理表达式中各字符
        if ( isdigit(*S) ) { //若当前字符为操作数，则
            readNumber(S, opnd);
            append( RPN, opnd.top() ); //读入操作数，并将其接至 PRN 末尾
        }
        else {//若当前字符为运算符
            switch( orderBetween(optr.top(), *S) ) { //视其与栈顶运算符之间优先级高低分别处理


                // 改进2： 在执行操作符入栈时，确保操作数栈的规模就比操作符栈的恰好大一
                case '<': //栈顶运算符低优先级时
                    optr.push( *S ); S++; //计算推迟，当前运算符进栈
                    break;
                case '=': //优先级相等，（当前运算符为右括号或尾部哨兵\0)时
                    optr.pop(); S++; //脱括号并接收下一个字符
                    break;
                case '>':{ //栈顶运算符高优先级时，可实施相应的计算，并将结果重新入栈
                    // 改进3： 在执行运算时确保 opnd 中有足够的操作数
                    char op = optr.pop(); append( RPN, op ); //栈顶运算符出栈并续接至 RPN 末尾
                    if ( '!' == op ) { //! 为一元操作符
                        float pOpnd = opnd.pop();
                        opnd.push( calcu( op, pOpnd ) ); //实施一元计算，结果入栈
                    } else { //其它都为二元操作符
                        float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();
                        opnd.push( calcu( pOpnd1, op, pOpnd2 ) ); //实施二元计算，结果入栈
                    }
                    break;
                }
                default:
                    exit(-1); //逢语法错误，不做处理直接退出
                    break;
            } //switch
        }
    }//while

    return opnd.pop(); //弹出并返回最后的计算结果
}


//国际象棋中皇后的热力范围覆盖其所在的水平线、垂直线及两条对角线，考查在 nXn 的棋盘上放置 n 个皇后，如何使得她们互不攻击。
//由鸽巢原理知，n 行 n 列棋盘上至多只能放 n 个皇后，反之，n 个皇后在 nXn 棋盘上的可行棋局通常也存在。
struct Queen { //皇后类
    int x, y; //皇后在棋盘上的位置坐标
    Queen(int xx=0, int yy=0): x(xx), y(yy) {};
    bool operator == (Queen const& q) const { //重载判等操作符，以检测不同皇后之间可能的冲突
        return (x == q.x) // 行冲突，这一情况其实并不会发生，可省略
            || (y == q.y)
            || (x+y == q.x+q.y) //右上、左下对角线冲突
            || (x-y == q.x-q.y); // 左上、右下对象线冲突
    }

    bool operator != (Queen const& q) const { return ! (*this == q); }

};

int nSolu = 0; // 保存 N 皇后问题的解的个数
int nCheck = 0; //保存 N 皇后问题求解过程中的尝试次数

//迭代版本中用栈保存位置。
//开始为空棋盘，并从原点位置出发开始尝试
void placeQueens( int N ){ //N 皇后算法迭代版：采用试探/回溯策略，借助栈记录查找的结果
    Stack<Queen> solu; //存放（部分）解的栈
    Queen q(0, 0); //从原点位置出发

    do { //反复试探与回溯
        if ( N <= solu.size() || N <= q.y ) { //若已出界，则
            q = solu.pop(); //回溯一行，并接着试探该行中的下一列
            q.y++; 
        } 
        else { //否则，试探下一行
            while ( (q.y < N) && ( 0 <= solu.find(q)) ) { //通过与已有皇后的对比
                q.y++; //尝试找到可摆放下一皇后的列
                nCheck++;
            }

            if (q.y < N) { //若找到可摆放的列，则
                solu.push(q); //摆上当前皇后，并
                if (N <= solu.size() ) {  //若部分解已成为全局解，则通过全局变量 nSolu 计数
                    nSolu++;
                }

                q.x++; //转入下一行，从第 0 列开始，试探下一皇后
                q.y = 0;
            }
        }
    } while( ( 0<q.x ) || (q.y < N) );
}


// 迷宫寻径
// nXn 个方格组成的迷宫，除了四周的围墙，还有分布期间的若干障碍物，只能水平或垂直移动。寻径的任务是：在任意指定的起始格点与目标格点之间，找出一条通路（如果的确存在）。

//迷宫格点 Cell
typedef enum { AVAILABLE, ROUTE, BACKTRACED, WALL } Status; //迷宫单元格点状态
// 共 4 种状态： 原始可用，在当前路径上的，所有方向均尝试失败后回溯过的，不可使用的（墙）
// 属于当前路径的格点，还需记录其前驱和后继格点的方向。

typedef enum { UNKNOWN, EAST, SOUTH, WEST, NORTH, NO_WAY } ESWN;  //单元格点的相对邻接方向
// 未定，东，南，西，北，无路可通
// 既然只有上下左右四个连接方向，故 E S W N 可区别。特别地，因尚未搜索到而仍处理初始 AVAILABLE
// 状态的格点，邻格的方向都是 UNKNOWN，经过回溯后处于 BACKTRACED 状态的格点，与邻格间的连接关系
// 均关闭，故标记为 NO_WAY

inline ESWN nextESWN( ESWN eswn ) { return ESWN(eswn+1); } //依次转至下一邻接方向

struct Cell { //迷宫格点
    int x, y; Status status; // 坐标; 状态/类型
    ESWN incoming, outgoing; //进入，走出方向，即其前驱和后续格点的方位
};


#define LABY_MAX 13 //最大迷宫尺寸
Cell laby[LABY_MAX][LABY_MAX]; //迷宫，是一个二维数组

inline Cell* neighbor (Cell* cell) { //查询当前格点的后继格点
    switch( cell->outgoing ){
        case EAST: return cell + 1;         // ------->
        case WEST: return cell - 1;         // <-------
        case SOUTH: return cell + LABY_MAX; //       |
                                            //       V
        case NORTH: return cell - LABY_MAX; //   ^
                                            //   |
        default: exit(-1);
    }
}

inline Cell* advance ( Cell* cell ) { //从当前格点转入相邻格点，并设置前驱格点的方向
    Cell* next;
    switch( cell->outgoing ) {
        case EAST:
            next = cell + 1;
            next->incoming = WEST; break;
        case WEST:
            next =  cell - 1;
            next->incoming = EAST; break;
        case SOUTH: 
            next = cell + LABY_MAX;
            next->incoming = NORTH; break;
        case NORTH:
            next = cell - LABY_MAX;
            next->incoming = SOUTH; break;
        default: exit(-1);
    }
    return next;
}

//实现：借助栈按次序记录组成当前路径的所有格点，
//并动态地随着试探、回溯做入栈、出栈操作。
//路径的起始格点、当前的末端格点分别对应于路径中的
//栈底和栈项，当后者抵达目标格点时探索成功。
// 迷宫寻径算法：在格点 s 至 t 之间规划一条通路(如果的确存在）
bool labyrinth( Cell Laby[LABY_MAX][LABY_MAX], Cell* s, Cell* t, Stack<Cell*> &path) {
    if ( (AVAILABLE != s->status ) || (AVAILABLE != t->status) )
        return false; //退化情况

    //Stack<Cell*> path; //用栈记录通路

    s->incoming = UNKNOWN; //起点
    s->status = ROUTE;
    path.push(s);

    do { //从起点出发不断试探、回溯，直到抵达终点、或穷尽所有可能
        Cell* c = path.top(); //检查当前位置（栈顶）
        if (c == t) //若已抵达终点，则找到了一条通路，否则沿尚未试探的方向继续试探
            return true;

        while ( (c->outgoing = nextESWN(c->outgoing)) < NO_WAY ) //逐一检查所有方向
            if (AVAILABLE == neighbor(c)->status) //直到找到一个未尝试过的方向
                break;

        if ( NO_WAY <= c->outgoing ) { //若所有方向都已尝试过
            c->status = BACKTRACED; //则标记并且回溯
            c = path.pop();
        }
        else { //还有若尝试的，则向前试探一步
            path.push( c=advance(c) );
            c->outgoing = UNKNOWN;
            c->status = ROUTE;
        }
    } while (!path.empty());

    return false;
}


//习题 4-3
//甄别栈混洗：对于 {1,2,3, ..., n} 的任一排序，判定其是否为栈混洗。
// A[n] = {1,2,3,...,n}
bool check_stack_permutation(int B[], int n) {
    Stack<int> S = Stack<int>(); //辅助中转栈
    int i = 1; //模拟输入栈 A （的栈顶元素）
    for (int k=1; k<=n; k++) { //通过迭代，依次输出每一项 B[k]
        while (S.empty() || B[k] != S.top() ) { //只要 B[k] 仍未出现在 S 栈顶
            S.push(i); //就反复地从栈 A 中取出顶元素，并随即压入栈 S
            //只要 B[] 不含任何禁形，则以上迭代就不可能导致栈 A 溢出
            //以上迭代退出时，S 栈必然非空，且 S 的栈顶元素就是 B[k]
            if (i>n)
                return false;
            i++;
        }
        S.pop(); //因此，至此只需要弹出 S 的栈顶元素，即为希望输出的 B[k]
    }

    return S.empty();
}

//习题 4-18
//Fermat-Lagrange 定理：任何一个自然数都可以表示为 4 个整数的平方和，
//如 30 = 1^2 + 2^2 + 3^2 + 4^2
//试采用试探回溯策略解
//分解的每个自然数都 <= sqrt(n) = N
// 类似 N 皇后问题，其中行数为 4 行，列数为 N 行（即每个自然数的取值）。
int nFLSolu = 0; // 保存解的个数
int nFLCheck = 0; //保存求解过程中的尝试次数

void fermat_lagrange( int n, int counts[] ){ //n 分解
    Stack<int> solu; //存放（部分）解的栈
    int q = 0; //从第一个自然数开始，相当于第一行
    int N = (int)sqrt(n); //列数
    int stack_sum = 0; //栈中所有元素的平方和

    do { //反复试探与回溯
        if ( 4 <= solu.size() || N < q ) { //若已出界，则
            q = solu.pop(); //回溯一行，并接着试探该行中的下一列
            stack_sum -= q*q;
            q++; 
        } 
        else { //否则，试探下一行
            if (q <=N && stack_sum + q*q <=n) {
                solu.push(q);
                stack_sum += q*q;

                if (4 == solu.size()){
                    counts[stack_sum] += 1; //统计不超过 n 的每一自然数的分解数

                    if (stack_sum == n){ //局部解是全局解时
                        nSolu ++;
                        solu.report("out");
                    }
                }
                //q = 0; //下一行开头
                //q = q; // 下一行值 >= q，从而能排除同一组数的不同排列

            }
            else { //q 值及以上的都不符合
                q = N+1; //使 q 越界
            }

        }
    } while( ( 0 < solu.size() ) || ( q <= N) );
}

int main() {
    Stack<int> s = Stack<int>();

    s.report("Init"); 
    cout << "Is empty = " << s.empty() << endl;

    s.push(5);
    s.report("push(5)");  //5
    s.push(3); 
    s.report("push(3)"); //5,3
    cout << "pop()=" << s.pop() << endl;
    cout << "top=" << s.top() << endl;
    s.report("Now"); 
    cout << "size()=" << s.size() << endl;

    //栈的应用
    convert_number(12345, 8); //30071

    char *parentheses_str1 = "a / ( b [ i - 1 ] [ j + 1 ] + c [ i + 1 ] [ j - 1 ] ) * 2";
    char *parentheses_str2 = "a / ( b [ i - 1 ] [ j + 1 ] ) + c [ i + 1 ] [ j - 1 ] ) * 2";
    cout << parentheses_str1 << " is match: " << parentheses_match(parentheses_str1, 0, 56) << endl;
    cout << parentheses_str2 << " is match: " << parentheses_match(parentheses_str2, 0, 58) << endl;

    cout << "test readNumber():" << endl;
    char* digits = "123,456.789";
    cout << "str=" << digits << endl;
    Stack<float> temp_stack = Stack<float>();
    float out =  readNumber(digits, temp_stack);
    cout << "first readNumber() = " << out << endl;
    digits ++;
    cout << "str=" << digits << endl;
    out =  readNumber(digits, temp_stack);
    cout << "next readNumber() = " << out << endl;

    cout << "test append():" << endl;
    string RPN = "";
    append(RPN, '!');
    cout << "append(RPN, '!')=" << RPN << endl;
    append(RPN, 34.56);
    cout << "append(RPN, 34.56)=" << RPN << endl;

    cout << "test calcu():" << endl;
    cout << "calcu('!', 5) = " << calcu('!', 5) << endl;//实施一元计算, 120
    cout << "calcu(2, '+', 3) = " << calcu(2, '+', 3) << endl;
    cout << "calcu(2, '-', 3) = " << calcu(2, '-', 3) << endl;
    cout << "calcu(2, '*', 3) = " << calcu(2, '*', 3) << endl;
    cout << "calcu(2, '/', 3) = " << calcu(2, '/', 3) << endl;
    cout << "calcu(2, '^', 3) = " << calcu(2, '^', 3) << endl;

    cout << "test evaluate:" << endl;

    char* exp = "(1+2)*3^4";
    RPN = "";
    cout << exp << "=" << evaluate(exp, RPN) << endl; //(1+2)*3^4=243
    cout << "RPN is:" << RPN << endl; //RPN is:12+34^*

    //习题 4-12
    //非正常表达式 (12)3+!4*+5 作为输入，也有返回 89
    exp = "(12)3+!4*+5";
    RPN = "";
    cout << exp << "=" << evaluate(exp, RPN) << endl; 
    cout << "RPN is:" << RPN << endl;

    placeQueens(8);
    cout << "8 Queens solutions = " << nSolu << endl; // 92
    cout << "8 Queens solution checks = " << nCheck << endl; //13664

    cout << "test labyrinth:" << endl; //见P104 的 13X13 实例
    for (int i=0; i<LABY_MAX; i++) {
        for ( int j=0; j<LABY_MAX; j++) {
            laby[i][j].x = i;
            laby[i][j].y = j;
            laby[i][j].status = AVAILABLE;
            laby[i][j].incoming = UNKNOWN;
            laby[i][j].outgoing = UNKNOWN;
        }
    }

    for (int i=0; i<LABY_MAX; i++) {
        laby[0][i].status = WALL; //第一行
        laby[LABY_MAX-1][i].status = WALL; //最后一行

        laby[i][0].status = WALL; //第一列
        laby[i][LABY_MAX-1].status = WALL; //最后一列
    }

    laby[1][2].status = WALL;
    laby[1][3].status = WALL;
    laby[1][6].status = WALL;
    laby[2][1].status = WALL;
    laby[2][3].status = WALL;
    laby[2][4].status = WALL;
    laby[2][7].status = WALL;
    laby[2][9].status = WALL;
    laby[3][5].status = WALL;
    laby[3][6].status = WALL;
    laby[3][8].status = WALL;
    laby[4][5].status = WALL;
    laby[5][1].status = WALL;
    laby[5][5].status = WALL;
    laby[6][5].status = WALL;
    laby[7][2].status = WALL;
    laby[7][3].status = WALL;
    laby[7][6].status = WALL;
    laby[7][9].status = WALL;
    laby[8][1].status = WALL;
    laby[8][6].status = WALL;
    laby[9][3].status = WALL;
    laby[9][6].status = WALL;
    laby[9][7].status = WALL;
    laby[9][9].status = WALL;
    laby[10][10].status = WALL;
    laby[11][8].status = WALL;
    laby[11][10].status = WALL;
    laby[11][11].status = WALL;

    Cell* ss = &laby[4][9];
    Cell* tt = &laby[4][1];
    Stack<Cell*> path = Stack<Cell*>();

    cout << "has path =" <<  labyrinth( laby, ss, tt, path) << endl;
    while (!path.empty()) {
        Cell* c = path.pop();
        cout << "(" << c->x << "," << c->y << ") <-- ";
    }
    cout << endl;

    //习题 4-3
    //A[] = {1,2,3,4}
    int B1[] = {-99, 1, 4, 3, 2};

    int B2[] = {-99, 4, 3, 1, 2};
    cout << "check_stack_permutation([1,4,3,2], 4): " << check_stack_permutation(B1, 4) << endl;
    cout << "check_stack_permutation([4,3,1,2], 4): " << check_stack_permutation(B2, 4) << endl;


    //习题 4-18
    int counts[101] = { 0 };
    cout << "fermat_lagrange(100):" << endl;
    fermat_lagrange( 100, counts );
    for (int i=0; i<=100; i++){
        cout << "fermat_lagrange " << i << " counts: " << counts[i] << endl;
    }
    cout << endl;
}

