#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     _scene = new QGraphicsScene(this);

     // Joins the graphicsView and scene..
     ui->graphicsView->setScene(_scene);

     // Dont know...
     ui->graphicsView->setRenderHint(QPainter::Antialiasing);

     //_scene -> setSceneRect(-200,-200,300,300);
    LIPM_model *model = new LIPM_model;
    _scene ->addItem(model);

    QTimer *timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), _scene, SLOT(advance()));
    timer -> start(1000/33);
}

MainWindow::~MainWindow()
{
    delete ui;
}
