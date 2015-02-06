#include<string>
#include<algorithm>
#include<vector>
#include<iostream>

#include"PPM.h"

#pragma region ManacherAlgorithm
std::vector<int> toc::ManacherAlgorithm::operator() (const std::string& s) {
	if (s.length() == 0) {
		return std::vector<int>();
	}
	int R(-1), p(-1);
	int D;
	std::vector<int> A(s.length(), 0);
	for (int i = 0; i < s.length(); i++) {
		D = 0;
		if (i <= R) {
			A[i] = std::min<int>(A[2 * p - i], R - i);
		}
		else {
			A[i] = 0;
		} 
		while (i - A[i] - 1 >= 0 && i + A[i] + 1 < s.length() &&
			s[i - A[i] - 1] == s[i + A[i] + 1]) {
			A[i]++;
		}
		if (i + A[i] > R) {
			R = i + A[i];
			p = i;
		}
		//Delete dump char
		for (int j = 1; j <= A[i]; j++) {
			if (s[i + j] == dump_char) {
				D++;
			}
		}
		A[i] -= D;
	}
	return A;
}
#pragma endregion

#pragma region Automaton
void toc::Automaton::NewStatus() {
	Status.push_back(State());
	Status.back().injectiveFunction = std::vector<int>(letterSize, invalid_data);
}
int toc::MAutomaton::NewStatus(int n) {
	MStatus.push_back(MState());
	MStatus.back().injectiveFunction = std::vector<int>(n, invalid_data);
	return MStatus.size() - 1;
}
#pragma endregion

#pragma region PalMatching
void toc::PalMatching::ConstructVariablePattern() {

	//for MAnacherAlgorithm make dump string
	std::string dump_Pattern(Pattern.length() * 2 - 1, dump_char);
	for (int i = 0; i < Pattern.length(); i++) {
		dump_Pattern[2 * i] = Pattern[i];
	}
	pPals = intarray(Pals(dump_Pattern));

	int c_odd, c_even;
	int r_odd, r_even;
	int index = 1;

	_Pattern[0] = index++;

	//Construct A
	_A.push_back(dump_char);
	_A.push_back(Pattern[0]);
	for (int i = 1; i < Pattern.length(); i++) {
		c_odd = 2 * i - 1;
		c_even = 2 * i;
		r_odd = pPals[c_odd];
		r_even = pPals[c_even];
		
		//odd case ex) AA or AAAA
		if (r_odd > 0) {
			for (int j = 1; j <= r_odd; j++) {
				_Pattern[i + j - 1] = _Pattern[i - j];
			}
		}

		//even case ex) ABA or ABCBA
		if (r_even > 0) {			
			for (int j = 1; j <= r_even; j++) {
				_Pattern[i + j] = _Pattern[i - j];
			}
		}

		if (_Pattern[i] == invalid_data) {
			_Pattern[i] = index++;
			_A.push_back(Pattern[i]);
		}
	}

	//Construct B
	_B = std::vector<std::set<int>>(_A.size(), std::set<int>());
	int rbound;

	for (int i = 1; i < Pattern.length(); i++) {
		c_odd = 2 * i - 1;
		c_even = 2 * i;
		r_odd = pPals[c_odd];
		r_even = pPals[c_even];

		//odd case ex) AA or AAAA
		rbound = r_odd + 1;
		if (i + rbound - 1 < Pattern.length() && i - rbound >= 0) {
			_B[_Pattern[i + rbound - 1]].insert(_Pattern[i - rbound]);
			_B[_Pattern[i - rbound]].insert(_Pattern[i + rbound - 1]);
		}

		//even case ex) ABA or ABCBA
		rbound = r_even + 1;
		if (i + rbound < Pattern.length() && i - rbound >= 0) {
			_B[_Pattern[i + rbound]].insert(_Pattern[i - rbound]);
			_B[_Pattern[i - rbound]].insert(_Pattern[i + rbound]);
		}
	}

	_ASize = _A.size();
	//Do not include last Pals value
	//c_odd = 2 * (Pattern.length() - 1) - 1;
	//r_odd = pPals[c_odd];
	//rbound = r_odd + 1;
	//_B[_Pattern[Pattern.length() - 1 + rbound - 1]].insert(_Pattern[Pattern.length() - 1 - rbound]);
	//_B[_Pattern[Pattern.length() - 1 - rbound]].insert(_Pattern[Pattern.length() - 1 + rbound - 1]);
}

