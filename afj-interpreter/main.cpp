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
#include "BoundedIndex.hpp"

using namespace std;

const char* VERSION="0.0.2";

typedef basic_string<unsigned char> ustring;

void showHelp (char *s);
bool bracketsPairCheck(string &source_code);
bool validateArguments (string &input_file, string &stream, string &stream_file);
bool fileExists (const string &file_name);
string normalizeInputStream(string &stream, const string &stream_file);
string normalizeSource(const string &source_file );
string readFile(const string &file_name);

string runInterpreter(const string &source_code, const string &input_stream);

ustring convert(const string &string);

int main (int argc, char *argv[])
{
    char get_opt;

    string source_file, source_code, stream_file, stream, output_stream;

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
    source_code = normalizeSource(source_file);

    cout << bracketsPairCheck(source_code) << endl;

    //output_stream = runInterpreter(source_code, stream);

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

string runInterpreter(const string &source_code, const string &input_stream)
{
    //int index_input_stream, index_source_code, index_turing_stream = 0;

    BoundedIndex index(0, 100);
    index.debug();
    index++;
    //cout << index << endl;
//    index.setAll(0, 0, 10);
//    for (int i = 0; i < 25; i++) {
//        cout << index.getCurrent() << endl;
//        index++;
//    }
//
//    cout << "minusing" << endl;
//    index = 0;
//    for (int i = 0; i < 25; i++) {
//        cout << index.getCurrent() << endl;
//        index--;
//    }
    //cout << index.getCurrent() << endl;

    //ustring turing_stream = ustring(100000u, '0');
    //ustring u_input_stream = convert(input_stream);


    return "";

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

ustring convert(const string &string)
{
    return ustring(string.begin(), string.end());
}

const char *match(const char *str)
{
    if( *str == '\0' || *str == ']' ) { return str; }
    if( *str == '[' )
    {
        const char *closer = match(++str);
        if( *closer == ']' )
        {
            return match(++closer);
        }
        return str - 1;
    }

    return match(++str);
}

bool bracketsPairCheck(string &source_code)
{
    //const char *result = match(source_code.c_str());
    if (*match(source_code.c_str()) == '\0') return true;
    return false;
}