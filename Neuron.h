#pragma once
#include<vector>
using namespace std;


struct neuron
{
	double out = 0;
	vector<double> w;
	double fi = 0;

	neuron() {}
	neuron(int size);
};


class NW
{
private:
	static const int _size = 4;
	neuron first[_size];
	neuron second;
	
protected:
	inline double f(double x);
	void ProcessNeuron(neuron& n, vector<double>& in);

public:
	double score = 0;
	double Calc(vector<double>& in);
	NW MakeChild(NW& nw);
	void Mutate();
	NW();
	//NW(NW& nw);
	NW& operator = (const NW& right);
};



class NWM
{
private:
	static const int _size = 100;
	vector<NW>gen;

	vector<vector<double>>test;
	vector<double> answer;
	
	NW Perfect;
protected:
	void CreateTests();
	inline double score(NW& nw);
public:
	NWM();
	
	int Train();
	void ShowTests();
};