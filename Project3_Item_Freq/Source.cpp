#define NOMINMAX // need to do this to suppress max() macro defined in Windows.h
#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <limits>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;


class PyInfoHolder {
public:
	PyInfoHolder() {
		Py_Initialize();
		PyObject *sys_path = PySys_GetObject("path");
		PyList_Append(sys_path, PyUnicode_FromString("Release/"));
		pName = PyUnicode_FromString("PythonFile");
		pModule = PyImport_ImportModule("PythonFile");
		// pDict is a borrowed reference 
		pDict = PyModule_GetDict(pModule);
	}

	~PyInfoHolder() {
		Py_Finalize();
		Py_DECREF(pName);
		Py_DECREF(pModule);
		Py_DECREF(pDict);
	}

	PyObject *pName, *pModule, *pDict;
	
};

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName, const PyInfoHolder &pyInfo) 
{
	char *procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(pyInfo.pModule, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callStringFunc(string proc, string param, const PyInfoHolder &pyInfo)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char *paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject *pFunc, *pValue = nullptr, *presult = nullptr;

	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pyInfo.pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);


	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	10 is returned to the C++
*/
//NOTE(Ezra): This function is not used and can be removed unless needed for another purpose
int callIntFunc(string proc, int param, const PyInfoHolder &pyInfo)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject *pFunc, *pValue = nullptr, *presult = nullptr;

	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pyInfo.pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}

	Py_DECREF(pValue);

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

// Get input from user and display how many times a given item was purchased
void CaseTwo(const PyInfoHolder &pyInfo) {
	string item_name;
	cout << "Enter an item name: ";
	cin >> item_name;
	int freq = callStringFunc("GetFreq", item_name, pyInfo);
	if (freq != -1)
	{
		cout << item_name << " was purchased " << freq << " times." << endl;
	}
	else
	{
		cout << item_name << " was not found in list of purchased items." << endl;
	}

	cout << endl;
}

// This function expects the name of the file that holds our frequency data, created in PythonFile.py
void ReadFreqFile(string fileName) {

	cout << "Histogram" << endl
		 << "---------" << endl;

	ifstream infile(fileName);
	string line;
	while (std::getline(infile, line))
	{
		// Parse the line into name and frequency
		string delimiter = " : ";
		string name = line.substr(0, line.find(delimiter));
		string freq = line.substr(line.find(delimiter) + delimiter.length(), line.length());

		// Remove the whitespace from the freq
		freq.erase(remove_if(freq.begin(), freq.end(), isspace), freq.end());

		// Convert frequency into star string
		int starCount =  stoi(freq);
		string stars(starCount, '*');

		// NOTE(Ezra): The max length of an item is hard coded here
		int wordLengthMax = 11;
		cout << left << setw(wordLengthMax) << setfill(' ') << name << " " <<  stars << endl;
	}
	cout << endl;
}

int main()
{
	// Create an obect to hold python info so variables in python module persist
	// between function calls.
	// NOTE(Ezra): I thought this was better than storing the data in a file
	const PyInfoHolder pyInfo{};

	string fileName = "frequency.dat";
	callStringFunc("SetDataFile", fileName, pyInfo);
	CallProcedure("MakeDict", pyInfo);
	
	int input_i = 0;
	while (input_i != 4) 
	{
		cin.clear();

		// Show menu
		cout << "1: List items purchased w/ frequencies\n"
			"2: How many times an item was purchased on a day\n"
			"3: Histogram\n"
			"4 : Exit\n"
			"Enter your selection as a number 1, 2, 3, or 4.\n";
		cin >> input_i;

		// Call chosen option
		int input_value = 0;
		string item_name = "";
		int freq = -1;
		switch (input_i)
		{
		case 1:
			CallProcedure("ShowFrequencies", pyInfo);
			break;
		case 2:
			CaseTwo(pyInfo);
			break;
		case 3: 
			ReadFreqFile(fileName);
			break;
		case 4: cout << "Exit" << endl;
			break;
		default:
			cout << "You did not enter a valid command, please enter 1, 2, or 3." << endl;
			break;
		}
	}

	return 0;
}