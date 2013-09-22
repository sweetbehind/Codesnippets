/*
*  单链表相关
*/
#include <iostream>

using namespace std;

struct node {
    node *next;
};

// 判断链表是否有环
bool checkloop(node* head){
    if(head==NULL)
      return false;
    node *slow=head, *fast=head->next;
    while(fast && fast->next){
        slow=slow->next;
        fast=fast->next->next;
        if(slow==fast) return true;
    }
    return false;
}

// 求链表长度
int getlistlength(node* pHead)
{
    int nLength = 0;
    while(pHead)
    {
        ++nLength;
        pHead = pHead->next;
    }
    return nLength;
}

// 求无环链表的第一个交点
node* firstnodewithoutloop(node* pHead1, node* pHead2)
{
    int nLength1 = getlistlength(pHead1);
    int nLength2 = getlistlength(pHead2);

    // 交换两链表，是1为较长的链表
    if(nLength1 < nLength2)
    {
        node* pTemp = pHead1;
        pHead1 = pHead2;
        pHead2 = pTemp;
    }
    for(int i = 0; i < nLength1 - nLength2; ++i)
    {
        pHead1 = pHead1->next;
    }
    while(pHead1 && pHead1 != pHead2)
    {
        pHead1 = pHead1->next;
        pHead2 = pHead2->next;
    }

    return pHead1;
}

// 求环的入口
node*  findloopport(node * head){
    node * slow  =  head,  * fast  =  head;
    while  ( fast  &&  fast -> next ) {
        slow  =  slow -> next;
        fast  =  fast -> next -> next;
        if  ( slow  ==  fast )  break ;
    }
    if  (fast  ==  NULL  ||  fast -> next  ==  NULL)
        return  NULL;
    slow  =  head;
    while  (slow  !=  fast){
         slow  =  slow -> next;
         fast  =  fast -> next;
    }
    return  slow;
}

// 有环链表的交点
node* firstnodewithloop(node *head1, node *head2)
{
    node* port1 = findloopport(head1);
    node* port2 = findloopport(head2);
    if (port1 == port2){ // 入口相同则交点在入口之前的部分
        node *port1tmp = port1->next;
        node *port2tmp = port2->next;
        port1->next = port2->next = NULL;
        node *pres = firstnodewithoutloop(head1,head2);
        port1->next = port1tmp;
        port1->next = port2tmp;
        return pres;
    } else { // 入口不同则选其中一个，若不相交则返回NULL
        node *pres = port1->next;
        while (pres != port1 && pres != port2) pres = pres->next;
        return pres == port2 ? pres : NULL;
    }
}
