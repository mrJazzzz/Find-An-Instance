#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class SameElements : public std::exception {
public:
	SameElements(const char* msg) : exception(msg) {}
};

class FileNotOpened : public std::exception {
public:
	FileNotOpened(const char* msg) : exception(msg) {}
};

class DatasetEmpty : public std::exception {
public:
	DatasetEmpty(const char* msg) : exception(msg) {}
};

class IrregularInput : public std::exception {
public:
	IrregularInput(const char* msg) : exception(msg) {}
};

class NameNotFound : public std::exception {
public:
	NameNotFound(const char* msg) : exception(msg) {}
};
#endif