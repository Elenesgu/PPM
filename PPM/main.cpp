#include"PPM.h"
#include<ppl.h>
#include<array>
#include<vector>
#include<random>
#include<chrono>
#include<iostream>
#include<fstream>
#include<vector>

struct Result {
	int Asize;
	std::chrono::milliseconds time;
};

std::ostream& out = std::ofstream("output.txt");
//std::ostream& out = std::cout;
auto checkTime = std::chrono::high_resolution_clock::now;
std::array<std::array<std::vector<Result>, 10U>, 10U > Results;

const std::array<char, 4U> Letter = { 'A', 'G', 'C', 'T' };
const int SourceLengthDiv = 100000;
const int PatternLengthDiv = 10;

void runalgo(int var);

int main() {
	concurrency::array_view<int, 1> view(1000U, 0);
	auto func = runalgo;
	concurrency::parallel_for(0, 1000, runalgo);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			out << 100000 * (i + 1) << "\t" << 10 * (j + 1) << "\n";
			for each(Result obj in Results[i][j]){
				out << obj.Asize << "\t" << obj.time.count() << "\n";
			}
		}		
	}
	/*std::vector<std::string> A;
	A.push_back("AGA");
	A.push_back("ACTG");
	A.push_back("ATAT");
	A.push_back("TCTGC");
	toc::PalMatching pm(A);*/
	return 0;
}

void runalgo(int var) {
	std::array<std::string, 10U> sources;
	std::array<std::string, 10U> patterns;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> sellector(0, 3);

	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;	

	int SourceLength = SourceLengthDiv;
	int PatternLength = 10;
	for (int counter = 0; counter < 10; counter++) {
		for (int i = 0; i < SourceLength; i++) {
			sources[counter].push_back(Letter[sellector(gen)]);
		}
		SourceLength += SourceLengthDiv;
		for (int i = 0; i < PatternLength; i++) {
			patterns[counter].push_back(Letter[sellector(gen)]);
		}
		PatternLength += PatternLengthDiv;
	}
	for (int testCase = 0; testCase < 10; testCase++) {
		Result tmpresult;
		start = checkTime();
		toc::PalMatching Palmat(patterns[testCase]);
		end = checkTime();
		/*out << MakeMessage(patterns[testCase].length(), Palmat._ASize,
		std::chrono::duration_cast<std::chrono::milliseconds>(end - start));*/
		for (int sourceIndex = 0; sourceIndex < 10; sourceIndex++) {
			start = checkTime();
			Palmat.run(sources[sourceIndex]);
			end = checkTime();
			tmpresult.Asize = Palmat._ASize;
			tmpresult.time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			Results[sourceIndex][testCase].push_back(tmpresult);
			std::cout << var << "\t" << testCase << "\t" << sourceIndex << "\t" << tmpresult.time.count() << "\n";
			/*out << "RunTime: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "us\n";*/
		}
	};
	//for (int loopcounter = 0; loopcounter < 10; loopcounter++) {
	//	SourceLength = 100000;
	//	PatternLength = 10;
	//	for (int counter = 0; counter < 10; counter++) {
	//		for (int i = 0; i < SourceLength; i++) {
	//			sources[counter].push_back(Letter[sellector(gen)]);
	//		}
	//		SourceLength += SourceLengthDiv;
	//		for (int i = 0; i < PatternLength; i++) {
	//			patterns[counter].push_back(Letter[sellector(gen)]);
	//		}
	//		PatternLength += PatternLengthDiv;
	//	}
	//	for (int testCase = 0; testCase < 10; testCase++) {
	//		start = checkTime();
	//		toc::PalMatching Palmat(patterns[testCase]);
	//		end = checkTime();
	//		/*out << MakeMessage(patterns[testCase].length(), Palmat._ASize,
	//		std::chrono::duration_cast<std::chrono::milliseconds>(end - start));*/
	//		for (int sourceIndex = 0; sourceIndex < 10; sourceIndex++) {
	//			start = checkTime();
	//			Palmat.run(sources[sourceIndex]);
	//			end = checkTime();
	//			tmpresult.Asize = Palmat._ASize;
	//			tmpresult.time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	//			Results[sourceIndex][testCase].push_back(tmpresult);
	//			std::cout << loopcounter << "\t" << testCase << "\t" << sourceIndex << "\t" << tmpresult.time.count() << "\n";
	//			/*out << "RunTime: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "us\n";*/
	//		}
	//	}
	//}
}