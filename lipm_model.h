#ifndef LIPM_MODEL_H
#define LIPM_MODEL_H

#include "network.h"
#include "cnb.h"

#include <QGraphicsItem>

class LIPM_model:  public QGraphicsItem
{
public:
    LIPM_model();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;

    void setAnkleFoot(bool f){_includeAnkleFoot = f; };
protected:
    void advance(int step) Q_DECL_OVERRIDE;

private:

    // Ankle is not modelled..
    QLineF *_leftFemur;
    QLineF *_leftTibia;

    QLineF *_rightFemur;
    QLineF *_rightTibia;

    QRectF *_rightKnee;
    QRectF *_leftKnee;

    QRectF * _rightHip;
    QRectF * _leftHip;

    // Joins the femur..
    QRectF * _hip;


    // Modelling the ankle ....


    QRectF * _rightAnkle;
    QRectF * _leftAnkle;


    QLineF * _rightFoot;
    QLineF * _leftFoot;



    int _rotL;
    int _kneeRotL; // This handles the secondary bending that is required...
    int _rotR;
    int _kneeRotR;

    int _endRot;
    int _begRot;

    int _endKneeRot;
    int _begKneeRot;
    int _midKneeRot;

    bool _reachEnd;
    bool _reachBeg;

    bool _includeAnkleFoot;

    bool _movebackHIP_L;// for version 2
    bool _movebackHIP_R;// for version 2


    // NUERON CONTROL...
    network *cortex;


    // LEARNING
    cnb* learner;

    int counter ;
};

#endif // LIPM_MODEL_H
