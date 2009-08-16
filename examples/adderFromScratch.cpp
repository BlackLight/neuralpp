/**
 * Similar to learnAdd.cpp, but this time the
 * training XML is generated as a string and not saved to a file, and parsed by the
 * program itself to build the network. Then, the program asks two real numbers, and
 * performs both the sum and the difference between them, putting the sum's output on
 * the first output neuron and the difference's on the second output neuron. Anyway,
 * using more than one neuron in the output layer is strongly discouraged, as the network
 * usually won't set correctly the synaptical weights to give satisfying and accurate
 * answers for all of the operations.
 *
 * by BlackLight, 2009
 */

#include <iostream>
#include <neural++.hpp>

using namespace std;
using namespace neuralpp;

int main()  {
	NeuralNet net(2, 2, 2, 0.005, 100);
	string xml;
	double tmp;
	int id = 0;

	// XML initialization. Then, I say XML that 2+3=5, 3+3=6, 5+4=9
	// Strings' format is "input1,input2,...,inputn;output1,output2,...,outputm
	NeuralNet::initXML(xml);
	xml += NeuralNet::XMLFromSet(id, "3,2;5,1");
	xml += NeuralNet::XMLFromSet(id, "4,2;6,2");
	xml += NeuralNet::XMLFromSet(id, "6,3;9,3");
	NeuralNet::closeXML(xml);
	cout << xml << endl;
	
	net.train(xml, NeuralNet::str);
	vector<double> v;
	cout << "Network status: trained\n\n";

	cout << "First number to add: ";
	cin >> tmp;
	v.push_back(tmp);

	cout << "Second number to add: ";
	cin >> tmp;
	v.push_back(tmp);

	net.setInput(v);
	net.propagate();
	cout << "Output: " << net.getOutputs()[0] << "; " << net.getOutputs()[1] << endl;
	return 0;
}

