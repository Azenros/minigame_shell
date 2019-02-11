#include "execute.h"
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
#include <algorithm>
#include <iterator>

using namespace std;
//HUGE EXECUTE OVERHAUL
//will add more execute functiion (vector function)
bool Execute::verify() {
    return true;
}

Execute::Execute(string userString) {
    char * cstr = new char[userString.size() + 1];
    strcpy(cstr, userString.c_str());
    char * token = strtok(cstr, " ");

    while (token != NULL) {
        execvpr.push_back(token);
        token = strtok(NULL, " ");
    }
}

bool Execute::execute(int indir, int outdir) {
    if (execvpr.size() == 0) {
        return false;
    }
    if (execvpr.at(0) == "exit") {
        exit(0);
    }
    if (execvpr.at(0) == "test" || execvpr.at(0) == "[") {
        if (execvpr.at(0) == "[") {
            if (execvpr.at(execvpr.size() - 1) != "]") {
                cout << "Missing brackets" << endl;
                return false;
            }
        }
        struct stat cont; //content info
        if (execvpr.at(1) == "-e") {
            if (stat(const_cast<char*>(execvpr.at(2).c_str()), &cont) == 0) {
                cout << "(True)" << endl;
                return true;
            }
            else {
                cout << "unable to access " << execvpr.at(2) << endl;
                cout << "(False)" << endl;
                return false;
            }
        }
        else if (execvpr.at(1) == "-f") {
            if (stat(const_cast<char*>(execvpr.at(2).c_str()), &cont) != 0) {
                cout << "unable to access " << execvpr.at(2) << endl;
                cout << "(False)" << endl;
                return false;
            }
                    
            else if (cont.st_mode & S_IFREG) {
                cout << "(True)" << endl;
                return true;
            }
                    
            else {
                cout << "(False)" << endl;
                return false;
            }
        }
        else if (execvpr.at(1) == "-d") {
            if (stat(const_cast<char*>(execvpr.at(2).c_str()), &cont) != 0) {
                cout << "unable to access " << execvpr.at(2) << endl;
                cout << "(False)" << endl;
                return false;
            }
                    
            else if (cont.st_mode & S_IFDIR) {
                cout << "(True)" << endl;
                return true;
            }
                    
            else {
                cout << "(False)" << endl;
                return false;
            }
        }
        
        else {
            if (stat(const_cast<char*>(execvpr.at(1).c_str()), &cont) == 0) {
                cout << "(True)" << endl;
                return true;
            }
                    
            else {
                cout << "unable to access " << execvpr.at(1) << endl;
                cout << "(False)" << endl;
                return false;
            }
        }
        
    }
    else {
        pid_t pid = fork();
        pid_t w;
        int status;

        if (pid < 0) {
            perror("process");
            exit(1);
        }

        else if (pid == 0) {
            // if (execvp(*args, args) < 0) {
            //     perror("exec");
            //     exit(1);
            // }
            vector<string> t;
            for (unsigned i = 0; i < execvpr.size(); i++) {
                if (execvpr.at(i) != "<" && execvpr.at(i) != ">" && execvpr.at(i) != ">>") {
                    t.push_back(execvpr.at(i));
                }
                else {
                    break;
                }
            }
            for (unsigned i = 0; i < execvpr.size() - 1; i++) {
                if (execvpr.at(i) == ">") {
                    output = execvpr.at(i + 1);
                }
                else if (execvpr.at(i) == ">>") {
                    output = execvpr.at(i + 1);
                }
                else if (execvpr.at(i) == "<") {
                    input = execvpr.at(i + 1);
                }
            }
            unsigned tempSz = t.size() + 1;
            char* targ[tempSz];
            for (unsigned i = 0; i < tempSz - 1; ++i) {
                targ[i] = const_cast<char*>(t.at(i).c_str());
            }
            targ[tempSz - 1] = NULL;

            if (!input.empty() || !output.empty()) {
                if (!input.empty() && !output.empty()) {
                    if (find(execvpr.begin(), execvpr.end(), ">") != execvpr.end()) {
                        int fin = open(input.c_str(), O_RDWR);
                        int fout = open(output.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRWXG);
                        if (fin == -1 || fout == -1) {
                            perror("Error: unable to open file");
                            return false;
                        }
                        dup2(fin, 0);
                        dup2(fout, 1);
                        close(fin);
                        close(fout);
                    }
                    else {
                        int fin = open(input.c_str(), O_RDWR);
                        int fout = open(output.c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG);
                        if(fin == -1 || fout == -1) {
                            perror("Error: unable to open file");
                            return false;
                        }
                        dup2(fin, 0);
                        dup2(fout, 1);
                        close(fin);
                        close(fout);
                    }
                }
                else if (!input.empty()) { // ">" or ">>" in use. (above was "<" or ">" OR "<" OR ">>"")
                    int fiDesc = open(input.c_str(), O_RDWR);
                    if (fiDesc == -1) {
                        perror("Error: unable to open file");
                        return false;
                    }
                    dup2(fiDesc, 0);
                    close(fiDesc);
                }
                else if (!output.empty()) {
                    if (find(execvpr.begin(), execvpr.end(), ">") != execvpr.end()) {
                        int fiDesc2;
                        fiDesc2 = open(output.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRWXG);
                        if (fiDesc2 < 0) {
                            perror("Error: open failed.");
                            return false;
                        }
                        dup2(fiDesc2, 1);
                        close(fiDesc2);
                    }
                    else {
                        int fiDesc3;
                        fiDesc3 = open(output.c_str(), O_CREAT | O_APPEND | O_WRONLY, S_IRWXU | S_IRWXG);
                        if (fiDesc3 < 0) {
                            perror("Error: open failed.");
                            return false;
                        }
                        dup2(fiDesc3, 1);
                        close(fiDesc3);
                    }
                }
                dup2(indir, 0);
                dup2(outdir, 1);
                if (indir != 0) {
                    close(indir);
                }
                else if (outdir != 1) {
                    close(outdir);
                }
                if (execvp(*targ, targ) < 0) {
                    cout << "*** ERROR: exec failed\n" << endl;
                    exit(1);
                }
            }
            else {
                char * args[execvpr.size() + 1];
                for (unsigned i = 0; i < execvpr.size(); i++) {
                    args[i] = const_cast<char*>(execvpr.at(i).c_str());
                // pid_t pid = fork();
                // pid_t w;
                }
                args[execvpr.size()] = NULL;
            // for (unsigned i = 0; i < execvpr.size() + 1; ++i) { //loop doesn't work
                // pid_t pid = fork();
                // pid_t w;

                // int status;
                dup2(indir, 0);
                dup2(outdir, 1);
                if (indir != 0) {
                    close(indir);
                }
                else if (outdir != 1) {
                    close(outdir);
                }
                if (execvp(*args, args) < 0) {
                    cout << "*** ERROR: exec failed\n" << endl;
                    exit(1);
                }
            }
        }

        else {
            w = waitpid(pid, &status, 0);
            if (w == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }

            if (WEXITSTATUS(status) == 0) {
                return true;
            }
            // }     // }
        }
    }
    return false;
}
Execute::Execute(vector<string> vectName) {
    for (unsigned i = 0; i < vectName.size(); ++i) {
        execvpr.push_back(vectName.at(i));
    }
}
