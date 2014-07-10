#include <omp.h>
#include <cstdlib>
#include <iostream>

#include "PPMfile.h"

using namespace std;


int main(int argc, char *argv[]) {
	if (argc < 6) {
		cout << "No enough arguments." << endl;
		return -1;
	}

	const int imageRows = atoi(argv[1]);
	const int imageColumns = atoi(argv[2]);
	const double imagePixelSize = atof(argv[3]);
	double centerPointReal = atof(argv[4]);
	double centerPointImaginary = atof(argv[5]);
	const int maximumIterations = atoi(argv[6]);

	PPMfile file(imageRows, imageColumns);

	//initial coordinate (center of the pixel (0, 0))
	double startReal = ((double) imageColumns * imagePixelSize) / 2.0
			- imagePixelSize / 2.0 + centerPointReal;
	double startImaginary = ((double) imageRows * imagePixelSize) / 2.0
			- imagePixelSize / 2.0 + centerPointImaginary;

	double transformation = 255.0 / (double) maximumIterations;

#pragma omp parallel
#pragma omp for
	for (int k = 0; k < imageRows * imageColumns; ++k) {
		int i = k / imageColumns;
		int j = k % imageColumns;

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
		RGBcolor color(iteration * transformation);
		file.setValue(color, j, i);

	}

	file.storeInFile("mandelbrot.ppm");

	return 0;
}
