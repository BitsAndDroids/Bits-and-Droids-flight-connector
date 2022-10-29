//
// Created by DaveRiedel on 21-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_WASMHANDLER_H
#define BITSANDDROIDSCONNECTOR_WASMHANDLER_H


class WASMHandler : public QObject {
Q_OBJECT
public:
    WASMHandler();

    void installWasm();

    bool isWASMModuleInstalled();

public slots:
    void updateEventFile();

signals:

    void sendWASMCommand(const char *data);

private:
    bool versionCheck();

    void copyFolder(const QString &sourceFolder, const QString &destinationFolder);

    QString WASMModulePath = "";

    QString getLocalVersion();

    void updateModule();
};


#endif //BITSANDDROIDSCONNECTOR_WASMHANDLER_H
