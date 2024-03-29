//
// Created by DaveRiedel on 18-8-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_OUTPUTTABBUILDER_H
#define BITSANDDROIDSCONNECTOR_OUTPUTTABBUILDER_H


#include <QWidget>

class OutputTabBuilder : public QWidget {
Q_OBJECT
private:
    QString activeSetId;
private slots:
    void saveEditedSet();
public:
    explicit OutputTabBuilder(QWidget *parent);

    QWidget *buildOutputTabContainer();

    QWidget* parent;
    signals:
    void setEdited(QString id);


public slots:

    void setCheckboxes(const QString& id);
};


#endif //BITSANDDROIDSCONNECTOR_OUTPUTTABBUILDER_H
