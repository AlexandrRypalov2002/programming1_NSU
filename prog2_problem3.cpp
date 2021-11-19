#include <fstream>
#include <iostream>

using namespace std;

ifstream f_in("input.txt");
ofstream f_out("output.txt");

template<typename K, typename V>
struct Pair {
	K key;
	V value;
	Pair<K, V>* next;

	Pair(K key, V value): key(key), value(value), next(nullptr) {

	}

	bool operator==(const Pair<K, V>& p) {
		return(this->key == p.key && this->value == p.value);
	}

	K get_key() {
		return key;
	}

	V& get_value() {
		return value;
	}
};

template<typename K, typename V>
class List {
	Pair<K, V>* head;
	int len;

	Pair<K, V>* get_previous(Pair<K, V>* that) {
		if (is_empty() || that == head) {
			return nullptr;
		}
		Pair<K, V>* t = head;
		while (t->next != that) {
			t = t->next;
		}
		return t;
	}

public:
	List() {
		head = nullptr;
		len = 0;
	}

	List(const List<K, V>& that) {
		copy_list(that);
	}

	void copy_list(const List<K, V>& that) {
		len = 0;
		if (that.head) {
			this->append(that.head->get_key(), that.head->get_value());
			if (!(that.head->next)) {
				return;
			}
		}
		else {
			return;
		}
		if (that.head->next) {
			Pair<K, V>* go = that.head->next;
			while (go->next != nullptr) {
				this->append(go->get_key(), go->get_value());
				go = go->next;
			}
		}
	}

	List<K, V>& operator=(const List<K, V>& that) {
		this->clear_list();
		this->copy_list(that);
	}

	bool is_empty() {
		return head == nullptr;
	}

	bool operator!=(const List<K, V>& l) {
		return !(this->head == l.head);
	}

	V& operator[](int i) {
		if (i >= len) {
			throw "Error";
		}
		else {
			Pair<K, V>* go = head;
			for (int j = 0; j < i; j++) {
				go = go->next;
			}
			return go->get_value();
		}
	}

	void clear_list() {
		Pair<K, V>* p = head;
		if (p == nullptr) {
			return;
		}
		while (p != nullptr) {
			Pair<K, V>* a = p;
			p = p->next;
			delete a;
		}
	}

	~List() {
		this->clear_list();
	}

	bool append(K k, V v) {
		Pair<K, V>* p = new Pair<K, V>(k, v);
		if (head == nullptr) {
			head = p;
			len++;
			return true;
		}
		else {
			if (head->get_key() == k) {
				head->value = v;
				return false;
			}
			Pair<K, V>* go = head;
			while (go->next != nullptr) {
				if (go->get_key() == k) {
					go->value = v;
					return false;
				}
				go = go->next;
			}
			go->next = p;
			len++;
			return true;
		}
	}

	void remove(K k) {
		if (is_empty()) {
			return;
		}
		if (head->key == k) {
			Pair<K, V>* p = head->next;
			delete head;
			if (p) {
				head = p;
			}
			else {
				head = nullptr;
			}
			len--;
			return;
		}
		Pair<K, V>* prev = nullptr;
		Pair<K, V>* current = head;
		while (current != nullptr && current->key != k) {
			prev = current;
			current = current->next;
		}
		if (current == nullptr) {
			return;
		}
		Pair<K, V>* p = current->next;
		delete current;
		prev->next = p;
		len--;
	}

	Pair<K, V>* get_head() {
		return head;
	}

	Pair<K, V>* search_pair(K key) {
		Pair<K, V>* p = head;
		while (p->get_key() != key && p != nullptr) {
			p = p->next;
		}
		return p;
	}

	int get_len() {
		return len;
	}

};

template<typename K, typename V>
class HashMap {
	List<K, V>* items;
	size_t size = 20;
	size_t size_ne;		//number of not empty lists in items
	float overflow;

	void add_to_items(List<K, V>* array, size_t s, K key, V value) {
		size_t index = get_hash(key);
		if (array[index].append(key, value)) {
			size_ne++;
		}
	}

public:
	
	HashMap(float overflow): size_ne(0) {
		items = new List<K, V>[size];
		this->overflow = overflow;
	}

	~HashMap() {
		delete[] items;
	}

	size_t get_hash(K key) {
		std::hash<K> hasher;
		return hasher(key) % size;
	}

