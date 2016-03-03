//
//  main.cpp
//  afj-interpreter
//
//  Created by Martin Kiesel on 17/02/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <sys/stat.h>
#include <unistd.h>
#include "lib/Interpreter/Interpreter.cpp"
#include <iomanip>

using namespace std;

const char* VERSION = "1.0.5";

void showHelp (char *s);

bool validateArguments (string &input_file, const string &input_stream, const string &stream_file, const string &out_option);
bool fileExists (const string &file_name);

int main (int argc, char *argv[])
{
    char get_opt;

    string source_code, stream_file, input_stream, out_option;

    if(argc == 1)
    {
        showHelp(argv[0]);
        return 1;
    }

    while((get_opt = getopt(argc, argv, "hvi:s:f:o:")) != -1)
    {
        switch(get_opt)
        {
            case 'h':
                showHelp(argv[0]);
                return 0;
            case 'v':
                cout << "Current version is " << VERSION << endl;
                return 0;
            case 'i':
                source_code = strdup(optarg);
                break;
            case 's':
                input_stream = strdup(optarg);
                break;
            case 'f':
                stream_file = strdup(optarg);
                break;
            case 'o':
                out_option = strdup(optarg);
                break;
            default:
                showHelp(argv[0]);
                break;
        }
    }

    if (validateArguments(source_code, input_stream, stream_file, out_option))
    {
        return 1;
    }

    Interpreter Interpreter(source_code, input_stream, stream_file);

    if (Interpreter.bracketsPairCheck())
    {
        Interpreter.run();
        Interpreter.printHex();
    }
    else
    {
        cout << "Source code syntax error: brackets do not match." << endl;
    }

    return 0;
}

void showHelp(char *s)
{
    cout << "Usage:   " << s << " [-option] [argument]" << endl;
    cout << "option:  " << "-h  show help information" << endl;
    cout << "         " << "-v  show version infomation" << endl;
    cout << "         " << "-i  input file (source file)" << endl;
    cout << "         " << "-s  byte stream enclosed in \" ex: \"myStream\"" << endl;
    cout << "         " << "-f  file with input stream (binary data)" << endl;
    cout << "         " << "-o  hex | str | hexstr | \"file name\" where output is saved as binary data" << endl;
    cout << endl;
    cout << "example: " << s << " -i source.afj -s \"hello world!\" -o hex" << endl;
    cout << "example: " << s << " -i source.afj -f stream.bin -o hexstr" << endl;
    cout << "example: " << s << " -i source.afj -o myoutfile.bin" << endl;
}

inline bool fileExists (const string& file_name)
{
    struct stat buffer;
    return (stat (file_name.c_str(), &buffer) == 0);
}

bool validateArguments (string &source_code, const string &input_stream, const string &stream_file, const string &out_option)
{
    if (source_code.empty())
    {
        cerr << "Input file (source file) not specified. Please use -i option. \nTrying \"source.afj\"" << endl;
        source_code = "source.afj";
    }
    else if (!fileExists(source_code))
    {
        cerr << "File \"" << source_code << "\" does not exists." << endl;
        return true;
    }

    if (input_stream.empty() && stream_file.empty())
    {
        cout << "Input stream (or stream file) not defined by user, please use either -s or -f option. \nUsing '0x00' (NULL) for all R instructions." << endl;
    }

    if (!stream_file.empty())
    {
        if(!fileExists(stream_file))
        {
            cerr << "File \"" << stream_file << "\" does not exists." << endl;
            return true;
        }
    }
    return false;
}