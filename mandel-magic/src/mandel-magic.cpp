//============================================================================
// Name        : mandel-magic.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

#include "PPMfile.h"

using namespace std;

PPMfile* file;
double imagePixelSize;
double centerPointReal;
double centerPointImaginary;
int imageRows;
int imageColumns;
int maximumIterations;
int minimumBoxSize;

RGBcolor evaluatePixel(const int row, const int column) {
	int i = row;
	int j = column;
	double startReal = ((double) imageColumns * imagePixelSize) / 2.0
			- imagePixelSize / 2.0 + centerPointReal;
	double startImaginary = ((double) imageRows * imagePixelSize) / 2.0
			- imagePixelSize / 2.0 + centerPointImaginary;
	double x0 = startReal + j * imagePixelSize;
	double y0 = startImaginary + i * imagePixelSize;
	double x = 0.0;
	double y = 0.0;
	int iteration = 0;

	while (x * x + y * y < 4.0 && iteration < maximumIterations) {
		double xtemp = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = xtemp;
		iteration++;
	}
	RGBcolor color(iteration * (255.0 / (double) maximumIterations));
	file->setValue(color, j, i);

	return color;
}

void explore(int top, int left, int bottom, int right) {
	int totalRows = bottom - top + 1;
	int totalColumns = right - left + 1;
	//STEP 1: check if the boundry has the same color
	RGBcolor color = evaluatePixel(top, left);
	bool hasSameColor = true;

	//first row
	for (int i = left + 1; i <= right; ++i) {
		RGBcolor c = evaluatePixel(top, i);
		if (hasSameColor && c != color)
			hasSameColor = false;
	}

	//last row
	for (int i = left; i <= right; ++i) {
		RGBcolor c = evaluatePixel(bottom, i);
		if (hasSameColor && c != color)
			hasSameColor = false;
	}

	//first column
	for (int i = top; i <= bottom; ++i) {
		RGBcolor c = evaluatePixel(i, left);
		if (hasSameColor && c != color)
			hasSameColor = false;
	}

	//last column
	for (int i = top; i <= bottom; ++i) {
		RGBcolor c = evaluatePixel(i, right);
		if (hasSameColor && c != color)
			hasSameColor = false;
	}

	//STEP 2: condition
	if (hasSameColor) {
		//the color IS the same
		for (int i = left + 1; i <= right - 1; ++i)
			for (int j = top + 1; j <= bottom - 1; ++j)
				file->setValue(color, i, j);
	} else {
		totalRows -= 2;
		totalColumns -= 2;

		if (min(totalColumns / 2, totalRows / 2) <= minimumBoxSize) {
			//if the box is too small
			RGBcolor def(0, 255, 0);
			for (int i = left + 1; i <= right - 1; ++i)
				for (int j = top + 1; j <= bottom - 1; ++j)
					file->setValue(def, i, j);
		} else {
			top++;
			left++;
			right--;
			bottom--;

			//UP-LEFT
#pragma omp task untied
			explore(top, left, top + totalRows / 2 - 1,
					left + totalColumns / 2 - 1);

			//BOTTOM-LEFT
#pragma omp task untied
			explore(bottom - totalRows / 2 + 1, left, bottom,
					left + totalColumns / 2 - 1);

			//UP-RIGHT
#pragma omp task untied
			explore(top, right - totalColumns / 2 + 1, top + totalRows / 2 - 1,
					right);

			//BOTTOM-RIGHT
#pragma omp task untied
			explore(bottom - totalRows / 2 + 1, right - totalColumns / 2 + 1,
					bottom, right);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc < 7) {
		cout << "No enough arguments." << endl;
		return -1;
	}

	imageRows = atoi(argv[1]);
	imageColumns = atoi(argv[2]);
	imagePixelSize = atof(argv[3]);
	centerPointReal = atof(argv[4]);
	centerPointImaginary = atof(argv[5]);
	maximumIterations = atoi(argv[6]);
	minimumBoxSize = atoi(argv[7]);

	file = new PPMfile(imageRows, imageColumns);

	explore(0, 0, imageRows - 1, imageColumns - 1);

	file->storeInFile("mandelbrot.ppm");

	delete file;

	return 0;
}
