/**************************************************************************************************
 * LibNeural++ v.0.2 - All-purpose library for managing neural networks                           *
 * Copyright (C) 2009, BlackLight                                                                 *
 *                                                                                                *
 * This program is free software: you can redistribute it and/or modify it under the terms of the *
 * GNU General Public License as published by the Free Software Foundation, either version 3 of   *
 * the License, or (at your option) any later version. This program is distributed in the hope    *
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
 * more details. You should have received a copy of the GNU General Public License along with     *
 * this program. If not, see <http://www.gnu.org/licenses/>.                                      *
 **************************************************************************************************/

#ifndef __cplusplus
#error "This is a C++ library, you know, so you'd better use a C++ compiler to compile it"
#else

#ifndef __NEURALPP
#define __NEURALPP

#include <vector>
#include <string>
#include <cmath>

#include "neural++_exception.hpp"
using namespace std;

//! Default rand value: |sin(rand)|, always >= 0 and <= 1
#define	RAND		( abs( sin(rand()) ) )

//! Initial value for the inertial momentum of the synapses
#define 	BETA0 	0.7

/**
 * @namespace neuralpp
 * @brief Main namespace for the library
 */
namespace neuralpp  {
	class Synapsis;
	class Neuron;
	class Layer;
	class NeuralNet;
	class NetworkFileNotFoundException;
	class InvalidXMLException;

	/**
	 * @class NeuralNet
	 * @brief Main project's class. Use *ONLY* this class, unless you know what you're doing
	 */
	class NeuralNet  {
		int epochs;
		int ref_epochs;
		double l_rate;
		double ex;

		Layer* input;
		Layer* hidden;
		Layer* output;

		/**
		 * @brief It updates the weights of the net's synapsis through back-propagation.
		 *   In-class use only
		 */
		void updateWeights();

		/**
		 * @brief It commits the changes made by updateWeights() to the layer l.
		 *   In-class use only
		 * @param l Layer to commit the changes
		 */
		void commitChanges (Layer *l);

		/**
		 * @brief It get the error made on the expected result as |v-v'|/v
		 * @param ex Expected value
		 * @return Mean error
		 */
		double error(double ex);
		
		/**
		 * @brief Private pointer to function, containing the function to
		 * be used as activation function
		 */
		double (*actv_f)(double);

		/**
		 * @brief Private pointer to function, containing the function to
		 * be used as derivate of the activation function
		 */
		double (*deriv)(double);

	public:
		/**
		 * @brief Enum to choose the eventual training source for our network (XML from a file or from a string)
		 */
		typedef enum  { file, str } source;

		/**
		 * @brief Empty constructor for the class - it just makes nothing
		 */
		NeuralNet()  {}

		/**
		 * @brief Constructor
		 * @param in_size Size of the input layer
		 * @param hidden_size Size of the hidden layer
		 * @param out_size Size of the output layer
		 * @param l learn rate (get it after doing some experiments, but generally try to
		 *   keep its value quite low to be more accurate)
		 * @param e Epochs (cycles) to execute (the most you execute, the most the network
		 *   can be accurate for its purpose)
		 */
		NeuralNet (size_t in_size, size_t hidden_size, size_t out_size, double l, int e);

		/**
		 * @brief Constructor
		 * @param file Binary file containing a neural network previously saved by save() method
		 * @throw NetworkFileNotFoundException
		 */
		NeuralNet (const char* file) throw(NetworkFileNotFoundException);
		
		
		/**
		 * @brief Constructor
		 * @param in_size Size of the input layer
		 * @param hidden_size Size of the hidden layer
		 * @param out_size Size of the output layer
		 * @param actv Activation function to use (default: f(x)=x)
		 * @param deriv Derivate for the activation function to use (default: f'(x)=1)
		 * @param l learn rate (get it after doing some experiments, but generally try to
		 *   keep its value quite low to be more accurate)
		 * @param e Epochs (cycles) to execute (the most you execute, the most the network
		 *   can be accurate for its purpose)
		 */
		NeuralNet (size_t in_size, size_t hidden_size, size_t out_size,
				double(*actv)(double), double(*deriv)(double), double l, int e);

		/**
		 * @brief It gets the output of the network (note: the layer output should contain
		 * an only neuron)
		 * @return The output value of the network
		 */
		double getOutput();

		/**
		 * @brief It gets the output of the network in case the output layer contains more neurons
		 * @return A vector containing the output values of the network
		 */
		vector<double> getOutputs();

		/**
		 * @brief It gets the value expected. Of course you should specify this when you
		 * build your network by using setExpected.
		 * @return The expected output value for a certain training phase
		 */
		double expected();

		/**
		 * @brief It sets the value you expect from your network
		 * @param ex Expected output value
		 */
		void setExpected(double ex);

