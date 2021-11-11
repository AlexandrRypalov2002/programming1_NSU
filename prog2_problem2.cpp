#include <fstream>

using namespace std;

ifstream f_in("input.txt");
ofstream f_out("output.txt");

class Animal {
protected:
	int x, y, d, k, age, speed, type, printed_age;

public:
	bool was_eaten;

	Animal(int x, int y, int d, int k) : x(x), y(y), d(d), k(k), age(0), speed(1), type(1), printed_age(0), was_eaten(false) {

	}

	void moving() {
		switch (d) {
		case 0:
			y -= speed;
			break;
		case 1:
			x += speed;
			break;
		case 2:
			y += speed;
			break;
		case 3:
			x -= speed;
			break;
		}
		if (!((age + 1) % k) && age) {
			d++;
		}
		d %= 4;
	}

	void aging() {
		age++;
	}

	int get_x() {
		return x;
	}

	int get_y() {
		return y;
	}

	int get_d() {
		return d;
	}

	int get_k() {
		return k;
	}

	int get_speed() {
		return speed;
	}

	int get_type() {
		return type;
	}

	int get_printed_age() {
		return printed_age;
	}

	void break_boundaries(int n, int m) {
		if (x < 0) {
			x += n;
		}
		if (x > (n - 1)) {
			x %= n;
		}
		if (y < 0) {
			y += m;
		}
		if (y > (m - 1)) {
			y %= m;
		}
	}

	virtual bool die() = 0;

	virtual bool ready_to_reproduct() = 0;

	virtual void eat(Animal* to_eat) = 0;

	virtual void eat_enough() = 0;

	virtual Animal* reproduction() = 0;

	void zero_age() {
		age = 0;
	}
};

class Rabbit : public Animal {
public:
	Rabbit(int x, int y, int d, int k) : Animal(x, y, d, k) {
		this->speed = 1;
		this->was_eaten = false;
	}

	void eat(Animal* to_eat) {}

	void eat_enough() {}

	bool ready_to_reproduct() {
		if (age == 5 || age == 10) {
			return true;
		}
		else {
			return false;
		}
	}

	bool die() {
		return age == 10;
	}

	Animal* reproduction() {
		Animal* a = new Rabbit(*this);
		a->zero_age();
		a->was_eaten = false;
		return a;
	}

};

class Wolf : public Animal {
protected:
	int eaten;		 //number of eaten rabbits, if 2 - reproduct
	int printed_age; //to compare ages of wolves written on 0
public:

	Wolf(int x, int y, int d, int k, int j) : Animal(x, y, d, k) {
		this->speed = 2;
		this->type = 2;
		this->eaten = 0;
		this->printed_age = j;
		this->was_eaten = false;
	}

	bool die() {
		return age == 15;
	}

	void eat(Animal* to_eat) {
		if (to_eat->get_type() == 1) {
			to_eat->was_eaten = true;
			eaten++;
		}
	}

	bool ready_to_reproduct() {
		return (!(eaten % 2) && eaten);
	}

	void eat_enough() {
		eaten = 0;
	}

	int get_eaten() {
		return eaten;
	}

	Animal* reproduction() {
		this->eaten = 0;
		Animal* a = new Wolf(*this);
		a->zero_age();
		a->was_eaten = false;
		return a;
	}
};

struct Node {
	Animal* data;
	class Node* next;

	Node(Animal* that) {
		data = that;
		next = nullptr;
	}
};

class List {
	Node* head;

	Node* previous(Node* that) {
		if (empty_list() || that == head) {
			return nullptr;
		}
		Node* t = head;
		while (t->next != that) {
			t = t->next;
		}
		return t;
	}

	void clear_list() {
		Node* t = head;
		if (t == nullptr) {
			return;
		}
		while (t != nullptr) {
			Node* a = t;
			t = t->next;
			delete a;
		}
	}

public:
	List() {
		head = nullptr;
	}

	~List() {
		this->clear_list();
	}

	bool empty_list() {
		return head == nullptr;
	}

	Animal& get_animal(Node* n) {
		return *n->data;
	}


	Node* append(Animal* to_add) {
		Node* a = new Node(to_add);
		if (head == nullptr) {
			head = a;
		}
		else {
			Node* go = head;
			while (go->next != nullptr) {
				go = go->next;
			}
			go->next = a;
		}
		return a;
	}

