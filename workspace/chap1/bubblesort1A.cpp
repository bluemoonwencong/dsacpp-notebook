#include <stdbool.h>
#include <stdio.h>


void bubblesort1A(int A[], int n) { // bubble sort (ver 1A): 0 <= n
    bool sorted = false; // totally sorted flag
    while (!sorted) { // scan & swap while not totally sorted
        sorted = true; // assume already sorted
        for (int i=1; i<n; i++) { // check neighbors
            if (A[i-1] > A[i]) {
                //swap(A[i-1], A[i]);
                int t = A[i-1];
                A[i-1] = A[i];
                A[i] = t;
                sorted = false; // here we can't sure totally sorted, so clear the flag
            }
        }
        n--; // after loop, the end elem is in order.
    }
} // 借助布尔型标志位 sorted, 可及时提前退出，而不致总是蛮力地做 n-1 趟扫描交换。

int main() {
    int A[] = { 5, 2, 7, 4, 6, 3, 1 };
    int n = 7;
    printf("Original:\n");
    for (int i=0; i<n; i++) {
        printf("%d, ", A[i]);
    }

    printf("\nSorted:\n");
    bubblesort1A(A, n);
    for (int i=0; i<n; i++) {
        printf("%d, ",  A[i]);
    }
    printf("\n");
}
