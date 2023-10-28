#include "Vector.h"
#include "Utils.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>

Vector::Vector(int d) {
	dimension = d;
	data = (float*)malloc(sizeof(float) * dimension);
}

Vector::Vector(const Vector& copy) {
	dimension = copy.dimension;
	data = (float*)calloc(dimension, sizeof(float));
	for (int i = 0; i < dimension; i++) {
		data[i] = copy.data[i];
	}
}

Vector::~Vector() {
	free(data);
}

void Vector::print() {
	std::cout << std::fixed << std::setprecision(4);
	for (int i = 0; i < dimension; i++) {
		std::cout << data[i] << "\n\n";
	}
	std::cout << "\n\n\n\n";
}

void Vector::randomize() {
	for (int i = 0; i < dimension; i++) {
		data[i] = (float)rand() / RAND_MAX;
	}
}

void Vector::fill(float val) {
	for (int i = 0; i < dimension; i++) {
		data[i] = val;
	}
}

void Vector::loadFromIDX(const char* path, int index) {
	int fileDimension;
	int tmpCount;
	int count = 1;
	int dimension = 1;
	unsigned char temp;
	std::ifstream file(path, std::ios::binary);
	if (file.is_open()) {
		file.read((char*)&fileDimension, sizeof(fileDimension));
		fileDimension = fileDimension >> 24;
		for (int i = 0; i < fileDimension - 1; i++) {
			file.read((char*)&tmpCount, sizeof(tmpCount));
			tmpCount = matModUtils::reverseInt(tmpCount);
			count *= tmpCount;
		}
		file.read((char*)&dimension, sizeof(dimension));
		dimension = matModUtils::reverseInt(dimension);
		assert(index <= count);
		assert(this->dimension == dimension);
		file.seekg((dimension * index) + 4 + (4*fileDimension), file.beg);
			for (int i = 0; i < dimension; i++) {
				file.read((char*)&temp, sizeof(temp));
				data[i] = (float)temp;
			}
	}
	else {
		throw std::runtime_error("File not found");
	}
}

void Vector::loadFromIDX2D(const char* path, int index) {
	int dimension;
	int tmpCount;
	int count = 1;
	int rows = 1;
	int columns = 1;
	unsigned char temp;
	std::ifstream file(path, std::ios::binary);
	if (file.is_open()) {
		file.read((char*)&dimension, sizeof(dimension));
		//magicNum = reverseInt(magicNum);
		dimension = dimension >> 24;
		for (int i = 0; i < dimension - 2; i++) {
			file.read((char*)&tmpCount, sizeof(tmpCount));
			tmpCount = matModUtils::reverseInt(tmpCount);
			count *= tmpCount;
		}
		file.read((char*)&rows, sizeof(rows));
		rows = matModUtils::reverseInt(rows);
		if (!(dimension < 2)) {
			file.read((char*)&columns, sizeof(columns));
			columns = matModUtils::reverseInt(columns);
		}
		assert(index <= count);
		assert(this->dimension == rows * columns);
		file.seekg((columns * rows * index) + 4 + (4 * dimension), file.beg);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				file.read((char*)&temp, sizeof(temp));
				data[j + (i * columns)] = (float)temp;
			}
		}
	}
	else {
		throw std::runtime_error("File not found");
	}
}

void Vector::writeToIDX(const char* path) {
	int magicNumber = 0xd01;
	std::ofstream file(path, std::ios::binary);
	file.write((char*)&dimension, sizeof(dimension));
	for (int i = 0; i < dimension; i++) {
		file.write((char*)&data[i], sizeof(data[i]));
	}
}

void Vector::bubbleSort() {
	for (int i = 1; i < dimension; i++) {
		int swap = false;
		for (int j = 0; j < dimension - i; j++) {
			if (data[j] > data[j + 1]) {
				std::swap(data[j], data[j + 1]);
				swap = true;
			}
		}
		if (!swap) {
			return;
		}
	}
	return;
}

float& Vector::operator[](const int index) {
	float val = data[index];
	return val;
}

Vector& Vector::operator=(const Vector& rhs) {
	if (this == &rhs) {
		return *this;
	}
	free(data);
	dimension = rhs.dimension;
	data = (float*)calloc(dimension, sizeof(float));
	for (int i = 0; i < dimension; i++) {
		data[i] = rhs.data[i];
	}
	return *this;
}

Vector& Vector::operator+=(const Vector& rhs) {
	assert(dimension == rhs.dimension);
	for (int i = 0; i < dimension; i++) {
		data[i] += rhs.data[i];
	}
	return *this;
}

Vector& Vector::operator-=(const Vector& rhs) {
	assert(dimension == rhs.dimension);
	for (int i = 0; i < dimension; i++) {
		data[i] -= rhs.data[i];
	}
	return *this;
}

Vector Vector::operator+(const Vector& rhs) {
	return Vector(*this) += rhs;
}

Vector Vector::operator-(const Vector& rhs) {
	return Vector(*this) -= rhs;
}

float Vector::dot(const Vector& rhs) {
	assert(dimension == rhs.dimension);
	float out = 0;
	for (int i = 0; i < dimension; i++) {
		out += data[i] * rhs.data[i];
	}
	return out;
}

Vector Vector::operator*(const int& rhs) {
	Vector out(dimension);
	for (int i = 0; i < dimension; i++) {
		out.data[i] = data[i] * rhs;
	}
	return out;
}

Vector Vector::operator*(const float& rhs) {
	Vector out(dimension);
	for (int i = 0; i < dimension; i++) {
		out.data[i] = data[i] * rhs;
	}
	return out;
}

Vector operator*(const int& lhs, const Vector& rhs) {
	Vector out(rhs.dimension);
	for (int i = 0; i < rhs.dimension; i++) {
		out.data[i] = rhs.data[i] * lhs;
	}
	return out;
}

Vector operator*(const float& lhs, const Vector& rhs) {
	Vector out(rhs.dimension);
	for (int i = 0; i < rhs.dimension; i++) {
		out.data[i] = rhs.data[i] * lhs;
	}
	return out;
}
