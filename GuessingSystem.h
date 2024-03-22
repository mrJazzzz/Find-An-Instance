#ifndef GUESSINGSYSTEM_H
#define GUESSINGSYSTEM_H
#include "Person.h"
#include <list> //fleksibilnost za dodavanje novih pitanja, -||- praznoj listi
#include <set> //fleksibilnost za nove odgovore, greske za pristupanje praznom skupu
#include <string>

class GuessingSystem {

public:
	enum Commands {
		LOAD,
		GUESS,
		TRY,
		INFO,
		NEW,
		DELETE,
		EXIT,
		OTHER
	};
	//Lako dodavanje nove komande, posle u insertCommand i openTerminal izmene 
	struct Question {
		std::string q;
		std::set<std::string> ans;
	};
	//svako pitanje ima string pitanja i skup odgovora koji su stringovi

	//GuessingSystem() = default;//je l ce lepo postaviti praznu listu?
	//virtual ~GuessingSystem(); // treba li mi destruktor za bilo sta ovde uopste?
	void openTerminal();

private:
	Commands insertCommand(std::string str);
	int num_of_questions_ = 0;//ovo bese kao default vrednost?
	std::list<Question> questions_; //lista pitanja, lista struktura Question
	std::set<Person> data_; //skup primeraka, skup objekata klase Person
	
	void load();
	Person guess();
	std::set<Person>::iterator tryGuess();
	Person info();
	void newPerson();
	void deletePerson();

	void reset();
	// prazni i skup primeraka i listu pitanja i vraca broj pitanja na 0
	void printQuestion(std::list<Question>::iterator question_it);
	// u guessu da mi ispise pitanja u odgovarajucem formatu
	std::set<Person>::iterator findByName(std::set<Person>::iterator start, const std::string name);
	//vraca iterator na osobu datog imena, data_.end() ako nema te

	std::set<std::string> name_set_;//skup imena zbog provere za ista imena
	void editSample(const std::string& name);
};
#endif