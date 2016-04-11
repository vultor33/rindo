#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctype.h>

void leiaRindoRef()
{
	using namespace std;

	vector<string> inpNames;
	string auxName;
	vector<int> inpStates;
	int auxStates;
	vector< vector<double> > inpSinglets;
	vector< vector<double> > inpTriplets;
	double auxS1, auxS2, auxS3, auxS4;
	double auxT1, auxT2, auxT3, auxT4;

	string auxline;
	ifstream in_("ref.txt");

	while (!in_.eof())
	{
		getline(in_, auxline);
		stringstream line;
		line << auxline;
		line >> auxName;
		inpNames.push_back(auxName);
		//cout << auxName << endl;

		getline(in_, auxline);
		stringstream line2;
		line2 << auxline;
		line2 >> auxStates;
		inpStates.push_back(auxStates);
		//cout << auxStates << endl;

		vector<double> auxSinglets(auxStates);
		vector<double> auxTriplets(auxStates);

		getline(in_, auxline);
		stringstream line3;
		line3 << auxline;
		line3 >> auxS1 >> auxT1;
		auxSinglets[0] = auxS1;
		auxTriplets[0] = auxT1;
		//cout << auxS1 << "   " << auxT1 << endl;
		if (auxStates > 1)
		{
			getline(in_, auxline);
			stringstream line4;
			line4 << auxline;
			line4 >> auxS2 >> auxT2;
			auxSinglets[1] = auxS2;
			auxTriplets[1] = auxT2;
			//cout << auxS2 << "   " << auxT2 << endl;
			if (auxStates > 2)
			{
				getline(in_, auxline);
				stringstream line5;
				line5 << auxline;
				line5 >> auxS3 >> auxT3;
				auxSinglets[2] = auxS3;
				auxTriplets[2] = auxT3;
				//cout << auxS3 << "   " << auxT3 << endl;
				if (auxStates > 3)
				{
					getline(in_, auxline);
					stringstream line6;
					line6 << auxline;
					line6 >> auxS4 >> auxT4;
					auxSinglets[3] = auxS4;
					auxTriplets[3] = auxT4;
				}
			}
		}
		inpSinglets.push_back(auxSinglets);
		inpTriplets.push_back(auxTriplets);
	}
	in_.close();

	ofstream of_("refRindo.txt");
	int size = inpNames.size();
	cout << "tamanho:  " << size << endl;
	for (int i = 0; i < size; i++)
	{
		of_ << "\tinpNames[" << i << "] = \"" << inpNames[i] << "\";" << endl;
		of_ << "\tinpStates[" << i << "] = " << inpStates[i] << ";" << endl;
		of_ << "\tinpSinglets[" << i << "].resize(" << inpStates[i] << ");" << endl;
		of_ << "\tinpTriplets[" << i << "].resize(" << inpStates[i] << ");" << endl;
		for (int j = 0; j < inpStates[i]; j++)
		{
			of_ << "\tinpSinglets[" << i << "][" << j << "] = " << inpSinglets[i][j] << ";" << endl;
			of_ << "\tinpTriplets[" << i << "][" << j << "] = " << inpTriplets[i][j] << ";" << endl;
		}
	}
	of_.close();
}
