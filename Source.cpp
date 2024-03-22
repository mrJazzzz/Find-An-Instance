#include "Exceptions.h"
#include "GuessingSystem.h"
#include "Person.h"
#include <fstream>
#include <iostream>

using namespace std;

int main() {

	GuessingSystem* gs = new GuessingSystem();
	gs->openTerminal();

	return 0;
}