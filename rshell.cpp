#include "rshell.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include <sys/utsname.h>
#include <boost/tokenizer.hpp>
using namespace std;
using namespace boost;
//process of rShell
//parse
//use shunting yard's algorithm to implement a tree
//create a main function for main.cpp to utilize and call
void rShell::parser(string userInput, vector<string> &mergeObj) {
    //parses input and puts it into vector
    bool subsequent1 = false;
    bool subsequent2 = false;
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> split("", "&;|#()", drop_empty_tokens);
    tokenizer tokens(userInput, split);
    for (tokenizer::iterator tok_i = tokens.begin(); tok_i != tokens.end(); tok_i++) {
        if (*tok_i == "|") {
            if (subsequent1) {
                mergeObj.pop_back();
                mergeObj.push_back("||");
                subsequent1 = false;
            }
            else {
                mergeObj.push_back("%");
                subsequent1 = true;
            }
        }
        else if (*tok_i == "&") {
            tok_i++;
            mergeObj.push_back("&");
            subsequent1 = false;
            subsequent2 = false;
        }
        else if (*tok_i == "#") {
            break;
        }
        // else if (*tok_i == ">") {
        //     if (subsequent2) {
        //         mergeObj.pop_back();
        //         mergeObj.push_back(">>");
        //         subsequent2 = false;
        //     }
        //     else {
        //         mergeObj.push_back(">");
        //         subsequent2 = true;
        //     }
        // }
        else if (*tok_i == "exit") {
            mergeObj.push_back(*tok_i);
            break;
        }
        else {
            string tok = *tok_i;
            while (tok.at(0) == ' ') {
                //clear beginning spaces
                tok.erase(tok.begin());
                if (tok == "") {
                    break;
                }
            }

            if (tok != "") {
                while (tok.at(tok.size() - 1) == ' ') {
                    //clear ending spaces
                    tok.erase(tok.end() - 1);
                }
                mergeObj.push_back(tok);
            }
            subsequent1 = false;
            subsequent2 = false;
        }
    }
}

void rShell::parser(string userInput, vector<string>& mergeObj, ostringstream& oss) {
    //parse function for testing purposes
    bool subsequent1 = false;
    bool subsequent2 = false;
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> split("", "&;|#()", drop_empty_tokens);
    tokenizer tokens(userInput, split);
    for (tokenizer::iterator tok_i = tokens.begin(); tok_i != tokens.end(); tok_i++) {
        if (*tok_i == "|") {
            if (subsequent1) {
                mergeObj.pop_back();
                mergeObj.push_back("||");
                subsequent1 = false;
            }
            else {
                mergeObj.push_back("%");
                subsequent1 = true;
            }
        }
        else if (*tok_i == "&") {
            tok_i++;
            mergeObj.push_back("&&");
            subsequent1 = false;
            subsequent2 = false;
        }
        else if (*tok_i == "#") {
            break;
        }
        // else if (*tok_i == ">") {
        //     if (subsequent2) {
        //         mergeObj.pop_back();
        //         mergeObj.push_back(">>");
        //         subsequent2 = false;
        //     }
        //     else {
        //         mergeObj.push_back(">");
        //         subsequent2 = true;
        //     }
        // }
        else if (*tok_i == "exit") {
            mergeObj.push_back(*tok_i);
            break;
        }
        else {
            string tok = *tok_i;
            while (tok.at(0) == ' ') {
                //clear beginning spaces
                tok.erase(tok.begin());
                if (tok == "") {
                    break;
                }
            }

            if (tok != "") {
                while (tok.at(tok.size() - 1) == ' ') {
                    //clear ending spaces
                    tok.erase(tok.end() - 1);
                }
                mergeObj.push_back(tok);
            }
            subsequent1 = false;
            subsequent2 = false;
        }
    }
    //fill buffer with parsed input
    for (unsigned i = 0; i < mergeObj.size(); i++) {
        oss << mergeObj.at(i) << " ";
    }
    oss << endl;
}

