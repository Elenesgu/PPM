#include"PPM.h"
#include<array>
#include<vector>
#include<random>
#include<chrono>
#include<iostream>
#include<fstream>

std::ostream& out = std::ofstream("output.txt");
auto checkTime = std::chrono::high_resolution_clock::now;

inline std::string MakeMessage(int slength, int asize, std::chrono::microseconds buildingtime);

int main() {
	const std::array<char, 4U> Letter = { 'A', 'G', 'C', 'T' };
	const int SourceLengthDiv = 100000;
	const int PatternLengthDiv = 10;
	std::array<std::string, 10U> sources;
	std::array<std::string, 10U> patterns;


	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> sellector(0, 3);

	std::chrono::time_point<std::chrono::system_clock> start, end;

	int SourceLength = 100000;
	int PatternLength = 10;

	/*for (int counter = 0; counter < 10; counter++) {
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
		start = checkTime();
		toc::PalMatching Palmat(patterns[testCase]);
		end = checkTime();
		out << MakeMessage(patterns[testCase].length(), Palmat._ASize, 
			std::chrono::duration_cast<std::chrono::microseconds>(end - start));
		for (int sourceIndex = 0; sourceIndex < 10; sourceIndex++) {
			start = checkTime();
			Palmat.run(sources[sourceIndex]);
			end = checkTime();
			out << "RunTime: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us\n";
		}
	}*/
	std::vector<std::string> A;
	A.push_back("AGA");
	A.push_back("ACTG");
	A.push_back("ATAT");
	A.push_back("TCTGC");
	toc::PalMatching pm(A);

}

std::string MakeMessage(int slength, int asize, std::chrono::microseconds buildingtime) {
	static int index = 1;
	std::string Message;
	Message = "#";
	Message += std::to_string(index++);
	Message += " Target Length: ";
	Message += std::to_string(slength);
	Message += " A_Size: ";
	Message += std::to_string(asize);
	Message += " Automaton Building Time: ";
	Message += std::to_string(buildingtime.count());
	Message += "us\n";
	return std::string(Message);
}