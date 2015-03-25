#include "brickdata.h"
#include <QDataStream>


BrickData::BrickData(int id)
{
    this->id = id;
    pairBrickId = -1;
    // Everything else is uninitialized
}

BrickData::BrickData(int id, const QString & labelStr)
{
    this->id = id;
    pairBrickId = -1;
    createBrickData(labelStr);
}

void BrickData::createBrickData(const QString& labelStr)
{    
    label = labelStr;
    if (labelStr == "Roll") unitsLabel = "inches";
    if (labelStr == "Back") unitsLabel = "inches";
    if (labelStr == "Left") unitsLabel = "degrees";
    if (labelStr == "Right") unitsLabel = "degrees";
    if (labelStr == "HeadUp") unitsLabel = "degrees";
    if (labelStr == "HeadDown") unitsLabel = "degrees";

    if (labelStr == "Roll" || labelStr == "Back")
    {
        valueItems.append("1");
        valueItems.append("2");
        valueItems.append("5");
        valueItems.append("10");
        selectedValueIndex = 2;
    }
    else if (labelStr == "Left" || labelStr == "Right")
    {
        valueItems.append("5");
        valueItems.append("10");
        valueItems.append("45");
        valueItems.append("90");
        valueItems.append("180");
        selectedValueIndex = 3;
    }
    else if (labelStr == "HeadUp" || labelStr == "HeadDown")
    {
        valueItems.append("5");
        valueItems.append("10");
        valueItems.append("20");
        selectedValueIndex = 1;
    }
    else if (labelStr == "Color")
    {
        valueItems.append("RED");
        valueItems.append("GREEN");
        valueItems.append("BLUE");
        valueItems.append("CYAN");
        valueItems.append("MAGENTA");
        valueItems.append("YELLOW");
        valueItems.append("WHITE");
        selectedValueIndex = 3;
    }
    else if (labelStr == "Music")
    {
        valueItems.append("NOTE_C");
        valueItems.append("NOTE_D");
        valueItems.append("NOTE_E");
        valueItems.append("NOTE_F");
        valueItems.append("NOTE_G");
        valueItems.append("CHICKEN_DANCE");
        valueItems.append("MACARENA");
        selectedValueIndex = 0;
    }
    else if (labelStr == "Dance")
    {
        valueItems.append("CHICKEN_DANCE");
        valueItems.append("MACARENA");
        selectedValueIndex = 0;
    }
    else if (labelStr == "If WhatColorUnder?")
    {
        valueItems.append("== RED");
        valueItems.append("== GREEN");
        valueItems.append("== BLUE");
        valueItems.append("== CYAN");
        valueItems.append("== MAJENTA");
        valueItems.append("== YELLOW");
        valueItems.append("== WHITE");
        valueItems.append("!= RED");
        valueItems.append("!= GREEN");
        valueItems.append("!= BLUE");
        valueItems.append("!= CYAN");
        valueItems.append("!= MAJENTA");
        valueItems.append("!= YELLOW");
        valueItems.append("!= WHITE");
        selectedValueIndex = 0;
    }
    else if (labelStr == "If IsFrontClear?")
    {
        valueItems.append("== YES");
        valueItems.append("== NO");
        selectedValueIndex = 0;
    }
    else if (labelStr == "If WhichSideBrighter?")
    {
        valueItems.append("== RIGHT");
        valueItems.append("== LEFT");
        selectedValueIndex = 0;
    }
    else if (labelStr == "If WhichSideOfLine?")
    {
        valueItems.append("== RIGHT");
        valueItems.append("== LEFT");
        selectedValueIndex = 0;
    }
}

/*
BrickData::~BrickData()
{
}

BrickData::BrickData(const BrickData &obj)
{
    id = obj.id;
    boundingBox = new QRect();
    boundingBox->setRect(obj.getBbox().x(), obj.getBbox().y(), obj.getBbox().width(), obj.getBbox().height());
    label = new QLabel(obj.getLabel().text(),obj.getParentWidget());
    label->setAttribute(Qt::WA_DeleteOnClose);
    valueSelector = new QComboBox(obj.getParentWidget());
    for (int i = 0; i < obj.getValueSelector().count(); ++i) {
        valueSelector->insertItem(i, obj.getValueSelector().itemText(i));
    }
    unitsLabel = new QLabel(obj.getUnitsLabel().text(), obj.getParentWidget());
    parentWidget = obj.getParentWidget();
}
*/

const QString& BrickData::getLabel() const
{
    return label;
}

const QList<QString>& BrickData::getValueItems() const
{
    return valueItems;
}

const QString& BrickData::getUnitsLabel() const
{
    return unitsLabel;
}

/*
void BrickData::draw() {
    label->move(boundingBox->topLeft() + QPoint(100,5));
    label->show();
    valueSelector->move(boundingBox->topLeft() + QPoint(400,1));
    valueSelector->show();
    unitsLabel->move(boundingBox->topLeft() + QPoint(600,5));
    unitsLabel->show();
}*/

int BrickData::getSelectedValueIndex() const
{
    return selectedValueIndex;
}

int BrickData::getId() const
{
    return id;
}

int BrickData::getPairBrickId() const
{
    return pairBrickId;
}

void BrickData::createBrickData(const QString& labelStr, const QString& unitsLabelStr,
                     const QList<QString> valueItems, int selectedValueIndex)
{
    label = labelStr;
    unitsLabel = unitsLabelStr;
    for (int i=0; i<valueItems.size(); i++)
    {
        this->valueItems.append(valueItems[i]);
    }
    this->selectedValueIndex = selectedValueIndex;
}

void BrickData::setSelectedValueIndex(int selIndex)
{
    selectedValueIndex = selIndex;
}

void BrickData::setUnitsLabel(QString unitsLabel)
{
    this->unitsLabel = unitsLabel;
}

void BrickData::setId(int id)
{
    this->id = id;
}

void BrickData::setPairBrickId(int pairBrickId)
{
    this->pairBrickId = pairBrickId;
}

QDataStream &operator<<(QDataStream &stream, const BrickData &obj) {
    //stream << (qint32) obj.getId();
    //stream << obj.getBbox();
    //stream << obj.getMasterBox();
    stream << obj.getId();
    stream << obj.getLabel();
    stream << obj.getUnitsLabel();
    stream << obj.getValueItems();
    stream << obj.getSelectedValueIndex();
    stream << obj.getPairBrickId();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, BrickData &obj)
{
    int id;
    QString label;
    QString unitsLabel;
    QList<QString> valueItems;
    int selectedValueIndex;
    int pairBrickId;

    stream >> id;
    stream >> label;
    stream >> unitsLabel;
    stream >> valueItems;
    stream >> selectedValueIndex;
    stream >> pairBrickId;

    obj.setId(id);
    obj.setPairBrickId(pairBrickId);
    obj.createBrickData(label,unitsLabel,valueItems,selectedValueIndex);

    return stream;
}

// For sorting based on vertical position
bool BrickData::operator<(const BrickData &i1) const
{
    return true;
   //return boundingBox.topLeft().y() < i1.getBbox().topLeft().y();
}
