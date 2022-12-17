//
// Created by 86187 on 2022/12/16.
//
#include <random>
#include "sadMat.h"
#include <iostream>
using namespace std;
template<typename T>
sadMat<T>::sadMat() {
    if (this!= nullptr){
        release();
    }
    row =col=fatherRow=fatherCol=offerSet=0;
    data=fatherData=0;
    addref();
}

template<typename T>
sadMat<T>::sadMat(size_t rows, size_t cols, bool random) {
    if (this!= nullptr){
        release();
    }
    create(rows,cols);
    addref();
    if (random){
        int min = 0,max = 100;
        random_device seed;
        ranlux48 engine(seed());
        uniform_int_distribution<> distrib(min, max);
        uniform_int_distribution<> distriba(1, 4);
        for (int i = 0; i < rows*cols; ++i) {
            data[i] = (T)distrib(engine)/distriba(engine);
        }
    }
}

template<typename T>
sadMat<T>::sadMat(T scalar) {
    row = 1;
    col = 1;
    data = new T{};
    data[0]=scalar;
    addref();
}

template<typename T>
template<typename T1>
sadMat<T>::sadMat(const sadMat<T1> &m) {
    if (this!= nullptr){
        release();
    }
    row = m.row;
    col = m.col;
    data = (m.data);
    fatherData = data;
    fatherRow = row;
    fatherCol = col;
    offerSet = fatherCol-col;
    refCount = m.refCount;
    *refCount = *refCount+1;
}
//ROI用的构造函数
template<typename T>
sadMat<T>::sadMat(const sadMat &m, size_t startRow, size_t endRow, size_t startCol, size_t endCol) {

    if (this!= nullptr){
        release();
    }
    row = endRow - startRow+1;
    col = endCol - startCol+1;
    fatherRow = m.fatherRow;
    fatherCol = m.fatherCol;
    offerSet  = fatherCol - col;
    fatherData = m.fatherData;
    data = &fatherData[startRow*fatherCol+startCol];
    refCount = m.refCount;
    *refCount = *refCount+1;
}

template<typename T>
void sadMat<T>::addref() {
    if(refCount!= nullptr) {
        *(refCount) = *(refCount) + 1;
    } else{
        refCount = new size_t ;
        *(refCount) = 1;
    }
}

template<typename T>
void sadMat<T>::create(size_t rowNum, size_t colNum) {
    row = rowNum;
    col = colNum;
    try{
        data = new T[row* col]();
        if (data == nullptr){
            throw -1;
        }
    } catch (int &e ) {
        cerr<< "Memory request failed." <<" At:" <<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<endl;
    }
    fatherData = data;
    fatherRow = rowNum;
    fatherCol = colNum;
    offerSet = fatherCol-col;
}

template<typename T>
bool sadMat<T>::release() {

    if (refCount == nullptr){
        return false;
    }
    if(*refCount == 1){
        row =col=fatherRow=fatherCol=offerSet=0;
        delete[] fatherData;
        data = nullptr;
        fatherData = nullptr;
        refCount = nullptr;
        cout<< "release"<<endl;
        return true;
    } else if (*refCount<=0){
        try{
            throw "refCount is smaller than 1";
        } catch (const char* msg) {
            cerr<< msg <<" At:" <<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<endl;
        }

    }else {
        *refCount =*refCount-1;
        return false;
    }
}

template<typename T>
sadMat<T>::~sadMat() {
    cout<<"refCount: " <<*refCount<<endl;
    release();
}

template<typename T>
void sadMat<T>::printMat() {
    size_t current = 0;
    for (int l = 0; l < row; ++l) {
        for (int m = 0; m < col; ++m) {
            try{
                cout<< data[current]<<" ";
            } catch (out_of_range& e) {
                cerr<< "Data is out of range at :"<<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<"Break point: "<<current<<endl;
            }
            current++;
        }
        current += offerSet;
        cout<<endl;
    }
}

template<typename T>
sadMat<T>& sadMat<T>::operator=(const sadMat &m) {
    if(*this == *&m){
        return *this;
    }
    if (this!= nullptr){
        release();
    }
    row = m.row;
    col = m.col;
    data = m.data;
    fatherData = m.fatherData;
    fatherRow = m.fatherRow;
    fatherCol = m.fatherCol;
    offerSet = m.offerSet;
    refCount = m.refCount;
    addref();
    return *this;
}

template<typename T>
template<typename T1>
bool sadMat<T>::operator==(const sadMat<T1> &m) const {
    if (typeid(T1).name()!= typeid(T).name()){
        return false;
    }
    if (row == m.row && col == m.col){
        for (int i = 0; i < row*col; ++i) {
            if (data[i] != m.data[i]){
                return false;
            }
        }
    } else{
        return false;
    }
    return true;
}

