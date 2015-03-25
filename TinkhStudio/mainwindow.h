#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsView>
#include <QPair>
#include "playpenscene.h"

namespace Ui {
class MainWindow;
}

class PiecesList;
class PuzzleWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    //void animate();
    void runCommand(QString commandName, QString value);

private:
    Ui::MainWindow *ui;
    void setupWidgets();
    void setupPieces();
    PiecesList *piecesList;
    PuzzleWidget *puzzleWidget;
    QPushButton *clearButton;
    QPushButton *runButton;
    PlayPenScene *scene;
    QGraphicsView *graphicsView;
    QList<QPair<QString,QString> > commandStack;
    bool isAccumulatingCodeBlock;
};

#endif // MAINWINDOW_H
