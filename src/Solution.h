#pragma once
#include <string>
#include <vector>
#include "Route.h"

class Solution
{
public:
	
	Solution(std::vector<Route*> routes);
	Solution(const Solution &old);
	~Solution();

	void saveToFile(std::string filename);
	double getLength();
	void mutate();
	void mutateSoftly();

private:
	std::vector<Route*> routes;
};

