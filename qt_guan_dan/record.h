#ifndef RECORD_H_
#define RECORD_H_
#include <iostream>
#include <vector>
#include <array>

class Record
{
public:
	Record() {}
	//@brief 将当前的局面放入record
	//@para cards: 待放入的状态
	//@ret int,int,int,int: 头游到末游
	void push_record();

	auto get_record() const { return record_; }

	// Debug
	void print() const
	{
		int len = record_.size();
		for (int i = 0; i < len; i++)
		{
			const auto& [fir, sec, trd, fur] = record_[i];
			std::cerr << fir << " " << sec << " " << trd << " " << fur << " in the " << i << " game.\n";
		}
	}
private:
	// 记录目前为止所有的输赢情况
	std::vector <std::array<int, 4>> record_;
};

#endif // RECORD_H_
