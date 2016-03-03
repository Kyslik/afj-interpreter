//
//  Interpreter.hpp
//  afj-interpreter
//
//  Created by Martin Kiesel on 26/02/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#ifndef Interpreter_hpp
#define Interpreter_hpp

#include <iostream>
#include <fstream>
#include <unistd.h>
#include "../../lib/BoundedIndex/BoundedIndex.cpp"
#include <iomanip>

using namespace std;

const char DEFAULT_NULL = NULL;

typedef basic_string<unsigned char> ustring;

class Interpreter
{
private:
    string source_code, input_stream;
    ustring u_output_stream;

public:
    Interpreter(string _source_code, string _input_stream, string _stream_file)
    {
        source_code = normalizeSource(_source_code);
        input_stream = normalizeInputStream(_input_stream, _stream_file);
    };

    void run();
    void printHex();
    void printString();
    bool bracketsPairCheck();
    
private:
    inline ustring convert(const string &string)
    {
        return ustring(string.begin(), string.end());
    }

    inline void printStreamAsHex(const string &string)
    {
        const ustring ustring = convert(string);
        printStreamAsHex(ustring);
    }

    inline void printStreamAsString(const string &string)
    {
        const ustring ustring = convert(string);
        printStreamAsString(ustring);
    }

    inline string normalizeInputStream(string &input_stream, const string &stream_file)
    {
        if (input_stream.empty())
            return readBinaryFile(stream_file);
        return input_stream;
    }

    inline string normalizeSource(const string &source_code)
    {
        return readFile(source_code, false);
    }

    string::iterator bracketMatchOpen(const string &source_code, string::iterator it);
    string::iterator bracketMatchClose(const string &source_code, string::iterator it);

    string readFile(const string &file_name, bool skip_white_space);
    string readBinaryFile(const string &file_name);
    string writeBinaryFile(const string &file_name);
    
    void printStreamAsHex(const ustring &ustring);
    void printStreamAsString(const ustring &ustring);

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
};

#endif /* Interpreter_hpp */