template<typename T>
template<typename T1>
bool sadMat<T>::operator!=(const sadMat<T1> &m) const {
    if (row == m.row && col == m.col){
        for (int i = 0; i < row*col; ++i) {
            if (data[i] != m.data[i]){
                return true;
            }
        }
    } else{
        return true;
    }
    return false;
}

template<typename T>
sadMat<T> &sadMat<T>::operator+=(const sadMat &m) {
    if (m.row == 1 && m.col==1){
        size_t current = 0;
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[current] = data[current]+m.data[0];
                current++;
            }
            current += offerSet;
        }
        return *this;
    }else if(row == m.row && col == m.col){
        size_t current1 = 0;
        size_t current2 = 0;
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[current1] = data[current1]+m.data[current2];
                current1++;
                current2++;
            }
            current1 += offerSet;
            current2 += m.offerSet;
        }
        return *this;
    } else{
        try {
            throw "Matrix format error.";
        }catch (const char* e){
            cerr<<e<< " At:"<<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<endl<<"the size of left one is:"<<endl<<
                "rows: "<<row<<"cols: "<<col<<endl<<"the size of right one is:"<<endl<<
                "rows: "<<m.row<<"cols: "<<m.col<<endl;
            return *this;
        }
    }
}

template<typename T>
sadMat<T> &sadMat<T>::operator-=(const sadMat &m) {
    if (m.row == 1 && m.col==1){
        size_t current = 0;
        size_t currentResult = 0;
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[current] = data[current]-m.data[0];
                current++;
                currentResult++;
            }
            current += offerSet;
        }
        return *this;
    }else if(row == m.row && col == m.col){
        size_t current1 = 0;
        size_t current2 = 0;
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[current1] = data[current1]-m.data[current2];
                current1++;
                current2++;
            }
            current1 += offerSet;
            current2 += m.offerSet;
        }
        return *this;
    } else{
        try {
            throw "Matrix format error.";
        }catch (const char* e){
            cerr<<e<< " At:"<<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<endl<<"the size of left one is:"<<endl<<
                "rows: "<<row<<"cols: "<<col<<endl<<"the size of right one is:"<<endl<<
                "rows: "<<m.row<<"cols: "<<m.col<<endl;
            return *this;
        }
    }
}

template<typename T>
sadMat<T> &sadMat<T>::operator*=(const sadMat &m) {
    if (m.row==1&&m.col==1){
        size_t current = 0;
        size_t currentResult = 0;
        auto *result = new sadMat<T>(row,col, false);
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                result->data[currentResult] = data[current]*m.data[0];
                current++;
                currentResult ++;
            }
            current += offerSet;
        }
        current = 0;
        currentResult = 0;
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[current] = result->data[currentResult];
                current++;
                currentResult ++;
            }
            current += offerSet;
        }
        return *this;
    } else if (col == m.row){
        size_t currentResult = 0;
        size_t current1 = 0;
        size_t current2 = 0;
        auto *result = new sadMat<T>(row,col, false);
        for (int i = 0; i < result->row; ++i) {
            for (int j = 0; j < result->col; ++j) {
                for (int k = 0; k < m.row; ++k) {
                    result->data[currentResult]+=data[current1]*m.data[current2];
                    current1++;
                    current2+= m.fatherCol;
                }
                current1-=col;
                current2-=(m.row-1)*m.fatherCol+1;
                currentResult++;
            }
            current1+=fatherCol;
            current2 = 0;
        }
        current1 = 0;
        currentResult = 0;
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[current1] = result->data[currentResult];
                current1++;
                currentResult ++;
            }
            current1 += offerSet;
        }
        return *this;
    } else{
        try {
            throw "Matrix format error.";
        }catch (const char* e){
            cerr<<e<< " At:"<<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<endl<<"the size of left one is:"<<endl<<
                "rows: "<<row<<"cols: "<<col<<endl<<"the size of right one is:"<<endl<<
                "rows: "<<m.row<<"cols: "<<m.col<<endl;
            return *this;
        }
    }
}

template<typename T>
T sadMat<T>::get(size_t targetRow, size_t targetCol) {
    try{
        if (targetRow>row||targetCol>col){
            throw "Data is out of range at :";
        }
    } catch (const char* e) {
        cerr<< e <<" At:" <<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<"The target row is :"<<targetRow<<"the bound is :"<<row
        <<" The target column is :"<<targetCol<<"the bound is :"<<col<<endl;
        return NULL;
    }
    return data[targetRow*fatherCol+targetCol];
}

