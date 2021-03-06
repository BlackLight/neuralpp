/**************************************************************************************************
 * LibNeural++ v.0.4 - All-purpose library for managing neural networks                           *
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

#include <cstdlib>
#include "neural++.hpp"

namespace neuralpp {
	Synapsis::Synapsis(Neuron * i, Neuron * o, double (*a) (double)) {
		srand((unsigned) time(NULL));

		delta = 0;
		prev_delta = 0;
		weight = RAND;
		in = i;
		out = o;

		actv_f = a;
	}

	Synapsis::Synapsis(Neuron * i, Neuron * o,
			   double w, double (*a) (double)) {
		
		delta = 0;
		prev_delta = 0;
		weight = w;
		in = i;
		out = o;

		actv_f = a;
	}

	Neuron *Synapsis::getIn() const  {
		return in;
	}

	Neuron *Synapsis::getOut() const  {
		return out;
	}

	double Synapsis::getWeight() const  {
		return weight;
	}

	double Synapsis::getDelta() const  {
		return delta;
	}

	double Synapsis::getPrevDelta() const  {
		return prev_delta;
	}

	void Synapsis::setWeight(double w) throw(InvalidSynapticalWeightException)  {
		if (weight > 1.0)
			throw InvalidSynapticalWeightException();

		weight = w;
	}

	void Synapsis::setDelta(double d) throw(InvalidSynapticalWeightException)  {
		prev_delta = delta;
		delta = d;
	}

	double Synapsis::momentum(int N, int x) const  {
		return (BETA0 * N) / (20 * x + N);
	}
}

