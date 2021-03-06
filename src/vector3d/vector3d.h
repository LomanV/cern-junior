#pragma once

#include <array> // for Vector3D::getCoords()
#include <random> // for distributions
#include <functional>

#include "../color/rgb.h"
#include "../general/drawable.h"
#include "../general/canvas.h"

class Vector3D{
	private:
		double x;
		double y;
		double z;
	public:
		// Constructor, getters, setters:
		explicit Vector3D(double a, double b, double c = 0) : x(a), y(b), z(c){}
		Vector3D(void) : Vector3D(0,0,0){}

		std::array<double,3> getCoords(void) const; // return coords in an array
		double operator[](int i) const; // overload of index operator allows easy access to individual coordinates

		// Algebraic operators:
		Vector3D operator+(Vector3D v) const; // vector addition
		Vector3D& operator+=(const Vector3D &); // for optimization purposes

		Vector3D operator-(const Vector3D &v) const; // vector subtraction
		Vector3D& operator-=(const Vector3D &v);
		Vector3D operator-(void) const; // additive inverse

		Vector3D operator*(const double &lambda) const; // scalar multiplication. note that the scalar comes AFTER the vector (i.e. u*lambda). we define lambda*u as a non-member operator
		Vector3D& operator*=(const double &lambda);

		double operator|(const Vector3D &v) const; // dot product
		Vector3D operator^(const Vector3D &v) const; // cross product

		// Zero-test and boolean operators
		bool is_zero(void) const; // returns true iff square of norm is "zero" i.e. less than a small constant
		bool operator ==(const Vector3D &v) const; // returns (*this - v).is_zero()
		bool operator !=(const Vector3D &v) const; // returns the logical negation of ==

		// Norm and distance
		double norm2(void) const; // square of Euclidian norm
		double norm(void) const; // Euclidian norm
		static double distance2(const Vector3D& u, const Vector3D& v);
		static double distance(const Vector3D& u, const Vector3D& v);

		// Various
		Vector3D normalize(void); // divides by the norm to get a unit vector and returns the result. undefined behavior if norm() == 0.0
		Vector3D unitary(void) const; // same as normalize(), but returns the result
		Vector3D rotated(Vector3D u, double alpha) const; // returns result of rotating around a given axis by a given angle angle
		Vector3D orthogonal(void) const; // returns a unitary orthogonal vector
		static double mixed_prod(const Vector3D &u, const Vector3D &v, const Vector3D &w){ return u|(v^w); }
		static bool are_orthogonal(const Vector3D &u, const Vector3D &v);

		std::ostream& print(std::ostream& output) const;
};

class RandomVector3D{
	// this class allows the creation of Vector3D-type random distributions around the origin using real distributions from the standard library
	private:
		std::function<double(std::random_device&)> distr;
	public:
		RandomVector3D(const std::function<double(std::random_device&)> &my_distr) :
			distr(my_distr)
		{}

		Vector3D operator()(std::random_device &gen); // overloaded call operator
};

class UniformVector3D : public RandomVector3D{
	// Uniformly-distributed Vector3D along 3 axes
	public:
		explicit UniformVector3D(double my_r) :
			RandomVector3D(std::uniform_real_distribution<double>(-my_r, my_r))
		{}
};

class GaussianVector3D : public RandomVector3D{
	// Gaussian-distributed Vector3D along 3 axes
	public:
		explicit GaussianVector3D(double my_sigma) :
			RandomVector3D(std::normal_distribution<double>(0.0, my_sigma))
		{}
};

Vector3D operator*(const double &, const Vector3D&); // scalar multiplication, but here the scalar is written before
std::ostream& operator<<(std::ostream& output, Vector3D const& v); // prints to output (e.g. std::cout or std::ofstream)

namespace vctr{
	// some useful vectors for general use
	const Vector3D ZERO_VECTOR(0,0,0);
	const Vector3D X_VECTOR(1,0,0);
	const Vector3D Y_VECTOR(0,1,0);
	const Vector3D Z_VECTOR(0,0,1);
}
