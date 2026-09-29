
/*You are given the heads of two sorted linked lists list1 and list2. 
Merge the two lists into one sorted list. The list should be made by splicing together the nodes of the first two lists. 
Return the head of the merged linked list. 
 Example: Input: list1 = [1,2,4], list2 = [1,3,4], Output: [1,1,2,3,4,4] */

 #include<iostream>
 using namespace std;

struct Node {
    int data;
    Node* next;
    
    Node(int val) {
        data = val;
        next = nullptr;
    }
};
    Node* mergelists(Node*list1,Node*list2){
Node model(0);
Node*tail=&model;
 while(list1!=nullptr && list2!=nullptr){

    if(list1->data<=list2->data){
        tail->next=list1;
        list1=list1->next;
    }
    else{
          tail->next=list2;
        list2=list2->next;
    }
    tail=tail->next;
    }
if(list1!=nullptr){
    tail->next=list1;
}
else{
     tail->next=list2;
}
return model.next;
    }
 void printlist(Node*head){
    while(head!=nullptr){
        cout<<head->data<<"->";
        head=head->next;
    }
    cout<<"NULL"<<endl;
 }

 int main(){
    //list1
    Node*list1=new Node(1);
     list1->next=new Node(2);
     list1->next->next=new Node(4);
      //list1
    Node*list2=new Node(1);
     list2->next=new Node(3);
     list2->next->next=new Node(4);

     Node*result= mergelists(list1,list2);
     cout<<"MERGE LISTS ARE:"<<endl;
printlist(result);

 }

   

