#include "record.h"
#include "status.h"

// 进行记录
void record::push_record()
{
    std::array<int,4> order;
    // 打包成array
    for(int i=0;i<4;i++)
    {
        order[i] = rank_list[i];
    }
    Record_.push_back(order);
}
