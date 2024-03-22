#include "Exceptions.h"
#include "GuessingSystem.h"
#include "Person.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
//pazi na abecedni redosled

void GuessingSystem::reset() {
	this->questions_.clear();
	this->data_.clear();
	this->num_of_questions_ = 0;
	this->name_set_.clear();
}

GuessingSystem::Commands GuessingSystem::insertCommand(std::string str) {
	return str == "LOAD" ? LOAD : str == "GUESS" ? GUESS : str == "TRY" ? TRY :
		str == "INFO" ? INFO : str == "NEW" ? NEW : str == "DELETE" ? DELETE :
		str == "EXIT" ? EXIT : OTHER;
}
void GuessingSystem::openTerminal() {
	std::cout << "Welcome to guessing game!\n" << std::endl;
	std::string inputStr;
	while(1){
		std::cout << "\n_______________________________________" << std::endl;
		std::cout << "What should I do next?\n" << std::endl;
		std::cin >> inputStr; //unos komande
		switch (this->insertCommand(inputStr)) {
			case(LOAD):
				this->load();
				break;
			case(GUESS):
				this->guess();
				break;
			case(TRY):
				this->tryGuess();
				break;
			case(INFO)://trazi po imenu, linearno
				this->info();
				break;
			case(NEW)://ako gledam isto ime 
				this->newPerson();
				break;
			case(DELETE)://trazi po imenu. linearno
				this->deletePerson();
				break;
			case(EXIT)://i ovo dok, izgleda, obrise sta treba
				return;
			case(OTHER):
				std::cout << "Not an option. Please enter valid option.";
				std::cout << std::endl << "///////////////////////////////////////";
				break;
		}
		//switch da mi odradi odgovarajucu komandu, moze pod nekim try-em mozda?
	}
}

void GuessingSystem::load() {
	
	std::string filepath;
	std::cout << "Enter file name: ";
	std::cin >> filepath;
	//unos imena datoteke, sa.csv
	std::ifstream dataFile(filepath);
	//otvaranje datoteke
	try {
		if (!dataFile.is_open()) 
			throw FileNotOpened("File could not be opened successfully.\n");
		//baca izuzetak ako datoteka nije otvorena
		
		if (!this->questions_.empty())
			this->reset();
		//ako je vec bila ucitana baza podataka onda da je obrise

		std::string tmp, ins;
		//tmp je red, sa pitanjima i imenom
		// ins je odgovor ili ime, odvojena rec, sa kojom se dalje radi
		
		//Odvajanje pitanja u listu struktura Question:
		getline(dataFile, tmp);
		std::stringstream row(tmp);
		while (getline(row, ins, ',')) {
			Question initQ;
			initQ.q = ins;
			//if (initQ.ans.empty()) std::cout << "OK JE" << std::endl;
			//std::set<std::string> a;
			//initQ.ans = a;  probaj ovo sa velikim skupom
			this->questions_.push_back(initQ);

			this->num_of_questions_++;
		}

		//ubacivanje podataka u skup primeraka
		std::string newOneFeat, tmpAns;
		while (getline(dataFile, tmp)) {
			newOneFeat = "";
			Person theNewOne;
			std::stringstream ss(tmp);
			for (std::list<Question>::iterator it = this->questions_.begin(); it != (this->questions_.end()); ++it) {
				getline(ss, tmpAns, ',');
				newOneFeat += (tmpAns + '|');
				it->ans.insert(tmpAns);
			}
			//petljom idem kroz jedan red i odvajam odgovore na pitanja 
			//i stavljam ih u skup odgovora na odgovarajuce pitanje, a novom
			//primerku dodajem to na skup osobina

			getline(ss, tmpAns, ',');
			theNewOne.setName(tmpAns);
			
			std::pair<std::set<std::string>::iterator, bool>  sameNameCheck = this->name_set_.insert(tmpAns);
			if (!sameNameCheck.second) {
				throw SameElements("Two samples with same name found, dataset is not proper, thus it is not loaded.\n");
				//bacam izuzetak ako vec postoji primerak sa istim imenom
			}
			
			theNewOne.setFeatures(newOneFeat);
			std::pair<std::set<Person>::iterator, bool> equalityCheck = this->data_.insert(theNewOne);
			//oformljavam novi primerak i ubacujem ga u skup

			if (!equalityCheck.second) {
				throw SameElements("Two equal samples found, dataset is not proper, thus it is not loaded.\n");
				//bacam izuzetak ako vec postoji primerak sa istim osobinama
			}
		}
		std::cout << "File opened successfuly." << std::endl;
		dataFile.close();
	}
	catch (FileNotOpened& e) {
		std::cout << e.what() << std::endl;
		return;
	}
	catch (SameElements& e) {
		this->reset();
		std::cout << e.what() << std::endl;
		dataFile.close();
		return;
	}

}

