#pragma once
#include<random>

namespace gzz
{
	class Random
	{
	public:
		Random();
		~Random();

	public:
		int GetRand(const int src, const int dst);
		double GetRand(const double src, const double dst, int precision);

	private:
		std::mt19937 generator;
		std::random_device rd;
	};
}
