#ifndef NETWORK_H
#define NETWORK_H

#include "neuron.h"

#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include <chrono>
typedef std::chrono::steady_clock::duration duration;

/*
This will handle the structure of the neuron connections.. How ?

Keep a vector of neurons with ids.

user will add the neuron .

Then say connect(1,2);

*/


class network
{

public:
    network(int nNeurons = 0 );
    ~network();

    void systemTick();

    void addNeuron();
    neuron* Neuron(int pos);
    int getNumNeurons(){ return _nNeurons; };
    void connect(int n1, int n2, double weight = 1);
    void setNeuronDebug(bool f);
    void setNetworkDebug(bool f);
    void runSetup(bool t) { _run = t; };

    void run(int ms);

    //LEARNING
        std::vector<neuron*>* getNeurons(){ return _neurons;};

private:
    bool _run;
    int _nNeurons;
    int _startNeuron; // This neuron should only be ticked once..
    bool _networkTicked;
    std::vector<neuron*> * _neurons;

    bool __debug__;

    template <typename T>
    static void debugN(const T* p){ std::cout << std::endl << "NETWORK-> " << p; }
    template <typename T>
    static void debugN(const T p){ std::cout << std::endl << "NETWORK-> " << p; }
    template <typename T>
    static void debug(const T* p){ std::cout << " " << p; }
    template <typename T>
    static void debug(const T p){ std::cout << " " << p; }
};


#endif // NETWORK_H
