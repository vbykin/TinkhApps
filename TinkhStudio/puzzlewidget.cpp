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

#include "puzzlewidget.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>

PuzzleWidget::PuzzleWidget(int imageSize, QWidget *parent)
    : QWidget(parent), m_ImageSize(imageSize)
{
    setAcceptDrops(true);
    setMinimumSize(m_ImageSize, m_ImageSize);
//    setMaximumSize(m_ImageSize, m_ImageSize);
    m_NextId = 1000;
}

int PuzzleWidget::generateNewId()
{
    m_NextId++;
    return m_NextId;
}

void PuzzleWidget::clearProgram()
{
    pieceBrickData.clear();
    for (int i=0; i<comboBoxList.count(); i++)
    {
        delete comboBoxList[i];
    }
    comboBoxList.clear();

    highlightedRect = QRect();
    update();
}

void PuzzleWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-brickdata"))
        event->accept();
    else
        event->ignore();
}

void PuzzleWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void PuzzleWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QRect updateRect = highlightedRect.united(targetSquare(event->pos()));

    if (event->mimeData()->hasFormat("application/x-brickdata"))
    {
        highlightedRect = targetSquare(event->pos());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        highlightedRect = QRect();
        event->ignore();
    }

    update(

                );
}

void PuzzleWidget::updateBrickWithValue(int selectedIndex)
{
    QComboBox *cbox = (QComboBox*)QObject::sender();
    for (int i=0; i<comboBoxList.count(); i++)
    {
        if (comboBoxList[i] == cbox)
        {
            pieceBrickData[i].setSelectedValueIndex(selectedIndex);
        }
    }
}

void PuzzleWidget::runProgram()
{
    QString program("");

    // Run the commands in order
    for (int i=0; i<pieceBrickData.count(); i++)
    {
        BrickData bd = pieceBrickData[i];
        //program += bd.getLabel() + " " + bd.getValueItems()[bd.getSelectedValueIndex()] + " " + bd.getUnitsLabel() + "\n";
        QString value = "";
        if (bd.getSelectedValueIndex() != -1)
        {
            value = bd.getValueItems()[bd.getSelectedValueIndex()];
        }
        emit queueCommand(bd.getLabel(), value);
    }
    //int len = program.length();
}

void PuzzleWidget::createComboBoxForBrick(int index, const BrickData& brickData)
{
    //Create and add combo box
    QComboBox *cbox = new QComboBox(this);
    for (int i=0; i<brickData.getValueItems().count(); i++)
    {
        cbox->insertItem(i,brickData.getValueItems().at(i));
        cbox->setCurrentIndex(brickData.getSelectedValueIndex());
    }
    bool result = QObject::connect(cbox, SIGNAL(currentIndexChanged(int)),this, SLOT(updateBrickWithValue(int)));
    comboBoxList.insert(index, cbox);
}

void PuzzleWidget::insertBrick(int index, const BrickData& brickData)
{    
    pieceBrickData.insert(index,brickData);

    //Create and add combo box
    createComboBoxForBrick(index, brickData);

    if (brickData.getLabel().startsWith("If"))
    {
        int endIfIndex;
        // If unpaired "If" brick, then add a corresponding "End If" brick.
        if (brickData.getPairBrickId() == -1)
        {
            endIfIndex = index+1;
            BrickData endIfBrick(generateNewId(),"End " + brickData.getLabel());
            endIfBrick.setPairBrickId(pieceBrickData[index].getId());
            pieceBrickData.insert(endIfIndex,endIfBrick);
            createComboBoxForBrick(endIfIndex,endIfBrick);
            pieceBrickData[index].setPairBrickId(pieceBrickData[endIfIndex].getId());
        }
        // If paired "If" brick is moved, then adjust the pair id.
        else
        {
            int endIfBrickIndex = findBrickIndexById(brickData.getPairBrickId());
            pieceBrickData[endIfBrickIndex].setPairBrickId(pieceBrickData[index].getId());
        }
    }

    // If "End If" brick moved then adjust the pair id.
    if (brickData.getLabel().startsWith("End If"))
    {
        int ifBrickIndex = findBrickIndexById(brickData.getPairBrickId());
        pieceBrickData[ifBrickIndex].setPairBrickId(pieceBrickData[index].getId());
    }
}

int PuzzleWidget::findIndex(BrickData* brickData) const
{
    for (int i=0; i<pieceBrickData.count(); i++)
    {
        if (brickData == &(pieceBrickData[i]))
        {
            return i;
        }
    }
    return -1;
}