void toc::PalMatching::ConstructSingleAutomaton() {

	ConstructVariablePattern();
	SingleAutomaton.letterSize = _A.size();
	SingleAutomaton.NewStatus();
	for (int i = 1; i <= _Pattern.size() + 1; i++) {
		if (i != _Pattern.size() + 1) {
			SingleAutomaton.NewStatus();
			SingleAutomaton.Status[i - 1].successRelation = intRelation(_Pattern[i - 1] , i);

		}
		if (i == 2) {
			SingleAutomaton.Status[1].failureRelation = 0;
		}
		else if (i > 2) {
			int _i = FindFailureCondition(i);
			SingleAutomaton.Status[i - 1].failureRelation = (i - 1) - _i;
			for (int j = 0; j < i - _i - 1; j++) {
				SingleAutomaton.Status[i - 1].injectiveFunction[_Pattern[j]] = _Pattern[_i + j];
			}
		}
	}
	SingleAutomaton.TerminalState.insert(_Pattern.size());
}

int toc::PalMatching::FindFailureCondition(const int index) {
	int _i = 1;
	std::string picker;
	std::string dump_Pattern;
	intarray leftPals, rightPals;
	while (_i <= index - 2) {
		picker = Pattern.substr(0, index - _i - 1);
		dump_Pattern = std::string(picker.length() * 2 - 1, dump_char);
		for (int i = 0; i < picker.length(); i++) {
			dump_Pattern[2 * i] = picker[i];
		}
		leftPals = intarray(Pals(dump_Pattern));
		picker = Pattern.substr(_i, index - _i - 1);
		dump_Pattern = std::string(picker.length() * 2 - 1, dump_char);
		for (int i = 0; i < picker.length(); i++) {
			dump_Pattern[2 * i] = picker[i];
		}
		rightPals = intarray(Pals(dump_Pattern));
		if (std::equal(leftPals.begin(), leftPals.end(), rightPals.begin())) {
			return _i;
		}
		else {
			_i++;
		}
	}
	return _i;
}

void toc::PalMatching::checkstring(const std::string& source) {
	std::for_each(_A.begin(), _A.end(), [](char& s) {
		s = dump_char;
	});
	curState = 0;
	int ACounter = 1;
	auto curInput = source.begin();
	int convertedInput;
	while (curInput != source.end()) {
		//Convert input using _A;
		convertedInput = invalid_data;
		if (ACounter < _A.size()) {
			for (int i : _B[ACounter]) {
				if (_A[i] == *curInput) {
					convertedInput = i;
				}
			}
			if (convertedInput == invalid_data) {
				_A[ACounter] = *curInput;
				convertedInput = ACounter;
				ACounter++;
			}
		}
		//Run Automaton
		while (SingleAutomaton.Status[curState].successRelation.first != convertedInput) {
			//run Injective Function
			convertedInput = invalid_data;
			std::vector<char> tmp_A = std::vector<char>(_A);
			for (int i = 1; i < SingleAutomaton.letterSize; i++) {
				if (SingleAutomaton.Status[curState].injectiveFunction[i] == invalid_data) {
					_A[i] = invalid_data;
				}
				else {
					_A[i] = tmp_A[SingleAutomaton.Status[curState].injectiveFunction[i]];
				}
			}
			for (int i = 1; i < SingleAutomaton.letterSize; i++) {
				if (_A[i] = invalid_data) {
					ACounter = i;
					break;
				}
			}
			//go to failure state
			curState = SingleAutomaton.Status[curState].failureRelation;
			if (ACounter < _A.size()) {
				for (int i : _B[ACounter]) {
					if (_A[i] == *curInput) {
						convertedInput = i;
					}
				}
				if (convertedInput == invalid_data) {
					_A[ACounter] = *curInput;
					convertedInput = ACounter;
					ACounter++;
				}
			}
		}
		curState = SingleAutomaton.Status[curState].successRelation.second;
		if (SingleAutomaton.TerminalState.find(curState) != SingleAutomaton.TerminalState.end()) {
			//run Injective Function
			std::vector<char> tmp_A = std::vector<char>(_A);
			for (int i = 1; i < SingleAutomaton.letterSize; i++) {
				if (SingleAutomaton.Status[curState].injectiveFunction[i] == invalid_data) {
					_A[i] = invalid_data;
				}
				else {
					_A[i] = tmp_A[SingleAutomaton.Status[curState].injectiveFunction[i]];
				}
				for (int i = 1; i < SingleAutomaton.letterSize; i++) {
					if (_A[i] = invalid_data) {
						ACounter = i;
						break;
					}
				}
			}
			//go to failure state
			curState = SingleAutomaton.Status[curState].failureRelation;
			//get Input
			Result.push_back(curInput - source.begin());
		}
		curInput++;
	}
}

