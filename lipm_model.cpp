#include "lipm_model.h"
#include <QPainter>

#include <iostream>

using namespace std;

// Global
#define x 0
#define y 0
#define hipCenterX x + 250
#define hipCenterY y - 150
#define legLen 500
#define legWidth 200
#define thick 20

#define hipLen 200
#define femurLen 200
#define tibiaLen 150
// r = right , l = left , R = radius
#define rHipR 50
#define lHipR 50

#define rKneeR 40
#define lKneeR 40

#define rAnkleR 25
#define lAnkleR 25

#define footRLen 30
#define footLLen 30

LIPM_model::LIPM_model()
{
    // CONTROL
    _includeAnkleFoot = false;


    // This is the front models ' begining.. Nothing much can be done with it. Not enough info..
    // Swtiching to sideways view...
    /*
    _hip = new QRectF((hipLen)/2 , (thick)/2 , hipLen , thick);
    _rightHip = new QRectF(_hip->left() - rHipR , _hip->top() - thick/2 , rHipR , rHipR);
    _leftHip = new QRectF(_hip->right() , _hip->top() - thick/2 , lHipR , lHipR);
    _rightFemur = new QRectF( _hip->right() + rHipR/2 - thick/2 , _rightHip->top() + rHipR ,thick,femurLen);
    */

    // Model the right side looking on.. 90 degree looking..
    //GRAPHICS
    _rightHip = new QRectF(hipCenterX, hipCenterY,rHipR, rHipR);

    _rightFemur = new QLineF(hipCenterX + rHipR/2 ,hipCenterY + rHipR ,hipCenterX + rHipR/2 ,hipCenterY + femurLen);
    _leftFemur = new QLineF(hipCenterX + rHipR/2 ,hipCenterY + rHipR ,hipCenterX + rHipR/2 ,hipCenterY + femurLen);

    _rightKnee = new QRectF(_rightFemur->x2(), _rightFemur ->y2() , rKneeR , rKneeR );
    _leftKnee = new QRectF(_leftFemur->x2(), _leftFemur ->y2() , lKneeR , lKneeR );

    _rightTibia = new QLineF( _rightKnee->right() - rKneeR ,_rightKnee->bottom()- rKneeR /2,_rightKnee->right() - rKneeR,_rightKnee->bottom() +  tibiaLen);
    _leftTibia = new QLineF( _leftKnee->right() - lKneeR ,_leftKnee->bottom()- lKneeR /2,_leftKnee->right() - lKneeR, _leftKnee->bottom() + tibiaLen);







    if(_includeAnkleFoot)
    {
        // Modelling Foot and the Ankle....

        _rightAnkle = new QRectF(_rightTibia->x2(),_rightTibia->y2(),rAnkleR, rAnkleR );
        _leftAnkle = new QRectF(_leftTibia->x2(),_leftTibia->y2(),lAnkleR, lAnkleR );

        _rightFoot = new QLineF( _rightAnkle -> right() - rAnkleR , _rightAnkle -> bottom() - rAnkleR/2 ,_rightAnkle -> right() - rAnkleR + footRLen, _rightAnkle -> bottom() - rAnkleR/2 );
        _leftFoot = new QLineF( _leftAnkle -> right() - lAnkleR , _rightAnkle -> bottom() - lAnkleR/2 ,_rightAnkle -> right() - lAnkleR + footRLen, _rightAnkle -> bottom()- lAnkleR/2  );
    }







// ANGLES
    _rotL = -30 ;
    _kneeRotL = 0 ;

    _rotR = 30 ;
    _kneeRotR = 0 ;


    _endRot = 30;
    _begRot = -30;

    //_reachEnd = false;_reachBeg = false;

    _endKneeRot = -30;


    //#############################__NUERAL_CONTROL__################################################//
    cortex = new network(23); // Uses 12 neurons...
    cortex->setNeuronDebug(1);

    // Network Starter.
    cortex->Neuron(0)->forceNeuron(true);
    cortex->Neuron(1)->setInputneuron(cortex->Neuron(0),1);
    // Oscillator
    cortex->connect(1, 2);
    cortex->connect(2, 3);
    cortex->connect(3, 2);


    // Connection to left hip
    cortex->Neuron(7)->sensoryNeuron();
    cortex->Neuron(4)->flipNeuron(cortex->Neuron(7));
    cortex->connect(3,4);
    cortex->connect(5,16);
    //cortex->connect(6,17); // 6 is not used as an input to knee , so it will not be reset like 5 is. So no need for another nueron.
    cortex->connect(4,5,1);
    cortex->connect(4,6,0);

    // Connection to right hip
    cortex->Neuron(11)->sensoryNeuron();
    cortex->Neuron(8)->flipNeuron(cortex->Neuron(11));
    cortex->connect(3,8);
    cortex->connect(9,18);
     //cortex->connect(8,23);
    cortex->connect(8,9,0);
    cortex->connect(8,10,1);


    // Connection to left knee from left hip ..
    cortex->Neuron(14)->sensoryNeuron();
    cortex->Neuron(12)->flipNeuron(cortex->Neuron(14));
    cortex->connect(5,12); // Input to flip Neuron
    cortex->connect(12,13,1); // The - side
    cortex->connect(12,15,0); // The + side


    // Connection to right knee from right hip
    cortex->Neuron(21)->sensoryNeuron();
    cortex->Neuron(19)->flipNeuron(cortex->Neuron(21));
    cortex->connect(9,19);
    cortex->connect(19,20,1); // The - side
    cortex->connect(19,22,0); // The + side

;
  //  cortex->setNeuronDebug(1);


        //#############################__NUERAL_CONTROL__################################################//
}

