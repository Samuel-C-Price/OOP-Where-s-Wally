
#pragma once
#include "Matrix.h"
#include "LargeImage.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

template <class T>
class MatchImage : public Matrix<T> {
	/*All signatures which will be coded below*/
private:
	//double normalised_correlation;
	int bestMatchRow, bestMatchCol, matrix_item_count;
	/* T values to store template variables, due to the elements stored in wally. */
	//nc variables
	T ncTotal;
	T wallySum;
	T wallyMean;
	T largeSum;
	T largeMean;
	T wallyXwally;
	T largeXlarge;
	T NC;
	T normalised_correlation_result;
	T nc_bestMatch;
	//ssd variables. 
	T diff;
	T sqr;
	T ssd_Total;
	T ssd_BestMatch;
	//Temp Matrix for NC.
	Matrix<T> TmpWallyImg;//tempMatrix to hold values of (wally item - wally mean).
	Matrix<T> TmpLargeImg;
public:
	MatchImage(const char *filename, int rows, int cols);
	~MatchImage();

	//method to calculate the difference. search matrix will be the large image. 
	void  ssd(Matrix<T>&searchMatrix);
	void calculate_normalised_correlation(Matrix<T>&search_matrix, int start_row, int start_column);
};

using namespace std;
//Constructor which holds wally.
template <class T>
MatchImage<T>::MatchImage(const char *filename, int rows, int cols) :Matrix<T>(filename, rows, cols), TmpWallyImg(rows, cols), TmpLargeImg(rows, cols)
{
	this->bestMatchCol = 0;
	this->bestMatchRow = 0;
	this->matrix_item_count = rows * cols;

	wallySum = this->sum(0, 0, this->getNumRows(), this->getNumCols());
	wallyMean = this->average(0, 0, this->getNumRows(), this->getNumCols());

	/*Code below will copy existing data which has been loaded into the tempMatric variable*/
	for (int row = 0; row < this->getNumRows(); ++row)
	{
		for (int col = 0; col < this->getNumCols(); ++col)
		{
			this->TmpWallyImg.setItem(row, col, (this->getItem(row, col) - wallyMean));/*wally item - mean will be stored in tempMatrix*/
			//wally times wally to then be used in normalised correlation method
			wallyXwally += this->TmpWallyImg.getItem(row, col) * this->TmpWallyImg.getItem(row, col);
		}
	}
	/*Code below will create tempMatrix to hold part of the larger image but will be the same size as wally.*/
	for (int row = 0; row < this->getNumRows(); ++row)
		for (int col = 0; col < this->getNumCols(); ++col)
			this->TmpLargeImg.setItem(row, col, 0);/*Values set to 0, values will be added in NC method*/

}
template <class T>
void MatchImage<T>::ssd(Matrix<T>&searchMatrix)
{
	//method to check the difference, and return, commented code on .cpp 
	ssd_BestMatch = 256 * 256 * this->getNumRows() * this->getNumCols();//Value set to highest value possible. Used for checking match values.
																		//Two outer for oops used to calcualte the diff, but not calculate over the edge of search_matrix. 
	for (int oRow = 0; oRow < searchMatrix.getNumRows() - this->getNumRows(); oRow++)
	{
		cout << "*";
		for (int oCol = 0; oCol < searchMatrix.getNumCols() - this->getNumCols(); oCol++)
		{
			ssd_Total = 0;
			//inner loops which select each item for the calculation to find the difference. 
			for (int row = 0; row < this->getNumRows(); ++row)
			{
				for (int col = 0; col < this->getNumCols(); ++col)
				{
					//use diff variabe to calculate the differene of wally(item) - search_matrix(item)
					//once difference is calculated, square the diff (difference * difference), then values need to be added to ssd_Total. 
					diff = this->getItem(row, col) - searchMatrix.getItem(oRow + row, oCol + col);
					sqr = diff * diff;
					ssd_Total = ssd_Total + sqr;
				}
			}
			//if statement needed to check whether the ssd_Total is smaller than the previous total. 
			//if new total is smaller, then store new value, which will then be printed later on. 
			if (ssd_Total < ssd_BestMatch)
			{
				ssd_BestMatch = ssd_Total;
				bestMatchRow = oRow;
				bestMatchCol = oCol;
			}
		}
	}
	cout << endl;

	cout << "Best Match Found:" << fixed << setprecision(0) << ssd_BestMatch << endl;
	cout << "Best Match Row  :" << fixed << setprecision(0) << bestMatchRow << endl;
	cout << "Best Match Col  :" << fixed << setprecision(0) << bestMatchCol << endl;
	//best row = 417 , best col = 211.
	searchMatrix.fillMatrix(bestMatchRow, bestMatchCol);
}
template <class T>
void MatchImage<T>::calculate_normalised_correlation(Matrix<T>&search_matrix, int start_row, int start_column)
{ 
	nc_bestMatch = 256 * 256 * 49 * 36;
	largeSum = search_matrix.sum(0, 0, search_matrix.getNumCols(), search_matrix.getNumRows());
	largeMean = search_matrix.average(0, 0, search_matrix.getNumCols(), search_matrix.getNumRows());//Mean of the large image.
	for (int outrow = 0; outrow < search_matrix.getNumRows() - this->getNumRows(); ++outrow)
	{
		cout << "*";//counter for loops
		for (int outcol = 0; outcol < search_matrix.getNumCols() - this->getNumCols(); ++outcol)
		{			
					for (int row = 0; row < this->getNumRows(); ++row)
					{
						for (int col = 0; col < this->getNumCols(); ++col)
						{
							this->TmpLargeImg.setItem(row,col, search_matrix.getItem(outrow, outcol) - largeMean);//original large values - mean then to be set in temporary matrix. 
							largeXlarge += this->TmpLargeImg.getItem(row, col) * this->TmpLargeImg.getItem(row, col);//Calculation of large image time itself.
							NC += this->TmpWallyImg.getItem(row, col) * this->TmpLargeImg.getItem(row, col);//total of temp 1 * temp 2
						}
					}
			//End result equals  : top calcualtion / bottom calculation.
			normalised_correlation_result = NC / std::sqrt(wallyXwally * largeXlarge);
			
			/*If statement to hold the lowest value*/
			if (normalised_correlation_result < nc_bestMatch)
			{
				nc_bestMatch = normalised_correlation_result;
				bestMatchRow = outrow;
				bestMatchCol = outcol;
			}
			
		}
	}	
	cout << endl;
	cout << "NC Result = " << nc_bestMatch << endl;
	cout << "Best Match Row = " << bestMatchRow << endl;
	cout << "Best Match Col = " << bestMatchCol << endl;
	search_matrix.fillMatrix(bestMatchRow, bestMatchCol);//saving pgm file.

}

template <class T>
MatchImage<T>::~MatchImage()
{
	//Destructor made, no code used due to errors. 
}
