//
// Created by DaveRiedel on 21-9-2022.
//

#ifndef BITSANDDROIDSCONNECTOR_WASMHANDLER_H
#define BITSANDDROIDSCONNECTOR_WASMHANDLER_H


class WASMHandler {
public:
    WASMHandler();
    void sendWASMCommand(const char *data);
    void installWasm();

    bool isWASMModuleInstalled();
private:
    bool versionCheck();
    void copyFolder(const QString &sourceFolder, const QString &destinationFolder);
    QString WASMModulePath = "";

    QString getLocalVersion();

    void updateModule();
};


#endif //BITSANDDROIDSCONNECTOR_WASMHANDLER_H
