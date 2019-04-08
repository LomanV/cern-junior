#include "accelerator.h"
#include "../physics/beam.h"

void Accelerator::weld(void){
	const int N(size());
	if(N <= 1) return;

	for(size_t i(0); i <= N-2; ++i){
		if(at(i)->getExit_point() != at(i+1)->getEntry_point()){
			throw excptn::NON_MATCHING_LINK_POINTS;
		}
		at(i)->setSuccessor(at(i+1).get());
		at(i+1)->setPredecessor(at(i).get());
	}
	back()->setSuccessor(front().get());
	front()->setPredecessor(back().get());
}

void Accelerator::activate(void){
	for(auto &b : beams){
		b->activate();
	}
}

void Accelerator::addParticle(Particle to_copy){
	to_copy.setCanvas(canvas);
	for(auto &E : *this){
		if(E->contains(to_copy)){
			std::unique_ptr<Particle> p(to_copy.copy());
			E->addParticle(p);
			return;
		}
	}
}

void Accelerator::addCircularBeam(const Particle &model, uint N, double lambda){
	beams.push_back(new CircularBeam(*this, model, N, lambda));
}

// TODO replace new with make_unique
void Accelerator::addStraightSection(double radius, const Vector3D &end){
	push_back(std::unique_ptr<Element>(new StraightSection(canvas, empty() ? origin : back()->getExit_point(), end, radius, time)));
	length += back()->getLength();
}

void Accelerator::addDipole(double radius, double curvature, double B_0, const Vector3D &end){
	push_back(std::unique_ptr<Element>(new Dipole(canvas, empty() ? origin : back()->getExit_point(), end, radius, curvature, time, B_0)));
	length += back()->getLength();
}

void Accelerator::addQuadrupole(double radius, double b, const Vector3D &end){
	push_back(std::unique_ptr<Element>(new Quadrupole(canvas, empty() ? origin : back()->getExit_point(), end, radius, time, b)));
	length += back()->getLength();
}

void Accelerator::addRadiofrequencyCavity(double radius, double curvature, double E_0, double omega, double kappa, double phi, const Vector3D &end){
	push_back(std::unique_ptr<Element>(new RadiofrequencyCavity(canvas, empty() ? origin : back()->getExit_point(), end, radius, curvature, time, E_0, omega, kappa, phi)));
	length += back()->getLength();
}

std::ostream& Accelerator::print(std::ostream& output, bool print_elements) const{
	if(print_elements){
		output << "\nELEMENTS:\n\n";
		if(empty()) output << "   none\n\n";
		for(auto &E : *this){
			output << *E << "\n\n";
		}
	}

	output << "PARTICLES:\n\n";
	bool no_particles(true);
	for(auto &E : *this){
		if(E->print_elements(output)) no_particles = false;
	}
	if(no_particles) output << "   none\n\n";

	return output;
}

std::ostream& operator<<(std::ostream& output, Accelerator &A){
	return A.print(output);
}

void Accelerator::evolve(double dt){
	*time += dt;
	for(std::unique_ptr<Element> &e : *this) e->evolve(dt);
}

std::array<Vector3D,2> Accelerator::position_and_trajectory(double s) const{
	if(length <= simcst::ZERO_DISTANCE) throw excptn::ACCELERATOR_DEGENERATE_GEOMETRY;
	assert(not empty());

	// shift s so that 0 ≤ s < length
	s = fmod(s, length);
	if(s < 0) s += length;

	int i(0);
	double l(at(i)->getLength());
	while(s > l){
		s -= l;
		++i;
		l = at(i)->getLength();
	}

	return {at(i)->inverse_curvilinear_coord(s), at(i)->local_trajectory(s)};
}
