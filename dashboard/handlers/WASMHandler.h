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
};
private:

};


#endif //BITSANDDROIDSCONNECTOR_WASMHANDLER_H
