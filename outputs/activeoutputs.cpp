#include "outputEnum.h"
#include "activeoutputs.h"



outputEnum outputEnum;
using namespace std;



ActiveOutputs::ActiveOutputs()
{   
}
void ActiveOutputs::changeOutputState(int index){
    activeOutputsMap.insert(index,!activeOutputsMap.value(index, false));
}
