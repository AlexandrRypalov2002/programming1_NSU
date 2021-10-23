#include <fstream>

using namespace std;

ifstream f_in("input.txt");
ofstream f_out("output.txt");

class Animal {
protected:
	int x;
	int y;
	int d;
	int k;
	int age;
	int speed;
	int type;
	int printed_age;

public:
	Animal(int x, int y, int d, int k) : x(x), y(y), d(d), k(k), age(0), speed(1), type(1), printed_age(0) {

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
		if (!((age+1)%k) && age) {
			d++;
		}
		d %= 4;
	}

	void aging(){
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
		if (x > (n-1)) {
			x %= n;
		}
		if (y < 0) {
			y += m;
		}
		if (y >(m-1)) {
			y %= m;
		}
	}

	virtual bool die() {
		if (age == 10 || age == 15) {
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool reproduct() {
		if (age == 5 || age == 10) {
			return true;
		}
		else {
			return false;
		}
	}

	virtual void eat() = 0;
};

class Rabbit : public Animal {
public:
	Rabbit(int x, int y, int d, int k) : Animal(x, y, d, k) {
		this->speed = 1;
	}

	void eat() {
		f_out << "&";
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
	}

	bool die() {
		if (age == 15) {
			return true;
		}
		else {
			return false;
		}
	}

	void eat() {
		eaten++;
	}

	bool reproduct() {
		if (!(eaten%2) && eaten) {
			eaten = 0;
			return true;
		}
		else {
			return false;
		}
	}

	int get_eaten() {
		return eaten;
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

};

class Simulation {
	int N;
	int M;
	int T;
	int r;		//number of rabbits
	int w;		//number of wolves
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
			if (zoo.get_animal(n).reproduct()) {
				if (zoo.get_animal(n).get_type() == 1) {
					zoo.append(new Rabbit(zoo.get_animal(n).get_x(), zoo.get_animal(n).get_y(), zoo.get_animal(n).get_d(), zoo.get_animal(n).get_k()));
				}
				else if (zoo.get_animal(n).get_type() == 2) {
					zoo.append(new Wolf(zoo.get_animal(n).get_x(), zoo.get_animal(n).get_y(), zoo.get_animal(n).get_d(), zoo.get_animal(n).get_k(), zoo.get_animal(n).get_printed_age()));
				}
			}
			n = n->next;
		}
	}

	void Feed() {
		Node* n = zoo.get_head();
		if (n == nullptr) {
			return;
		}
		while (n != nullptr) {
			if (zoo.get_animal(n).get_type() == 1) {
				n = n->next;
				continue;
			}
			if (zoo.get_animal(n).get_type() == 2) {
				Node* p = zoo.get_head();
				while (p != nullptr) {
					if (zoo.get_animal(p).get_type() == 1 && zoo.get_animal(p).get_x() == zoo.get_animal(n).get_x() && zoo.get_animal(p).get_y() == zoo.get_animal(n).get_y()) {
						Node* h = p;
						p = p->next;
						zoo.delete_from_list(h);
						zoo.get_animal(n).eat();
					}
					else {
						p = p->next;
					}
				}
				n = n->next;
			}
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