#include <string>
using std::string;

#include <iostream>

#include "httpsRequests.h"


int main( int argc, char* argv[] ) {

    const string url{ "https://www.google.com" };
    httpsRequest req{ url, vector<string>{}, vector<array<string, 2>> {}, httpsRequest::SSL_TYPE::NONE };

    std::cout << req.getResponseString();

    return 0;
}