int PuzzleWidget::findBrickIndexById(int id) const
{
    for (int i=0; i<pieceBrickData.count(); i++)
    {
        if (pieceBrickData[i].getId() == id)
        {
            return i;
        }
    }
    return -1;
}

bool PuzzleWidget::isLogicalPosition(int index, const BrickData & brickData)
{
    if (brickData.getLabel().startsWith("End If")
        && (index <= findBrickIndexById(brickData.getPairBrickId())) )
    {
        return false;
    }

    if (brickData.getLabel().startsWith("If")
        && (brickData.getPairBrickId() != -1)
        && (index > findBrickIndexById(brickData.getPairBrickId())) )
    {
        return false;
    }

    if (brickData.getLabel().startsWith("If"))
    {
        int ifIndex = index;
        int endIfIndex = findBrickIndexById(brickData.getPairBrickId());
        if (endIfIndex != -1)
        {            
            for (int i=ifIndex; i<endIfIndex; i++)
            {
                if (pieceBrickData[i].getLabel().startsWith("If")
                    || pieceBrickData[i].getLabel().startsWith("End If"))
                {
                    return false;
                }
            }
        }
    }

    if (brickData.getLabel().startsWith("End If"))
    {
        int endIfIndex = index;
        int ifIndex = findBrickIndexById(brickData.getPairBrickId());
        for (int i=ifIndex+1; i<endIfIndex; i++)
        {
            if (pieceBrickData[i].getLabel().startsWith("If")
                || pieceBrickData[i].getLabel().startsWith("End If"))
            {
                return false;
            }
        }
    }

    return true;
}

void PuzzleWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-brickdata"))
    {
            QByteArray pieceData = event->mimeData()->data("application/x-brickdata");
            QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
            BrickData brickData(-1);

            dataStream >> brickData;
            QRect dropBox = targetSquare(event->pos());

            int maxIndex = pieceBrickData.count();
            int index = dropBox.y()/pieceSize();
            if (index > maxIndex)
            {
                index = maxIndex;
            }

            if (isLogicalPosition(index,brickData))
            {
                if (brickData.getId() == -1) brickData.setId(generateNewId());
                insertBrick(index, brickData);
                highlightedRect = QRect();
                update();

                event->setDropAction(Qt::MoveAction);
                event->accept();
            }
            else
            {
                highlightedRect = QRect();
                event->ignore();
            }
    }
    else
    {
        highlightedRect = QRect();
        event->ignore();
    }
    update();
}

int PuzzleWidget::findPiece(const QRect &pieceRect) const
{
    for (int i = 0; i < pieceBrickData.size(); ++i) {
        QRect bbox(0,i*pieceSize(),imageSize(),pieceSize());
        if (pieceRect == bbox)
            return i;
    }
    return -1;
}

void PuzzleWidget::removeBrickAt(int removeIndex)
{

//    QRect bbox = pieceBrickData[removeIndex].getBbox();

    // If removing "If' brick, find the matching "End If" brick and remove that as well
//    if (pieceBrickData[removeIndex].getLabel().startsWith("If"))
//    {
//        QRect ifRect = bbox;
//        int index = 1;
//        while (true)
//        {
//            QRect endIfRect = targetSquare(ifRect.topLeft()+QPoint(0,index*pieceSize()));
//            int found = findPiece(endIfRect);
//            if (found != -1 && pieceBrickData[found].getLabel().startsWith("End If")
//                && pieceBrickData[found].getMasterBox() == pieceBrickData[removeIndex].getBbox())
//            {
//                pieceBrickData.removeAt(found);
//                QComboBox *cbox = comboBoxList[found];
//                comboBoxList.removeAt(found);
//                delete cbox;
//                update(endIfRect);
//                break;
//            }
//            index++;
//        }
//    }

    pieceBrickData.removeAt(removeIndex);
    QComboBox* cbox = comboBoxList[removeIndex];
    comboBoxList.removeAt(removeIndex);
    delete cbox;
    //update(bbox);
}

