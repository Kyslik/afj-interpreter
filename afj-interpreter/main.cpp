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

using namespace std;

const char* VERSION="0.0.1";

typedef basic_string<unsigned char> ustring;

void showHelp (char *s);
bool validateArguments (string &input_file, string &stream, string &stream_file);
bool fileExists (const string &file_name);
string normalizeInputStream(string &stream, const string &stream_file);
string normalizeSource(const string &source_file );
string readFile(const string &file_name);

string runInterpreter(const string &source_code, const string &input_stream);

ustring convert(const string &string);

class BoundedIndex {
    int current, min, max;
public:
    void setMinMax (int,int);
    void setCurrent(int);
    void setAll(int, int, int);
    int getCurrent () {return current;}
    void operator++(int)
    {
        if (current == max) {
            current = min;
        }
        else
        {
            current++;
        }
    }

    void operator--(int)
    {
        if (current == min) {
            current = max;
        }
        else
        {
            current--;
        }
    }

    void operator=(int i) {current = i;}
};

void BoundedIndex::setMinMax(int _min, int _max)
{
    min = _min;
    max = _max;
}

void BoundedIndex::setCurrent(int _current)
{
    current = _current;
}

void BoundedIndex::setAll(int _current, int _min, int _max)
{
    current = _current;
    min = _min;
    max = _max;
}


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

    output_stream = runInterpreter(source_code, stream);

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
    int index_input_stream, index_source_code, index_turing_stream = 0;

    BoundedIndex index;
    index.setAll(0, 0, 10);
    for (int i = 0; i < 25; i++) {
        cout << index.getCurrent() << endl;
        index++;
    }

    cout << "minusing" << endl;
    index = 0;
    for (int i = 0; i < 25; i++) {
        cout << index.getCurrent() << endl;
        index--;
    }
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