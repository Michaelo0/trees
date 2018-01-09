#include "RBT.h"
#include "RandomTree.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <ctime>


bool FileIsEqual(const std::string &, const std::string &);
bool LineIsOk(const std::string &, const std::string &);

int FindKey(const std::string &);
int FindValue(const std::string &);


int main(int argc, char *argv[]) {
	if (argc < 4) {
		return 0;
	}
	std::string line;
	std::ifstream fileIn(argv[1]);
	assert(fileIn);
	std::ofstream fileOut(argv[2]);
	assert(fileOut);

	

	random_tree<int, int> _random_tree;
	srand(time(0));

	while (getline(fileIn, line)) {
		if (line.find("delete") == 0) {
			if (LineIsOk(line, "delete")) {
				if (!_random_tree._remove(FindKey(line))) {
					fileOut << "error" << std::endl;
				}
				else {
					fileOut << "Ok" << std::endl;
				}
			}
			else {
				fileOut << "error" << std::endl;
			}
		}
		if (line == "print") {
			_random_tree._in_order_traversal(fileOut);
			fileOut << std::endl;
		}
		if (line.find("add") == 0) {
			if (LineIsOk(line, "add") != 0) {
				_random_tree.insert(FindKey(line), FindValue(line));
			}
			else {
				fileOut << "error" << std::endl;
			}
		}
		if (line.find("search") == 0) {
			if (LineIsOk(line, "search") != 0) {
				if (!_random_tree.search(FindKey(line))) {
					fileOut << "error" << std::endl;
				}
				else {
					fileOut << "Ok" << std::endl;
				}

			}
			else {
				fileOut << "error" << std::endl;
			}
		}
		if (line == "min") {
			fileOut << _random_tree.minimum() << std::endl;
		}
		if (line == "max") {
			fileOut << _random_tree.maximum() << std::endl;
		}
		if (line == " ") {
			fileOut << "error" << std::endl;
		}
	}

	std::cout << "runtime = " << clock() / 1000.0 << std::endl;

	if (FileIsEqual(argv[2], argv[3])) {
		std::cout << "Correct" << std::endl;
	}
	else {
		std::cout << "Not correct" << std::endl;
	}


	fileIn.clear();
	fileIn.seekg(0);
	fileOut.clear();
	fileOut.seekp(0);

	

	red_black_tree<int,int> redBlackTree;
	
	srand(time(0));

	while (getline(fileIn, line)) {
		if (line.find("delete") == 0) {
			if (LineIsOk(line, "delete")) {
				/*if (!redBlackTree._remove(FindKey(line))) {
					fileOut << "error" << std::endl;
				}
				else {
					fileOut << "Ok" << std::endl;*/
			}
		}
		else {
			fileOut << "error" << std::endl;
		}
	
		
		if (line == "print") {
			redBlackTree._in_order_traversal(fileOut);
			fileOut << std::endl;
		}
		if (line.find("add") == 0) {
			if (LineIsOk(line, "add") != 0) {
				redBlackTree.insert(FindKey(line), FindValue(line));
			}
			else {
				fileOut << "error" << std::endl;
			}
		}
		if (line.find("search") == 0) {
			if (LineIsOk(line, "search") != 0) {
				if (!redBlackTree.search(FindKey(line))) {
					fileOut << "error" << std::endl;
				}
				else {
					fileOut << "Ok" << std::endl;
				}
			}
			else {
				fileOut << "error" << std::endl;
			}
		}
		if (line == "min") {
			fileOut << redBlackTree.minimum() << std::endl;
		}
		if (line == "max") {
			fileOut << redBlackTree.maximum() << std::endl;
		}
		if (line == " ") {
			fileOut << "error" << std::endl;
		}
	}

	std::cout << "runtime = " << clock() / 1000.0 << std::endl;

	if (FileIsEqual(argv[2], argv[3])) {
		std::cout << "Correct" << std::endl;
	}
	else {
		std::cout << "Not correct" << std::endl;
	}

	fileIn.close();
	fileOut.close();

	return 0;
}
bool LineIsOk(const std::string &s, const std::string &command) {
	std::istringstream iss(s);
	std::string tmp;

	iss >> tmp;
	if (tmp != command) {
		return false;
	}
	tmp.clear();
	iss >> tmp;

	if (tmp.empty()) {
		return false;
	}
	tmp.clear();
	iss >> tmp;

	if (tmp.empty()) {
		return false;
	}

	tmp.clear();
	iss >> tmp;

	return tmp.empty();
}
bool FileIsEqual(const std::string &firstFileName, const std::string &secondFileName) {
	std::ifstream firstFile(firstFileName), secondFile(secondFileName);
	assert(firstFile);
	assert(secondFile);

	std::string buffer1((std::istreambuf_iterator<char>(firstFile)), std::istreambuf_iterator<char>());
	std::string buffer2((std::istreambuf_iterator<char>(secondFile)), std::istreambuf_iterator<char>());
	firstFile.close();
	secondFile.close();

	return (buffer1 == buffer2);
}

int FindKey(const std::string &s) {
	std::istringstream iss(s);
	std::string tmp;
	iss >> tmp;
	tmp.clear();
	iss >> tmp;
	int key = atoi(tmp.c_str());
	return key;
}
int FindValue(const std::string &s) {
	std::istringstream iss(s);
	std::string tmp;
	iss >> tmp;
	tmp.clear();
	iss >> tmp;
	tmp.clear();
	iss >> tmp;
	int value = atoi(tmp.c_str());
	return value;
}


