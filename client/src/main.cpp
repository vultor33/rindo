//RINDO-CLIENT

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <cmath>
#include <stdlib.h>
#include <iomanip>

#include "refRindo.h"

using namespace std;


const string orcaPath = "/home/guga/ORCA/orca";
const string thisPath = "/home/guga/FRED/GA-uni/rodando/";
const string serverPath = "/home/guga/FRED/GA-uni/";

bool stopRead(vector<string> &auxStop)
{
	//the weight of the individual excitations
	if (
		(auxStop[0] == "the") &&
		(auxStop[1] == "weight") &&
		(auxStop[2] == "of") &&
		(auxStop[3] == "the") &&
		(auxStop[4] == "individual") &&
		(auxStop[5] == "excitations")
		)
	{
		return true;
	}
	else
		return false;
}

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		cout << "need orca input name" << endl;
		return 1;
	}
	stringstream auxCalcNumber;
	auxCalcNumber << argv[1];
	int calcNumber;
	auxCalcNumber >> calcNumber;

	defineRindoRef();

	string buildInp = "cat " + thisPath + inpNames[calcNumber] + ".mol "
                + thisPath + "paras.txt > "
		+ thisPath + inpNames[calcNumber] + ".inp";
	system(buildInp.c_str());
	//cout << buildInp << endl;

	string exec = orcaPath + " " + thisPath + inpNames[calcNumber] +
		".inp  >  " + thisPath + inpNames[calcNumber] + ".out";
	system(exec.c_str());
	//cout << exec << endl;

	// opening output
	string outFitness = thisPath + inpNames[calcNumber] + ".fit";
	ofstream fit_(outFitness.c_str());

	string auxline;
	string fileName = thisPath + inpNames[calcNumber] + ".out";
	ifstream in_(fileName.c_str());
	bool singlet = false;
	bool triplet = false;
	bool readSinglet = false;
	bool readTriplet = false;
	int iSing = 0;
	int iTrip = 0;
	string dum1, dum2, dum3;
	double auxDouble;

	while (!in_.eof())
	{
		getline(in_, auxline);
		if (auxline == "") continue; //ignore empty

		stringstream line;
		vector<string> auxStop(6);
		line << auxline;
		//cout << auxline << endl;

		line >> auxStop[0] >> auxStop[1] >> auxStop[2] >> auxStop[3]
			>> auxStop[4] >> auxStop[5];

		if (stopRead(auxStop))
		{
			if (!readSinglet)
				readSinglet = true;
			else
				readTriplet = true;
			continue;
		}

		if (
			(!singlet)&&(readSinglet)
			)
		{
			line.seekg(0);
			line >> dum1;
			if (dum1 == "STATE")
			{
				line >> dum1 >> dum2 >> auxDouble;
				//cout << "leu essa bosta:   " << dum1 << "  " << auxDouble << endl;
				inpSinglets[calcNumber][iSing] = abs(auxDouble - inpSinglets[calcNumber][iSing])
					/ inpSinglets[calcNumber][iSing];

				fit_ << setprecision(16) << inpSinglets[calcNumber][iSing] << endl;

				if (iSing == (inpStates[calcNumber]) - 1)
					singlet = true;
				iSing++;

			}
		}

		if (
			(!triplet) && (readTriplet)
			)
		{
			line.seekg(0);
			line >> dum1;
			if (dum1 == "STATE")
			{
				line >> dum1 >> dum2 >> auxDouble;
				//cout << "leu essa bosta:   " << dum1 << "  " << auxDouble << endl;
				inpTriplets[calcNumber][iTrip] = abs(auxDouble - inpTriplets[calcNumber][iTrip])
					/ inpTriplets[calcNumber][iTrip];

				fit_ << setprecision(16) << inpTriplets[calcNumber][iTrip] << endl;

				if (iTrip == (inpStates[calcNumber]) - 1)
					triplet = true;
				iTrip++;
			}
		}

		if (triplet)
			break;
	}
	in_.close();
	fit_.close();
	system(("mv "+thisPath+inpNames[calcNumber]+".fit "+serverPath).c_str());


	return 0;
}

/*
Projeto rindo

Passo 0
O otimizador chama rindo.exe

Passo 1
O rindo.exe copia o paras.txt para todas
as máquinas destino.

Passo 2
O rindo.exe fornece os nomes e ordena que os clientes
sejam executados criando vários processos.

Passo 2.1
O cliente copia o paras.txt no final do exemplo.mol
criando o exemplo.inp

Passo 2.2
O cliente executa o orca

Passo 2.3
O cliente deleta os arquivos lixo: .cw
O cliente procura pelo nome a sua referencia
e calcula abs(ref - resultado)
ele escreve no exemplo.fit a quantidade que precisar ser escrita

Passo 3
O servidor saberá que todos os clientes foram executados.
Portanto ele irá coletar (sftp get) todos os arquivos
de saída.

Passo 4
O servidor irá escrever um fitness.txt para o nsga-II
realizar a leitura.
--> Nesse ponto será feito uma mediana ou a fitness normal
a cargo do operador.

Passo 5
shut down.

ip branquinha: 150.161.5.110
*/


/*
IDEIAS PARA MELHORAR O CODIGO

Trocar define rindo por uma punhada de const string.




*/






/*
//	cat $FILENAME".mol" paras.txt > $FILENAME".inp"
//		$EXEC $FILENAME".inp" > $FILENAME".out"

string name = argv[1];
if (argc == 1)
{
cout << "need orca input name" << endl;
return 1;
}
Comecar fazendo mediana, depois eu evoluou

Primeiro ler a resposta para saber a quantidade de estados.
Vou incluir a resposta no executavel (.h)
Ter as funções: nome -> numero de estados.

Sempre vai ter n singletos e n tripletos
Ele le a energia
a palavra chave é: the weight of the individual

no final, pra saber se o thread terminou, o servidor pode
setar um bool depois da execucao: metanoExec = true;
-> depois join no que terminou.
tem pra rodar? sim, tem vaga? roda. Não? Espera.


*/
