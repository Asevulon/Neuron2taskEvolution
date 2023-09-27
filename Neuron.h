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


protected:
	inline double f(double x);
	void ProcessNeuron(neuron& n, vector<double>& in);

public:
	neuron first[_size];
	neuron second;
	double score = 0;
	bool LowScore = false;
	inline double Calc(vector<double>& in);
	inline NW MakeChild(NW& nw);
	inline void Mutate();
	NW();
	//NW(NW& nw);
	NW& operator = (const NW& right);
};



class NWM
{
private:
	static const int _size = 5000;
	const double MCver = 0.25;
	const double MutationVer = 0.2;

	vector<vector<double>>test;
	vector<double> answer;

	NW Perfect;


protected:
	void CreateTests();
	inline double score(NW& nw);
public:
	NWM();
	~NWM();
	int Train();
	void ShowTests();
	vector<NW>gen;

};