/*
*  ���ֲ������
*/
#include <iostream>

// ���ֲ����ж�ĳ�����Ƿ���ڣ������ڷ�������λ�ã����򷵻�-1
int bsearch(int* arr, int lhs, int rhs, int value){
    while(lhs <= rhs){    // ������[lhs,rhs]
        int mid = lhs + (rhs - lhs) / 2;
        if(arr[mid] == value)
            return mid;
        else if(arr[mid] > value)
            rhs = mid - 1;
        else
            lhs = mid + 1;
    }
    return -1;
}
// ���½�
int bs_lower_bound(int* arr, int low, int high, int value){
    // ����������С��value���½粻����
	if (low > high || arr[high] < value) return -1;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if(arr[mid] >= value)    	// �������ʾ����ڵ���value���½�
// �������ʾ�����value���½�
            high = mid - 1;
        else
            low = mid + 1;
    }
    return low;
}
// ���Ͻ�
int bs_upper_bound(int* arr, int low, int high, int value){
    // ������С��������value���Ͻ粻����
	if (low > high || arr[low] > value) return -1;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if(arr[mid] <= value)	// �������ʾ��С�ڵ���value���Ͻ�
							// �������ʾ��С��value���Ͻ�
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}
