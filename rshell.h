#ifndef RSHELL_H
#define RSHELL_H
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <cstring>
#include <sstream>
#include "commands.h"
#include "connector.h"
#include "base.h"
#include "execute.h"
#include <stack>
#include <queue>
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

using namespace std;
//these are our functions
class rShell {
    public:
        int mainFunc();
        Base* shuntTree(vector<string>&);
        void parser(string, vector<string>&);
        void parser(string, vector<string>&, ostringstream&);
        bool verify(vector<string>);
        void verify(string, vector<string>, ostringstream&);
};

#endif
