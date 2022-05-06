//
// Created by Dave Riedel on 06-May-22.
//

#ifndef BITSANDDROIDSGUI_MODEINDEXCHECKBOX_H
#define BITSANDDROIDSGUI_MODEINDEXCHECKBOX_H


#include <QCheckBox>

class ModeIndexCheckbox: public QCheckBox {
    Q_OBJECT
private:
    int mode;
    int index;
    QString prefix;
public:
    ModeIndexCheckbox(const QString& label,int mode, int index);

    int getMode() const;

    int getIndex() const;

    const QString &getPrefix() const;
};


#endif //BITSANDDROIDSGUI_MODEINDEXCHECKBOX_H