template<typename T>
bool sadMat<T>::set(size_t targetRow, size_t targetCol, T num) {
    try{
        if (targetRow>row||targetCol>col){
            throw "Data is out of range at :";
        }
    } catch (const char* e) {
        cerr<< e <<" At:" <<__FILE__<<":"<<__FUNCTION__ <<":"<<__LINE__<<"The target row is :"<<targetRow<<" The bound is :"<<row
            <<" The target column is :"<<targetCol<<" The bound is :"<<col<<endl;
        return false;
    }
    data[targetRow*fatherCol+targetCol] = num;
    return true;
}












//template<typename T,typename T2>
//sadMat<T> &operator+(const sadMat<T> &m1, const sadMat<T2> &m2) {
//    if(m1.row==1&&m1.col==1){
//        sadMat<T> result = new sadMat<T>(m2.row,m2.col);
//        for (int i = 0; i < m2.row*m2.col; ++i) {
//            result.data[i] = m2.data[i]+m1.data[0];
//        }
//        return *result;
//    } else if (m2.row==1&&m2.col==1){
//        sadMat<T> result = new sadMat<T>(m1.row,m1.col);
//        for (int i = 0; i < m1.row*m1.col; ++i) {
//            result.data[i] = m2.data[0]+m1.data[i];
//        }
//        return *result;
//    } else if (m1.row == m2.row && m1.col == m2.col){
//        sadMat<T> result = new sadMat<T>(m1.row,m1.col);
//        for (int i = 0; i < m1.row*m1.col; ++i) {
//            result.data[i] = m2.data[i]+m1.data[i];
//        }
//        return *result;
//    } else{
//        return NULL;
//    }
//}

//template<typename T,typename T2>
//sadMat<T> &operator-(const sadMat<T> &m1, const sadMat<T2> &m2) {
//    if(m1.row==1&&m1.col==1){
//        sadMat<T> result = new sadMat<T>(m2.row,m2.col);
//        for (int i = 0; i < m2.row*m2.col; ++i) {
//            result.data[i] = m1.data[0]-m2.data[i];
//        }
//        return *result;
//    } else if (m2.row==1&&m2.col==1){
//        sadMat<T> result = new sadMat<T>(m1.row,m1.col);
//        for (int i = 0; i < m1.row*m1.col; ++i) {
//            result.data[i] = m1.data[i]-m2.data[0];
//        }
//        return *result;
//    } else if (m1.row == m2.row && m1.col == m2.col){
//        sadMat<T> result = new sadMat<T>(m1.row,m1.col);
//        for (int i = 0; i < m1.row*m1.col; ++i) {
//            result.data[i] = m1.data[i]-m2.data[i];
//        }
//        return *result;
//    } else{
//        return NULL;
//    }
//}

//template<typename T,typename T2>
//sadMat<T> &operator*(const sadMat<T> &m1, const sadMat<T2> &m2) {
//    if(m1.row==1&&m1.col==1){
//        sadMat<T> result = new sadMat<T>(m2.row,m2.col);
//        for (int i = 0; i < m2.row*m2.col; ++i) {
//            result.data[i] = m1.data[0]*m2.data[i];
//        }
//        return *result;
//    } else if (m2.row==1&&m2.col==1){
//        sadMat<T> result = new sadMat<T>(m1.row,m1.col);
//        for (int i = 0; i < m1.row*m1.col; ++i) {
//            result.data[i] = m1.data[i]*m2.data[0];
//        }
//        return *result;
//    } else if (m1.col == m2.row ){
//        sadMat<T> result = new sadMat<T>(m1.row,m2.col);
//        for (int i = 0; i < result.row; ++i) {
//            for (int j = 0; j < result.col; ++j) {
//                for (int k = 0; k < m2.row; ++k) {
//                    result.data[i*result.col+j]+=m1.data[i*m1.col+k]*m2.data[k*m2.col+j];
//                }
//            }
//        }
//        return *result;
//    } else{
//        return NULL;
//    }
//}

//template<typename T>
//sadMat<T> &sadMat<T>::operator+(const sadMat<T> &m) {
//    if (m.row==1&&m.col==1){
//        sadMat<T> *result = new sadMat<T>(row,col, false);
//        for (int i = 0; i < row*col; ++i) {
//            result->data[i] = m.data[0]+data[i];
//        }
//        return *result;
//    } else if(row == m.row&&col==m.col){
//        sadMat<T> *result = new sadMat<T>(row,col, false);
//        for (int i = 0; i < row*col; ++i) {
//            result->data[i] = m.data[i]+data[i];
//        }
//        return *result;
//    } else{
//        sadMat<T> *result = new sadMat<T>();
//        return *result;
//    }
//}

//template<typename T,typename T1>
//sadMat<T> &operator+(T1 n1, const sadMat<T> &m2) {
//    auto *result = new sadMat<T>(m2.row,m2.col, false);
//    for (int i = 0; i < m2.row*m2.col; ++i) {
//        result->data[i] =
//    }
//}

