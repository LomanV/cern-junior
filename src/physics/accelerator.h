#pragma once

#include <iostream>
#include <iterator>

#include "element.h"

class Accelerator : public Drawable, private std::vector<std::unique_ptr<Element>>{
	protected:
		std::shared_ptr<double> time;

		std::vector<std::unique_ptr<Particle>> particles;
		std::vector<Beam*> beams;

		Vector3D origin;

		double length = 0.0; // geometric length of the accelerator, i.e. length of the ideal orbit
	public:
		explicit Accelerator(Canvas* canvas, Vector3D my_origin) : Drawable(canvas), time(std::make_shared<double>(0.0)), origin(my_origin){}

		// Prohibiting copies:
		Accelerator(const Accelerator &to_copy) = delete;
		Accelerator& operator=(const Accelerator &to_copy) = delete;

		// Masking the "deletion" operators to guarantee data integrity:
		void erase(iterator) const{ throw excptn::ILLEGAL_ACCESS; }
		void erase(iterator, iterator) const{ throw excptn::ILLEGAL_ACCESS; }
		void clear(void){ throw excptn::ILLEGAL_ACCESS; }

		void weld(void);
		void activate(void);
		void initialize(void);

		void draw_elements(void) const;
		void draw_particles(void) const;
		void draw_beams(void) const;

		bool is_empty(void) const{ return empty(); }

		double getLength(void) const{ return length; }
		Vector3D getOrigin(void) const{ return origin; }

		Particle* getLastParticle(void) const{ return particles.back().get(); }

		void addParticle(const Particle &to_copy);
		void addGaussianCircularBeam(const Particle &model, uint N, double lambda, double sigma_x, double sigma_v);
		void addUniformCircularBeam(const Particle &model, uint N, double lambda, double delta_x, double delta_v);

		void addStraightSection(double radius, const Vector3D &end);
		void addDipole(double radius, double curvature, double B_0, const Vector3D &end);
		void addQuadrupole(double radius, double b, const Vector3D &end);
		void addRadiofrequencyCavity(double radius, double E_0, double omega, double kappa, double phi, const Vector3D &end);
		void addFodoCell(double radius, double b, double L, const Vector3D &end);

		void addStraightSection(double radius){ addStraightSection(radius, origin); }
		void addDipole(double radius, double curvature, double B_0){ addDipole(radius, curvature, B_0, origin); }
		void addQuadrupole(double radius, double b){ addQuadrupole(radius, b, origin); }
		void addRadiofrequencyCavity(double radius, double E_0, double omega, double kappa, double phi){ addRadiofrequencyCavity(radius, E_0, omega, kappa, phi, origin); }
		void addFodoCell(double radius, double b, double L){ addFodoCell(radius, b, L, origin); }

		void buildPolygon(Vector3D center, uint n, double major_radius, double minor_radius, double b, double B_0);

		virtual void draw(void) override{ canvas->draw(*this); }

		std::ostream& print(std::ostream& output, bool print_elements = false) const;

		void evolve(double dt);

		std::array<Vector3D,2> position_and_trajectory(double s) const; // returns coordinate and local trajectory of point on the ideal orbit with given curvilinear coordinate
};

std::ostream& operator<<(std::ostream& output, const Accelerator &A);

namespace cernjunior{
	void build_default_accelerator(Accelerator &w);
};
