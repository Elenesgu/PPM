#include<string>
#include<functional>
#include<vector>
#include<map>
#include<set>
#include<functional>

#pragma once

namespace toc {
	struct ManacherAlgorithm {
		std::vector<int> operator() (const std::string& s);
		/*
		std::string operator() (char* s, size_t l);
		std::wstring operator() (std::wstring s);
		std::wstring operator() (wchar_t* s, size_t l);
		*/
	};

	const char dump_char = 'a' - 1;
	const int invalid_data = -1;
	typedef std::vector<int> intarray;
	typedef std::pair<int, int> intRelation;

	struct State {
		//first is condition, second is target;
		intRelation successRelation = intRelation(-1, -1);
		int failureRelation = invalid_data;
		std::vector<int> injectiveFunction;
	};

	struct MState {
		std::map<int, int> successRelation;
		int failureRelation;
		std::vector<int> injectiveFunction;
		int suffixTransition = -1;
	};

	struct Automaton {
		int letterSize;
		std::vector<State> Status;
		inline void NewStatus();
		std::set<int> TerminalState;
	};
	
	struct MAutomaton {
		int letterSize;
		std::vector<MState> MStatus;
		inline int NewStatus(int n);
		std::set<int> TerminalState;
	};

	class PalMatching {
	private:
		ManacherAlgorithm Pals;

		Automaton SingleAutomaton;
		std::string Pattern;
		intarray _Pattern;
		intarray pPals;
		std::vector<char> _A;
		std::vector<std::set<int>> _B;

		MAutomaton MultiAutomaton;
		std::vector<std::string> MPattern;
		std::vector<intarray> _MPattern;
		std::vector<intarray> MpPals;
		std::vector<std::vector<char>> _MA;
		std::vector<std::vector<std::set<int>>> _MB;

		intarray Result;
		int curState;

		void ConstructVariablePattern();
		void ConstructSingleAutomaton();
		int FindFailureCondition(const int index);

		void ConstructMultiVariablePattern();
		void ConstructMultiAutomaton();
		int MFindFailureCondition(const int index, int& refj, int sourcej);
		int MFindFailureConditionSub(const int index, int j, int sourcej);
		int MFindTransitionCondition(const int index, int refj);
		int StateForVP(int j, int _i);
	public:
		int _ASize;
		PalMatching(const std::string& pattern);
		PalMatching(const std::vector<std::string>& patterns);
		//for Debugging
		void run(const std::string& target);
		void checkstring(const std::string& source);
	};
}