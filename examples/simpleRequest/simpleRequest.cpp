#include <string>
using std::string;

#include <iostream>

#include "httpsRequests.h"


int main( int argc, char* argv[] ) {

    const string url{ "https://google.com" };
    const string postData{ "data" };
    const vector<string> header{ "" };

    vector<char> response = request_POST( url, header, postData );
    string responseStr { response.begin(), response.end() };

    std::cout << responseStr;

    return 0;
}