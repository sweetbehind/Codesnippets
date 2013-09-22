/*
*  ���ֲ������
*/
#include <iostream>

using namespace std;

struct node {
    node *next;
};

// �ж������Ƿ��л�
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

// ��������
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

// ���޻�����ĵ�һ������
node* firstnodewithoutloop(node* pHead1, node* pHead2)
{
    int nLength1 = getlistlength(pHead1);
    int nLength2 = getlistlength(pHead2);

    // ������������1Ϊ�ϳ�������
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

// �󻷵����
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

// �л�����Ľ���
node* firstnodewithloop(node *head1, node *head2)
{
    node* port1 = findloopport(head1);
    node* port2 = findloopport(head2);
    if (port1 == port2){ // �����ͬ�򽻵������֮ǰ�Ĳ���
        node *port1tmp = port1->next;
        node *port2tmp = port2->next;
        port1->next = port2->next = NULL;
        node *pres = firstnodewithoutloop(head1,head2);
        port1->next = port1tmp;
        port1->next = port2tmp;
        return pres;
    } else { // ��ڲ�ͬ��ѡ����һ���������ཻ�򷵻�NULL
        node *pres = port1->next;
        while (pres != port1 && pres != port2) pres = pres->next;
        return pres == port2 ? pres : NULL;
    }
}
