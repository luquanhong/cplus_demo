#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{

	vector<int> va;
	va.push_back(1);
	va.push_back(2);
	va.push_back(3);
	va.push_back(4);
	va.push_back(5);


	for(vector<int>::iterator it = va.begin(); it != va.end(); it++) {
		cout << *it << endl;

		if(*it == 3) {
			va.erase(it);

			break;
		}
	}

	cout << endl;

	for(vector<int>::iterator it = va.begin(); it != va.end(); it++) {
		cout << *it << endl;
	}

	va.clear();

	cout << endl;
	
	for(vector<int>::iterator it = va.begin(); it != va.end(); it++) {
		cout << *it << endl;
	}
	
	cout << endl;
	cout << va.size() << endl;
	return 0;
}




