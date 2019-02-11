#include "connector.h"
#include "base.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>	
#include <string.h>
#include <string>
#include <fcntl.h>


using namespace std;

/*MAJOR OVERHAUL WITH CONNECTOR CLASS
ALL CONNECTORS IN CONNECTOR.H
ALL SEPARATE CONNECTOR FILES COMMENTED OUT
ALL CONNECTORS IN SINGLE AREA*/

//this one is ;
//AlwaysRunNext::AlwaysRunNext() : Connector() {}

bool AlwaysRunNext::execute(int indir, int outdir) {
    left->execute(0, 1);
    bool rightExec = right->execute(0, 1);
    if (rightExec) {
        return true;
    }
    return false;
}

bool AlwaysRunNext::verify() {
    return false;
}
//this one is ||
//FailRunNext::FailRunNext() : Connector() {}

bool FailRunNext::execute(int indir, int outdir) {
    if(left->execute(0, 1)) {
        return true;
    }

    else { 
        if(right->execute(0, 1)) {
            return true;
        }
        else {
            return false;
        }
    }
}


bool FailRunNext::verify() {
	return false;
}

//this one is &&
//PassRunNext::PassRunNext() : Connector() {}

bool PassRunNext::execute(int indir, int outdir) {
    if(left->execute(0, 1)) {
        if(right->execute(0, 1)) {
            return true;
        }	
        else {
            return false;
        }
    }
    return false;
}

bool PassRunNext::verify() {
	return false;
}

bool Piping::execute(int indir, int outdir) {
    int fid[2];
    pipe(fid);
    left->execute(indir, fid[1]);
    close(fid[1]);

    right->execute(fid[0], outdir);
    close(fid[0]);

    return true;
}
bool Piping::verify() {
    return false;
}

