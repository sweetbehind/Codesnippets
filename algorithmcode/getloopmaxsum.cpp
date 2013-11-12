/*
*  ��β�������������������
*/
#include <iostream>

const int inf = ~0u>>1;

int getloopmaxsum(int a[], int n){
    // ���ҵ������е���С�������β�±�
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
    // ����С�������β�±���һ����ʼ��ѭ��ɨ������������
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
