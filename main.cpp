#include<iostream>
#include<string.h>

#include"core_start.h"

using namespace std;

int main()
{
    int test_train_predict;
    float data_division;
    //core_starter();  
    string file_name="NOT_AVAILABLE",network_save_file_name="NOT_AVAILABLE";
    int no_of_threads=1;
    menu(file_name,test_train_predict,data_division,network_save_file_name,no_of_threads); 
    if(test_train_predict!=-1)
    {   core_starter(file_name,test_train_predict,data_division,network_save_file_name,no_of_threads);}
    return 0;
}