//
// Created by dave- on 30-6-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_LOGWINDOW_H
#define BITSANDDROIDSCONNECTOR_LOGWINDOW_H


#include <QWidget>
#include <QStringList>

class LogWindow: public QWidget {
    Q_OBJECT
private:
    QStringList logList;

public:
    LogWindow(QWidget *parent);
    ~LogWindow();

public slots:
    void addLogRow(QString);
    void openWindow();
};


#endif //BITSANDDROIDSCONNECTOR_LOGWINDOW_H
