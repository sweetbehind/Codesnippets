/*
*  Ñ°ÕÒ³óÊı
*  http://www.cnblogs.com/coser/archive/2011/03/07/1976525.html
*/
#include <iostream>

using namespace std;

int min3(int a, int b, int c){
    return min(min(a,b),c);
}

int uglynumber(int k)
{
    int index=1;
    int *arr= new int[k];
    arr[0]=1;
    int index2=0, index3=0, index5=0;
    while(index<k)
    {
        int m=min3(arr[index2]*2,arr[index3]*3,arr[index5]*5);
        arr[index]=m;
        while(arr[index2]*2<=arr[index]) index2++;
        while(arr[index3]*3<=arr[index]) index3++;
        while(arr[index5]*5<=arr[index]) index5++;
        index++;
    }
    //for(int i=0;i<k;i++) cout<<arr[i]<<" ";cout<<endl;
    int ans=arr[k-1];
    delete[] arr;
    return ans;
}

