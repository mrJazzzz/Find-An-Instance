#include "Person.h"
#include <sstream>
#include <iostream>
using namespace std;

Person::Person() {
	this->features_ = "";
	this->name_ = "";
}
Person::Person(string row, int numOfQuestions) {
	this->features_ = "";
	this->name_ = "";
	stringstream s(row); string tmp;
	int i = 0;
	while (getline(s, tmp, ',') && i < numOfQuestions) {
		i++;
		this->features_ += tmp;
		this->features_ += '|';
	}
	getline(s, tmp, ',');
	this->name_ += tmp;

}

void Person::setFeatures(const std::string& features) {
	features_ = features;
}
void Person::setName(const std::string& name) {
	name_ = name;
}

std::string Person::getFeatures() const{
	return this->features_;
}
std::string Person::getName() const{
	return this->name_;
}