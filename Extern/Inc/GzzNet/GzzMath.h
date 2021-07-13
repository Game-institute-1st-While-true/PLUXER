#pragma once
#include <vector>
namespace gzz
{
	template <typename T>
	void AddCountNumber(double& out, T rhs, int& cnt)
	{
		cnt++;
		out += rhs;
	}

	// 편차 제곱
	template <typename T>
	void Deviation_Squared(double& out, T num, double average)
	{
		double input = num - average;
		input = num - average;
		out += (input * input);
	}

	// 평균
	template<typename T>
	double Average(std::vector<T> vec)
	{
		double res = 0;
		int size = vec.size();
		for (int i = 0; i < size; i++)
		{
			res += vec[i];
		}
		res /= size;
		return res;
	}

	template<typename T>
	double Variance(std::vector<T> vec)
	{
		double average = 0;
		int cnt = vec.size();
		average = Average(vec);

		double ret = 0;
		for (int i = 0; i < cnt; i++)
		{
			Deviation_Sqaured(ret, vec[i], average);
		}
		return ret / cnt;
	}


	template <typename T>
	double StandardDeviation(std::vector<T> vec)
	{
		double average = 0;
		double variance = 0;
		int cnt = vec.size();

		average = Average(vec);

		for (int i = 0; i < cnt; i++)
		{
			Deviation_Squared(variance, vec[i], average);
		}
		variance /= cnt;
		return sqrt(variance);
	}

	template<typename ...Args>
	double Average(Args... numbers)
	{
		double res = 0;
		int cnt = 0;
		(AddCountNumber(res, numbers, cnt), ...);
		res /= cnt;

		return res;
	}


	// 분산
	template <typename ...Args>
	double Variance(Args... numbers)
	{
		double average = 0;
		int cnt = 0;
		(AddCountNumber(average, numbers, cnt), ...);
		average /= cnt;

		double ret = 0;
		(Deviation_Squared(ret, numbers, average), ...);
		return ret / cnt;
	}


	// 표준 편차
	template <typename ...Args>
	double StandardDeviation(Args... numbers)
	{
		double average = 0;
		int cnt = 0;
		(AddCountNumber(average, numbers, cnt), ...);
		average /= cnt;

		double variance = 0;
		(Deviation_Squared(variance, numbers, average), ...);
		variance /= cnt;

		return sqrt(variance);
	}
}