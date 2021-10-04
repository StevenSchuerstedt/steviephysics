#pragma once
#include <vector>
#include <chrono>
#include <ctime>  
#include <iostream>
 


struct vector {

	double x = 0;
	double y = 0;
	vector() {}
	vector(double x, double y) {
		this->x = x;
		this->y = y;
	}

	vector normalize();
	double length();

	
};
//maybe move to own header/cpp file one day
inline vector operator*(double k, vector vec)
{
	vec.x *= k;
	vec.y *= k;
	return vec;
}
inline vector operator*(vector vec, double k)
{
	vec.x *= k;
	vec.y *= k;
	return vec;
}

inline vector operator/(vector vec, double k)
{
	vec.x /= k;
	vec.y /= k;
	return vec;
}
inline vector operator+(vector vec1, vector vec2)
{
	vector vecSum;
	vecSum.x = vec1.x + vec2.x;
	vecSum.y = vec1.y + vec2.y;
	return vecSum;
}
inline vector operator-(vector vec1, vector vec2)
{
	vector vecSum;
	vecSum.x = vec1.x - vec2.x;
	vecSum.y = vec1.y - vec2.y;
	return vecSum;
}


class physicsSquare {

	
public:
	vector x0;
	vector v0;
	vector x;
	vector v;
	double mass;
	bool m_RunOnceExplicitEuler = false;
	//air resistance constant
	double k = 1;
	std::chrono::time_point<std::chrono::system_clock> startTime;

public:
	physicsSquare(vector x0, vector v0, double mass, double k) {

		this->x0 = x0;
		this->v0 = v0;
		this->mass = mass;
		this->k = k;
		startTime = std::chrono::system_clock::now();
	}
	vector getPosition() {
		return x;
	}
	void reset() {
		this->x = this->x0;
	}

};

enum IntegrationScheme { EXPLICIT_EULER, ANALYTIC };

class steviePhysics {

	std::vector<physicsSquare*> physicsSquares;
	
	vector* gravity = new vector(0.0, 9.81);
	bool m_stop = false;
	
	
public:
	bool useAirResistance = false;
	IntegrationScheme integrationScheme = EXPLICIT_EULER;

	void init();
	void simulate();
	void add(physicsSquare* square) {
		physicsSquares.push_back(square);

	}
	void reset();
	void stop() { m_stop = true; }
	void log(double k) { std::cout << k << std::endl; }
	void log(std::string k ) { std::cout << k << std::endl; }
};