	Node* get_last(Node* n) {
		Node* go = head;
		if (go == nullptr) {
			return nullptr;
		}
		while (go->next != nullptr) {
			go = go->next;
		}
		return go;
	}

	Node* get_head() {
		return head;
	}

	Node* delete_from_list(Node* n) {
		if (n == nullptr) {
			return nullptr;
		}
		if (n->data->was_eaten || n->data->die()) {
			if (n == head) {
				head = head->next;
				delete n;
				return head;
			}
			Node* prev = this->previous(n);
			prev->next = n->next;
			delete n;
			return prev;
		}
	}

};

class Simulation {
	int N, M, T, r, w;	
	List zoo;

public:
	Simulation(int n, int m, int t, int r, int w) {
		this->N = n;
		this->M = m;
		this->T = t;
		this->r = r;
		this->w = w;

		for (int i = 0; i < r; i++) {
			int x, y, d, k;
			f_in >> x >> y >> d >> k;
			zoo.append(new Rabbit(x, y, d, k));
		}

		for (int j = 0; j < w; j++) {
			int x, y, d, k;
			f_in >> x >> y >> d >> k;
			zoo.append(new Wolf(x, y, d, k, j));
		}
	}

	void Move() {
		Node* n = zoo.get_head();
		if (n == nullptr) {
			return;
		}
		while (n != nullptr) {
			zoo.get_animal(n).moving();
			zoo.get_animal(n).break_boundaries(N, M);
			n = n->next;
		}
	}

	void Age() {
		Node* n = zoo.get_head();
		if (n == nullptr) {
			return;
		}
		while (n != nullptr) {
			zoo.get_animal(n).aging();
			n = n->next;
		}
	}

	void Reproduct() {
		Node* n = zoo.get_head();
		if (n == nullptr) {
			return;
		}
		while (n != nullptr) {
			if (zoo.get_animal(n).ready_to_reproduct()) {
				zoo.append(zoo.get_animal(n).reproduction());
			}
			n = n->next;
		}
	}

	void Feed() {
		Node* n = zoo.get_head();
		while (n != nullptr) {
			Node* t = zoo.get_head();
			while (t != nullptr) {
				if (t != n && zoo.get_animal(n).get_x() == zoo.get_animal(t).get_x() && zoo.get_animal(n).get_y() == zoo.get_animal(t).get_y()) {
					zoo.get_animal(n).eat(&zoo.get_animal(t));
					if (zoo.get_animal(t).was_eaten) {
						Node* h = t;
						t = t->next;
						zoo.delete_from_list(h);
						continue;
					}
				}
				t = t->next;
			}
			n = n->next;
		}
	}

	void Extinct() {
		Node* n = zoo.get_head();
		if (n == nullptr) {
			return;
		}
		while (n != nullptr) {
			if (zoo.get_animal(n).die()) {
				Node* del = n;
				n = n->next;
				zoo.delete_from_list(del);
			}
			else {
				n = n->next;
			}
		}
	}

	void Start() {
		for (int i = 0; i < this->T; i++) {
			Move();
			Feed();
			Age();
			Reproduct();
			Extinct();
		}
	}

	void Print() {
		int** f = new int* [N];
		for (int i = 0; i < N; i++) {
			f[i] = new int[M];
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				f[i][j] = 0;
			}
		}

		Node* n = zoo.get_head();
		while (n != nullptr) {
			int s = zoo.get_animal(n).get_x();
			int c = zoo.get_animal(n).get_y();
			if (zoo.get_animal(n).get_type() == 1) {
				f[s][c]++;
			}
			if (zoo.get_animal(n).get_type() == 2) {
				f[s][c]--;
			}
			n = n->next;
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (!f[i][j]) {
					f_out << "#";
				}
				else {
					f_out << f[i][j];
				}
			}
			f_out << endl;
		}

		for (int i = 0; i < N; i++) {
			delete[] f[i];
		}
		delete[] f;
	}
};

void main() {
	int n, m, t, r, w;
	f_in >> n >> m >> t >> r >> w;
	Simulation Transilvanian_Hunger(n, m, t, r, w);
	Transilvanian_Hunger.Start();
	Transilvanian_Hunger.Print();
	f_in.close();
	f_out.close();
}