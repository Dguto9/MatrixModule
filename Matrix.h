#pragma once
class Vector;

class Matrix
{
public:
	int rows;
	int columns;
	float* data;
	Matrix(int r, int c);
	Matrix(const Matrix& copy);
	~Matrix();
	void print();
	void randomize();
	void fill(float val);
	void loadFromIDX(const char* path, int index);
	void writeToIDX(const char* path);
	Matrix convolve(const Matrix& kernel);
	float*& operator[](const int index);
	Matrix& operator=(const Matrix& rhs);
	Matrix& operator+=(const Matrix& rhs);
	Matrix& operator-=(const Matrix& rhs);
	Matrix operator+(const Matrix& rhs);
	Matrix operator-(const Matrix& rhs);
	Matrix operator*(const Matrix& rhs);
	Matrix operator*(const int& rhs);
	Matrix operator*(const float& rhs);
	friend Matrix operator*(const int& lhs, const Matrix& rhs);
	friend Matrix operator*(const float& lhs, const Matrix& rhs);
	Vector operator*(const Vector& rhs);
	Matrix elementWiseMultiply(const Matrix& rhs);
	Matrix elementWiseAdd(const Matrix& rhs);
};