toc::PalMatching::PalMatching(const std::string& pattern) {
	Pattern = pattern;
	_Pattern = std::vector<int>(Pattern.length(), invalid_data);
	ConstructSingleAutomaton();
}

//Multi

void toc::PalMatching::ConstructMultiVariablePattern() {
	for (int i = 0; i < MPattern.size(); i++) {
		Pattern = MPattern[i];
		_Pattern = _MPattern[i];
		ConstructVariablePattern();
		_MA.push_back(_A);
		_MB.push_back(_B);
		_MPattern[i] = _Pattern;
		_A.clear();
		_B.clear();
	}
}

void toc::PalMatching::ConstructMultiAutomaton() {
	ConstructMultiVariablePattern();
	//Initailize
	int q = MultiAutomaton.NewStatus(1);
	int ql;
	int failureJ;
	std::vector<int> p(_MPattern.size(), 0);
	for (int i = 0; i < p.size(); i++) {
		p[i] = q;
	}
	//Get Max pattern_length
	int maxPatternLength = 0;
	for (int i = 0; i < _MPattern.size(); i++) {
		if (maxPatternLength < _MPattern[i].size()) {
			maxPatternLength = _MPattern[i].size();
		}
	}
	//Start Algorithm
	for (int i = 0; i < maxPatternLength + 1; i++) {
		for (int j = 0; j < _MPattern.size(); j++) {
			if (i < _MPattern[j].size() + 1) {
				//First: add new state
				if (i < _MPattern[j].size()) {
					q = p[j];
					if (MultiAutomaton.MStatus[q].successRelation.count(_MPattern[j][i]) != 1) {
						ql = MultiAutomaton.NewStatus(_MPattern[j].size());
						MultiAutomaton.MStatus[q].successRelation[_MPattern[j][i]] = ql;
					}
					else {
						ql = MultiAutomaton.MStatus[q].successRelation[_MPattern[j][i]];
					}
				}
				//Second: add injectiveFunc to 'SECOND' state
				if (i == 1) {
					MultiAutomaton.MStatus[q].failureRelation = 0;
					MultiAutomaton.MStatus[q].injectiveFunction[0] = invalid_data;
				}
				//add injectiveFunc to another state
				else if (i > 1) {
					int _i = MFindFailureCondition(i, failureJ, j);
					MultiAutomaton.MStatus[q].failureRelation = StateForVP(failureJ, i - _i);
					for (int g = 0; g < i - _i - 1; g++) {
						MultiAutomaton.MStatus[q].injectiveFunction[_MPattern[failureJ][g]]
							= _MPattern[j][_i + g];
					}
				}
				//add state to final state
				if (i == _MPattern[j].size() - 1) {
					MultiAutomaton.TerminalState.insert(ql);
				}
				//find suffix transition;
				int max_i(0), cor_j(0);
				for (int transj = 0; transj < _MPattern.size(); transj++) {
					auto value = MFindTransitionCondition(i, transj);
					if (max_i < value) {
						max_i = value;
						cor_j = transj;
					}
				}
				if (max_i == _MPattern[cor_j].size()) {
					MultiAutomaton.MStatus[p[j]].suffixTransition = StateForVP(cor_j, _MPattern[cor_j].size());
				}
				p[j] = ql;
			}
		}
	}
	int a = 3;
}

