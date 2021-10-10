#include <fstream>
#include <iostream>
#include <cstdio>

using namespace std;

ifstream f_in("input.txt");
ofstream f_out("output.txt");

class Matrix {
	int s;
	int** matrix;

	void memory_alloc(int size) {
		this->matrix = new int* [size];
		for (int i = 0; i < size; i++) {
			this->matrix[i] = new int[size];
		}
	}

	void delete_matrix() {
		for (int i = 0; i < this->s; i++) {
			delete[] this->matrix[i];
		}
		delete[] this->matrix;
	}

	void copy_matrix(const Matrix& orig) {
		this->s = orig.s;
		memory_alloc(orig.s);
		for (int i = 0; i < this->s; i++) {
			for (int j = 0; j < this->s; j++) {
				this->matrix[i][j] = orig.matrix[i][j];
			}
		}
	}

public:
	Matrix(): s(0), matrix(nullptr) {	//constructor by default
		
	}

	Matrix(const Matrix& orig) {
		this->copy_matrix(orig);
	}

	~Matrix() {
		this->delete_matrix();
	}

	Matrix(int size) {		//constructor to create matrix with 1 on main diagonal
		this->s = size;
		memory_alloc(this->s);
		for (int i = 0; i < s; i++) {
			for (int j = 0; j < s; j++) {
				matrix[i][j] = (i == j);
			}
		}
	}

	Matrix(int size, int* arr) {		//constructor to create matrix with array of integers on main diagonal
		this->s = size;
		memory_alloc(this->s);
		for (int i = 0; i < s; i++) {
			for (int j = 0; j < s; j++) {
				if (i == j) {
					matrix[i][j] = arr[i];
				}
				else {
					matrix[i][j] = 0;
				}
			}
		}
	}

	friend istream& operator>>(istream& input, const Matrix& m) {
		for (int i = 0; i < m.s; i++) {
			for (int j = 0; j < m.s; j++) {
				input >> m.matrix[i][j];
			}
		}
		return input;
	}

	friend ostream& operator <<(ostream& output, const Matrix& m) {
		for (int i = 0; i < m.s; i++) {
			for (int j = 0; j < m.s; j++) {
				output << m.matrix[i][j] << " ";
			}
			output << endl;
		}
		return output;
	}

	Matrix operator+ (const Matrix& m) {
		if (this->s != m.s) {
			f_out << "Can't count sum of matrixes with different sizes";
			exit(0);
		}
		else {
			Matrix sum(s);
			for (int i = 0; i < s; i++) {
				for (int j = 0; j < s; j++) {
					sum.matrix[i][j] = this->matrix[i][j] + m.matrix[i][j];
				}
			}
			return sum;
		}
	}

	Matrix operator- (const Matrix& m) {
		if (this->s != m.s) {
			f_out << "Impossible to count difference of matrixes with different sizes";
			exit(0);
		}
		else {
			Matrix dif(s);
			for (int i = 0; i < s; i++) {
				for (int j = 0; j < s; j++) {
					m.matrix[i][j] *= (-1);
				}
			}
			return *this + m;
		}
	}

	Matrix operator* (const Matrix& m) {
		if (this->s != m.s) {
			f_out << "Can't multiply matrixes with different sizes";
			exit(0);
		}
		else {
			Matrix mult(s);
			for (int i = 0; i < mult.s; i++) {
				mult.matrix[i][i] = 0;
			}
			for (int i = 0; i < s; i++) {
				for (int j = 0; j < s; j++) {
					for (int k = 0; k < s; k++) {
						mult.matrix[i][j] += this->matrix[i][k] * m.matrix[k][j];
					}
				}
			}
			return mult;
		}
	}

	Matrix operator& () {	//matrix transposition
		Matrix tr(this->s);
		for (int i = 0; i < s; i++) {
			for (int j = 0; j < s; j++) {
				tr.matrix[i][j] = this->matrix[j][i];
			}
		}
		return tr;
	}

	bool operator==(const Matrix& m) {
		if (this->s != m.s) {
			f_out << "Impossiple to compare";
			return 0;
		}
		else {
			for (int i = 0; i < s; i++) {
				for (int j = 0; j < s; j++) {
					if (this->matrix[i][j] != m.matrix[i][j]) {
						return 0;
					}
				}
			}
		}
		return 1;
	}

	bool operator!= (const Matrix& m) {
		if (this->s != m.s) {
			f_out << "Impossible to compare";
			return 1;
		}
		else {
			if (*this == m) {
				return 0;
			}
		}
		return 1;
	}

	Matrix& operator=(const Matrix& m) {
		if (this != &m) {
			this->delete_matrix();
			this->copy_matrix(m);
		}
		return *this;
	}

	Matrix operator() (int str, int col) {	//create minor
		Matrix minor(this->s - 1);
		int m_str = 0;
		int m_col = 0;
		for (int i = 0; i < this->s; i++) {
			if (i == str) {
				continue;
			}
			for (int j = 0; j < this->s; j++) {
				if (j == col) {
					continue;
				}
				minor.matrix[m_str][m_col] = this->matrix[i][j];
				m_col++;
			}
			m_str++;
			m_col = 0;
		}
		return minor;
	}

	int* operator[](int str) {
		int* res = new int[this->s];
		for (int i = 0; i < this->s; i++) {
			res[i - 1] = this->matrix[str][i];
		}
		return res;
	}
};

void main() {
	int size; //size of matrix
	int k_diag; //values on main diagonal of matrix K
	if (f_in.is_open()) {
		f_in >> size >> k_diag;
		int* k = new int[size];
		if (k) {
			for (int i = 0; i < size; i++) {
				k[i] = k_diag;
			}
		}
		Matrix K(size, k);
		Matrix A(size);
		Matrix B(size);
		Matrix C(size);
		Matrix D(size);
		f_in >> A;
		f_in >> B;
		f_in >> C;
		f_in >> D;
		f_out << ((A + B * (&C) + K) * (&D));
		f_in.close();
		f_out.close();
	}
}
