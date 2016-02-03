#include "neuron.h"

// Static Variable

#define THRES 1

int neuron::_number;

neuron::neuron()
{
    _forced = false;


    _nInputs = 0;
    _nOutputs = 0;

    _nOutputsToSend = _nOutputs;

    _potential = 0;
    _threshold = THRES;  // .........

    _inputBuffer = FLAT;
    _outputBuffer = FLAT;

    _inputs = new std::vector < std::tuple<neuron*, float> >(0) ;
    _outputs = new std::vector < neuron* >(0);

    _id = _number;
    _number++;

    // FOR LEARNING
    _activityPos = 0 ;
 //   _activity = new std::bitset<200>;

    // Bookkeeping
    __debug__ = false;
    if (__debug__) { debugN("CREATE");   debug("ID "); debug(_id); };


    // Exclusively for network usage
    _ticked = false;


    // FOR FLIP NEURON
    _flip = false;
}


neuron::~neuron()
{
    //delete _inputs; // Not required.. I do not want to delete neurons I do not have ownership of ..
    //delete _outputs;
    if (__debug__) { debugN("DELETE ");  debug("ID ");  debug(_id); };
}


neuron::neuron(const neuron &a) :_potential(a._potential),_nInputs(a._nInputs), _nOutputs(a._nOutputs), _inputBuffer(a._inputBuffer), _outputBuffer(a._outputBuffer), _threshold(a._threshold), _forced(a._forced)
{
    _inputs = a._inputs;
    _outputs = a._outputs;
    _nOutputsToSend = _nOutputs;
    if (__debug__){ debugN("copy cons "); debug(_id); };
}

neuron& neuron::operator=(const neuron &a)
{
    if (&a == this)
        return *this;
    _potential = a._potential;
    _nInputs = a._nInputs;
    _nOutputs = a._nOutputs;
    _nOutputsToSend = _nOutputs;
    _inputBuffer = a._inputBuffer;
    _outputBuffer = a._outputBuffer;
    _threshold = a._threshold;
    _forced = a._forced;
    _inputs = a._inputs;
    _outputs = a._outputs;
    if (__debug__) debugN("copy assign ");
    return *this;
}



//############################################################## IMPORTANT ########################################################################//

void neuron::setInputneuron(neuron* _neuron, double _weight)
{
    _nInputs++;
    _inputs->push_back(std::make_tuple(_neuron, _weight)); // Only store pointers to neuron, do not store the actual neuron.
    if (__debug__) { debugN("ADDING INPUT ");  debug(_nInputs); };
}


void neuron::setOutputneuron(neuron* _neuron)
{
    _nOutputs++;
    _nOutputsToSend = _nOutputs;
    _outputs->push_back(_neuron);
    if (__debug__) { debugN("ADDING OUTPUT "); debug(_nOutputs); };
}

/*
If the inputs are ready then we update the neuron and retun true ,
else we return false.

The handler should call this function again , until it returns a true..

*/