void GuessingSystem::printQuestion(std::list<Question>::iterator question_it) {
	// format je: pitanje(odgovor1, odgovor2, ..., odgovorn)
	std::cout << question_it->q;
	std::cout << '(';

	std::set<std::string>::iterator itAns;
	//iterator koji prolazi kroz odgovore na pitanje 
	//koje je odredjeno argumentom funkcije

	for (itAns = question_it->ans.begin(); itAns != std::prev(question_it->ans.end()); itAns++)
		std::cout << *itAns << ", ";
	std::cout << *itAns << ")" << std::endl;
}
Person GuessingSystem::guess() {
	try {
		if (this->data_.empty())
			throw DatasetEmpty("Dataset is empty.");
		//Ukoliko je skup primeraka prazan baca izuzetak

		std::string feat = "", tmpAns;
		//u feat ide string sa osobinama, u tmpAns trenutni odgovor
		//koji se ubacuje u feat

		std::list<Question>::iterator it;
		for (it = this->questions_.begin(); it != this->questions_.end(); ++it) {
			printQuestion(it);
			std::cin >> tmpAns;
			feat += (tmpAns + ',');
			//zapeta jer na osnovu feat formiram primerak konstruktorom
		}
		//unosim odgovor na svako pitanje, kroz koja prolazim iteratorom it
		//i formiram string osobina koji ide u primerak tmpPerson koji 
		//sluzi za nalazenje primerka iz skupa poredjenjem
		feat += " ";
		//za ime
		Person tmpPerson(feat, num_of_questions_);

		std::set<Person>::iterator pos = data_.find(tmpPerson);
		//iterator koji govori o polozaju trazenog primerka
		/*
		std::cout << "Aj sad uredi: ";
		std::string a;
		std::cin >> a;
		editSample(a);
		*/
		if (pos == data_.end()) {
			std::cout << "\nThere is no such sample in this dataset." << std::endl;
			return *(this->data_.begin());
		}
		else {
			std::cout << "\nSample you are looking for is: " << pos->getName() << std::endl;
			return *pos;
		}
	}
	catch (DatasetEmpty& e) {
		std::cout << e.what() << std::endl;
		Person emptyPerson = Person("",0);
		return emptyPerson;
	}
}
//'|'+1 = '}' = 125      '#' = 23
//da vrati pokazivac na prvu odgovarajucu osobu
std::set<Person>::iterator GuessingSystem::tryGuess() {
	try {
		if (this->data_.empty())
			throw DatasetEmpty("Dataset is empty.");
		int numOfAttempts;
		std::cout << "Enter the number of attempts: ";
		std::cin >> numOfAttempts;
		if (numOfAttempts <= 0)
			throw IrregularInput("Number of attempts must be greater than zero.");

		std::string tmpAns, partFeat = "";
		//tmpAns je trenutni odgovor koji se ubacuje u partFeat

		std::set<Person>::iterator itL;
		std::set<Person>::iterator itU;
		//iteratori koji mi daju granice izmedju kojih su primerci
		//sa trenutno unetim osobinama
		Person tmpL, tmpU;
		//Primerci se formiraju radi poredjenja

		std::list<Question>::iterator questionIt = this->questions_.begin();
		for (int i = 0; i < numOfAttempts; ++i) {
			printQuestion(questionIt);
			std::cin >> tmpAns;
			partFeat += (tmpAns + '|');
			//unosenje trazenih osobina

			if (i < this->num_of_questions_ - 1) {
				tmpL.setFeatures(partFeat + '#');
				tmpU.setFeatures(partFeat + '}');
			}
			else {
				tmpL.setFeatures(partFeat);
				tmpU.setFeatures(partFeat); // pazi ovo sto si izmeniio, bilo je uvek sa + '}'
			}
			tmpL.setName("");
			tmpU.setName("");
			//formiranje primeraka za poredjenje

			itL = this->data_.lower_bound(tmpL);
			if (itL == this->data_.end()) {
				std::cout << "\nThere are no such samples." << std::endl;
				return itL;
			}
			if ((itL->getFeatures().rfind(partFeat, 0) && (itL == this->data_.begin())))
				itL = std::next(itL);
			itU = this->data_.upper_bound(tmpU);
			//nalazenje granica primeraka sa datim osobinama

			if (std::next(itL) == itU) {
				std::cout << "\nThe only such sample is: " << itL->getName() << std::endl;
				return itL;
			}
			if (itL == itU) {
				std::cout << "\nThere are no such samples." << std::endl;
				return this->data_.begin();
			}
			//ispitivanje da li je broj odgovarajucih osoba sveden na 0 ili 1
			//da bi se prekinula pretraga
			questionIt = next(questionIt);
		}
		//if (!earlyEnd) {
			std::cout << "\nSuch samples are: " << std::endl;
			for (; itL != itU; itL++) 
				std::cout << itL->getName() << std::endl;
			return itL;
		//}
	}
	catch (IrregularInput& e) {
		std::cout << e.what() << std::endl;
		return this->data_.begin();
	}
	catch (DatasetEmpty& e) {
		std::cout << e.what() << std::endl;
		Person emptyPerson = Person("", 0);
		return this->data_.begin();
	}
}

