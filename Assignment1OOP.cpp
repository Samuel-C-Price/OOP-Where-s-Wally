// Assignment1OOP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sstream> // stringstream
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <istream>
#include <iomanip>
#include "Matrix.h"
#include "MatchImage.h"
#include "LargeImage.h"

using namespace std;

int main()
{
	//Two images stored in their own class using inheritance to call matrix.h methods. 
	MatchImage<double>wally("..\\..\\Assignment1OOP\\Supporting Files\\Wally_grey.txt", 49, 36);
	LargeImage<double> clutteredScene("..\\..\\Assignment1OOP\\Supporting Files\\Cluttered_scene.txt", 768, 1024);

	cout << "*********************---SSD Nearest Neighbor Search---************************" << endl;
	wally.ssd(clutteredScene);
	cout << "#################################################################################################" << endl;
	cout << "*********************---Normalised Correlation Nearest Neighbor Search---************************" << endl;
	
	wally.calculate_normalised_correlation(clutteredScene,0,0);
	cout << "#################################################################################################" << endl;
	cout << "Saving PGM !" << endl;
	clutteredScene.saveImage("..\\..\\Assignment1OOP\\Supporting Files\\FoundWally.pgm", 255);
	
	system("pause");
	return 0;
}

