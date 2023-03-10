# httpsRequest
Simple library for HTTP/HTTPS requests with GET or POST method based on libcurl.


# Prerequisits
## CURL
To use this library you need the curl library. You can build it yourself or <a href = 'https://curl.se/download.html'>download</a> a package.
You will need to modify the CURL_DIR variable in the CMakeLists.txt. Its value must be the path to the downloaded or build curl package.

# Usage
Construct a httpsRequest object. Its parameters a the URL, the header, data and SSL type.
## URL
The URL will not get processed in any way by the library. Only the GET data will be appended.
## Header
The custom header is given by a vector of strings ("key: value").
## Data
Data is either a string which will perform a POST request with the given string as data. When a vector of 2-element string array is given a GET request is performed. Each element of the vector will be appended to the URL.
## SSL Type
Type of SSL. Available types are NONE, VERIFY_PEER, VERIFY_HOST and VERIFY_PEER_HOST. SSL is not tested!

# Examples
There are two examples available. One with a simple request. And another one which requests JSON content with JSON content.
