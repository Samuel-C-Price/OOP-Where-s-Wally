#pragma once
#include "MatchImage.h"
#include "Matrix.h"
#include <iostream>

using namespace std;

template <class T>
class LargeImage : public Matrix<T> {

private:
	int bestMatchRow, bestMatchCol, matrix_item_count;
public:
	LargeImage(const char *filename, int rows, int cols);
	void saveImage(const char *fileName, int Q);
	~LargeImage();
};

using namespace std;

template <class T>
LargeImage<T>::LargeImage(const char *filename, int rows, int cols) :Matrix<T>(filename, rows, cols) {
	this->bestMatchCol = 0;
	this->bestMatchRow = 0;
	this->matrix_item_count = rows * cols;
}
template <class T>
void LargeImage<T>::saveImage(const char *fileName, int Q)
{
	int index = 0;

	ofstream myfile;

	int number_of_items = this->getNumRows() * this->getNumCols();

	unsigned char *image;

	image = (unsigned char *) new unsigned char[number_of_items];

	// convert the integer values to unsigned char

	for (int row = 0; row < this->getNumRows(); row++)   // Print the table
	{
		for (int col = 0; col < this->getNumCols(); col++)
		{
			image[index] = (unsigned char)this->getItem(row, col);
			index++;
		}
	}


	myfile.open(fileName, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		cout << "Can't open file: " << fileName << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << this->getNumCols() << " " << this->getNumRows() << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), number_of_items * sizeof(unsigned char));

	if (myfile.fail()) {
		cout << "Can't write image " << fileName << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;
}
template <class T>
LargeImage<T>::~LargeImage() {}

