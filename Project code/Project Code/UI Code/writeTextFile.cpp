// writeTextFile.cpp : Defines the entry point for the console application.
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


int main()
{
	int answer = 0;
	char license[8];
	string search;
	ifstream inFile;
	string line;
	while(1){
	cout<<"Enter 1 to add license plate or 2 to remove license plate: "<<endl;
	cin>>answer;
	ofstream outfile;
	outfile.open("licensePlates.txt",ios_base::app);
	if(answer == 1)
	{
		cout<<"Enter license plate: "<<endl;
		cin>>license;
		outfile << license <<endl;
		cout << license <<" successfully entered into system." << endl;

	}
	else if(answer == 2)
	{

		inFile.open("licensePlates.txt");

		if(!inFile){
			cout << "Unable to open file" << endl;
		}
		cout << "Enter word to search for: ";
		cin >>search;


		size_t pos;
		while(inFile.good())
		{
			getline(inFile,line); // get line from file
			while(search.size() != 7)
			{
				cout<<"Invalid length of input. Please enter it again:"<<endl;
				cin>>search;
			}
			pos=line.find(search); // search
			if(pos!=string::npos) // string::npos is returned if string is not found
			{
				cout <<"Found!"<<endl;
				search = "";
				break;
			}
			else
			{
				cout<<"Not found"<<endl;
				break;
			}
		}
	}
	}

	return 0;
}

