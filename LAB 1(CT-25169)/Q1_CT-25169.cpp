//1. Write a C++ program to copy data of a 2D array in a 1D array using Column Major Order. 
#include<iostream>
using namespace std;
int main(){
    int r,c;
    cout<<"enter rows:";
    cin>>r;
     cout<<"enter columns:";
    cin>>c;

    int array2D[r][c];
    cout<<"enter elements of 2D array:"<<endl;
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            cin>>array2D[i][j];
        }
    }
    int array1D[r*c];
    int k=0;
    for(int i=0;i<c;i++){
  for(int j=0;j<r;j++){
  array1D[k] = array2D[j][i];
  k++;
    }
}; 
 
    cout<<"elements of 1D array become";
    for(int i=0;i<r*c;i++){
            cout<<array1D[i]<<" ";
        }
    }

