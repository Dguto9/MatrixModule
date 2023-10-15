#include "Matrix.h"
#include "Vector.h"
#include "Utils.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>

Matrix::Matrix(int r, int c) {
	rows = r;
	columns = c;
	data = (float*)malloc(sizeof(float)*rows*columns);
}

Matrix::Matrix(const Matrix& copy) {
	rows = copy.rows;
	columns = copy.columns;
	data = (float*)calloc(rows * columns, sizeof(float));
	for (int i = 0; i < rows * columns; i++) {
		data[i] = copy.data[i];
	}
}

Matrix::~Matrix() {
	free(data);
}

void Matrix::print() {
	std::cout << std::fixed << std::setprecision(2);
	for (int i = 0; i < rows * columns; i++) {
		if (i % columns == 0) { std::cout << "\n\n"; }
		std::cout << data[i] << "\t";
	}
	std::cout << "\n\n\n\n";
}

void Matrix::randomize() {
	for (int i = 0; i < rows * columns; i++) {
		data[i] = (float)rand() / RAND_MAX;
	}
}

void Matrix::fill(float val) {
	for (int i = 0; i < rows * columns; i++) {
		data[i] = val;
	}
}

void Matrix::loadFromIDX(const char* path, int index) {
	int magicNum;
	int dimension;
	int tmpCount;
	int count = 1;
	int rows = 1;
	int columns = 1;
	int size = 1;
	void* temp;
	std::ifstream file(path, std::ios::binary);
	if (file.is_open()) {
		file.read((char*)&magicNum, sizeof(magicNum));
		dimension = magicNum >> 24;
		temp = calloc(1, sizeof(float));
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
		assert(this->rows == rows && this->columns == columns);
		file.seekg((columns * rows * index) + 4 + (4*dimension), file.beg);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				switch ((magicNum >> 16) & 0x000000FF) {
				case 0x08:
					file.read((char*)temp, sizeof(char));
					break;
				}
				data[j + (i * columns)] = *(float*)temp;
			}
		}
	}
	else {
		throw std::runtime_error("File not found");
	}
}

void Matrix::writeToIDX(const char* path) {
	int magicNumber = 0xd02;
	std::ofstream file(path, std::ios::binary);
	file.write((char*)&rows, sizeof(rows));
	file.write((char*)&columns, sizeof(columns));
	for (int i = 0; i < rows * columns; i++) {
		file.write((char*)&data[i], sizeof(data[i]));
	}
}

Matrix Matrix::convolve(const Matrix& kernel) {
	assert((kernel.rows % 2) && (kernel.columns % 2));
	Matrix buffer(rows, columns);
	buffer.fill(0);
	std::cout << "buffer made " << buffer.rows << ", " << buffer.columns << std::endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			for (int k = 0; k < kernel.rows; k++) {
				for (int l = 0; l < kernel.columns; l++) {
					if (!(((j + (l - ((kernel.columns - 1) / 2))) < 0) || ((j + (l - ((kernel.columns - 1) / 2))) >= columns) || ((i + (k - ((kernel.rows - 1) / 2))) < 0) || ((i + (k - ((kernel.rows - 1) / 2))) >= rows))) {
						buffer.data[j + (i * columns)] += (data[(j + (l - ((kernel.columns - 1) / 2))) + ((i + (k - ((kernel.rows - 1) / 2))) * columns)] * kernel.data[l + (k * kernel.columns)]);// (kernel.rows * kernel.columns);
						//std::cout << "buffer updated " << i << ", " << j << ", " << k << ", " << l << std::endl;
					}
				}
			}
		}
	}
	return buffer;
}

float*& Matrix::operator[](const int index) {
	float* ptr = &data[index * columns];
	return ptr;
}

Matrix& Matrix::operator=(const Matrix& rhs) {
	if (this == &rhs) {
		return *this;
	}
	free(data);
	rows = rhs.rows;
	columns = rhs.columns;
	data = (float*)calloc(rows * columns, sizeof(float));
	for (int i = 0; i < rows * columns; i++) {
		data[i] = rhs.data[i];
	}
	return *this;
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
	assert((rows == rhs.rows) && (columns == rhs.columns));
	for (int i = 0; i < rows * columns; i++) {
		data[i] += rhs.data[i];
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& rhs) {
	assert((rows == rhs.rows) && (columns == rhs.columns));
	for (int i = 0; i < rows * columns; i++) {
		data[i] -= rhs.data[i];
	}
	return *this;
}

Matrix Matrix::operator+(const Matrix& rhs) {
	return Matrix(*this) += rhs;
}

Matrix Matrix::operator-(const Matrix& rhs) {
	return Matrix(*this) += rhs;
}

Matrix Matrix::operator*(const Matrix& rhs) {
	assert(columns == rhs.rows);
	Matrix out(rows, rhs.columns);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < rhs.columns; j++) {
			out.data[j + (i * rhs.columns)] = 0.0f;
			for (int h = 0; h < columns; h++) {
				out.data[j + (i * rhs.columns)] += data[h + (i * columns)] * rhs.data[j + (h * rhs.columns)];
			}
		}
	}
	return out;
}

Matrix Matrix::operator*(const int& rhs) {
	Matrix out(rows, columns);
	for (int i = 0; i < rows * columns; i++) {
		out.data[i] = data[i] * rhs;
	}
	return out;
}

Matrix Matrix::operator*(const float& rhs) {
	Matrix out(rows, columns);
	for (int i = 0; i < rows * columns; i++) {
		out.data[i] = data[i] * rhs;
	}
	return out;
}

Matrix operator*(const int& lhs, const Matrix& rhs) {
	Matrix out(rhs.rows, rhs.columns);
	for (int i = 0; i < rhs.rows * rhs.columns; i++) {
		out.data[i] = rhs.data[i] * lhs;
	}
	return out;
}

Matrix operator*(const float& lhs, const Matrix& rhs) {
	Matrix out(rhs.rows, rhs.columns);
	for (int i = 0; i < rhs.rows * rhs.columns; i++) {
		out.data[i] = rhs.data[i] * lhs;
	}
	return out;
}

Vector Matrix::operator*(const Vector& rhs) {
	assert(columns == rhs.dimension);
	Vector out(rows);
	for (int i = 0; i < rows; i++) {
		out.data[i] = 0.0f;
		for (int h = 0; h < columns; h++) {
			out.data[i] += data[h + (i * columns)] * rhs.data[i];
		}
	}
	return out;
}

Matrix Matrix::elementWiseMultiply(const Matrix& rhs) {
	assert(rows == rhs.rows && columns == rhs.columns);
	Matrix out(rows, columns);
	for (int i = 0; i < rows * columns; i++) {
		out.data[i] = data[i] * rhs.data[i];
	}
	return out;
}

Matrix Matrix::elementWiseAdd(const Matrix& rhs) {
	assert(rows == rhs.rows && columns == rhs.columns);
	Matrix out(rows, columns);
	for (int i = 0; i < rows * columns; i++) {
		out.data[i] = data[i] + rhs.data[i];
	}
	return out;
}

