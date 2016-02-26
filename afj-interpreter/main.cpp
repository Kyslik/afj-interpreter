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
#include <iomanip>

using namespace std;

const char* VERSION = "1.0.2";
const char DEFAULT_NULL = NULL;

typedef basic_string<unsigned char> ustring;

void showHelp (char *s);
void printStreamAsHex(const string &string);
void printStreamAsHex(const ustring &ustring);
void printStreamAsString(const string &string);
void printStreamAsString(const ustring &ustring);

bool bracketsPairCheck(string &source_code);
bool validateArguments (string &input_file, string &stream, string &stream_file);
bool fileExists (const string &file_name);

string normalizeInputStream(string &stream, const string &stream_file);
string normalizeSource(const string &source_code );
string readFile(const string &file_name, bool skip_white_space = true);
string readBinaryFile(const string &file_name);

ustring runInterpreter(string &source_code, const string &input_stream);
ustring convert(const string &string);

int main (int argc, char *argv[])
{
    char get_opt;

    string source_code, stream_file, stream;

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
                return 0;
            case 'i':
                source_code = strdup(optarg);
                break;
            case 's':
                stream = strdup(optarg);
                break;
            case 'f':
                stream_file = strdup(optarg);
                break;
            default:
                showHelp(argv[0]);
                break;
        }
    }

    if (validateArguments(source_code, stream, stream_file))
    {
        return 1;
    }

    stream = normalizeInputStream(stream, stream_file);
    source_code = normalizeSource(source_code);

    if (bracketsPairCheck(source_code))
    {
        const ustring output = runInterpreter(source_code, stream);
        //printStreamAsHex(output);
        printStreamAsString(output);
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
    cout << "         " << "-f  file with input stream (first line read only)" << endl;
    cout << "example: " << s << " -i source.afj -s \"hello world!\"" << endl;
    cout << "example: " << s << " -i source.afj -f stream.byte" << endl;
}

bool fileExists (const string& file_name)
{
    struct stat buffer;
    return (stat (file_name.c_str(), &buffer) == 0);
}

bool validateArguments (string &source_code, string &stream, string &stream_file)
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

    if (stream.empty() && stream_file.empty())
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

string::iterator bracketMatchOpen(const string &source_code, string::iterator it)
{
    int brackets = 0;

    for (;it != source_code.end(); ++it)
    {
        if (*it == ']' && brackets == 1)
            return it;
        else if (*it == ']' && brackets != 1)
            brackets--;

        if (*it == '[') brackets++;
    }
    return it;
}

string::iterator bracketMatchClose(const string &source_code, string::iterator it)
{
    int brackets = 0;
    for (;it != source_code.begin(); --it)
    {
        if (*it == '[' && brackets == 1)
            return it;
        else if (*it == '[' && brackets != 1)
            brackets--;

        if (*it == ']') brackets++;
    }
    return it;
}

ustring runInterpreter(string &source_code, const string &input_stream)
{
    const int input_stream_size = input_stream.length() & numeric_limits<int>::max();
    const int data_size = 100000;

    BoundedIndex data_index(0, data_size - 1),
                 input_index(0, input_stream_size);

    ustring u_data_stream = ustring(data_size, DEFAULT_NULL),
            u_input_stream = convert(input_stream),
            u_output_stream;

    int i = 0;

    for(string::iterator it = source_code.begin(); it != source_code.end(); ++it)
    {
        //cout << "0x" << hex << setfill('0') << setw(2) << uppercase << (unsigned int)u_data_stream[data_index.curr()] << "\t";
        switch (*it) {
            case '<':
                data_index--;
                break;
            case '>':
                data_index++;
                break;
            case 'R':
                if (u_input_stream.empty() || input_index.isNextIncrementOverflowing()) {
                    u_data_stream[data_index.curr()] = DEFAULT_NULL;
                } else {
                    u_data_stream[data_index.curr()] = u_input_stream[input_index.curr()];
                    input_index++;
                }
                break;
            case 'W':
                u_output_stream += (u_data_stream[data_index.curr()]);
                break;
            case '+':
                u_data_stream[data_index.curr()]++;
                break;
            case '-':
                u_data_stream[data_index.curr()]--;
                break;
            case 'N':
                u_data_stream[data_index.curr()] = NULL;
                break;
            case '!':
                u_data_stream[data_index.curr()] = ~u_data_stream[data_index.curr()];
                break;
            case '[':
                if (u_data_stream[data_index.curr()] == DEFAULT_NULL)
                    it = bracketMatchOpen(source_code, it);
                break;
            case ']':
                if (u_data_stream[data_index.curr()] != DEFAULT_NULL)
                    it = bracketMatchClose(source_code, it);
                break;
            default:
                break;
        }
        i++;
    }
    return u_output_stream;

}

string normalizeInputStream(string &stream, const string &stream_file)
{
    if (stream.empty())
    {
        return readBinaryFile(stream_file);
    }
    return stream;
}

string normalizeSource(const string &source_code)
{
    return readFile(source_code);
}

string readFile(const string &file_name, bool skip_white_space)
{
    string stream;
    char ch;

    fstream fin(file_name, fstream::in);
    while(fin >> ((skip_white_space) ? skipws : noskipws) >> ch)
    {
        stream.append(&ch);
    }

    return stream;
}

string readBinaryFile(const string &file_name)
{
    string stream;
    ifstream fin(file_name, ios::binary);
    while (fin)
    {
        stream+=(char) fin.get();

    }
    stream.pop_back();
    return stream;
}

ustring convert(const string &string)
{
    return ustring(string.begin(), string.end());
}

void printStreamAsHex(const string &string)
{
    const ustring ustring = convert(string);
    printStreamAsHex(ustring);
}

void printStreamAsHex(const ustring &ustring)
{
    if (!ustring.empty()) {
        cout << "Printing stream as hex (each byte prefixed with \"0x\"):" << endl;
        for (int i = 0; i < ustring.length(); i++) {
            cout << "0x" << hex << setfill('0') << setw(2) << uppercase << (unsigned int)ustring[i] << "\t";
        }
        cout << endl;
    }
    else
    {
        cout << "Stream is empty, nothing to print." << endl;
    }
}

void printStreamAsString(const string &string)
{
    const ustring ustring = convert(string);
    printStreamAsString(ustring);
}

void printStreamAsString(const ustring &ustring)
{
    if (!ustring.empty()) {
        cout << "Printing stream as string: " << endl;
        for (int i = 0; i < ustring.length(); i++) {
            cout << ustring[i];
        }
        cout << endl;
    }
    else
    {
        cout << "Stream is empty, nothing to print." << endl;
    }

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
    if (*match(source_code.c_str()) == '\0') return true;
    return false;
}