bool neuron::tick()
{
    if (_flip)
    {
        if (_nOutputs > 2) { debugN("ERROR : "); debug("FLIP Nueron can only have two inputs "); }
        if (_flipNueron->getOutput())
        {
            if (_outputs->at(0)->getInputWeight(this->getId()))
            {
                _outputs->at(0)->changeInputWeight(this->getId(), 0);
                _outputs->at(1)->changeInputWeight(this->getId(), 1);
            }
            else
            {
                _outputs->at(0)->changeInputWeight(this->getId(), 1);
                _outputs->at(1)->changeInputWeight(this->getId(), 0);
            }
            // we do take an input from the flipNeuron to determine the potential , but rather it is only used to flip the outputs.
            // The remaining part of the functions handles the input of this neurons from other neurons other than the flip neuron
            // which are used in determining the potential..
            // flip is not added as an input neuron.. it is kept seperate..

            // Now that we have used obtianed the _flipNeuron's output. We will reset it ..
            _flipNueron->_readyForTick = false;
            _flipNueron->_outputBuffer = FLAT;

        }
    }


    if (_forced) // No checking.. Used to start network..
    {
        _readyForTick = true;
        _outputBuffer = SPIKE;
        if (__debug__) {
            debugN("FORCE ");  debug("ID ");  debug(_id);  debug("OUT "); debug(_outputBuffer); //debug(_potential);
        };
        _ticked = true;// FOR NETWORK USAGE ONLY
        return true;
    }


     //_potential = 0 ; // We want to remeber the potential..
     _nOutputsToSend = _nOutputs;
    // If inputs all updated..
     if (__debug__)
     {
        // if no inputs warn..
         if (_inputs->size() == 0)
         {
             debugN("TICKING ");  debug("ID "); debug(_id); debug("INPUTS "); debug(0);
         }
     }
    for (auto i : *_inputs) // __1__
    {
        if (!std::get<0>(i)->readyForTick()) // unwrapping the tuple to extract nueron . Ensure that all neuron are ready for update if not we return false;
        {
            if (__debug__) {
                debugN("TICKING  FAILED ");  debug("ID "); debug(_id);  debug("OUT "); debug(_outputBuffer); //debug(_potential);
            };
            _ticked = false; // FOR NETWORK USAGE ONLY
            return false; //
        }
        // For efficeny. If we do not go out of this, we can use the same iteration to calculate the _spikeValue;

        //if (__debug__)  { debugN("for loop __1__"); debug(std::get<1>(i)); }
        _potential += std::get<0>(i)->getOutput() * std::get<1>(i);

        // To simulate the passage of time..
        // After this neuron has got the stimulus , the previous neuron goes cold.
        std::get<0>(i)->reset();
    }
    _readyForTick = true; // Input all ready..

    // intially Simple.
    if (_potential >= _threshold)
    {
         _outputBuffer = SPIKE;
         _potential = 0 ;
    }
    else
    {
       _outputBuffer = FLAT;
       _potential = 0.8* _potential; // We do not do anything , we remeber this.
       // We add decay ..
    }


    if (__debug__) {
        debugN("TICKING "); debug("ID "); debug(_id);   debug("OUT ");  debug(_outputBuffer); //debug(_potential);
    };
    // FOR LEARNING ...
    insertToActivity(_outputBuffer);
    if(__debug__) { debugN(_activity);};


    _ticked = true;// FOR NETWORK USAGE ONLY
    return true;
}



/*
Used to ensure that the activity of the previous neuron ends.

After the outputs have recieved the input , then previous neuron goes into an inactive state..
*/
void neuron::reset()
{
    if (__debug__) { debugN("RESET"); debug("ID "); debug(_id); debug("BEF ");  debug(_outputBuffer); debug("OUTS ");  debug(_nOutputsToSend); };

    if (_forced) // Once the force neruon has started the network , we no longer assume that the start neuron spikes..
    {
        _readyForTick = true;
        _outputBuffer = FLAT;
    }
    else
    {
        _nOutputsToSend--;
        if (_nOutputsToSend <= 0)
        {
            // All the outputs have received the input
            // Now the current neuron goes cold.
            // Fires only if new input during the tick...

            _readyForTick = false;
            _outputBuffer = FLAT;
        }
    }
    if (__debug__) { debug("AFT "); debug(_outputBuffer); };
};



void neuron::forceNeuron(bool _f){
    if (_f)_forced = _f;
    else
    {
        _forced = _f;
        _readyForTick = true;
    }
};


bool neuron::changeInputWeight(int inputID, float weight)
{
    for (int i = 0; i < _nInputs; ++i)
    {
        if (std::get<0>( _inputs->at(i) )->getId() == inputID)
        {
            std::get<1>(_inputs->at(i) ) = weight;
            if (__debug__)
            {
                debugN("weight : "); debug(std::get<1>(_inputs->at(i)));
            }
            return true;
        }
    }
    return false;
}



double neuron::getInputWeight(int inputID)
{
    double num;
    for (auto i : *_inputs)
    {
        if (std::get<0>(i)->getId() == inputID)
        {
            return std::get<1>(i);
        }

    }
}


void neuron::connect(neuron* n1,  double _weight)
{
    setOutputneuron(n1);
    n1->setInputneuron(this, _weight);
}


// FOR FLIP NEURON
void neuron::flipNeuron(neuron* input)
{
    if (_nOutputs > 2) { debugN("ERROR : "); debug("FLIP Nueron can only have two inputs "); return; }
    _flip = true;
    _flipNueron = input;
}

void neuron::stimulus()
{
    _outputBuffer = SPIKE; // This will be made low by the tick progress..
}

// FOR LEARNING

void neuron::insertToActivity(bool n)
{
    if(_activityPos < _activity.size()-1)
    {
        _activity[_activityPos] = n;
        _activityPos++;
    }
    else
    {
        _activity <<= 1;
        _activity[_activityPos] = n;
    }
}







