#pragma once

#ifndef _PERLINNOISEGENERATOR_H_
#define _PERLINNOISEGENERATOR_H_

#include <vector>
#include <random>
#include <algorithm>
#include <numeric>

class PerlinNoiseGenerator
{
public:
	PerlinNoiseGenerator();
	PerlinNoiseGenerator(unsigned int seed);
	~PerlinNoiseGenerator();

	double Generate(double x, double y, double z = 0.0);

private:
	double Fade(double t);
	double Lerp(double t, double a, double b);
	double Grad(int hash, double x, double y, double z);

private:
	std::vector<int> permutation;
};

#endif