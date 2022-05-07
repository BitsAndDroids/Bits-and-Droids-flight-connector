//
// Created by Dave Riedel on 07-May-22.
//

#ifndef BITSANDDROIDSGUI_RUNGUARD_H
#define BITSANDDROIDSGUI_RUNGUARD_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>


class RunGuard
{

public:
    RunGuard( const QString& key );
    ~RunGuard();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString key;
    const QString memLockKey;
    const QString sharedmemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY( RunGuard )
};


#endif //BITSANDDROIDSGUI_RUNGUARD_H
