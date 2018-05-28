#pragma once
//Code for the base image, parent class
//Child class's will be made inheriting methods from this class. 

#include <iostream>

using namespace std;

template <class T>
class Matrix {
	//Add Constructors, destructors, method signatures, operating overloads

	private:
		int numRows, numCols;
		T mSum;
		T mSquare;

		T *ptrMatrix;
	public:
		//Constructors & Destructors
		//Matrix();
		Matrix(int rows, int cols);
		Matrix(const char *filename, int rows, int cols);
		~Matrix();

		//copy constructors
		Matrix(const Matrix<T>&sourceMatrix);
		
		//operator overloads
		Matrix<T>&operator=(const Matrix<T>&sourceMatrix);
		Matrix<T>&operator-(const Matrix<T>& sourceMatrix);
		
		//calculation method signatures
		T average(int row, int col, int height, int width);
		T sum(int row, int col, int height, int width);
		T square(int sRow, int sCol, int height, int width);
		
		// Getters 
		int getNumRows();
		int getNumCols();
		T getItem(int row, int col);

		// Setters
		void setItem(int row, int col, T value);

		// Display
		void fillMatrix(int bestMatchRow, int bestMatchCol);
		void printMatrix();
		void printSubMatrix(int row, int col, int height, int width);


};

using namespace std;
//template <class T>
//Matrix<T>::Matrix()
//{
//	this->numRows = 5;
//	this->numCols = 5;
//
//	this->ptrMatrix = new (nothrow) T[this->numRows * this->numCols];
//
//	if(this->ptrMatrix == nullptr)
//		throw std::bad_alloc();  // Throw an exception at the point of failure
//	cout << "Matrix()" << endl;
//
//}
template <class T>
Matrix<T>::Matrix(int rows, int cols)
{
	this->numRows = rows;
	this->numCols = cols;
	this->ptrMatrix = new (nothrow) T[this->numRows * this->numCols]{};

	if (this->ptrMatrix == nullptr)
		throw std::bad_alloc();  // Throw an exception at the point of failure

}
template <class T>
Matrix<T>::Matrix(const char *fileName, int rows, int cols)
{
	this->numRows = rows;
	this->numCols = cols;
	this->ptrMatrix = new (nothrow) T[this->numRows * this->numCols]{};

	if (this->ptrMatrix == nullptr)
		throw std::bad_alloc();  // Throw an exception at the point of failure
	else
	{
		int i = 0;

		ifstream readFile(fileName);

		if (readFile.is_open())
		{

			while (readFile.good())
			{
				if (i > rows*cols - 1) break;
				readFile >> ptrMatrix[i];
				i++;
			}
			readFile.close();
		}
		else
			cout << "Unable to open file";
	}
}
/*
*	Deallocate the memory for the matrix
*/
template <class T>
Matrix<T>::~Matrix()
{

	if (this->ptrMatrix != nullptr) delete[] ptrMatrix;

}

