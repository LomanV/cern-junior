#include "particle.h"

const double DEFAULT_MASS = 1.0;
const double DEFAULT_RADIUS = 1.0;
const double G = 1;

double Particle::getMass(void) const{
	return mass;
}

double Particle::getRadius(void) const{
	return radius;
}

RGB Particle::getColor(void) const{
	return color;
}

const Vector3D* Particle::getPosition(void) const{
	return &r;
}

const Vector3D* Particle::getForce(void) const{
	return &F;
}

void Particle::setForce(const Vector3D& my_F){
	F = my_F;
}

void Particle::incrementForce(const Vector3D& my_F){
	F += my_F;
}

void Particle::evolve(double dt){
	std::swap(v, v_p);
	v = v_p + (dt / mass) * F;

	std::swap(r, r_p);
	r = r_p + (dt * v);
}

std::vector<Particle>* Universe::getParticle_list(void){
	return &particle_list;
}

void Universe::new_particle(double x, double y, double z, double v_x, double v_y, double v_z, double my_mass, double my_radius, RGB my_color){
	particle_list.push_back(Particle(x,y,z,v_x,v_y,v_z,my_mass,my_radius, my_color));
}

void Universe::new_particle(Vector3D x_0, Vector3D v_0, double my_mass, double my_radius, RGB my_color){
	particle_list.push_back(Particle(x_0, v_0, my_mass, my_radius, my_color));
}

void Universe::clear_forces(void){
	for(Particle &p : particle_list) p.setForce(Vector3D());
}

void Universe::calculate_gravitational_forces(void){
	int N(particle_list.size()-1);
	for(int i(0); i <= N; ++i){
		for(int j(i+1); j <= N; ++j){
			Particle* A(&particle_list[i]);
			Particle* B(&particle_list[j]);
			double k(pow((*B->getPosition() - *A->getPosition()).norm2(),1.5));// == d^3
			Vector3D F((*B->getPosition()) - (*A->getPosition()));

			F *= (G * A->getMass() * B->getMass()) / k;
			A->incrementForce(F);
			B->incrementForce(-F);
		}
	}
}

void Universe::evolve(double dt){
	clear_forces();
	calculate_gravitational_forces();
	for(Particle &p : particle_list) p.evolve(dt);
}