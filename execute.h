#ifndef EXECUTE_H
#define EXECUTE_H
#include <vector>
#include <iostream>
#include "base.h"

using namespace std;
//HUGE EXECUTE OVERHAUL
class Execute: public Base {
    protected:
        vector<string> execvpr;
        string input;
        string output;
    public:
        Execute() {};
        Execute(vector<string>);
        Execute(string userString);
        bool execute(int indir, int outdir);
        bool verify();

};

#endif
