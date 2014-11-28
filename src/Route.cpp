#include "Route.h"


Route::Route(City* startingCity) {
	route.push_back(startingCity);
}

Route::Route(const Route& old) {
	route = old.route;
}

Route::~Route()
{
}

void Route::append(City* city) {
	route.push_back(city);
}

double Route::getLength() {
	double length = 0.0;
	int prev = route.size() - 1;
	for (int i = 0; i < route.size(); ++i) {
		length += route[i]->distanceTo(route[prev]);
		prev = i;
	}
	return length;
}

int Route::getNumberOfCities() {
	return route.size();
}

City* Route::getCityAt(int id) {
	return route.at(id);
};

void Route::setCityAt(City* toSet, int id) {
	route.at(id) = toSet;
};

void Route::removeCityAt(int id) {
	for (int i=id; i<route.size()-1; ++i) {
		route.at(i) = route.at(i+1);
	}
	route.pop_back();
}

void Route::moveCity(int position, int newPosition){
	//Przestawia miasta z pierwszej pozycji na drug�

	int i;
	City* temporary = route.at(position);
	
	if (newPosition > position) {

		for (i=position; i<newPosition; ++i) {
			route.at(i) = route.at(i+1);
		}

	}
	else {

		for (i=position; i>newPosition; --i) {
			route.at(i) = route.at(i-1);
		}
	}

	route.at(i) = temporary;
};

std::ostream& operator<<(std::ostream& os, Route& route) {
	for (int i = 0; i < route.route.size(); ++i) {
		os << route.route[i]->id << ' ';
	}
	os << '0';
	return os;
}

void Route::removeLast() {
	route.erase(route.end() - 1);
}

double Route::getPartialLength() {
	double length = 0.0;
	for (int i = 0; i < route.size() - 1; ++i) {
		length += route[i]->distanceTo(route[i + 1]);
	}
	return length;
}