#ifndef PERSON_H
#define PERSON_H

#include <string>
class Person {
public:
	Person();
	Person(std::string row, int numOfQuestions);
	//row sadrzi i ime pa beojem pitanja shvatam gde da zavrsim
	// sa osobinama i da gledam ime do kraja 
	//Primerak(const string& features,const string& name):features_ = features; name_ = name;
	virtual ~Person() = default;

	void setFeatures(const std::string& features);
	void setName(const std::string& name);
	//setteri

	std::string getFeatures() const;
	std::string getName() const;
	//getteri

	bool operator<(const Person& other)const {
		return features_ < other.features_;
	}
	//komparatorska funkcija potrebna za ubacivanje u skup
	//poerdjenje se vrsi po osobinama
		
private:
	std::string features_; // string koji sadrzi osobine, bice u formatu osob1|osob2|...osobn|
	std::string name_;
};








#endif