void LIPM_model::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    /* Front Model.. Low Information content...
     * // draw hips
    painter->setBrush(Qt::red);
    painter->drawRect(*_hip);

    painter->setBrush(Qt::blue);
    painter->drawEllipse(*_rightHip);

    painter->setBrush(Qt::blue);
    painter->drawEllipse(*_leftHip);

    painter->setBrush(Qt::red);
    painter->drawRect(*_rightFemur);
    */

    // RIght Hip
    painter->setBrush(Qt::blue);
    painter->drawEllipse(*_rightHip);

    // Femur
    QPen femurPen = QPen(Qt::red);
    femurPen.setWidth(thick);
    femurPen.setCapStyle(Qt::RoundCap);
    painter->setPen(femurPen);
    painter->drawLine(*_rightFemur);
    painter->drawLine(*_leftFemur);

    // Reset
    // Knee Joints
    painter->restore();
    painter->setBrush(Qt::blue);
    painter->drawEllipse(*_rightKnee);
    painter->drawEllipse(*_leftKnee);

    //Tibia...
    QPen tibiaPen = QPen(Qt::red);
    tibiaPen.setWidth(thick);
    tibiaPen.setCapStyle(Qt::RoundCap);
    painter->setPen(tibiaPen);
    painter->drawLine(*_rightTibia);
    painter->drawLine(*_leftTibia);

    if(_includeAnkleFoot)
    {
        // Ankle Joints..
        painter->restore();
        QPen anklePen = QPen(Qt::black);
        anklePen.setWidth(.1);
        painter->setPen(anklePen);
        painter->setBrush(Qt::black);
        painter->drawEllipse(*_rightAnkle);
        painter->drawEllipse(*_leftAnkle);

        painter->restore();
        QPen footPen = QPen(Qt::red);
        footPen.setWidth(thick);
        footPen.setCapStyle(Qt::RoundCap);
        painter->setPen(footPen);
        painter->drawLine(*_rightFoot);
        painter->drawLine(*_leftFoot);


    }





}

QRectF LIPM_model::boundingRect() const
{
    return QRectF(x,y - 200,500,500 );
}

