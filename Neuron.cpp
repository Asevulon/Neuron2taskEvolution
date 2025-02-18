#include"Neuron.h"
#include<algorithm>
#include<time.h>
#include<iostream>
using namespace std;
double rand(double left, double right)
{
	return left + (right - left) * double(rand()) / double(RAND_MAX);
}


neuron::neuron(int size)
{
	w = vector<double>(size, 0);
	for (int i = 0; i < size; i++)
	{
		w[i] = rand(-0.5, 0.5);
	}
	fi = rand(-0.5, 0.5);
}


NW::NW()
{

	for (int i = 0; i < _size; i++)
	{
		first[i] = neuron(3);
	}
	second = neuron(_size);
}
NWM::~NWM()
{
}


NW& NW::operator = (const NW& nw)
{
	second = nw.second;
	for (int i = 0; i < _size; i++)first[i] = nw.first[i];
	return *this;
}


void NW::ProcessNeuron(neuron& n, vector<double>& in)
{
	double res = 0;
	for (int i = 0; i < in.size(); i++)
	{
		res += n.w[i] * in[i];
	}
	res -= n.fi;
	n.out = f(res);
}
double NW::Calc(vector<double>& in)
{
	vector<double>tin(_size, 0);
	for (int i = 0; i < _size; i++)
	{
		ProcessNeuron(first[i], in);
		tin[i] = first[i].out;
	}

	ProcessNeuron(second, tin);
	return second.out;
}

double NW::f(double x)
{
	return 1. / (1. + exp(-2 * x));
}


void NWM::CreateTests()
{
	test = { {0,0,0}, {0,0,1}, {0,1,0}, {0,1,1},
	{1,0,0}, {1,0,1}, {1,1,0}, {1,1,1} };
	answer = { 0,1,1,0,1,0,0,1 };
}

NW NW::MakeChild(NW& nw)
{
	NW res;

	double chance = 0;
	for (int i = 0; i < _size; i++)
	{
		chance = rand(0, 1);
		if (chance < 0.5)res.second.w[i] = second.w[i];
		else res.second.w[i] = nw.second.w[i];
	}

	chance = rand(0, 1);
	if (chance < 0.5)res.second.fi = second.fi;
	else res.second.fi = nw.second.fi;

	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			chance = rand(0, 1);
			if (chance < 0.5)res.first[i].w[j] = first[i].w[j];
			else res.first[i].w[j] = nw.first[i].w[j];
		}

		chance = rand(0, 1);
		if (chance < 0.5)res.first[i].fi = first[i].fi;
		else res.first[i].fi = nw.first[i].fi;
	}
	return res;
}

void NW::Mutate()
{
	for (int i = 0; i < _size; i++)
	{
		second.w[i] *= rand(0.99, 1.01);
	}
	second.fi *= rand(0.99, 1.01);

	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			first[i].w[j] *= rand(0.99, 1.01);
		}
		first[i].fi *= rand(0.99, 1.01);
	}
}




//-----------------------------------------------------------------------------
NWM::NWM()
{
	srand(time(NULL));
	for (int i = 0; i < _size; i++)
	{
		NW temp;
		gen.push_back(temp);
	}
	CreateTests();
}


inline double NWM::score(NW& nw)
{
	double res = 0;
	for (int i = 0; i < test.size(); i++)
	{
		double temp = nw.Calc(test[i]);
		temp -= answer[i];
		temp *= temp;
		res += temp;
	}
	res /= test.size();
	return res;
}



int GetPerfectId(vector<NW>& gen)
{
	int id = 0;
	int _size = gen.size();
	for (int i = 1; i < _size; i++)
	{
		if (gen[id].score > gen[i].score)id = i;
	}
	return id;
}

int NWM::Train()
{
	int ctr = 0;

	while (1)
	{
		int cursize = gen.size();
		if (cursize == 1)break;

		for (int i = 0; i < gen.size(); i++)
		{
			gen[i].score = score(gen[i]);
		}
		if (ctr % 500 == 0)
		{
			cout <<"Iterations: " << ctr << endl;
			int id = GetPerfectId(gen);
			Perfect = gen[id];
			ShowTests();
		}

		cursize = gen.size();
		for (int i = 0; i < cursize; i += 2)
		{
			if (gen[i].score < gen[i + 1].score)
			{
				gen[i].LowScore = true;
				gen[i + 1].LowScore = false;
			}
			else
			{
				gen[i].LowScore = false;
				gen[i + 1].LowScore = true;
			}
		}

		vector<NW> newgen;
		for (int i = 0; i < gen.size(); i++)
		{
			if (gen[i].LowScore)newgen.push_back(gen[i]);
		}
		gen = newgen;

		cursize = gen.size();
		for (int i = 0; i < cursize *0.9; i++)
		{
			int id = rand(0, cursize - 0.1);
			while (id == i)id = rand(0, cursize - 0.1);
			NW temp;
			temp = gen[i].MakeChild(gen[id]);
			gen.push_back(temp);
		}

		for (int i = 0; i < cursize / 10; i++)
		{
			NW temp;
			gen.push_back(temp);
		}

		for (int i = 0; i < gen.size(); i++)
		{
			gen[i].Mutate();
		}
		if (gen[0].score < 1e-4)
		{
			Perfect = gen[0];
			break;
		}
		ctr++;
		
		if (ctr >= 1e5)break;
	}
	int id = GetPerfectId(gen);
	Perfect = gen[id];

	return ctr;
}




void NWM::ShowTests()
{
	cout << "\n\nTest showcase:\n\n";
	for (int i = 0; i < test.size(); i++)
	{
		double calc = Perfect.Calc(test[i]);
		double mist = calc - answer[i];
		mist *= mist;
		string str = "ok";
		if (mist > 1e-2) str = "WA";
		printf("{ %d, %d, %d }\t%.3f\tans: %d\t%s\n", (int)test[i][0], (int)test[i][1], (int)test[i][2], calc, (int)answer[i], str.c_str());

	}
	cout << "\n\n";
}