	void rehash() {
		List<K, V>* new_list = new List<K, V>[size * 2];
		size_t t = size;
		size *= 2;
		for (int i = 0; i < t; ++i) {
			if (items[i].is_empty()) {
				continue;
			}
			for (int j = 0; j < items[i].get_len(); j++) {
				Pair<K, V>* p = items[i].get_head();
				size_t a = get_hash(p->get_key());
				add_to_items(new_list, size, p->get_key(), p->get_value());
				items[i].remove(p->get_key());
			}
		}
		delete[] items;
		items = new_list;
	}

	void add(K key, V value) {
		if (float(size_ne) / size >= overflow) {
			rehash();
		}
		add_to_items(items, size, key, value);
	}

	void remove(K key) {
		size_t r = get_hash(key);
		if (!items[r].is_empty()) {
			items[r].remove(key);
			size_ne--;
		}
	}

	int get_size() {
		return size;
	}

	List<K, V>& operator[] (int i) {
		return items[i];
	}

	class Iterator {
		size_t size;
		size_t position;
		HashMap<K, V>* table;
		Pair < K, V>* pair;

	public:

		Iterator(): table(nullptr), pair(nullptr), size(-1), position(-1){}

		Iterator(HashMap<K, V>* table, Pair<K, V>* pair, size_t size, size_t position): table(table), pair(pair), size(size), position(position){}

		bool operator!= (const Iterator& end) {
			return(!(pair == end.pair));
		}

		Iterator& operator++() {
			if (pair->next != nullptr) {
				pair = pair->next;
				return *this;
			}
			else {
				int i = 1;
				while (position + i <= size) {
					if (position + i == size) {
						*this = Iterator();
						return *this;
					}
					List<K, V> l = (*table)[position + i];
					if (l.is_empty()) {
						i++;
						continue;
					}
					else {
						pair = (*table)[position + i].get_head();
						this->position = position + i;
						return *this;
					}
				}
			}
		}

		Pair<K, V>* operator->() {
			if (size != -1) {
				return pair;
			}
		}
	};

	HashMap<K, V>::Iterator begin() {
		for (size_t i = 0; i <= size; i++) {
			if (i == size) {
				return HashMap<K, V>::Iterator();
			}
			if (!(items[i].is_empty())) {
				return HashMap<K, V>::Iterator(this, items[i].get_head(), size, i);
			}
		}
	}

	HashMap<K, V>::Iterator end() {
		return Iterator();
	}

	HashMap<K, V>::Iterator search(K key) {
		for (typename HashMap<K, V>::Iterator iter = this->begin(); iter != this->end(); ++iter) {
			if (iter->get_key() == key) {
				return iter;
			}
		}
	}
};

template<typename K, typename V>
void list() {
	HashMap<K, V> map(0.5);
	int n;
	f_in >> n;
	char to_do;
	K key;
	for (int i = 0; i < n; i++) {
		f_in >> to_do >> key;
		if (to_do == 'A') {
			V value;
			f_in >> value;
			map.add(key, value);
		}
		else if (to_do == 'R') {
			map.remove(key);
		}
	}
	int len = 0;
	int unique_number = 0;
	int recent = 0;
	for (int i = 0; i < map.get_size(); i++) {
		if (map[i].is_empty()) {
			continue;
		}

		len += map[i].get_len();
		unique_number += map[i].get_len();
		for (int a = 0; a < map[i].get_len(); a++) {
			for (int b = a + 1; b < map[i].get_len(); b++) {
				if (map[i][a] == map[i][b]) {
					unique_number--;
				}
			}
		}

		for (int j = i+1; j < map.get_size(); j++) {
			if (map[j].is_empty()) {
				continue;
			}

			for (int a = 0; a < map[i].get_len(); a++) {
				for (int b = 0; b < map[j].get_len(); b++) {
					if (map[i][a] == map[j][b]) {
						unique_number--;
					}
				}
			}

			if (unique_number < 0) {
				unique_number = 1;
			}
		}
	}
	f_out << len << ' ' << unique_number << endl;
	for (typename HashMap<K, V>::Iterator iter = map.begin(); iter != map.end(); ++iter) {
		f_out << iter->get_key() << ' ' << iter->get_value() << endl;
	}
}

template<typename K>
void check_type_of_value(char value) {
	if (value == 'I') {
		list<K, int>();
	}
	if (value == 'S') {
		list<K, std::string>();
	}
	if (value == 'D') {
		list<K, double>();
	}
}

int main() {
	char type_of_key;
	char type_of_value;
	f_in >> type_of_key >> type_of_value;

	if (type_of_key == 'I') {
		check_type_of_value<int>(type_of_value);
	}

	if (type_of_key == 'D') {
		check_type_of_value<double>(type_of_value);
	}

	if (type_of_key == 'S') {
		check_type_of_value<std::string>(type_of_value);
	}

	f_in.close();
	f_out.close();
}