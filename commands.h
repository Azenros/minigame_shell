#ifndef COMMAND_H
#define COMMAND_H
#include "base.h"
#include <vector>
//not sure what to do here
using namespace std;

class Command: public Base {
    protected:
        vector<char*> arguments();
    public:
        Command() : Base() {};
        bool execute(int indir, int outdir);
        bool isConnector() {return false;};
};


#endif