		/**
		 * @brief It updates through back-propagation the weights of the synapsis and
		 * computes again the output value for <i>epochs</i> times, calling back
		 * updateWeights and commitChanges functions
		 */
		void update();

		/**
		 * @brief It propagates values through the network. Use this when you want to give
		 * an already trained network some new values the get to the output
		 */
		void propagate();

		/**
		 * @brief It sets the input for the network
		 * @param v Vector of doubles, containing the values to give to your network
		 */
		void setInput (vector<double>& v);

		/**
		 * @brief It links the layers of the network (input, hidden, output). Don't use unless
		 * you exactly know what you're doing, it is already called by the constructor
		 */
		void link();

		/**
		 * @brief Save a trained neural network to a binary file
		 * @param fname Binary file where you're going to save your network
		 */
		bool save(const char* fname);

		/**
		 * @brief Train a network using a training set loaded from an XML file. A sample XML file
		 *   is available in examples/adder.xml
		 * @param xml XML file containing our training set
		 * @param src Source type from which the XML will be loaded (from a file [default] or from a string)
		 * @throw InvalidXMLException
		 */
		void train(string xml, source xrc) throw(InvalidXMLException);

		/**
		 * @brief Initialize the training XML for the neural network
		 * @param xml String that will contain the XML
		 */
		static void initXML (string& xml);

		/**
		 * @brief Splits a string into a vector of doubles, given a delimitator
		 * @param delim Delimitator
		 * @param str String to be splitted
		 * @return Vector of doubles containing splitted values
		 */
		static vector<double> split (char delim, string str);

		/**
		 * @brief Get a training set from a string and copies it to an XML
		 *   For example, these strings could be training sets for making sums:
		 *   "2,3;5" - "5,6;11" - "2,2;4" - "4,5:9"
		 *   This method called on the first string will return an XML such this:
		 *   '&lt;training id="0"&gt;&lt;input id="0"&gt;2&lt;/input&gt;&lt;input id="1"&gt;3&lt;/input&gt;&lt;output id="0"&gt;5&lt;/output&gt;
		 *   &lt/training&gt;'
		 *
		 * @param id ID for the given training set (0,1,..,n)
		 * @param set String containing input values and expected outputs
		 * @return XML string
		 */
		static string XMLFromSet (int id, string set);

		/**
		 * @brief Closes an open XML document generated by "initXML" and "XMLFromSet"
		 * @param xml XML string to be closed
		 */
		static void closeXML(string& xml);
	};

	/**
	 * @class Synapsis
	 * @brief Class for managing synapsis. Don't use this class directly unless you know what
	 *  you're doing, use NeuralNet instead
	 */
	class Synapsis  {
		double delta;
		double prev_delta;
		double weight;

		Neuron *in;
		Neuron *out;
		
		double (*actv_f)(double);
		double (*deriv)(double);

	public:
		/**
		 * @brief Constructor
		 * @param i Input neuron
		 * @param o Output neuron
		 * @param w Weight for the synapsis
		 * @param d Delta for the synapsis
		 */
		Synapsis(Neuron* i, Neuron* o, double w, double d);

		/**
		 * @brief Constructor
		 * @param i Input neuron
		 * @param o Output neuron
		 * @param a Activation function
		 * @param d Derivate for activation function
		 */
		Synapsis (Neuron* i, Neuron* o, double(*a)(double), double(*d)(double));
		
		/**
		 * @brief Constructor
		 * @param i Input neuron
		 * @param o Output neuron
		 * @param w Weight for the synapsis (default: random)
		 * @param a Activation function
		 * @param d Derivate for activation function
		 */
		Synapsis (Neuron* i, Neuron* o,
				double w, double(*a)(double), double(*d)(double));
	
		/**
		 * @return Reference to input neuron of the synapsis
		 */
		Neuron* getIn();
		
		/**
		 * @return Reference to output neuron of the synapsis
		 */
		Neuron* getOut();

		/**
		 * @brief Set the weight of the synapsis
		 * @param w Weight to be set
		 */
		void setWeight(double w);
		
		/**
		 * @brief It sets the delta (how much to change the weight after an update) 
		 * of the synapsis
		 * @param d Delta to be set
		 */
		void setDelta(double d);

		/**
		 * @brief Return the weight of the synapsis
		 * @return Weight of the synapsis
		 */
		double getWeight();
		
		/**
		 * @brief Return the delta of the synapsis
		 * @return Delta of the synapsis
		 */
		double getDelta();
		
		/**
		 * @brief Get the delta of the synapsis at the previous iteration
		 * @return The previous delta
		 */
		double getPrevDelta();

		/**
		 * @brief Get the inertial momentum of a synapsis. This value is inversely proportional
		 * to the number of steps done in the learning phase (quite high at the beginning, decreasing
		 * to zero towards the end of the learning algorithm), and is needed to avoid strong
		 * oscillations in output values at the beginning, caused by the random values assigned to
		 * the synaptical weights
		 * @param N The number of iterations the network must have to adjust the output values
		 * @param x The number of iterations already taken
		 * @return The inertial momentum of the synapsis
		 */
		double momentum (int N, int x);
	};

