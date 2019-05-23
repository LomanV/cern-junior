#include <QMainWindow>
#include <QApplication>
#include <vector>
#include <cmath>

#include <unistd.h> // temporary

#include "acceleratorwidgetgl.h"

int main(int argc, char* argv[]){
	QApplication a(argc, argv);

	AcceleratorWidgetGL w(nullptr, Vector3D(3,2,0));

	const double r(0.5);
	const double k(1.0);

	const double B(5.89158);
	const double b(1.2);

	//*******************
	/* w.addFodoCell(r, b, 1.0, Vector3D(3,-2)); */
	/* w.addDipole(r,k,B, Vector3D(2,-3)); */
	/* w.addFodoCell(r, b, 1.0, Vector3D(-2,-3)); */
	/* w.addDipole(r,k,B, Vector3D(-3,-2)); */
	/* w.addFodoCell(r, b, 1.0, Vector3D(-3,2)); */
	/* w.addDipole(r,k,B, Vector3D(-2,3)); */
	/* w.addFodoCell(r, b, 1.0, Vector3D(2,3)); */
	/* w.addDipole(r,k,B); */
	//*******************

	w.buildPolygon(
		Vector3D(0,0,0),
		4,
		2.0,
		0.1,
		1.0,
		1.0
	);

	w.addCircularBeam(Proton(Vector3D(3,1.5), 2.0, Vector3D(1,1)), 1e3, 1.0);
	w.initialize();

	w.show();

	return a.exec();
}