// test || test ; test && test
bool rShell::verify(vector<string> cmdList) {
    //if empty, nothing to execute
    if (cmdList.size() == 0) {
        return false;
    }
    //if size 1, check for connector
    else if (cmdList.size() == 1) {
        if (cmdList.at(0) == "||" || cmdList.at(0) == "&&" || cmdList.at(0) == ";"
           || cmdList.at(0) == "(" || cmdList.at(0) == ")" || cmdList.at(0) == "["
           || cmdList.at(0) == "]" || cmdList.at(0) == "|" || cmdList.at(0) == "<"
           || cmdList.at(0) == ">" || cmdList.at(0) == ">>") {
            return false;
        }
        else {
            return true;
        }
    }
    //more than 2, check for beginning connector (instant fail)
    else if (cmdList.at(0) == "||" || cmdList.at(0) == "&&"
             || cmdList.at(0) == ")" || cmdList.at(0) == "]"
             || cmdList.at(0) == "|" || cmdList.at(0) == "<"
             || cmdList.at(0) == ">" || cmdList.at(0) == ">>") {
        return false;
    }
    else {
        //check for consecutive connectors in input
        bool connector1 = false;
        bool connector2 = false;

        for (unsigned i = 1; i < cmdList.size(); i++) {
            if (cmdList.at(i - 1) == "&&" || cmdList.at(i - 1) == "||"
                || cmdList.at(i - 1) == ";" || cmdList.at(i - 1) == "|"
                || cmdList.at(i - 1) == "<" || cmdList.at(i - 1) == ">"
                || cmdList.at(i - 1) == ">>") {
                connector1 = true;
            }
            if (cmdList.at(i) == "&&" || cmdList.at(i) == "||"
                || cmdList.at(i) == ";" || cmdList.at(i) == "|"
                || cmdList.at(i) == "<" || cmdList.at(i) == ">"
                || cmdList.at(i) == ">>") {
                connector2 = true;
            }
            if (connector1 && connector2) {
                return false;
            }
            connector1 = false;
            connector2 = false;
        }

        //check last item for connector (instant fail)
        if (cmdList.at(cmdList.size() - 1) == "&&"
            || cmdList.at(cmdList.size() - 1) == "||"
            || cmdList.at(cmdList.size() - 1) == ";") {
            return false;
        }

        //check for uneven numbers of parentheses/brackets
        int leftpNum = 0, rightpNum = 0, leftbNum = 0, rightbNum = 0;
        for (unsigned i = 0; i < cmdList.size(); i++) {
            if (cmdList.at(i) == "(") {
                leftpNum++;
            }
            else if (cmdList.at(i) == ")") {
                rightpNum++;
            }
            else if (cmdList.at(i) == "[") {
                leftbNum++;
            }
            else if (cmdList.at(i) == "]") {
                rightbNum++;
            }
        }
        if (leftpNum != rightpNum || leftbNum != rightbNum) {
            return false;
        }
        //check for consecutive parentheses/brackets (if both open / both close, continue)
        connector1 = false;
        connector2 = false;
        for (unsigned i = 1; i < cmdList.size(); i++) {
            if (cmdList.at(i - 1) == cmdList.at(i)
                || (cmdList.at(i - 1) == "(" && cmdList.at(i) == "[")
                || (cmdList.at(i - 1) == ")" && cmdList.at(i) == "]")
                || (cmdList.at(i - 1) == "[" && cmdList.at(i) == "(")
                || (cmdList.at(i - 1) == "]" && cmdList.at(i) == ")") ) {
                continue;
            }
            if (cmdList.at(i - 1) == "(" || cmdList.at(i - 1) == ")"
                || cmdList.at(i - 1) == "[" || cmdList.at(i - 1) == "]") {
                connector1 = true;
            }
            if (cmdList.at(i) == "(" || cmdList.at(i) == ")"
                || cmdList.at(i) == "[" || cmdList.at(i) == "]") {
                connector2 = true;
            }
            if (connector1 && connector2) {
                return false;
            }
            connector1 = false;
            connector2 = false;
        }
    }
    return true;
}

void rShell::verify(string cmdFull, vector<string> cmdList, ostringstream& ossOut) {
    //for testing purposes
    ostringstream ossIn;
    parser(cmdFull, cmdList, ossIn);

    if (verify(cmdList)) {
        ossOut << "Valid input";
    }
    else {
        ossOut << "Invalid input";
    }
}

Base* rShell::shuntTree(vector<string> &commandList) {
    //Tree to store input using the shunting yard algorithm
    stack<string>stackMand; //stack that stores the operands
    queue<string> queueMand; //queue that stores the commands
    stack<Base*> treeStack;//tree stack
    Base* branch; //branch
    for (unsigned int i = 0; i < commandList.size(); i++) {
        if (commandList.at(i) != ";" && commandList.at(i) != "||"
            && commandList.at(i) != "&" && commandList.at(i) != "("
            && commandList.at(i) != ")" && commandList.at(i) != "%") {
            queueMand.push(commandList.at(i));
        }
        else if (commandList.at(i) == ";" || commandList.at(i) == "||"
                 || commandList.at(i) == "&" || commandList.at(i) == "%") {
            if (stackMand.empty()) {
                stackMand.push(commandList.at(i));
                continue;
            }
            else if (stackMand.top() != "(") {
                queueMand.push(stackMand.top());
                stackMand.pop();
            }
            stackMand.push(commandList.at(i));
        }
        else if (commandList.at(i) == "(") {
            stackMand.push(commandList.at(i));
        }
        else if (commandList.at(i) == ")") {
            while (stackMand.top() != "(") {
                queueMand.push(stackMand.top());
                stackMand.pop();
            }
            stackMand.pop();
        }
    }
    while (!stackMand.empty()) {
        queueMand.push(stackMand.top());
        stackMand.pop();
    }
    while (!treeStack.empty()) {
        treeStack.pop();
    }
    while (!queueMand.empty()) {
        if (queueMand.front() == "&") {
            branch = new PassRunNext();
        }

        else if (queueMand.front() == "|") {
            branch = new FailRunNext();
        }
        else if (queueMand.front() == ";") {
            branch = new AlwaysRunNext();
        }
        else if (queueMand.front() == "%") {
            branch = new Piping();
        }
        else {
            branch = new Execute(queueMand.front());
        }

        if (branch->verify()) {
            treeStack.push(branch);
        }
        else {
            Base* right = treeStack.top();
            treeStack.pop();
            Base* left = treeStack.top();
            treeStack.pop();
            branch->readIn(left, right);
            treeStack.push(branch);
        }
        queueMand.pop();
    }
    Base* beg = treeStack.top();
    while (!treeStack.empty()) {
        treeStack.pop();
    }
    return beg;
}


//this main function will be called in main.cpp
//this will create the command shell and add connectors and commands
int rShell::mainFunc() {
    bool checkthis = true;
    while(checkthis) {
        if (getlogin() != NULL && getenv("HOSTNAME") != NULL) {
            cout << "[" << getlogin() << "@" << getenv("HOSTNAME") << "]";
        }
        cout << "$ "; // reprint console
        string userCmd;
        vector<string> combined;
        getline(cin, userCmd); //gets user input

        if (userCmd.find('#') != string::npos) {
            userCmd = userCmd.substr(0, userCmd.find('#'));
        }

        parser(userCmd, combined);
        if (verify(combined)) {
            shuntTree(combined)->execute(0, 1);
        }
    }
    return 0;
}
