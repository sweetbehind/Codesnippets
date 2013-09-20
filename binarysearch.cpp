/*
*  二分查找相关
*/
#include <iostream>

// 二分查找判断某个数是否存在，若存在返回所在位置，否则返回-1
int bsearch(int* arr, int lhs, int rhs, int value){
    while(lhs <= rhs){    // 闭区间[lhs,rhs]
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
// 求下界
int bs_lower_bound(int* arr, int low, int high, int value){
    // 数组最大的数小于value，下界不存在
	if (low > high || arr[high] < value) return -1;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if(arr[mid] >= value)    	// 闭区间表示求大于等于value的下界
// 开区间表示求大于value的下界
            high = mid - 1;
        else
            low = mid + 1;
    }
    return low;
}
// 求上界
int bs_upper_bound(int* arr, int low, int high, int value){
    // 数组最小的数大于value，上界不存在
	if (low > high || arr[low] > value) return -1;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if(arr[mid] <= value)	// 闭区间表示求小于等于value的上界
							// 开区间表示求小于value的上界
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}
