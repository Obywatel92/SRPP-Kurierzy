#include "PizzaSolver.h"
#include "PizzaDivider.h"

PizzaSolver::PizzaSolver() : liczbaOptymalizacji(10)
{
}

PizzaSolver::~PizzaSolver()
{
}

vector<Solution*> PizzaSolver::makeSolutions(InputData* input) {

	warehouse = input->getWarehouse();

	PizzaDivider* divider = new PizzaDivider();
	vector<InputData*> divided = divider->divide(input, 0);

	vector<Solution*> solutions;
	solutions.push_back(singleProcess(divided));

	k = input->getK();

	// Obracanie pizzy po kawa�ku
	for (int i = 1; i < k; ++i) { // Ma si� wykona� k-1 razy
		
		vector<InputData*> divided = divider->divide(input, i);
		solutions.push_back(singleProcess(divided));
	}

	delete divider;

	return solutions;
}

Solution* PizzaSolver::process(InputData* input) {
	
	vector<Solution*> solutions = makeSolutions(input);
	Solution* bestSolution = solutions.at(0);

	for (int i = 1; i < k; ++i) { // Ma si� wykona� k-1 razy

		if (solutions.at(i)->getLength() < bestSolution->getLength()) {
			bestSolution = solutions.at(i);
		}
	}

	return bestSolution;
}

Solution* PizzaSolver::singleProcess(vector<InputData*> input) {

	int i, j;
	vector<Route*> routes;

	//Posortowanie ka�dego fragmentu
	for (i = 0; i < input.size(); ++i) {
		routes.push_back(processPart(input[i]->getCities()));
	}

	return new Solution(routes);

};

Route* PizzaSolver::processPart(vector<City*> partOfCities) {

	Route* route = new Route(warehouse);
	City* temporary;
	int i, j;

	// Je�li k�t jest ostry, sortujemy wg innego algorytmu
	if ((partOfCities[partOfCities.size()-1]->angle - partOfCities[0]->angle) < 1.57)
	{
		//Znalezienie indeksu najbardziej odleg�ego elementu
		int max_id = 0;		

		for (i = 1; i < partOfCities.size(); ++i) {
			if (partOfCities[i]->distanceToCenter > partOfCities[max_id]->distanceToCenter) {
				max_id = i;
			}
		}

		//Posortowanie lewej strony listy rosnaco wg odleglosci
		for (i = 1; i < max_id; ++i) {
			temporary = partOfCities[i];
			for (j = i; j > 0; --j) {
				if (partOfCities[j-1]->distanceToCenter < temporary->distanceToCenter) {
					break;
				}
				else {
					partOfCities[j] = partOfCities[j-1];
				}
			}
			partOfCities[j] = temporary;
		}

		//Posortowanie prawej strony listy rosnaco wg odleglosci
		for (i = max_id+2; i < partOfCities.size(); ++i) {
			temporary = partOfCities[i];
			for (j = i; j > max_id+1; --j) {
				if (partOfCities[j-1]->distanceToCenter >= temporary->distanceToCenter) {
					break;
				}
				else {
					partOfCities[j] = partOfCities[j-1];
				}
			}
			partOfCities[j] = temporary;
		}
	}

	// Pr�ba optymalizacji listy poprzez proste zamiany
	bool sthChanged;
	for (i = 0; i < liczbaOptymalizacji; ++i) {		
		sthChanged = optimalizePart(partOfCities);
		if (!sthChanged) {
			break; // Jesli nic si� nie zmieni�o, nie ma sensu d�u�ej optymalizowa�
		}
	}

	//Przepisanie wektora do �cie�ki
	for (i = 0; i < partOfCities.size(); ++i) {
		route->append(partOfCities[i]);
	}

	return route;
};

bool PizzaSolver::optimalizePart(vector<City*> &partOfCities)
{
	int i;
	bool sthChanged = false;
	City* temporary;

	// Pr�ba optymalizacji listy poprzez zamiany s�siednich w�z��w miejscami
	for (i = 0; i < partOfCities.size()-1; ++i) {
		
		double d1, d2; // Odleg�o�ci obecne
		double a1, a2; // Odleg�o�ci alternatywne

		if (i == 0) {
			d1 = partOfCities[0]->distanceToCenter;
			a1 = partOfCities[1]->distanceToCenter;
		}
		else {
			d1 = partOfCities[i]->distanceTo(partOfCities[i-1]);
			a1 = partOfCities[i+1]->distanceTo(partOfCities[i-1]);
		}
				
		if (i == (partOfCities.size()-2)) {
			d2 = partOfCities[i+1]->distanceToCenter;
			a2 = partOfCities[i]->distanceToCenter;
		}
		else {
			d2 = partOfCities[i+1]->distanceTo(partOfCities[i+2]);
			a2 = partOfCities[i]->distanceTo(partOfCities[i+2]);
		}

		if ((a1 + a2) < (d1 + d2)) {
			// Stwierdzili�my, �e zamieniamy
			temporary = partOfCities[i+1];
			partOfCities[i+1] = partOfCities[i];
			partOfCities[i] = temporary;
			sthChanged = true;
		}
		

	}

	return sthChanged;

};