#include"Neuron.h"
#include <iostream>

int main()
{

	NWM nwm;
	cout << nwm.Train() << endl;
	nwm.ShowTests();
}