void LIPM_model::advance(int step)
{

    //#############################__NUERAL_CONTROL__################################################//
    cortex->systemTick();

    if(cortex->Neuron(16)->getOutput())
    {
        _rotL++;
       // _kneeRotL++;
   //     std::cout << "+" << std::endl;
    }
    else if(cortex->Neuron(6)->getOutput())
    {
        _rotL--;
        //_kneeRotL--;
       // _kneeRotL--;
     //   std::cout << "-" << std::endl;
    }

    if(_rotL >= _endRot)
    {
        cortex-> Neuron(7)->stimulus();
    }
    else if(_rotL <= _begRot)
    {
       cortex-> Neuron(7)->stimulus();
    }

    // OUTPUT
    if(cortex->Neuron(18)->getOutput())
    {
        _rotR++;
       // _kneeRotR++;
    }
    else if(cortex->Neuron(10)->getOutput())
    {
        _rotR--;
        //_kneeRotR--;
    }
    // FEEDBACK
    if(_rotR >= _endRot)
    {
        //std::cout << "#######" << endl;
        cortex-> Neuron(11)->stimulus();
    }
    else if(_rotR <= _begRot)
    {
       cortex-> Neuron(11)->stimulus();
    }

    // OUTPUT KNEE LEFT
    if(cortex->Neuron(13)->getOutput())
    {
     //   std::cout << "knee Rot -" << std::endl;
        _kneeRotL--;
    }
    else if(cortex->Neuron(15)->getOutput())
    {
       // std::cout << "knee Rot +" << std::endl;
        _kneeRotL++;
    }

        //std::cout << _kneeRotL << std::endl;
    // FEEBACK FROM KNEE LEFT
    if(_kneeRotL <= _endKneeRot)
    {
        cortex->Neuron(14)->stimulus();
        //std::cout << "*" << std::endl;
    }
    else if(_kneeRotL >= 0)
    {
        cortex->Neuron(14)->stimulus();
        //std::cout << "*" << std::endl;
    }


    // OUTPUT KNEE RIGHT
    if(cortex->Neuron(20)->getOutput())
    {
        //std::cout << "knee Rot -" << std::endl;
        _kneeRotR--;
    }
    else if(cortex->Neuron(22)->getOutput())
    {
        //std::cout << "knee Rot +" << std::endl;
        _kneeRotR++;
    }

     //   std::cout << _kneeRotR << std::endl;
    // FEEBACK FROM KNEE RIGHT
    if(_kneeRotR <= _endKneeRot)
    {
        cortex->Neuron(21)->stimulus();
       // std::cout << "*" << std::endl;
    }
    else if(_kneeRotR >= 0)
    {
        cortex->Neuron(21)->stimulus();
        //std::cout << "*" << std::endl;
    }




        //#############################__NUERAL_CONTROL__################################################//
/*
    if(!_reachEnd)
    {
        _rot++;
        _kneeRot++;
        // Alternating between start and end....
        if(_rot >= _endRot)
        {
            _reachEnd = true ; _reachBeg = false;
        }
        else
        {
            _reachEnd = false;
        }
    }
    else if (!_reachBeg)
    {
        _rot--;
        _kneeRot--;
        if(_rot <= _begRot)
        {
            _reachBeg = true ; _reachEnd = false;
        }
        else
        {
            _reachBeg = false;
        }
    }
//_rightFemur->setAngle(270 );
//_leftFemur->setAngle(270);
    _rightFemur->setAngle(270 + _rot);
    _leftFemur->setAngle(270  - _rot);
    _rightKnee->moveTo(_rightFemur->p2() - QPoint(rKneeR/2, rKneeR/2));
    _leftKnee->moveTo(_leftFemur->p2() - QPoint(lKneeR/2, lKneeR/2));
    _rightTibia->setP1(_rightKnee->center() + QPoint(0,rKneeR/2));
    _leftTibia->setP1(_leftKnee->center() + QPoint(0,lKneeR/2));


    // Bending of the knees occur only during the forward motion...
    _rightTibia->setAngle(270  + _kneeRot);
    _leftTibia->setAngle(270  - _kneeRot);

    // The knees bend , this might be leading to an increase in speed of the leg
    // with the bend knee. Well the Com argument. But it might not be contributing much...

    // How to do single knee ???

    /*
     * If at the end , then from there I start bending the tibia back
     * Somewhere between the end and the start, the tibia straightens out..
     * It seems to follow a curve. Not a Gaussian , a gaussian with the bump
     * shifted to the left...
     *
     * Like a capacitor which charges and discharges exponentially.
     * The motion is smooth though..
     *
     * I am assuming that at the mid point.. between the end and start ,
     * the bending is maximum..
     *
     *
     * How many degrees does it bend ?? Assume 15 degrees for now...
     */


    // Secondary Being at Knee for Right leg..

    // Start simple..Do not try to capture everything. My brain will only hurt...



    // How how do we model the bending of the knees...


    // GRAPHICS....
    _rightFemur->setAngle(270 + _rotR);
    _leftFemur->setAngle(270  + _rotL);
    _rightKnee->moveTo(_rightFemur->p2() - QPoint(rKneeR/2, rKneeR/2));
    _leftKnee->moveTo(_leftFemur->p2() - QPoint(lKneeR/2, lKneeR/2));
    _rightTibia->setP1(_rightKnee->center() );
    _rightTibia->setP2(_rightKnee->center() + QPoint(0,tibiaLen));
    _leftTibia->setP1(_leftKnee->center() );
    _leftTibia->setP2(_leftKnee->center() + QPoint(0,tibiaLen));

    _rightTibia->setAngle(270  + _kneeRotR+ _rotR);
    _leftTibia->setAngle(270  + _kneeRotL + _rotL);


    if(_includeAnkleFoot)
    {
        // Ankles..
        _rightAnkle->moveTo(_rightTibia->p2() - QPoint(rAnkleR/2,rAnkleR/2));
        _leftAnkle->moveTo(_leftTibia->p2() - QPoint(lAnkleR/2,lAnkleR/2));
        _rightFoot->setP1(_rightAnkle->center());
        _rightFoot->setP2(_rightAnkle->center() + QPoint(footRLen,0));


        _leftFoot->setP1(_leftAnkle->center());
        _leftFoot->setP2(_leftAnkle->center() + QPoint(footLLen,0));

    }


    setRotation(0); // To force update.. Nothing else..

}