template <class T>
Matrix<T>::Matrix(const Matrix<T> & sourceMatrix)
{
	numRows = sourceMatrix.numRows;
	numCols = sourceMatrix.numCols;


	this->ptrMatrix = new (nothrow) T[this->numRows * this->numCols];


	if (this->ptrMatrix == nullptr)
		throw std::bad_alloc();  // Throw an exception at the point of failure
	else
	{
		// Copy source data to destination
		for (int i = 0; i < this->numRows * this->numCols; i++)
			this->ptrMatrix[i] = sourceMatrix.ptrMatrix[i];
	}
	cout << "Copy constructor finished" << endl;
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& sourceMatrix)
{
	// If we are assigning to ourself i.e. m1 = m1
	if (this == &sourceMatrix)
		return *this;  // Return a pointer to ourselves (why copy yourself to yourself)


	numRows = sourceMatrix.numRows;
	numCols = sourceMatrix.numCols;


	// Clear any already allocated memory
	if (this->ptrMatrix != nullptr) delete[] ptrMatrix;


	// Allocate the right amount of memory for the deep copy of the data
	this->ptrMatrix = new (nothrow) T[this->numRows * this->numCols];


	if (this->ptrMatrix == nullptr)
		throw std::bad_alloc();  // Throw an exception at the point of failure
	else
	{
		// Copy source data to destination
		for (int i = 0; i < this->numRows * this->numCols; i++)
			this->ptrMatrix[i] = sourceMatrix.ptrMatrix[i];
	}
	cout << "Assignment = finished" << endl;
	// Return a pointer to the copied matrix object
	return *this;
	
}
/*
*	 - subtraction operator
*/
template <class T>
Matrix<T>& Matrix<T>::operator-(const Matrix<T>& sourceMatrix)
{
	if (this->numRows != sourceMatrix.numRows || this->numCols != sourceMatrix.numCols)
		throw(std::runtime_error("Subtraction error matrix dimensions do not match!!"));

	// Allocate the right amount of memory for the new matrix of subtracted data
	Matrix<T> newMatrix(this->numRows, this->numCols);

	for (int row = 0; row < this->numRows; ++row)   // Print the table
	{
		for (int col = 0; col < this->numCols; ++col)
			newMatrix.ptrMatrix[row * this->numCols + col] = ptrMatrix[row * this->numCols + col] - sourceMatrix.ptrMatrix[row * this->numCols + col];
	}
	// Return a matrix object uses copy constructor to do this
	return newMatrix;
}
/*
*	Returns the matrix average screen
*/
template <class T>
T Matrix<T>::average(int startRow, int startCol, int width, int height)
{
	T itemCount = height * width;
	return (T)(mSum / (T)itemCount);
}
/*
*	Returns matrix sum
*/
template <class T>
T Matrix<T>::sum(int startRow, int startCol, int width, int height)
{
	mSum = 0;
	if (this->ptrMatrix != nullptr) {
		for (int row = startRow; row < startRow + height; ++row)
		{
			for (int col = startCol; col < startCol + width; ++col)
				  mSum += this->getItem(row, col);
		}
		return mSum;
	}
	else {
		throw(std::runtime_error("Sum error occured."));
	}
}
/*
*	Square method squares each matrix element by its self returning a new matrix with the result.
*/
template <class T>
T Matrix<T>::square(int sRow, int sCol, int height, int width)
{ 
	mSquare = 0; 
	if (this->ptrMatrix != nullptr)
	{
		
		for (int row = sRow; row <  height; ++row)
		{
			for (int col = sCol; col < width; ++col)
			{
				mSquare += this->getItem(row, col) * this->getItem(row, col);
			}
		}
		return mSquare;
	}
	else {
		throw std::bad_alloc();
	}
}
/*
*	Return the number of rows in the matrix
*/
template <class T>
int Matrix<T>::getNumRows()
{
	return this->numRows;
}

/*
*	Return the number of Cols in the matrix
*/
template <class T>
int Matrix<T>::getNumCols()
{
	return this->numCols;
}

/*
*	Get a single element from the matrix
*/
template <class T>
T Matrix<T>::getItem(int row, int col)
{
	if (this->ptrMatrix != nullptr)
		return ptrMatrix[row * this->numCols + col];
	else
		throw std::bad_alloc();
}

/*
*	Set a single element in the matrix
*/
template <class T>
void Matrix<T>::setItem(int row, int col, T value)
{
	if (this->ptrMatrix != nullptr)
	{
		ptrMatrix[row * this->numCols + col] = value;
	}
	else {
		throw std::bad_alloc();
	}

}

/*
*	Fill the matrix with a single value
*/
template <class T>
void Matrix<T>::fillMatrix(int bestMatchRow, int bestMatchCol)
{
	if (this->ptrMatrix != nullptr)
	{
		for (int row = bestMatchRow; row < bestMatchRow + 49; row++)
		{
			for (int col = bestMatchCol; col < bestMatchCol + 36; col++)
			{
				this->setItem(row, col, 0);//used to fill a 49x36 grid black to shade out results.
			}
		}
	}
	else {
		throw std::bad_alloc();
	}

}


/*
*	Prints the matrix to the screen */
template <class T>
void Matrix<T>::printMatrix()
{
	if (this->ptrMatrix != nullptr)
	{
		for (int row = 0; row < numRows; ++row)
		{
			for (int col = 0; col < numCols; ++col)
			{
				cout << this->getItem(col, row) << "\t";
			}
			cout << endl;
		}
	}
	else {
		throw std::bad_alloc();
	}
}
template <class T>
void Matrix<T>::printSubMatrix(int sRow, int sCol, int height, int width)
{
	if (this->ptrMatrix != nullptr)
	{

		for (int row = sRow; row < sRow + height; ++row)
		{
			for (int col = sCol; col < sCol + width; ++col)
			{
				cout << (this->getItem(row,col))*(this->getItem(row, col)-255) << "\t";
			}
			cout << endl;
		}
	}
	else {
		throw std::bad_alloc();
	}
}