std::set<Person>::iterator GuessingSystem::findByName(std::set<Person>::iterator start, const std::string name){
	std::set<Person>::iterator it = start;
	for (; it != this->data_.end(); ++it)
		if (name == it->getName())
			return it;
	return it;
}
Person GuessingSystem::info() {
	std::string name;
	std::cout << "Enter the sample name: ";
	std::cin.ignore();
	getline(std::cin,name);
	//unosenje imena primerka
	
	std::set<Person>::iterator pos;
	pos = this->findByName(this->data_.begin(), name);
	//nalazim mesto primerka sa unetim imenom

	if (pos == this->data_.end()) {
		std::cout << "There is no such sample." << std::endl;
		Person noPerson = Person("NonexistentPerson|NoName", 1);
		return noPerson;
	}
	//obradjujem slucaj gde nema tog imena u datasetu

	std::stringstream s(pos->getFeatures());
	std::string tmp;
	//sluze za vadjenje osobina iz stringa osobina primerka

	for (auto itQu = this->questions_.begin(); itQu != this->questions_.end(); ++itQu) {
		getline(s, tmp, '|');
		std::cout << itQu->q << " " << tmp << std::endl;
	}
	//ispis osobina

	return *pos;
}
void GuessingSystem::deletePerson() {
	try {
		int numName = 0;
		//brojace koliko je primeraka sa tim imenom obrisano
		if (this->data_.empty())
			throw DatasetEmpty("Dataset is empty.");
		//baca izuzetak ako je dataset prazan

		std::cout << "Enter the name of the sample that you want to be deleted: ";
		std::string name;
		std::cin.ignore();
		getline(std::cin, name);
		//unos imena za brisanje

		std::set<Person>::iterator it;
		for (it = this->data_.begin(); it != this->data_.end(); ++it)
			if (it->getName() == name) 
				if (it == std::prev(this->data_.end())){
					this->data_.erase(it);
					std::cout << "\nSample removed successfully." << std::endl;
					numName++;
					break;
				}
				else {
					it = this->data_.erase(it);
					std::cout << "\nSample removed successfully." << std::endl;
					numName++;
				}
				//data_.erase(it) daje pokazivac na sledbenik i cini iterator it nevalidnim
				//osim ako kazem it=data_.erase(it) pa zato moram da delim na slucajeve	
			
		if (!numName)
				throw NameNotFound("There is no sample with this name in dataset.");
		//ako nema tog imena u datasetu baca izuzetak
	}
	catch (DatasetEmpty& e) {
		std::cout << e.what() << std::endl;
		return;
	}
	catch (NameNotFound& e) {
		std::cout << e.what() << std::endl;
		return;
	}
}

