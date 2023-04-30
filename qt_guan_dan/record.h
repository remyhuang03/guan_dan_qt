#ifndef RECORD_H_
#define RECORD_H_
#include "status.h"
#include <iostream>
#include <vector>
#include <array>

class record
{
public:
    record(){}
    //@brief 将当前的局面放入record
	//@para cards: 待放入的状态
	//@ret int,int,int,int: 头游到末游
    void push_record();  
    
    // 输出记录
    void display(/*to-do*/);
    // Debug
    void print() const
    {
        int len = Record_.size();
        for(int i=0;i<len;i++)
        {
            const auto &[fir,sec,trd,fur] = Record_[i];
            std::cerr<<fir<<" "<<sec<<" "<<trd<<" "<<fur<<" in the "<<i<<" game.\n";
        }
    }
private:
    // 记录目前为止所有的输赢情况
    std::vector <std::array<int,4>> Record_;
};

#endif // RECORD_H_
