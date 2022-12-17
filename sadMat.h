//
// Created by 86187 on 2022/12/16.
//

#ifndef PROJECT_5_SADMAT_H
#define PROJECT_5_SADMAT_H
#include <cstddef>
#include <iostream>
using namespace std;
template<typename T>
class sadMat {
public:
    size_t  row{}, col{}, fatherRow{}, fatherCol{}, offerSet{};
    T* data{};
    T* fatherData{};
    size_t* refCount{};

    //构造函数
    sadMat();
    sadMat(size_t rows, size_t cols, bool random);
    sadMat(T scalar);

    //析构函数
    ~sadMat();

    //复制构造函数
    template<typename T1>
    sadMat(const sadMat<T1>& m);

    //roi
    sadMat(const sadMat& m, size_t startRow, size_t endRow, size_t startCol, size_t endCol);

    //成员方法
    void addref();
    void create(size_t row, size_t col);
    bool release();
    void printMat();
    T get(size_t targetRow, size_t targetCol);
    bool set(size_t targetRow, size_t targetCol,T num);

    //重载操作符
    sadMat<T>& operator= (const sadMat& m);
    template<typename T1>
    bool operator== (const sadMat<T1>& m) const ;
    template<typename T1>
    bool operator!= (const sadMat<T1>& m) const ;

    //问题 a=a*b 这个的refcount 问题
    template<typename T1>
    friend sadMat<T>& operator* (const sadMat<T>& m1,const sadMat<T1>& m2){
        if(m1.row==1&&m1.col==1){
            size_t current = 0;
            size_t currentResult = 0;
            auto *result = new sadMat<T>(m2.row,m2.col, false);
            for (int i = 0; i < m2.row; ++i) {
                for (int j = 0; j < m2.col; ++j) {
                    result->data[currentResult] = m1.data[0]*m2.data[current];
                    current++;
                    currentResult ++;
                }
                current += m2.offerSet;
            }
            return *result;
        } else if (m2.row==1&&m2.col==1){
            size_t current = 0;
            size_t currentResult = 0;
            auto *result = new sadMat<T>(m1.row,m1.col, false);
            for (int i = 0; i < m1.row; ++i) {
                for (int j = 0; j < m1.col; ++j) {
                    result->data[currentResult] = m1.data[current]*m2.data[0];
                    current++;
                    currentResult ++;
                }
                current += m1.offerSet;
            }
            return *result;
        } else if (m1.col == m2.row ){
            size_t currentResult = 0;
            size_t current1 = 0;
            size_t current2 = 0;
            auto *result = new sadMat<T>(m1.row,m2.col, false);
            for (int i = 0; i < result->row; ++i) {
                for (int j = 0; j < result->col; ++j) {
                    for (int k = 0; k < m2.row; ++k) {
                        result->data[currentResult]+=m1.data[current1]*m2.data[current2];
                        current1++;
                        current2+= m2.fatherCol;
                    }
                    current1-=m1.col;
                    current2-=(m2.row-1)*m2.fatherCol+1;
                    currentResult++;
                }
                current1+=m1.fatherCol;
                current2 = 0;
            }
            return *result;
        } else{
            auto *result = new sadMat<T>();
            return *result;
        }
    }
    template<typename T1>
    friend sadMat<T>& operator+ (const sadMat<T>& m1,const sadMat<T1>& m2){
        if(m1.row==1&&m1.col==1){
            size_t current = 0;
            size_t currentResult = 0;
            auto *result = new sadMat<T>(m2.row,m2.col, false);
            for (int i = 0; i < m2.row; ++i) {
                for (int j = 0; j < m2.col; ++j) {
                    result->data[currentResult] = m1.data[0]+m2.data[current];
                    current++;
                    currentResult++;
                }
                current += m2.offerSet;
            }
            return *result;
        } else if (m2.row==1&&m2.col==1){
            size_t current = 0;
            size_t currentResult = 0;
            auto *result = new sadMat<T>(m1.row,m1.col, false);
            for (int i = 0; i < m1.row; ++i) {
                for (int j = 0; j < m1.col; ++j) {
                    result->data[currentResult] = m1.data[current]+m2.data[0];
                    current++;
                    currentResult++;
                }
                current += m1.offerSet;
            }
            return *result;
        } else if (m1.row == m2.row && m1.col == m2.col){
            size_t current1 = 0;
            size_t current2 = 0;
            size_t currentResult = 0;
            auto *result = new sadMat<T>(m1.row,m1.col, false);
            for (int i = 0; i < m1.row; ++i) {
                for (int j = 0; j < m1.col; ++j) {
                    result->data[currentResult] = m1.data[current1]+m2.data[current2];
                    current1++;
                    current2++;
                    currentResult++;
                }
                current1 += m1.offerSet;
                current2 += m2.offerSet;
            }
            return *result;
        } else{
            auto *result = new sadMat<T>();
            return *result;
        }
    }
    template<typename T1>
    friend sadMat<T>& operator- (const sadMat<T>& m1,const sadMat<T1>& m2){
        if(m1.row==1&&m1.col==1){
            size_t current = 0;
            size_t currentResult = 0;
            auto *result = new sadMat<T>(m2.row,m2.col, false);
            for (int i = 0; i < m2.row; ++i) {
                for (int j = 0; j < m2.col; ++j) {
                    result->data[currentResult] = m1.data[0]-m2.data[current];
                    current++;
                    currentResult++;
                }
                current += m2.offerSet;
            }
            return *result;
        } else if (m2.row==1&&m2.col==1){
            size_t current = 0;
            size_t currentResult = 0;
            auto *result = new sadMat<T>(m1.row,m1.col, false);
            for (int i = 0; i < m1.row; ++i) {
                for (int j = 0; j < m1.col; ++j) {
                    result->data[currentResult] = m1.data[current]-m2.data[0];
                    current++;
                    currentResult++;
                }
                current += m1.offerSet;
            }
            return *result;
        } else if (m1.row == m2.row && m1.col == m2.col){
            size_t current1 = 0;
            size_t current2 = 0;
            size_t currentResult = 0;
            auto *result = new sadMat<T>(m1.row,m1.col, false);
            for (int i = 0; i < m1.row; ++i) {
                for (int j = 0; j < m1.col; ++j) {
                    result->data[currentResult] = m1.data[current1]-m2.data[current2];
                    current1++;
                    current2++;
                    currentResult++;
                }
                current1 += m1.offerSet;
                current2 += m2.offerSet;
            }
            return *result;
        } else{
            auto *result = new sadMat<T>();
            return *result;
        }
    }

    sadMat<T>& operator+= (const sadMat& m);
    sadMat<T>& operator-= (const sadMat& m);
    sadMat<T>& operator*= (const sadMat& m);

    friend ostream& operator<<(ostream &out,sadMat<T> &m ){
        size_t current = 0;
        for (int l = 0; l < m.row; ++l) {
            for (int k = 0; k < m.col; ++k) {
                try{
                    out<< m.data[current]<<" ";
                } catch (out_of_range& e) {
                    cerr<< "Data is out of range at :"<<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<"Break point: "<<current<<endl;
                }
                current++;
            }
            current += m.offerSet;
            out<<endl;
        }
        return out;
    }

};




#endif //PROJECT_5_SADMAT_H
