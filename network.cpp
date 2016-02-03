#include "network.h"


network::network(int nNeurons)
{
    _nNeurons = nNeurons;
    _networkTicked = false;
    __debug__ = false;
    _neurons = new std::vector<neuron*>(nNeurons);
    _startNeuron = 0; // Assume that the zero th neuron is used to start the network..
    for (int i = 0; i < _nNeurons; i++)
    {
        _neurons->at(i) = new neuron();
        if (__debug__) { debugN("NEURON ADDED"); debug("ID"); debug(_neurons->at(i)->getId()); }
    }
}


network::~network()
{
    for (int i = 0; i < _nNeurons; i++)
    {
        if (__debug__) { debugN("NEURON REMOVED"); debug("ID"); debug(_neurons->at(i)->getId()); }
        delete _neurons->at(i);
    }
    delete _neurons;

}



/*
Keeps on doing the systemTick , until all neurons has responded to tick and has progressed to the next state..

*/
void network::systemTick()
{
    _networkTicked = false;
    for (auto i : *_neurons)
    {
        if (!i->isTickComplete()) // Ensure that neuron is not ticked multiple times in a single network tick.
        {
            _networkTicked = i->tick();
        }
        else
        {
            if (!_networkTicked)
                _networkTicked = true; // If there are no cases of failed tick . and tick is complete , then we can assume
                                        // that entire network has ticked...
        }
    }

    if (!_networkTicked)
    {
        /// we call this function again.
        // will it lead to infinite recursion.
        // No as in each call more and more of the neurons will be ticked.
        systemTick();
    }

    for (auto i : *_neurons)
    {
        if (i->getId() == _startNeuron) continue; // Ensure that the starter neuron is only ticked once. This prevents the reset , which allows for the next tick.
        i->resetTicked();// for the next network tick..
    }
}


void network::addNeuron()
{
    neuron *n = new neuron();
    _nNeurons++;
    n->setDebug(__debug__);
    _neurons->push_back(n);

    if (__debug__) { debugN("NEURON ADDED"); debug("ID"); debug(_neurons->at(_nNeurons-1)->getId()); }
}


/*
Allow the user to make changes to the neuron...
0 indexed
*/

neuron* network::Neuron(int id)
{
    if (_neurons->at(id)->getId() == id)
    return (_neurons->at(id));
    else // Find
    {
        for (auto i : *_neurons)
        {
            if (i->getId() == id)
            {
                return i;
            }
        }
    }
}

/*
0 indexed

The second neuron is set as the output of the first neuron
The first neuron is set as the input of the second neuron

*/
void network::connect(int n1, int n2, double weight)
{
    _neurons->at(n1)->setOutputneuron((_neurons->at(n2)));
    _neurons->at(n2)->setInputneuron((_neurons->at(n1)), weight);
    if (__debug__) { debugN("NEURON CONNECTED"); }
}


void network::setNetworkDebug(bool f)
{
    __debug__ = f;
    for (auto i : *_neurons)
    {
        i->setDebug(__debug__);
    }
}

void network::setNeuronDebug(bool f)
{
    for (auto i : *_neurons)
    {
        i->setDebug(f);
    }
}



void network::run(int ms)
{
    std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::now();

    bool period = false;
    int diffI = 0, diffPrev = 0;
    while (_run)
    {
        auto diff = std::chrono::system_clock::now() - timePoint;
        diffI = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        if (__debug__){ debugN("RUN"); debug(diffI); debug(diffI % ms); }

        if (diffI % ms == 0 && diffI != diffPrev) // Produces 20 milliseconds ticks..
        {
            systemTick();
            period = true;
            timePoint = std::chrono::system_clock::now();
        }

        diffPrev = diffI;
    }

}
