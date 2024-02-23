#include <intrin.h>
#include <chrono>
#include <iostream>
#include <string>

using DataType = double;

struct Data {
	DataType A;
	DataType B;
	DataType C;
	DataType D;
	DataType E;
	DataType F;
	DataType G;
	DataType H;
	DataType X;
};

constexpr unsigned int REPEAT_COUNT = 1000000;

void run_native(Data& data) {
	data.X = (data.A - data.B) - (data.C - data.D) + (data.E + data.F) - (data.G + data.H);
}

void run_SSE2(Data& data) {
	__m128d d__A_B__ = _mm_set_pd(data.A, data.B);
	__m128d d__C_D__ = _mm_set_pd(data.C, data.D);
	__m128d d__E_F__ = _mm_set_pd(data.E, data.F);
	__m128d d__G_H__ = _mm_set_pd(data.G, data.H);

	__m128d d__A_minus_B__ = _mm_sub_sd(d__A_B__, _mm_unpackhi_pd(d__A_B__, d__A_B__));
	__m128d d__C_minus_D__ = _mm_sub_sd(d__C_D__, _mm_unpackhi_pd(d__C_D__, d__C_D__));
	__m128d d__E_plus_F__ = _mm_add_sd(d__E_F__, _mm_unpackhi_pd(d__E_F__, d__E_F__));
	__m128d d__G_plus_H__ = _mm_add_sd(d__G_H__, _mm_unpackhi_pd(d__G_H__, d__G_H__));

	__m128d d__A_minus_B__minus__C_minus_D__ = _mm_sub_sd(d__A_minus_B__, d__C_minus_D__);
	__m128d d__E_plus_F__minus__G_plus_H__ = _mm_sub_sd(d__E_plus_F__, d__G_plus_H__);

	__m128d d__result__ = _mm_add_sd(d__A_minus_B__minus__C_minus_D__, d__E_plus_F__minus__G_plus_H__);

	data.X = _mm_cvtsd_f64(d__result__);
}

void printResult(const std::string& type, const Data& data, std::chrono::nanoseconds runTime) {
	std::cout << type << std::endl;
	std::cout << "x = (A - B) - (C - D) + (E + F) - (G + H)" << std::endl;
	std::cout << "A=" << data.A << " B=" << data.B << " C=" << data.C << " D=" << data.D << " E=" << data.E << " F=" << data.F << " G=" << data.G << " H=" << data.H << std::endl;
	std::cout << "x = " << data.X << std::endl;
	std::cout << "run time: " << runTime.count() << " ns." << std::endl;
}

int main() {
	Data data;

	data.A = 1.;
	data.B = 2.;
	data.C = 3.;
	data.D = 4.;
	data.E = 5.;
	data.F = 6.;
	data.G = 7.;
	data.H = 8.;

	auto startTime = std::chrono::high_resolution_clock::now();
	for (unsigned int indexIteration = REPEAT_COUNT; indexIteration--;) {
		run_native(data);
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	printResult("x86", data, std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime) / REPEAT_COUNT);

	data.X = 0;

	startTime = std::chrono::high_resolution_clock::now();
	for (unsigned int indexIteration = REPEAT_COUNT; indexIteration--;) {
		run_SSE2(data);
	}
	endTime = std::chrono::high_resolution_clock::now();
	printResult("SSE2", data, std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime) / REPEAT_COUNT);

	std::cin.get();

	return 0;
}
