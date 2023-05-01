#include "record.h"
#include "status.h"

// 进行记录
void Record::push_record()
{
    std::array<int,4> order;
    // 打包成array
    for(int i=0;i<4;i++)
    {
        order[i] = rank_list[i];
    }
    record_.push_back(order);
}
