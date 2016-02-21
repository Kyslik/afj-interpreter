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

const char* VERSION="0.0.1";

using namespace std;

void showHelp (char *s);
bool validateArguments (string &input_file, string &stream, string &stream_file);
bool fileExists (const string &file_name);
string normalizeInputStream(string &stream, const string &stream_file);
string normalizeSource(const string &source_file );
string readFile(const string &file_name);
//void readFile(string &file_name);


int main (int argc, char *argv[])
{
    char get_opt;
    string source_file, source, stream_file, stream;

    if(argc == 1)
    {
        showHelp(argv[0]);
        return 1;
    }

    while((get_opt = getopt(argc, argv, "hvi:s:f:")) != -1)
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
                source_file = strdup(optarg);
                break;
            case 's':
                stream = strndup(optarg, strlen(optarg));
                break;
            case 'f':
                stream_file = strndup(optarg, strlen(optarg));
                break;
            default:
                showHelp(argv[0]);
                break;
        }
    }

    if (validateArguments(source_file, stream, stream_file))
    {
        return 1;
    }

    stream = normalizeInputStream(stream, stream_file);
    source = normalizeSource(source_file);
    //stream = "0";
    //stream[0]++;

    cout << stream << endl;

    //readFile(source_file);

    return 0;
}

void showHelp(char *s)
{
    cout << "Usage:   " << s << " [-option] [argument]" << endl;
    cout << "option:  " << "-h  show help information" << endl;
    cout << "         " << "-v  show version infomation" << endl;
    cout << "         " << "-i  input file (source file)" << endl;
    cout << "         " << "-s  byte stream enclosed in \" ex: \"myStream\"" << endl;
    cout << "         " << "-f  file with input stream (first line read only)" << endl;
    cout << "example: " << s << " -i source.afj -s \"hello world!\"" << endl;
    cout << "example: " << s << " -i source.afj -f stream.byte" << endl;
}

bool fileExists (const string& file_name)
{
    struct stat buffer;
    return (stat (file_name.c_str(), &buffer) == 0);
}

bool validateArguments (string &input_file, string &stream, string &stream_file)
{
    bool error = false;

    if (!fileExists(input_file))
    {
        cerr << "File \"" << input_file << "\" does not exists." << endl;
        error = true;
    }

    if (stream.empty() && stream_file.empty())
    {
        cout << "Input stream (or stream file) not defined by user. Use either -s or -f option." << endl;
        error = true;
    }

    if (!stream_file.empty())
    {
        if(!fileExists(stream_file))
        {
            cerr << "File \"" << stream_file << "\" does not exists." << endl;
            error = true;
        }
    }

    return error;
}

string normalizeInputStream(string &stream, const string &stream_file)
{
    if (stream.empty())
    {
        return readFile(stream_file);
    }
    return stream;
}

string normalizeSource(const string &source_file)
{
    return readFile(source_file);
}

string readFile(const string &file_name)
{
    string stream;
    char ch;

    fstream fin(file_name, fstream::in);
    while(fin >> skipws >> ch)
    {
        stream.append(&ch);
    }
    return stream;
}

//void readFile(string &file_name)
//{
//    char ch;
//    fstream fin(file_name.c_str(), fstream::in);
//
//    while (fin >> skipws >> ch)
//    {
//        cout << ch;
//    }
//    cout << endl;
//}











