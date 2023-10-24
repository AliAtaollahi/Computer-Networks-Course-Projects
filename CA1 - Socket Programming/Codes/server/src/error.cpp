// In the Name of God

#include <iostream>
#include <string>
#include "error.hpp"
#include "manual.hpp"

using namespace std;

Error::Error (int error_number , string extra_info)
    :
    error_number(error_number),
    extra_info(extra_info)
{
}

string Error::what(bool more_info)
{
    string red_color = KRED;
    string error_msg = red_color + "Error " + to_string(error_number) + 
                    ": " + Info::status[error_number] + KWHT;

    return more_info ? error_msg + " -- " + extra_info : error_msg;
}

Message::Message(string text_)
    :
    text(text_)
{
}

string Message::what()
{
    return text;
}
