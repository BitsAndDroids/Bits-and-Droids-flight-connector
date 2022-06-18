//
// Created by DaveRiedel on 18-Jun-22.
//

#ifndef BITSANDDROIDSCONNECTOR_INPUTREADER_H
#define BITSANDDROIDSCONNECTOR_INPUTREADER_H


class InputReader {

public:
    InputReader();
    void readInputs();
    std::map<int, Input> getInputs();
private:
    std::map<int, Input> inputs = std::map<int, Input>();

};


#endif //BITSANDDROIDSCONNECTOR_INPUTREADER_H
