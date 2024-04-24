#include <string>
using std::string;

#include <iostream>

#include "httpsRequests.h"


int main( int argc, char* argv[] ) {

    // API URL
    const string url{ "https://app.divera247.com/api/v2/auth/login" };

    // Only accept json responens
    const vector<string> header{ "accept: application/json",
								 "Content-Type: application/json" };

    // Request data
    const string postData{ "{ \"Login\": { \"username\": \"heinz.meinz@musterstadt.de\", \"password\": \"string\", \"jwt\": false } }" };


    HTTPSRequest req{ url, header, postData, HTTPSRequest::SSL_Type::VERIFY_HOST };

    std::cout << req.GetResponseString();

    return 0;
}