#include"Neuron.h"
#include <iostream>

int main()
{

	NWM nwm;
	cout <<"Iterations: " << nwm.Train() << endl;
	nwm.ShowTests();

	/*int tests = 100;
	vector<int> iter;
	for (int i = 0; i < tests; i++)
	{
		NWM nwm; 
		iter.push_back(nwm.Train());
	}
	double avg = 0;
	for (auto& item : iter)avg += item;
	avg /= tests;
	cout << "Tests: " << tests << "Avg: " << avg << endl;
	for (int i = 0; i < tests / 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << i * 4 + j << ":[Iterations] ( " << iter[i * 4 + j] << " )\t";
		}
		cout << endl;
	}*/
}

