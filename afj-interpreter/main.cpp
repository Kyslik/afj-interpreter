//
//  main.cpp
//  afj-interpreter
//
//  Created by Martin Kiesel on 17/02/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include <iostream>
#include <sys/stat.h>
#include "lib/Interpreter/Interpreter.cpp"

using namespace std;

const char* VERSION = "2.0.0";

void showHelp (char *s);
void showExamples (char *s);

bool validateArguments (string &input_file, const string &input_stream, const string &stream_file, string *out_file, bool *overwrite_out_file, const string &print_option);
bool fileExists (const string &file_name);

int main (int argc, char *argv[])
{
    char get_opt;

    string source_code, stream_file, input_stream, print_option, out_file;
    bool overwrite_out_file = false;

    while((get_opt = getopt(argc, argv, "hvei:s:f:p:o:x")) != -1)
    {
        switch(get_opt)
        {
            case 'h':
                showHelp(argv[0]);
                return 0;
            case 'v':
                cout << "Current version is " << VERSION << endl;
                return 0;
            case 'e':
                showExamples(argv[0]);
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
            case 'p':
                print_option = strdup(optarg);
                break;
            case 'o':
                out_file = strdup(optarg);
                break;
            case 'x':
                overwrite_out_file = true;
                break;
            default:
                showHelp(argv[0]);
                break;
        }
    }

    if (!validateArguments(source_code, input_stream, stream_file, &out_file, &overwrite_out_file, print_option))
    {
        return 1;
    }

    Interpreter Interpreter(source_code, input_stream, stream_file, out_file, print_option);

    if (Interpreter.bracketsPairCheck())
    {
        Interpreter.run();
        Interpreter.writeOutputToFile();
        Interpreter.print();
    }

    return 0;
}

void showHelp(char *s)
{
    cout << "Usage:   " << s << " [-option] [argument]" << endl;
    cout << "option:  " << "-h  show help" << endl;
    cout << "         " << "-v  show version infomation" << endl;
    cout << "         " << "-e  show examples" << endl;
    cout << "         " << "-i  input file (source code file) / default \"source.afj\"" << endl;
    cout << "         " << "-s  byte stream enclosed in \" ex: \"myStream\"" << endl;
    cout << "         " << "-f  file with input stream (binary file)" << endl;
    cout << "         " << "-p  hex (default) | str | hexstr | strhex / print to console" << endl;
    cout << "         " << "-o  \"file name\" where output is saved as binary data" << endl;
    cout << "         " << "-x  do not ask to overwrite output file (DOES OVERWRITE FILE)" << endl;
    cout << endl;
    cout << "example: " << s << endl;
    cout << "example: " << s << " -i source.afj -s \"hello world!\" -p hex" << endl;
    cout << "example: " << s << " -i source.afj -f stream.bin -p hexstr" << endl;
    cout << "example: " << s << " -i source.afj -o myoutfile.bin" << endl;
    cout << "See more examples using -e option." << endl;
}

void showExamples(char *s)
{
    cout << "Lets assume source code is saved in \"./source.afj\", our input stream of bytes is in binary file \"./stream.bin\" and file we write to is \"out.bin\"." << endl;
    cout << "Showing examples for " << s << " program:" << endl;
    cout << s << " -i source.afj -s \"ABCDE\" -o out.bin" << endl;
    cout << s << " -i source.afj -s \"ABCDE\" -o out.bin -x" << endl;
    cout << s << " -i source.afj -f stream.bin -o out.bin -x" << endl;
    cout << s << " -i source.afj -f stream.bin -p hexstr" << endl;
    cout << s << " -i source.afj -f stream.bin -p str -o out.bin -x" << endl;

}

inline bool fileExists (const string& file_name)
{
    struct stat buffer;
    return (stat (file_name.c_str(), &buffer) == 0);
}

bool validateArguments (string &source_code,
                        const string &input_stream,
                        const string &stream_file,
                        string *out_file,
                        bool *overwrite_out_file,
                        const string &print_option)
{
    string _out_file = *out_file;

    if (source_code.empty())
    {
        cerr << "Input file (source file) not specified. Please use -i option... trying \"source.afj\"." << endl;
        source_code = "source.afj";
    }

    if (!fileExists(source_code))
    {
        cerr << "File \"" << source_code << "\" does not exist." << endl;
        return false;
    }

    if (input_stream.empty() && stream_file.empty())
    {
        cout << "Input stream of bytes (or file) not defined by user, please use either -s or -f option. \nUsing '0x00' (NULL) for all \"R\" instructions." << endl;
    }

    if (!stream_file.empty() && !fileExists(stream_file))
    {
        cerr << "File \"" << stream_file << "\" does not exist." << endl;
        return false;
    }
    
    if (!_out_file.empty() && !*overwrite_out_file && fileExists(_out_file))
    {
        char yes_no = NULL;

        cout << "File \"" << _out_file << "\" already exist (use option -x to make this question disappear in future), do you want to overwrite?" << endl;

        do
        {
            cout << "[y/n]" << endl;
            cin >> yes_no;
        }
        while(!cin.fail() && yes_no != 'y' && yes_no != 'n');

        if (yes_no == 'y')
        {
            *overwrite_out_file = true;
            cout << "File \"" << _out_file << "\" will be overwritten." << endl;
        }
        else
        {
            cout << "Program continues without writing any data to file \"" << _out_file << "\"." << endl;
            *out_file = _out_file.erase();
        }
    }

    if (!print_option.empty() && !(!print_option.compare("hex") || !print_option.compare("str") || !print_option.compare("hexstr")))
    {
        cout << "Printing option is unrecognized, using default printing method (hex)." << endl;
    }

    return true;
}