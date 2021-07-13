#pragma once
#include<string_view>	

class IPaper abstract
{
public:
	IPaper() = default;
	virtual ~IPaper() = default;

public:
	virtual void Print(std::wstring text) abstract;
};