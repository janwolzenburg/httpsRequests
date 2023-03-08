

#include <string>
using std::string;

#include "httpsRequests.h"


int main( int argc, char* argv[] ) {

    const string url{ "https://google.com" };
    const string postData{ "data" };
    const vector<string> header{ "" };

    request_POST( url, header, postData );


    return 0;
}