void PuzzleWidget::mousePressEvent(QMouseEvent *event)
{
    QRect square = targetSquare(event->pos());
    int found = findPiece(square);
    if (found == -1) return;

    BrickData brickData = pieceBrickData[found];    
    QComboBox* cbox = comboBoxList[found];
    //int pieceId = pieceBrickData[found].getId();
    QString label = pieceBrickData[found].getLabel();

    // Remove the brick from current position
    removeBrickAt(found);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << brickData;
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-brickdata", itemData);
    mimeData->setText(label);

    // Make the image shown during the drag operationIf WhatColorUnder?
    QPixmap pixmap(m_ImageSize,pieceSize());
    QPainter *painter = new QPainter(&pixmap);
    painter->setBrush(QColor("#46ccb8"));
    painter->setPen(Qt::NoPen);
    painter->drawRect(QRect(0,0,m_ImageSize,pieceSize()));
    painter->setPen(Qt::darkGreen);
    painter->setFont(QFont("Arial", fontSize()));
    painter->drawText(QPoint(labelLocation(),pieceSize()/2), brickData.getLabel());
    if (brickData.getValueItems().count() > 0)
    {
        painter->drawText(QPoint(valueLocation(),pieceSize()/2), brickData.getValueItems()[brickData.getSelectedValueIndex()]);
    }
    painter->drawText(QPoint(unitsLocation(),pieceSize()/2), brickData.getUnitsLabel());

    QPoint hotSpot = QPoint(2,2);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(hotSpot);

    Qt::DropAction dropAct = drag->exec(Qt::MoveAction);
    if (dropAct == Qt::IgnoreAction) // If drop failed, put brick back in original location
    {
        insertBrick(found,brickData);
        update(square);
    }
    else if (dropAct == Qt::MoveAction) // If successfully dropped in another open slot
    {
    }
}

void PuzzleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::white);

    if (highlightedRect.isValid()) {
        painter.setBrush(QColor("#fae8c7"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
    }

    for (int i = 0; i < pieceBrickData.size(); ++i)
    {
       BrickData brickData = pieceBrickData[i];

       //const QRect& bbox = brickData.getBbox();
       QRect bbox(0,i*pieceSize(),imageSize(),pieceSize());
       if (highlightedRect.isValid() && highlightedRect.y() <= bbox.y())
       {
           bbox.moveTop(bbox.y()+pieceSize());
       }
       QLinearGradient gradient(bbox.topLeft()+QPoint(bbox.width()/2,0),bbox.bottomRight()-QPoint(bbox.width()/2,0));
       gradient.setColorAt(0, QColor("#ffa341"));
       gradient.setColorAt(1, QColor("#46ccb8"));
       QBrush brush(gradient);

       if (brickData.getLabel().startsWith("If"))
       {
           painter.setBrush(QColor("#ffa341"));
       }
       else if (brickData.getLabel().startsWith("End If"))
       {
            painter.setBrush(QColor("#ffa341"));
       }
       else
       {
            painter.setBrush(brush);
       }
       painter.setPen(Qt::NoPen);
       painter.drawRect(bbox);

       painter.setPen(Qt::darkGreen);
       painter.setFont(QFont("Arial", fontSize()));
       painter.drawText(bbox.topLeft() + QPoint(labelLocation(),pieceSize()/2), brickData.getLabel());

       if (!brickData.getLabel().startsWith("End If"))
       {
            QComboBox* cbox = comboBoxList[i];
            cbox->move(bbox.topLeft() + QPoint(valueLocation(),0));
            cbox->show();

            painter.setPen(Qt::darkGreen);
            painter.setFont(QFont("Arial", fontSize()));
            painter.drawText(bbox.topLeft() + QPoint(unitsLocation(),pieceSize()/2), brickData.getUnitsLabel());
       }
    }

//    // Connect the "If" bricks to the "End If" bricks.
//    for (int i = 0; i < pieceBrickData.size(); ++i)
//    {
//       BrickData brickData = pieceBrickData[i];
//       if (brickData.getLabel().startsWith("End If"))
//       {
//           QPoint topPt = brickData.getMasterBox().topLeft();
//           QPoint bottomPt = brickData.getBbox().bottomLeft();
//           qreal thickness = (imageSize() - bottomPt.y())/10;
//           painter.setBrush(QColor("#46ccb8"));
//           painter.drawRect(QRect(topPt,bottomPt+QPoint(thickness,0)));
//       }
//    }

    painter.end();
}

const QRect PuzzleWidget::targetSquare(const QPoint &position) const
{
    return QRect(0, position.y()/pieceSize() * pieceSize(), m_ImageSize, pieceSize());
}

int PuzzleWidget::pieceSize() const
{
    return m_ImageSize / 12;
}

int PuzzleWidget::imageSize() const
{
    return m_ImageSize;
}

int PuzzleWidget::labelLocation() const
{
    return (int)(m_ImageSize * 0.1);
}

int PuzzleWidget::valueLocation() const
{
    return (int)(m_ImageSize * 0.5);
}

int PuzzleWidget::unitsLocation() const
{
    return (int)(m_ImageSize * 0.85);
}

int PuzzleWidget::fontSize() const
{
    return pieceSize() / 4;
}
