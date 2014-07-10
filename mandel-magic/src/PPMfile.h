/*
 * PPMfile.h
 *
 *  Created on: 24/02/2014
 *      Author: molina
 */

#ifndef PPMFILE_H_
#define PPMFILE_H_

#include <fstream>

using namespace std;

struct RGBcolor {
	int R;
	int G;
	int B;

	bool operator!=(const RGBcolor& color) {
		return R != color.R || G != color.G || B != color.B;
	}

	RGBcolor() {
		R = G = B = 0;
	}

	RGBcolor(int r, int g, int b) {
		R = r;
		G = g;
		B = b;
	}

	RGBcolor(int value) {
		R = G = B = value;
	}
};

class PPMfile {
private:
	RGBcolor* matrix;
	int rows;
	int columns;

public:
	PPMfile(int rows, int colums);
	virtual ~PPMfile();
	void setValue(RGBcolor value, int row, int column);
	bool storeInFile(const char* filename);

	int getColumns() const {
		return columns;
	}

	RGBcolor* getMatrix() const {
		return matrix;
	}

	int getRows() const {
		return rows;
	}
};

#endif /* PPMFILE_H_ */
