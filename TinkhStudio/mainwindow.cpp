#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pieceslist.h"
#include "puzzlewidget.h"
#include <QtWidgets>
#include <QSplitter>
#include "playpenscene.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupWidgets();
    setupPieces();
    isAccumulatingCodeBlock = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupWidgets()
{
    QFrame *frame = new QFrame;

    puzzleWidget = new PuzzleWidget(400); // 900 for nexus7, 400 for desktop
    piecesList = new PiecesList(this);

//    connect(puzzleWidget, SIGNAL(puzzleCompleted()), this, SLOT(setCompleted()), Qt::QueuedConnection);

    clearButton = new QPushButton("Clear");
    runButton = new QPushButton("Run");

    connect(clearButton, SIGNAL(clicked()), puzzleWidget, SLOT(clearProgram()));
    connect(runButton, SIGNAL(clicked()), puzzleWidget, SLOT(runProgram()));


    QSplitter *splitter = new QSplitter();
    splitter->addWidget(piecesList);
    splitter->addWidget(puzzleWidget);

    graphicsView = new QGraphicsView(this);
    QSize gvSize(600,400); // for desktop
    //QSize gvSize(1200,900); // for nexus7
    graphicsView->setMinimumSize(gvSize);
    graphicsView->setMaximumSize(gvSize);

    QVBoxLayout *vLayout = new QVBoxLayout(frame);    
    vLayout->addWidget(splitter);
    vLayout->addWidget(clearButton);
    vLayout->addWidget(runButton);
    vLayout->addWidget(graphicsView);

    scene = new PlayPenScene(this);
    graphicsView->setScene(scene);
    graphicsView->show();

    connect(puzzleWidget, SIGNAL(queueCommand(QString,QString)), this, SLOT(runCommand(QString,QString)));

    setCentralWidget(frame);
}

void MainWindow::setupPieces()
{
    piecesList->clear();

    piecesList->addPiece("Roll");
    piecesList->addPiece("Back");
    piecesList->addPiece("Left");
    piecesList->addPiece("Right");
    piecesList->addPiece("HeadUp");
    piecesList->addPiece("HeadDown");
    piecesList->addPiece("Color");
    piecesList->addPiece("Music");
    piecesList->addPiece("Dance");
    piecesList->addPiece("If WhatColorUnder?");
    piecesList->addPiece("If IsFrontClear?");
    piecesList->addPiece("If WhichSideBrighter?");
    piecesList->addPiece("If WhichSideOfLine?");
    //piecesList->addPiece("Set Var=Value");
    //piecesList->addPiece("If/Then/Else");
    piecesList->addPiece("Repeat Until");
    piecesList->addPiece("Repeat N times");

    for (int i = 0; i < piecesList->count(); ++i) {
        if (int(2.0*qrand()/(RAND_MAX+1.0)) == 1) {
            QListWidgetItem *item = piecesList->takeItem(i);
            piecesList->insertItem(i, item);
        }
    }

    piecesList->show();

    puzzleWidget->clearProgram();
}

/*void MainWindow::animate()
{
    for (int i=0; i<10; i++)
    {
        scene->moveRobot();
        graphicsView->viewport()->update();
        repaint();
        QThread::msleep(500);
   }
}*/

void MainWindow::runCommand(QString commandName, QString value)
{
    if (commandName.startsWith("If"))
    {
        isAccumulatingCodeBlock = true;
        return;
    }
    if (isAccumulatingCodeBlock)
    {
        if (commandName.startsWith("End If"))
        {
            for (int i=0; i<commandStack.count(); i++)
            {
                runCommand(commandStack[i].first,commandStack[i].second);
            }
            isAccumulatingCodeBlock = false;
        }
        else
        {
            QPair<QString,QString> command;
            command.first = commandName;
            command.second = value;
            commandStack.append(command);
        }
        return;
    }
    if (commandName == "Roll")
    {
        int valueInInches = value.toInt();
        int valueInPixels = valueInInches*10;
        scene->moveRobot(valueInPixels,0);
    }
    else if (commandName == "Back")
    {
        int valueInInches = value.toInt();
        int valueInPixels = -valueInInches*10;
        scene->moveRobot(valueInPixels,0);
    }
    else if (commandName == "Left")
    {
        scene->turnRobot(-value.toInt());
    }
    else if (commandName == "Right")
    {
        scene->turnRobot(value.toInt());
    }    

    graphicsView->viewport()->update();
    graphicsView->viewport()->repaint();
    graphicsView->repaint();
    repaint();
    QThread::msleep(500);
}