void GuessingSystem::newPerson(){//prijaviti ako ne ubaci primerak
	std::cout << "Unesite ime novog primerka: ";
	std::string name;
	std::cin.ignore();
	getline(std::cin, name);
	//unos imena
	try {
		
		std::pair<std::set<std::string>::iterator, bool>  sameNameCheck = this->name_set_.insert(name);
		if (!sameNameCheck.second)
			throw SameElements("Sample could not be added as one with the same name already exists.\n");
		//bacam izuzetak ako vec postoji primerak sa istim imenom

		std::string newFeatName = "";
		std::string tmp;
		//red iz koga ce se konstruisati primerak i tmp za trenutnu osobinu koja se dodaje
		std::list<Question>::iterator itQu;
		for (itQu = this->questions_.begin(); itQu != this->questions_.end(); ++itQu) {
			std::cout << itQu->q << ' '; 
			std::cin >> tmp;
			newFeatName += (tmp + ',');
			itQu->ans.insert(tmp); 
			//ako je nov odgovor ubacice ga a ako nije nece praviti kopiju
		}
		newFeatName += name;
	
		Person theNewOne(newFeatName, this->num_of_questions_);
		//konstruise se ime
		std::pair<std::set<Person>::iterator, bool> equality_check = this->data_.insert(theNewOne);
		if (!equality_check.second)
			throw SameElements("Sample could not be added as one with the same features already exists.");
		else
			std::cout << "\nSample added successfully." << std::endl;
	}
	catch (SameElements& e) {
		std::cout << e.what() << std::endl;
		return;
	}
}



void GuessingSystem::editSample(const std::string& name) {
	try {
		std::set<Person>::iterator pos = this->findByName(this->data_.begin(), name);
		if (pos == this->data_.end())
			throw NameNotFound("The name does not exist in dataset.");
		std::cout << "What is the number of question that needs to have it's answer changed?" << std::endl;
		int quNum;
		std::cin >> quNum;
		if (quNum < 0 || this->num_of_questions_ - 1 < quNum)
			throw IrregularInput("Entered number is not in correct range.");
		std::cout << "Enter the new answer: ";
		std::string newAns;
		std::cin >> newAns;
		std::string feat = pos->getFeatures();
		std::stringstream row(feat);
		std::string newPerson = "", tmp;
		for(int i=0; i<num_of_questions_; i++) 
			if (i == quNum) {
				getline(row, tmp, '|');
				newPerson += (newAns + ',');
			}
			else {
				getline(row, tmp, '|');
				newPerson += (tmp + ',');
			}
		newPerson += name;
		this->data_.erase(pos);
		std::cout << "Nova osoba: " << newPerson << std::endl;
		Person newOne(newPerson, num_of_questions_);
		this->data_.insert(newOne);
	}
	catch (IrregularInput& e) {
		std::cout << e.what() << std::endl;
		return;
	}
	catch (NameNotFound& e) {
		std::cout << e.what() << std::endl;
		return;
	}


	

}