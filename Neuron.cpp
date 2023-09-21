#include"Neuron.h"
#include<algorithm>
double rand(double left, double right)
{
	return left + (right - left) * double(rand()) / double(RAND_MAX);
}


neuron::neuron(int size)
{
	w = vector<double>(size, 0);
	for (int i = 0; i < size; i++)
	{
		w[i] = rand(-1, 1);
	}
	fi = rand(-1, 1);
}


NW::NW()
{
	for (int i = 0; i < _size; i++)
	{
		first[i] = neuron(3);
	}
	second = neuron(_size);
}
//NW::NW(NW& nw)
//{
//	second = nw.second;
//	for (int i = 0; i < _size; i++)first[i] = nw.first[i];
//}
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
	
	for (int i = 0; i < _size; i++)
	{
		res.second.w[i] = (second.w[i] + nw.second.w[i]) / 2.;
	}
	res.second.fi = (second.fi + nw.second.fi) / 2.;


	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			res.first[i].w[j] = (first[i].w[j] + nw.first[i].w[j]) / 2.;
		}
		res.first[i].fi = (first[i].fi + nw.first[i].fi) / 2.;
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
	gen = vector<NW>(_size, NW());
	CreateTests();
}


inline double NWM::score(NW& nw)
{
	double res = 0;
	for (int i = 0; i < test.size(); i++)
	{
		double temp= nw.Calc(test[i]);
		temp -= answer[i];
		temp *= temp;
		res += temp;
	}
	res /= test.size();
	return res;
}




bool compare(NW& left, NW& right)
{
	return left.score > right.score;
}
int NWM::Train()
{
	int ctr = 0;

	while (1)
	{
		double AvgMist = 0;
		int cNum = gen.size() / 2. - 1;
		int half = gen.size() / 2. + 1;
		int cSize = gen.size();
		for (int i = 0; i < cNum; i++)
		{
			NW temp;
			temp = gen[i].MakeChild(gen[rand(half, cSize)]);
			gen.push_back(temp);
		}
		
		for (int i = 0; i < gen.size(); i++)
		{
			gen[i].score = score(gen[i]);
			AvgMist += gen[i].score;
		}
		AvgMist /= gen.size();

		for (int i = 0; i < gen.size(); i++)
		{
			if (AvgMist > gen[i].score)
			{
				gen[i] = gen[0];
				gen.erase(gen.begin());
				i--;
			}
		}
		sort(gen.begin(), gen.end(), compare);
		if (gen[0].score < 1e-2)
		{
			Perfect = gen[0];
			break;
		}
		NW mutant = gen[0];
		mutant.Mutate();
		gen.push_back(mutant);

		if (ctr >= 1e6)break;
	}


	return ctr;
}



#include<iostream>
using namespace std;

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