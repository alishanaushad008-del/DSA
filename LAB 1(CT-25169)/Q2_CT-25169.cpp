//Write a program to calculate the GPA of students of all subjects of a single semester . 
//Assume all the courses have the same credit hour (let’s assume 3 credit hours). 
#include<iostream>
using namespace std;
int main(){
    string name[]={"Ali","Hiba","Asma","Zain","Faisal"};
    //for "--" we take -1.0
    float marks[5][5]={
    {3.66, 3.33, 4.0,  3.0,  2.66},
    {3.33, 3.0,  3.66, 3.0, -1.0},
    {4.0,  3.66, 2.66, -1.0, -1.0},
    {2.66, 2.33, 4.0,  -1.0, -1.0},
    {3.33, 3.66, 4.0,  3.0,  3.33}
    
};

for(int i=0;i<5;i++){
    float sum=0.0;
    int no_of_valid_sub=0;
    for(int j=0;j<5;j++){
        if(marks[i][j]!=-1.0){
            sum=sum+marks[i][j];
            no_of_valid_sub++;
        }
    }
    float gpa=sum/no_of_valid_sub;
    cout<<name[i]<<" has GPA"<<gpa<<endl;

}


}