//
// Created by Dave Riedel on 06-May-22.
//

#ifndef BITSANDDROIDSGUI_MODEINDEXCOMBOBOX_H
#define BITSANDDROIDSGUI_MODEINDEXCOMBOBOX_H


#include <QComboBox>

class ModeIndexCombobox: public QComboBox{
    Q_OBJECT



private:
    int mode;
    int index;
    QString prefix;
public:
    ModeIndexCombobox(QString prefix, int index);
    ModeIndexCombobox(QString prefix,int mode, int index);
};


#endif //BITSANDDROIDSGUI_MODEINDEXCOMBOBOX_H
