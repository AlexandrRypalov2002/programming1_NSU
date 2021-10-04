#include <fstream>
#include <iostream>
#include <cstdio>

using namespace std;

ifstream f_in("input.txt");
ofstream f_out("output.txt");

class Matrix {
private:
	int s;
	int** matrix;
	void delete_matrix() {
		for (int i = 1; i < this->s; i++) {
			delete[] this->matrix[i];
		}
		delete[] this->matrix;
	}

	void copy_matrix(const Matrix& orig) {
		this->s = orig.s;
		this->matrix = new int* [this->s];
		if (this->matrix) {
			for (int i = 1; i < this->s; i++) {
				this->matrix[i] = new int[this->s];
				if (this->matrix[i]) {
					continue;
				}
				else {
					f_out << "Error";
					exit(0);
				}
			}
			for (int i = 1; i < this->s; i++) {
				for (int j = 1; j < this->s; j++) {
					this->matrix[i][j] = orig.matrix[i][j];
				}
			}
		}
	}
public:
	Matrix() {				//constructor by default
		s = 0;
		matrix = nullptr;
	}

	Matrix(const Matrix& orig) {
		this->copy_matrix(orig);
	}

	~Matrix() {
		this->delete_matrix();
	}

	Matrix(int size) {		//constructor to create matrix with 1 on main diagonal
		this->s = size + 1;
		matrix = new int* [s];
		if (matrix) {
			for (int i = 1; i < s; i++) {
				matrix[i] = new int[s];
				if (matrix[i]) {
					continue;
				}
				else {
					exit(0);
				}
			}
			for (int i = 1; i < s; i++) {
				for (int j = 1; j < s; j++) {
					if (i == j) {
						matrix[i][j] = 1;
					}
					else {
						matrix[i][j] = 0;
					}
				}
			}
		}
	}

	Matrix(int size, int* arr) {		//constructor to create matrix with array of integers on main diagonal
		this->s = size + 1;
		matrix = new int* [s];
		if (matrix) {
			for (int i = 1; i < s; i++) {
				matrix[i] = new int[s];
				if (matrix[i]) {
					continue;
				}
				else {
					exit(0);
				}
			}
			for (int i = 1; i < s; i++) {
				for (int j = 1; j < s; j++) {
					if (i == j) {
						matrix[i][j] = arr[i - 1];
					}
					else {
						matrix[i][j] = 0;
					}
				}
			}
		}
	}

	void read_f() {
		for (int i = 1; i < this->s; i++) {
			for (int j = 1; j < this->s; j++) {
				f_in >> this->matrix[i][j];
			}
		}
	}

	void write_f() {
		for (int i = 1; i < this->s; i++) {
			for (int j = 1; j < this->s; j++) {
				f_out << this->matrix[i][j] << ' ';
			}
			f_out << endl;
		}
	}

	Matrix operator+ (Matrix m) {
		if (this->s != m.s) {
			f_out << "Can't count sum of matrixes with different sizes";
			exit(0);
		}
		else {
			Matrix sum(s - 1);
			for (int i = 1; i < s; i++) {
				for (int j = 1; j < s; j++) {
					sum.matrix[i][j] = this->matrix[i][j] + m.matrix[i][j];
				}
			}
			return sum;
		}
	}

	Matrix operator- (Matrix m) {
		if (this->s != m.s) {
			f_out << "Impossible to count difference of matrixes with different sizes";
			exit(0);
		}
		else {
			Matrix dif(s - 1);
			for (int i = 1; i < s; i++) {
				for (int j = 1; j < s; j++) {
					dif.matrix[i][j] = this->matrix[i][j] - m.matrix[i][j];
				}
			}
			return dif;
		}
	}

	Matrix operator* (Matrix m) {
		if (this->s != m.s) {
			f_out << "Can't multiply matrixes with different sizes";
			exit(0);
		}
		else {
			Matrix mult(s - 1);
			for (int i = 1; i < mult.s; i++) {
				mult.matrix[i][i] = 0;
			}
			for (int i = 1; i < s; i++) {
				for (int j = 1; j < s; j++) {
					for (int k = 1; k < s; k++) {
						mult.matrix[i][j] += this->matrix[i][k] * m.matrix[k][j];
					}
				}
			}
			return mult;
		}
	}

	Matrix operator& () {	//matrix transposition
		Matrix tr(this->s - 1);
		for (int i = 1; i < s; i++) {
			for (int j = 1; j < s; j++) {
				tr.matrix[i][j] = this->matrix[j][i];
			}
		}
		return tr;
	}

	bool operator==(Matrix m) {
		if (this->s != m.s) {
			f_out << "Impossiple to compare";
			return 0;
		}
		else {
			for (int i = 1; i < s; i++) {
				for (int j = 1; j < s; j++) {
					if (this->matrix[i][j] != m.matrix[i][j]) {
						return 0;
					}
				}
			}
		}
		return 1;
	}

	bool operator!= (Matrix m) {
		if (this->s != m.s) {
			f_out << "Impossible to compare";
			return 1;
		}
		else {
			for (int i = 1; i < s; i++) {
				for (int j = 1; j < s; j++) {
					if (this->matrix[i][j] == m.matrix[i][j]) {
						return 0;
					}
				}
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
		Matrix minor(this->s - 2);
		for (int i = 1; i < str; i++) {
			for (int j = 1; j < col; j++) {
				minor.matrix[i][j] = this->matrix[i][j];
			}
		}
		for (int i = str + 1; i < s; i++) {
			for (int j = 1; j < col; j++) {
				minor.matrix[i - 1][j] = this->matrix[i][j];
			}
		}
		for (int i = 1; i < str; i++) {
			for (int j = col + 1; j < s; j++) {
				minor.matrix[i][j - 1] = this->matrix[i][j];
			}
		}
		for (int i = str + 1; i < s; i++) {
			for (int j = col + 1; j < s; j++) {
				minor.matrix[i - 1][j - 1] = this->matrix[i][j];
			}
		}
		return minor;
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
		A.read_f();
		B.read_f();
		C.read_f();
		D.read_f();
		((A + B * (&C) + K) * (&D)).write_f();
		f_in.close();
		f_out.close();
	}
}
