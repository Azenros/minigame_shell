//this is the base.h file
#ifndef BASE_H
#define BASE_H

#include <string>

using namespace std;

class Base {
    public:
        Base* left;
        Base* right;
        Base() {};
        virtual bool verify() = 0;
        virtual bool execute(int indir, int outdir) = 0;
        void readIn(Base* left, Base* right) {
            this->left = left;
            this->right = right;
        }
};

#endif
