/*
 * PPMfile.cpp
 *
 *  Created on: 24/02/2014
 *      Author: molina
 */

#include "PPMfile.h"

PPMfile::PPMfile(int rows, int columns) {
	this->rows = rows;
	this->columns = columns;
	matrix = new RGBcolor[rows * columns];
}

PPMfile::~PPMfile() {
	delete[] matrix;
}

void PPMfile::setValue(RGBcolor value, int row, int column) {
	matrix[row * rows + column] = value;
}

bool PPMfile::storeInFile(const char* filename) {
	ofstream os;
	os.open(filename, ios::trunc);
	if (os.good()) {
		os << "P3" << endl;
		os << "# " << filename << endl;
		os << rows << " " << columns << endl;

		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < columns; ++j) {
				RGBcolor& color = matrix[i * columns + j];
				os << color.R << " " << color.G << " " << color.B << " ";
			}
			os << endl;
		}

		os.close();
		return true;
	}
	return false;
}
