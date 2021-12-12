#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <list>

using namespace std;

ifstream f_in("input.txt");
ofstream f_out("output.txt");

struct Table {
	vector<vector<string>> symb_table;
	int size;

	Table(int s, int t, vector<vector <int>>& from_to, string& symbols) {
		size = s;
		symb_table.resize(size);

		for (int i = 0; i < size; i++) {
			symb_table[i].resize(size);
		}

		for (int i = 0; i < t; i++) {
			symb_table[from_to[i][0]][from_to[i][1]] += symbols[i];
		}
	}

	Table(): size(-1) {}
};

class Automaton {
	int size;
	int start;
	set<int> end_states;
	Table table;
	bool is_DFA;

public:

	Automaton(int s, const int st, int t, set<int>& end_states, vector<vector<int>>& from_to, string& symbols, bool check_DFA): size(s), start(st), table(Table(s, t, from_to, symbols)), end_states(end_states), is_DFA(check_DFA) {}

	bool check_end(int state) {
		return (end_states.find(state) != end_states.end());
	}

	bool check_expression(string& expression) {
		if (is_DFA == false) {
			this->is_NFA(expression);

			if (is_DFA == false) {
				this->determinate();
			}
		}
		
		size_t count = 0;
		size_t len = expression.length();
		size_t expr_count = 0;
		int go = start;

		while (count < len) {
			for (int i = 0; i < size; i++) {
				size_t l = table.symb_table[go][i].length();

				if (l >= 1) {
					for (int j = 0; j < l; j++) {
						if (expression.at(expr_count) == (table.symb_table[go][i][j])) {
							go = i;
							expr_count++;
							break;
						}
					}

					if (expr_count >= len) {
						break;
					}
				}
				
			}

			if (expr_count >= len) {
				break;
			}

			count++;
		}

		return check_end(go);
	}

	Automaton& determinate() {
		set<char> alphabet;
		for (int i = 0; i < table.size; i++) {
			for (int j = 0; j < table.size; j++) {
				for (const auto& l : table.symb_table[i][j]) {
					alphabet.insert(l);
				}
			}
		}

		list<set<int>> sets;		//list to keep sets of automaton's states
		set<int> first{ this->start };
		sets.push_back(first);

		vector<vector<set<int>>> new_transitions;	//transitions between sets of states
		set<set<int>> form, new_end;		//sets of new states and end-states of new automaton
		form.insert(first);
		string new_symbols;		//new sequence of symbols to build new automaton

		while (!sets.empty()) {
			set<int> current(sets.front());		//set of states where symbols from alphabet can lead
			sets.pop_front();

			for (const auto& i : alphabet) {
				set<int> symb_current;		//set of states for one symbol from alphabet
				for (const auto& j : current) {
					for (int l = 0; l < table.size; l++) {
						for (const auto& g : table.symb_table[j][l]) {
							if (g == i) {
								symb_current.insert(l);
							}
						}
					}
				}

				if (!symb_current.empty()) {
					new_transitions.push_back(vector<set<int>>{current, symb_current});
					new_symbols += i;
				}

				bool is_end_state = false;
				for (const auto& a : this->end_states) {
					for (const auto& b : symb_current) {
						if (a == b) {
							new_end.insert(symb_current);
							is_end_state = true;
							break;
						}
					}
					if (is_end_state) {
						break;
					}
				}

				bool not_add_in_list = false;
				for (const auto& c : form) {
					if (symb_current == c) {
						not_add_in_list = true;
						break;
					}
				}
				if (!not_add_in_list) {
					form.insert(symb_current);
					sets.push_back(symb_current);
				}
			}
		}

		int new_size = form.size();
		int new_start = start;
		int new_transitions_len = new_transitions.size();
		set<int> new_end_states;

		for (const auto& i : new_end) {
			if (i == first) {
				new_end_states.insert(new_start);
			}
		}

		form.erase(first);
		new_end.erase(first);

		int current_count = 0;
		if (new_start == 0) {
			current_count = 1;
		}

		for (auto i = form.begin(); i != form.end(); i++) {

			for (auto& j : new_transitions) {
				if (j[0] == *i) {
					j[0] = set<int>{ current_count };
				}
				if (j[1] == *i) {
					j[1] = set<int>{ current_count };
				}
			}

			for (auto& j : new_end) {
				if (j == *i) {
					new_end_states.insert(current_count);
				}
			}

			current_count++;

		}

		vector<vector<int>> new_from_to;
		for (auto i : new_transitions) {
			auto j0 = i[0].begin();
			auto j1 = i[1].begin();
			new_from_to.push_back(vector<int>{*j0, * j1});
		}

		*this = Automaton(new_size, new_start, new_transitions_len, new_end_states, new_from_to, new_symbols, true);
		return *this;
	}

	bool is_NFA(string& symbols) {
		set<string> alphabet;
		for (const auto& i : symbols) {
			alphabet.insert(string(1, i));
		}

		for (const auto& i : alphabet) {
			int count_column = 0;

			while (count_column < size) {
				int symb_count = 0;

				for (int j = 0; j < size; j++) {
					size_t l = table.symb_table[count_column][j].length();

					if (l >= 1) {
						for (int s = 0; s < l; s++) {
							if (i == string(1, table.symb_table[count_column][j][s])){
								symb_count++;
								if (symb_count > 1) {
									return true;
								}
								break;
							}
						}
					}
				}

				count_column++;
			}
		}

		is_DFA = true;
		return false;
	}

};

int main() {
	int N, start_state, end_state, transitions, T;
	f_in >> N >> start_state >> end_state;

	set<int> end_states;
	for (int i = 0; i < end_state; i++) {
		int temp;
		f_in >> temp;
		end_states.insert(temp);
	}

	f_in >> transitions;
	vector<vector<int>> from_to(transitions);
	string symbols;

	for (int i = 0; i < transitions; i++) {
		from_to[i].resize(2);
	}

	for (int i = 0; i < transitions; i++) {
		char s;
		f_in >> from_to[i][0] >> from_to[i][1] >> s;
		symbols += s;
	}

	f_in >> T;
	string expression;
	bool read_by_automaton;
	Automaton A(N, start_state, transitions, end_states, from_to, symbols, false);

	for (int i = 0; i < T; i++) {
		f_in >> expression;
		read_by_automaton = A.check_expression(expression);
		
		if (read_by_automaton) {
			f_out << "YES\n";
		}
		else {
			f_out << "NO\n";
		}
	}

	f_in.close();
	f_out.close();
}