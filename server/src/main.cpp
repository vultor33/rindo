#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>

#include "Socket.h"
#include "ClientSocket.h"
#include "SocketException.h"
#include "refRindo.h"
#include "refData.h"

using namespace std;

//globals
vector<bool> done;
vector<int> waysToRun;
const string clientPath =  "/home/guga/FRED/GA-uni/rodando/";
const string serverPath =  "/home/guga/FRED/GA-uni/";

const int paramsNumber = 47;

double get_cpu_time()
{
	return (double)clock() / CLOCKS_PER_SEC;
}

bool is_file_exist(const char *fileName)
{
	ifstream infile(fileName);
	return infile.good();
}

void lerPontoFit(int molNumber)
{
	ifstream in_;
	string readName;

	for(int i=0; i<molNumber; i++)
	{
		readName = inpNames[i] + ".fit";
		in_.open(readName.c_str());
		for(int j=0; j<inpStates[i]; j++)
		{
			in_ >> inpSinglets[i][j];
		}
		for(int k=0; k<inpStates[i]; k++)
		{
			in_ >> inpTriplets[i][k];
		}
		in_.close();
	}
}

double mediaVector(vector<double> &vec)
{
        int size = vec.size();
        double media = 0.0e0;
        for(int i=0; i<size; i++)
        {
                media += vec[i];
        }
        return media/size;

}

double pontoVectorMediana(int molNumber)
{
        vector<double> pontoVector(statesNumber);
        int k=0;

        for(int i=0; i<molNumber; i++)
        {
                for(int j=0; j<inpStates[i]; j++)
                {
                        pontoVector[k] = inpSinglets[i][j]*100.0e0;
                        k++;
                }
                for(int l=0; l<inpStates[i]; l++)
                {
                        pontoVector[k] = inpTriplets[i][l]*100.0e0;
                        k++;
                }
        }

        sort(pontoVector.begin(), pontoVector.end());
        //for(int i=0; i<statesNumber; i++)
        //{
        //      cout << "pV  " << i << "  " << pontoVector[i] << endl;
        //}

        int i1 = -1+statesNumber/2;
        int i2 = i1+1;
       	//cout << i1 << "  i2  " << i2 << "   :   " << (pontoVector[i1]+pontoVector[i2])*0.5e0  << endl;

	//cout << "states num" << statesNumber << endl;
	//cout << pontoVector[i1-1] << "   " << pontoVector[i1] << "   " << pontoVector[i2]
	//	<< "   " << pontoVector[i2+1] << endl;

        return ((pontoVector[i1]+pontoVector[i2])*0.5e0);
}



void geraBatch(string caminho, int molNumber)
{
//get /home/fred/rindo/rindo/client/teste2/Z_ethanimine.fit

	string batchName, batchCommand;
	ofstream of_;
	for (int i = 0; i<molNumber; i++)
	{
		batchName = inpNames[i]+".b";
		of_.open(batchName.c_str());
		batchCommand = "get " + caminho + inpNames[i] + ".fit";
		of_ << batchCommand << endl;
		of_.close();
	}
}

void runAll(int molNumber)
{
	system("sbatch ./sremes/roda-paral-1.srm");
	system("sbatch ./sremes/roda-paral-2.srm");
	system("sbatch ./sremes/roda-paral-3.srm");
	system("sbatch ./sremes/roda-paral-4.srm");
	system("sbatch ./sremes/roda-paral-5.srm");
	system("sbatch ./sremes/roda-paral-6.srm");
	system("sbatch ./sremes/roda-paral-7.srm");
	system("sbatch ./sremes/roda-paral-8.srm");
	system("sbatch ./sremes/roda-paral-9.srm");
	system("sbatch ./sremes/roda-paral-10.srm");
}

void waitUntilFinish()
{
	ifstream in_;
	int numberOfFiles;
	while(true)
	{
		sleep(0.01);
		system(("find "+serverPath+"*.fit -type f | wc -l > numberFile.txt").c_str());
		in_.open("numberFile.txt");
		in_ >> numberOfFiles;
		if(numberOfFiles == 200)
			break;

		in_.close();
		remove("numberFile.txt");
	}
	remove("numberFile.txt");
}




