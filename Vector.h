#pragma once

class Vector
{
public:
	int dimension;
	float* data;
	Vector(int d);
	Vector(const Vector& copy);
	~Vector();
	void print();
	void randomize();
	void fill(float val);
	void loadFromIDX(const char* path, int index);
	void loadFromIDX2D(const char* path, int index);
	void writeToIDX(const char* path);
	float& operator[](const int index);
	Vector& operator=(const Vector& rhs);
	Vector& operator+=(const Vector& rhs);
	Vector& operator-=(const Vector& rhs);
	Vector operator+(const Vector& rhs);
	Vector operator-(const Vector& rhs);
	float dot(const Vector& rhs);
	Vector operator*(const int& rhs);
	Vector operator*(const float& rhs);
	friend Vector operator*(const int& lhs, const Vector& rhs);
	friend Vector operator*(const float& lhs, const Vector& rhs);
};

