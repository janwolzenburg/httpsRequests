#include <string>
using std::string;

#include <iostream>

#include "httpsRequests.h"


int main( int argc, char* argv[] ) {

    const string url{ "https://www.google.com" };
    HTTPSRequest req{ url, vector<string>{}, vector<pair<string, string>>{}, HTTPSRequest::SSL_Type::NONE };

    std::cout << req.GetResponseString();

    return 0;
}