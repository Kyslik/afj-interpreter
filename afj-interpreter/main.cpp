//
//  main.cpp
//  afj-interpreter
//
//  Created by Martin Kiesel on 17/02/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

const char* VERSION="0.0.1";

using namespace std;

void showHelp(char *s);
inline bool fileExists (const string& name);
int main(int argc, char *argv[]);

int main (int argc, char *argv[])
{
    char get_opt;

    if(argc == 1)
    {
        showHelp(argv[0]);
        return 1;
    }
    //hvi:s::f::
    while((get_opt = getopt(argc, argv, "hvi:s::f::")) != -1)
    {
        switch(get_opt)
        {
            case 'h':
                showHelp(argv[0]);
                break;
            case 'v':
                cout << "The current version is " << VERSION << endl;
                break;
            case 'i':
                cout << "Input source file is: " << optarg << endl;
                if (fileExists(optarg)) cout << "file found" << endl;
                else cout << "file not found" << endl;
                break;
            case 's':
                cout << "Stream of bytes: " << optarg << endl;
                break;
            case 'f':
                cout << "Filename of file with input stream: " << optarg << endl;
                break;
                /*option v show the version infomation*/
            default:
                showHelp(argv[0]);
                break;
        }
    }
    return 0;
}

/*funcion that show the help information*/
void showHelp(char *s)
{
    cout << "Usage:   " << s << " [-option] [argument]" << endl;
    cout << "option:  " << "-h  show help information" << endl;
    cout << "         " << "-v  show version infomation" << endl;
    cout << "         " << "-i  input file (source file)" << endl;
    cout << "         " << "-s  byte stream enclosed in \"" << endl;
    cout << "         " << "-f  file with input stream (first line read only)" << endl;
    cout << "example: " << s << " -i source.afj -s \"hello world!\"" << endl;
}

inline bool fileExists (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