vector<double> enviaParas()
{
	fstream in_("gen.ga");
        vector<double> params(paramsNumber);
        for (int i=0; i<paramsNumber; i++)
        {
                in_ >> params[i];
        }
        in_.close();

        ofstream of_("paras.txt");

        of_ << "%ndoparas" << endl;
        of_ <<  refdataNames[0] << "      "
                << setprecision(16) << refdata[0] << endl;
        for(int i=0; i<paramsNumber; i++)
        {
                of_ << refdataNames[i+1] << "      " <<
                        setprecision(16) << refdata[i+1]*params[i] << endl;
        }
        of_ << "end" << endl;
        of_.close();

	system(("mv paras.txt "+clientPath).c_str());

	return params;
}

void writeMultiFitness(int molNumber, ofstream &res_)
{
        ofstream fit_("fitness.ga");
        double auxMedia;
        for(int i=0; i<molNumber; i++)
        {
                vector<double> pontoVector(2*inpStates[i]);
                int k=0;
                for(int j=0; j<inpStates[i]; j++)
                {
                        pontoVector[k] = inpSinglets[i][j]*100.0e0;
                        k++;
                }
                for(int l=0; l<inpStates[i]; l++)
                {
                        pontoVector[k] = inpTriplets[i][l]*100.0e0;
                        k++;
                }
                auxMedia = mediaVector(pontoVector);
                if(isnan(auxMedia))
                {
                        fit_ << 1.0e5 << endl;
                        res_ << 1.0e5 << "   ";
                }
                else
                {
                        if(auxMedia < 0.0e0)
                        {
                                fit_ << 1.0e5 << endl;
                                res_ << 1.0e5 << "   ";
                        }
                        else
                        {
                                fit_ << setprecision(16) << auxMedia << endl;
                                res_ << setprecision(16) << auxMedia << "   ";
                        }
                }
        }
        fit_.close();
}



int main()
{
	const int molNumber = 200;
	vector<string> allFit(molNumber);
	string fitness;
	ofstream fitGa_;
	ofstream restart_;
	bool trySocketAgain = true;
	if (!is_file_exist("restartEnd.ga"))
	{
		while(trySocketAgain)
		{
			try
			{
				ClientSocket client_socket("localhost", 30000);
				try
				{
					client_socket << "r";
					if (is_file_exist("univariate.ga"))
					{
						client_socket >> fitness;
					}
					else
					{
						client_socket >> fitness;
						ifstream pass_("rindo-multi-pass.txt");
						for(int i=0; i<molNumber; i++)
						{
							pass_ >> allFit[i];
						}
						pass_.close();
						remove("rindo-multi-pass.txt");
					}
				}
				catch (SocketException&) {}
	
			}
			catch (SocketException& e)
			{
				std::cout << "Exception was caught:" << e.description() << "\n";
				sleep(0.001);
			}
	
			fitGa_.open("fitness.ga");
			if (is_file_exist("univariate.ga"))
			{
				fitGa_ << fitness << endl;
			}
			else
			{
				for(int i=0; i<molNumber; i++)
				{
					fitGa_ << allFit[i] << endl;
				}
			}

			fitGa_.close();
			trySocketAgain = false;
			cout << "pegou:  " << fitness << endl;
		}
	}
	else
	{
		// essa parte e quando os pontos esgotarem
		defineRindoRef();
		defineRefData();

		vector<double> genParams = enviaParas();

		runAll(molNumber);
		waitUntilFinish();

		lerPontoFit(molNumber);

		double auxfitness = pontoVectorMediana(molNumber);
		stringstream convert;
		convert << setprecision(16) << auxfitness;
		convert >> fitness;
		if (is_file_exist("univariate.ga"))
		{
			fitGa_.open("fitness.ga");
			fitGa_ << fitness << endl;
			fitGa_.close();

			//restart_.open("restart.ga", ios_base::app);
			//restart_ << fitness << endl;
			//restart_.close();
		}
		else
		{
			restart_.open("restart-multi.ga",ios_base::app);
			restart_ << fitness << "   ";

			writeMultiFitness(molNumber,restart_);

			for(int i=0; i<(int)genParams.size();i++)
			{
				restart_ << "   " << genParams[i];
			}
			restart_ << endl;
			restart_.close();
		}

		system("rm slur*");
		system("rm *.fit");
		system(("rm " + clientPath + "*.inp").c_str());
		system(("rm " + clientPath + "*.cis").c_str());
		system(("rm " + clientPath + "*.prop").c_str());
		system(("rm " + clientPath + "*.gbw").c_str());
		system(("rm " + clientPath + "*.out").c_str());
		system(("rm " + clientPath + "*.ges").c_str());
	}
	return 0;
}


