/*
*  首尾相连数组的最大子数组和
*/
#include <iostream>

const int inf = ~0u>>1;

int getloopmaxsum(int a[], int n){
    // 先找到数组中的最小子数组结尾下标
    int mmin = inf;
    int idx = -1, cur = 0;
    for(int i=0; i<n; i++){
        if (cur > 0) cur = a[i];
        else cur += a[i];
        if (cur < mmin){
            mmin = cur;
            idx = i;
        }
    }
    // 从最小子数组结尾下标下一个开始，循环扫描数组查找最大
    cur = 0;
    int mmax = -inf;
    for(int i=(idx+1)%n;;i=(i+1)%n){
        if (cur < 0) cur = a[i];
        else cur += a[i];
        if(cur > mmax) mmax = cur;
        if (i == idx) break;
    }
    return mmax;
}