int toc::PalMatching::MFindFailureCondition(const int index, int& refj, int sourcej) {
	intarray _iarray(_MPattern.size(), 0);
	for (int i = 0; i < _iarray.size(); i++) {
		_iarray[i] = MFindFailureConditionSub(index, i, sourcej);		
	}
	int smallest_i = INT_MAX;
	for (int i = 0; i < _iarray.size(); i++){
		if (smallest_i > _iarray[i]) {
			smallest_i = _iarray[i];
			refj = i;
		}
	}
	return smallest_i;
}

int toc::PalMatching::MFindFailureConditionSub(const int index, int j, int sourcej) {
	int _i = 1;
	std::string picker;
	std::string dump_Pattern;
	intarray leftPals, rightPals;
	while (_i <= index) {
		picker = MPattern[j].substr(0, index - _i + 1);
		dump_Pattern = std::string(picker.length() * 2 - 1, dump_char);
		for (int i = 0; i < picker.length(); i++) {
			dump_Pattern[2 * i] = picker[i];
		}
		leftPals = intarray(Pals(dump_Pattern));
		picker = MPattern[sourcej].substr(_i, index - _i + 1);
		dump_Pattern = std::string(picker.length() * 2 - 1, dump_char);
		for (int i = 0; i < picker.length(); i++) {
			dump_Pattern[2 * i] = picker[i];
		}
		rightPals = intarray(Pals(dump_Pattern));
		if (leftPals.size() == rightPals.size() && 
			std::equal(leftPals.begin(), leftPals.end(), rightPals.begin())) {
			return _i;
		}
		else {
			_i++;
		}
	}
	return _i;
}

int toc::PalMatching::MFindTransitionCondition(const int index, int refj) {
	int _i = index;
	std::string picker;
	std::string dump_Pattern;
	intarray leftPals, rightPals;
	while (_i > 0) {
		picker = MPattern[refj].substr(0, _i + 1);
		dump_Pattern = std::string(picker.length() * 2 - 1, dump_char);
		for (int i = 0; i < picker.length(); i++) {
			dump_Pattern[2 * i] = picker[i];
		}
		leftPals = intarray(Pals(dump_Pattern));
		picker = MPattern[refj].substr(index - _i, _i + 1);
		dump_Pattern = std::string(picker.length() * 2 - 1, dump_char);
		for (int i = 0; i < picker.length(); i++) {
			dump_Pattern[2 * i] = picker[i];
		}
		rightPals = intarray(Pals(dump_Pattern));
		if (leftPals.size() == rightPals.size() && 
			std::equal(leftPals.begin(), leftPals.end(), rightPals.begin())) {
			return _i;
		}
		else {
			_i--;
		}
	}
	return 0;
}

int toc::PalMatching::StateForVP(int j, int _i) {
	int q = 0;
	for (int i = 0; i < _i; i++) {
		auto search = MultiAutomaton.MStatus[q].successRelation.find(_MPattern[j][i]);
		if (search != MultiAutomaton.MStatus[q].successRelation.end()) {
			q = search->second;
		}
	}
	return q;
}

toc::PalMatching::PalMatching(const std::vector<std::string>& patterns) {
	for each (std::string var in patterns)	{
		MPattern.push_back(var);
		_MPattern.push_back(intarray(var.size(), invalid_data));
	}
	ConstructMultiAutomaton();
}

void toc::PalMatching::run(const std::string& target) {
	checkstring(target);
}

#pragma endregion