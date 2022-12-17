//
// Created by 86187 on 2022/12/17.
//
#include "iostream"
#include "sadMat.h"

using namespace std;
int main(){

    sadMat<int> intMatrix(3,2, true);
    sadMat<float> floatMatrix(4,3, true);
    //左的列数要等于右的行数
    //*****重载双目算术运算符***************************************************************
    sadMat<float> product = floatMatrix*intMatrix;

    cout<<"--------intMatrix--------"<<endl
        <<intMatrix<<endl;
    cout<<"-------floatMatrix-------"<<endl
        <<floatMatrix<<endl;
    cout<<"---------product---------"<<endl
        <<product<<endl;
    
    
    //*****重载关系运算符以及等于号（=）******************************************************************
    sadMat<int> intMatrix2(3,2, false) ;
    intMatrix2 = intMatrix;
    bool isEqual = intMatrix2==intMatrix;
    bool isNotEqual = floatMatrix!=intMatrix;

    cout<<"--------intMatrix2--------"<<endl
        <<intMatrix2<<endl;
    cout<<"The Equality of intMatrix and intMatrix2: "<<isEqual<<endl;
    cout<<"The Inequality of floatMatrix and intMatrix2: "<<isNotEqual<<endl;

    //*****ROI截取****************************************************************************************
    sadMat<float> roi (floatMatrix,0,1,2,2);
    cout<<"--------ROI of floatMatrix--------"<<endl
        <<roi<<endl;
    //*****set get 函数***********************************************************************************
    cout<<"-------the (1,2) element of intMatrix------"<<endl
        <<intMatrix.get(1,2)<<endl;

    intMatrix.set(1,2,0);
    cout<<"-------After set to 0, the (1,2) element of intMatrix------"<<endl
        <<intMatrix.get(1,2)<<endl;
    //*****重载赋值运算符**********************************************************************************
    roi+=roi;
    cout<<"-------After adding itself, the roi Matrix------"<<endl
        <<roi<<endl;
}
