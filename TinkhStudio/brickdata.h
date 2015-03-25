#ifndef BRICKDATA_H
#define BRICKDATA_H

#endif // BRICKDATA_H

#include <QRect>
#include <QLabel>
#include <QComboBox>

class BrickData
{
    private:
        int id;
        //QRect boundingBox;
        QString label;
        QString unitsLabel;
        QList<QString> valueItems;
        int selectedValueIndex;
        int pairBrickId;
        //QRect masterBox;

  //  protected:
        //void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    public:
        BrickData(int id);
        BrickData(int id, const QString& labelStr);
        //~BrickData();
        //BrickData( const BrickData &obj);  // copy constructor

        void draw();

        int getId() const;
        //const QRect& getBbox() const;
        //const QRect& getMasterBox() const;
        const QString& getLabel() const;
        const QList<QString>& getValueItems() const;
        const QString& getUnitsLabel() const;
        int getSelectedValueIndex() const;
        int getPairBrickId() const;

        void createBrickData(const QString& labelStr);
        void createBrickData(const QString& labelStr, const QString& unitsLabelStr, const QList<QString> valueItems, int selectedValueIndex);
        //void setBbox(const QRect& bbox);
        //void setMasterBox(const QRect& bbox);
        void setId(int id);
        void setSelectedValueIndex(int selIndex);
        void setUnitsLabel(QString unitsLabel);
        void setPairBrickId(int pairBrickId);

        bool operator<(const BrickData &i1) const;
};

QDataStream &operator<<(QDataStream &stream, const BrickData &obj);
QDataStream &operator>>(QDataStream &stream, BrickData &obj);
