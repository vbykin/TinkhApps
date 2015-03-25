/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "PlayPenScene.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsColorizeEffect>
#include <QTransform>
#include <QThread>

PlayPenScene::PlayPenScene(QObject *parent)
    : QGraphicsScene(parent)
{
    addItem(new QGraphicsRectItem(QRect(0,0,550,350)));
    // Draw vertical lines for grid
    for (int i=0; i<550; i+=50)
    {
        addItem(new QGraphicsLineItem(QLine(i,0,i,350)));
    }
    // Draw horizontal lines for grid
    for (int i=0; i<350; i+=50)
    {
        addItem(new QGraphicsLineItem(QLine(0,i,550,i)));
    }
    QPixmap pixmap;
    //pixmap.load("D://MyQt//TinkhStudio//LadyBugImage.png");
    pixmap.load("://LadyBugImage.png");
    roling = new QGraphicsPixmapItem(pixmap);
    addItem(roling);
}

PlayPenScene::~PlayPenScene()
{
    delete roling;
    delete tform;
}


void PlayPenScene::moveRobot(int xPixels, int yPixels)
{
    QPointF pos = roling->mapToScene(roling->boundingRect().center());
    QTransform oldTransform = roling->transform();
    roling->setTransform(QTransform().translate(xPixels,yPixels),true);
    QPointF newPos = roling->mapToScene(roling->boundingRect().center());
    // If roling went out of bounds
    if (newPos.x() < -0.5 || newPos.x() > 500.5 ||
        newPos.y() < -0.5 || newPos.y() > 300.5)
    {
        // reset to old transform to bring back to within boundary
        roling->setTransform(oldTransform);

        // Tint it red to indicate there was a problem
        QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
        effect->setColor(QColor(255,0,0));
        roling->setGraphicsEffect(effect);
    }
    else
    {
        // If everything went smoothly, take out the red tint in case it was added earlier
        roling->setGraphicsEffect(0);
    }
}

void PlayPenScene::turnRobot(int angle)
{
    //Rotate about the origin
    QPointF pos = roling->scenePos();

    qreal w = roling->boundingRect().width();
    qreal h = roling->boundingRect().height();
    qreal tx = pos.x() + w/2;
    qreal ty = pos.y() + h/2;

    roling->setTransform(QTransform().translate(w/2,h/2).rotate(angle).translate(-w/2,-h/2),true);

    /*if (angle > 0)
    {

    }
    else if (angle < 0)
    {
        roling->setTransform(QTransform().translate(0,h).rotate(angle),true);
    }*/
}
