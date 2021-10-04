#include "steviePhysics.h"


vector vector::normalize() {
	double length = std::sqrt(x * x + y * y);
	return 1.0 / length * vector(x, y);
}

double vector::length() {
	double length = std::sqrt(x * x + y * y);
	return length;
}

void steviePhysics::init() {
	
}

void steviePhysics::reset() {
	//startTime = std::chrono::system_clock::now();

	for (int i = 0; i < physicsSquares.size(); i++) {

		physicsSquares[i]->reset();

	}
}
void steviePhysics::simulate() {
	if (!m_stop) {

		for (int i = 0; i < physicsSquares.size(); i++) {

			auto time = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_time = time - physicsSquares[i]->startTime;

			double t = elapsed_time.count();

			bool AIR_RESISTANCE = useAirResistance;

			if (integrationScheme == ANALYTIC) {
				if (AIR_RESISTANCE) {

					double m = physicsSquares[i]->mass;
					double g = gravity->y;
					vector v0 = physicsSquares[i]->v0;
					double k = physicsSquares[i]->k;
					double v_s = std::sqrt(m / k * g);
					double t_U = v_s / g * std::atan(v0.y / v_s);
					int changeDir = 1;

					if (v0.x < 0) {
						//change direction as log cant handle negative velocity
						v0.x *= -1;
						changeDir = -1;
					}

					//vertical component
					physicsSquares[i]->x.x = changeDir * m / k * std::log(k / m * v0.x * t + 1) + physicsSquares[i]->x0.x;
					if (t >= t_U) {
						//downwards motion

						//horizontal component
						physicsSquares[i]->x.y = -v_s * (t - t_U) - std::pow(v_s, 2) / g * std::log(0.5 * (1 + std::exp(-2 * g * (t - t_U) / v_s)) * std::cos(g * t_U / v_s)) + physicsSquares[i]->x0.y;

					}
					else {

						//upwards motion

						//horizontal component
						physicsSquares[i]->x.y = std::pow(v_s, 2) / g * std::log(std::cos(g * (t_U - t) / v_s)) - std::pow(v_s, 2) / g * std::log(std::cos(g * t_U / v_s)) + physicsSquares[i]->x0.y;

					}

				}
				if (!AIR_RESISTANCE) {
					physicsSquares[i]->x = -0.5 * *gravity * std::pow(t, 2)
						+ physicsSquares[i]->v0 * t + physicsSquares[i]->x0;
				}


			}

			if (integrationScheme == EXPLICIT_EULER) {
				if (!physicsSquares[i]->m_RunOnceExplicitEuler) {
					physicsSquares[i]->x = physicsSquares[i]->x0;
					physicsSquares[i]->v = physicsSquares[i]->v0;
					physicsSquares[i]->m_RunOnceExplicitEuler = true;
				}

				double h = 0.02;
				vector F;
				if(!AIR_RESISTANCE)
					F = vector(0, -9.81);
				if (AIR_RESISTANCE)
					F = vector(0, -9.81) - physicsSquares[i]->k * std::pow(physicsSquares[i]->v.length(), 2) * physicsSquares[i]->v.normalize();

				physicsSquares[i]->x = physicsSquares[i]->x + h * physicsSquares[i]->v;
				physicsSquares[i]->v = physicsSquares[i]->v + h * F / physicsSquares[i]->mass;

			}

		}
	}
}