	/**
	 * @class Neuron
	 * @brief Class for managing neurons. Don't use this class directly unless you know what
	 *  you're doing, use NeuralNet instead
	 */
	class Neuron  {
		double actv_val;
		double prop_val;

		vector< Synapsis > in;
		vector< Synapsis > out;

		double (*actv_f)(double);
		double (*deriv)(double);
	public:
		/**
		 * @brief Constructor
		 * @param a Activation function
		 * @param d Its derivate
		 */
		Neuron (double (*a)(double), double(*d)(double));

		/**
		 * @brief Alternative constructor, that gets also the synapsis linked to the neuron
		 * @param in Input synapses
		 * @param out Output synapses
		 * @param a Activation function
		 * @param d Derivate of the activation function
		 */
		Neuron (vector<Synapsis> in, vector<Synapsis> out,
				double (*a)(double), double(*d)(double));

		/**
		 * @brief Get the i-th synapsis connected on the input of the neuron
		 * @param i Index of the input synapsis to get
		 * @return Reference to the i-th synapsis
		 */
		Synapsis& synIn (size_t i);
		
		/**
		 * @brief Get the i-th synapsis connected on the output of the neuron
		 * @param i Index of the output synapsis to get
		 * @return Reference to the i-th synapsis
		 */
		Synapsis& synOut (size_t i);

		/**
		 * @brief It pushes a new input synapsis
		 * @param s Synapsis to be pushed
		 */
		void push_in (Synapsis& s);
		
		/**
		 * @brief It pushes a new output synapsis
		 * @param s Synapsis to be pushed
		 */
		void push_out (Synapsis& s);

		/**
		 * @brief Change the activation value of the neuron
		 * @param a Activation value
		 */
		void setActv (double a);

		/**
		 * @brief Change the propagation value of the neuron
		 * @param p Propagation value
		 */
		void setProp (double p);

		/**
		 * @brief Get the activation value of the neuron
		 * @return Activation value for the neuron
		 */
		double getActv();
		
		/**
		 * @brief Get the propagation value of the neuron
		 * @return Propagation value for the neuron
		 */
		double getProp();

		/**
		 * @brief It propagates its activation value to the connected neurons
		 */
		double propagate();

		/**
		 * @brief Get the number of input synapsis for the neuron
		 * @return Number of input synapsis
		 */
		size_t nIn();
		
		/**
		 * @brief Get the number of output synapsis for the neuron
		 * @return Number of output synapsis
		 */
		size_t nOut();

		/**
		 * @brief Remove input and output synapsis from a neuron
		 */
		void synClear();
	};

	/**
	 * @class Layer
	 * @brief Class for managing layers of neurons. Don't use this class directly unless you know what
	 *  you're doing, use NeuralNet instead
	 */
	class Layer  {
		vector<Neuron> elements;

		void (*update_weights)();
		double (*actv_f)(double);
		double (*deriv)(double);

	public:
		/**
		 * @brief Constructor
		 * @param sz Size of the layer
		 * @param a Activation function
		 * @param d Its derivate
		 */
		Layer (size_t sz, double (*a)(double), double(*d)(double));

		/**
		 * @brief Alternative constructor. It directly gets a vector of neurons to build
		 * the layer
		 * @param neurons Vector of neurons to be included in the layer
		 * @param a Activation function
		 * @param d Its derivate
		 */
		Layer (vector<Neuron>& neurons, double(*a)(double), double(*d)(double));

		/**
		 * @brief Redefinition for operator []. It gets the neuron at <i>i</i>
		 * @param i Index of the neuron to get in the layer
		 * @return Reference to the i-th neuron
		 */
		Neuron& operator[] (size_t i);

		/**
		 * @brief It links a layer to another
		 * @param l Layer to connect to the current as input layer
		 */
		void link (Layer& l);

		/**
		 * @brief It sets a vector of propagation values to all its neurons
		 * @param v Vector of values to write as propagation values
		 */
		void setProp (vector<double>& v);
		 
		/** 
		 * @brief It sets a vector of activation values to all its neurons
		 * @param v Vector of values to write as activation values
		 */
		void setActv (vector<double>& v);

		/**
		 * @brief It propagates its activation values to the output layers
		 */
		void propagate();

		/**
		 * @return Number of neurons in the layer
		 */
		size_t size();
	};

	struct netrecord  {
		int input_size;
		int hidden_size;
		int output_size;

		int epochs;
		double l_rate;
		double ex;
	};

	struct neuronrecord  {
		double prop;
		double actv;
	};

	struct synrecord  {
		double w;
		double d;
	};
}

#endif
#endif

