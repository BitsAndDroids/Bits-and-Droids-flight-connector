#include "outputbundle.h"

outputBundle::outputBundle()
{

}

bool outputBundle::isOutputInBundle(int id)
{
    QList<Output*>::iterator i;
    bool found = false;
    for(i = outputsInSet.begin(); i != outputsInSet.end(); ++i){
        if((*i)->getId() == id){
            bool found = true;
        }
    }
}
