#pragma once

#include <iostream>

#include "element.h"

class Accelerator : public Drawable{
	protected:
		std::vector<std::unique_ptr<Element>> element_list; // (ordered) list of elements that make up the accelerator
		std::vector<Particle*> particle_list; // list of particles
		double time;
		void initialize(Canvas* c);
	public:
		Accelerator(Canvas* canvas = nullptr) : Drawable(canvas){}

		void draw_particles(void) const{ for(auto &p : particle_list) p->draw(); }
		void draw_elements(void) const{ for(auto &e : element_list) e->draw(); }

		void addParticle(Vector3D r, Vector3D v, double mass, double charge, double radius);
		void addStraightSection(Vector3D start, Vector3D end, double radius);
		void addDipole(Vector3D start, Vector3D end, double radius, double curvature, double B_0);
		void addQuadrupole(Vector3D start, Vector3D end, double radius, double curvature, double b);

		virtual void draw(void) override{ canvas->draw(*this); }

		std::ostream& print(std::ostream& output);

		virtual void evolve(double dt) override;
};

std::ostream& operator<<(std::ostream& output, const Accelerator &A);
