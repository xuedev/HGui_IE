#pragma once
template<typename Data,typename RetData>
class AutoArray
{
public:
	Data* data_;
	AutoArray(Data* data):data_(data)
	{
	}

	~AutoArray(void)
	{
		delete[] data_;
	}

	Data& operator= (Data& tmp){
		return *tmp;
	}

	RetData operator()(){
		return RetData(data_);
	}
};