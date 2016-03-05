//
//  Interpreter.cpp
//  afj-interpreter
//
//  Created by Martin Kiesel on 26/02/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include "Interpreter.hpp"

void Interpreter::run()
{
    const int input_stream_size = input_stream.length() & numeric_limits<int>::max();
    const int data_size = 100000;

    BoundedIndex data_index(0, data_size - 1),
    input_index(0, input_stream_size);

    const ustring
    u_input_stream = convert(input_stream);

    ustring u_data_stream = ustring(data_size, DEFAULT_NULL);

    for(string::iterator it = source_code.begin(); it != source_code.end(); ++it)
    {
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
    }
}

void Interpreter::print()
{
    if (!u_output_stream.empty())
    {
        if (print_option.empty()) return;
        if (!print_option.compare("str"))
        {
            printStreamAsString(u_output_stream);
        }
        else if (!print_option.compare("hexstr"))
        {
            printStreamAsHex(u_output_stream);
            printStreamAsString(u_output_stream);
        }
        else if (!print_option.compare("strhex"))
        {
            printStreamAsString(u_output_stream);
            printStreamAsHex(u_output_stream);
        }
        else
        {
            printStreamAsHex(u_output_stream);
        }
    }
    else
    {
        cout << "(out) Stream is empty, nothing to print." << endl;
    }
}

void Interpreter::printHex()
{
    printStreamAsHex(u_output_stream);
}

void Interpreter::printString()
{
    printStreamAsString(u_output_stream);
}

bool Interpreter::bracketsPairCheck()
{
    if (*match(source_code.c_str()) == '\0') return true;
    cout << "Source code syntax error: brackets do not match." << endl;
    return false;
}

string::iterator Interpreter::bracketMatchOpen(const string &source_code, string::iterator it)
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

string::iterator Interpreter::bracketMatchClose(const string &source_code, string::iterator it)
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

void Interpreter::printStreamAsHex(const ustring &ustring)
{
    if (!ustring.empty()) {
        cout << "Printing (output) stream as hex (each byte prefixed with \"0x\"):" << endl;
        for (int i = 0; i < ustring.length(); i++) {
            cout << "0x" << hex << setfill('0') << setw(2) << uppercase << (unsigned int)ustring[i] << "\t";
        }
        cout << endl;
    }
}


void Interpreter::printStreamAsString(const ustring &ustring)
{
    if (!ustring.empty()) {
        cout << "Printing (output) stream as string: " << endl;
        for (int i = 0; i < ustring.length(); i++) {
            cout << ustring[i];
        }
        cout << endl;
    }
}

string Interpreter::readFile(const string &file_name, bool skip_white_space)
{
    string input_stream;
    char ch;

    fstream fin(file_name, fstream::in);
    while(fin >> ((skip_white_space) ? skipws : noskipws) >> ch)
    {
        input_stream.append(&ch);
    }

    return input_stream;
}

string Interpreter::readBinaryFile(const string &file_name)
{
    string in_stream;
    ifstream fin(file_name, ios::binary);
    while (fin)
    {
        in_stream+=(char) fin.get();
    }
    in_stream.pop_back();
    return in_stream;
}

void Interpreter::writeBinaryFile(const string &file_name)
{
    if (!file_name.empty())
    {
        if (u_output_stream.empty())
        {
            cout << "(out) Stream is empty, nothing to write to file." << endl;
            return;
        }

        ofstream fo(file_name, ios::binary);

        if (fo.is_open())
        {
            fo.write ((char *) u_output_stream.data(), u_output_stream.length());
            fo.close();
        }
    }
}