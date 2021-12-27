#ifndef LOADER_H
#define LOADER_H
#include "INCLUDS.h"
#include "Objects.h"

class Loader
{
public:
    Model& loadmodel(std::string &name);
    std::vector <std::vector <Color>>& loadpng(std::string &name);
};
#endif // LOADER_H
