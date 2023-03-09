/*!
*
*	@file httpsRequests.cpp
*	@brief
*	@author Jan Wolzenburg
*	@date 09.03.2023
*
*/


/*******************************************************************
 * Includes
********************************************************************/

#include <string>
using std::string;

#include <exception>
using std::exception;


#include "httpsRequests.h"
#include "curl/curl.h"



/*******************************************************************
 * Implementations
********************************************************************/

// Initialise flag to false
bool httpsRequest::CURL_INITIALISED{ false };


size_t httpsRequest::write_callback( const char* const ptr, const size_t size, const size_t nmemb, void* const userdata ) {

    // Typecast argument
    vector<char>* data = (vector<char>*) userdata;

    // Iterate through data pointer ptr
    size_t i = 0;

    // Move bytes to vector
    try{
        for (; i < size * nmemb; i++) { data->push_back( ptr[ i ] ); }
    }
    catch(...){
        return i;
    }

    return i;
}


httpsRequest::httpsRequest( const vector<string> header_v, const SSL_TYPE type ) : 
    success( false ),
    curlHandle( nullptr ),
    header( nullptr )
{

    // Initialise CURL
	if( !CURL_INITIALISED ){
		CURL_INITIALISED = true;
		curl_global_init( CURL_GLOBAL_ALL );
	}

	curlHandle = curl_easy_init();
	if (curlHandle == NULL) success = false;

    // Set options for SSL verification
    switch( type ){
        case NONE:
            if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, 0L ) != CURLE_OK) success = false;
	        if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYHOST, 0L ) != CURLE_OK) success = false;    
            break;

        case VERIFY_PEER:
            if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, 1L ) != CURLE_OK) success = false;
	        if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYHOST, 0L ) != CURLE_OK) success = false;    
            break;

         case VERIFY_HOST:
            if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, 0L ) != CURLE_OK) success = false;
	        if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYHOST, 1L ) != CURLE_OK) success = false;    
            break;

         case VERIFY_PEER_HOST:
            if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, 1L ) != CURLE_OK) success = false;
	        if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYHOST, 1L ) != CURLE_OK) success = false;    
            break;
    }


	// Build curl header
	for (auto const& s : header_v) {
		header = curl_slist_append( header, s.c_str() );
		if (header == NULL) success = false;
	}


	// Build header from list
	if (curl_easy_setopt( curlHandle, CURLOPT_HTTPHEADER, header ) != CURLE_OK) success = false;

	// Set callback function for response
	if (curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, write_callback ) != CURLE_OK) success = false;

	// Set respone as data destination
	if (curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, (void*) &response ) != CURLE_OK) success = false;

}


void httpsRequest::performRequest( void ){

	// Perform request
	CURLcode res = curl_easy_perform( curlHandle );
	if (res != CURLE_OK) success = false;

	// Cleanup
	curl_easy_cleanup( curlHandle );
	curl_slist_free_all( header );

    success = true;

}


httpsRequest::httpsRequest( const string url, const vector<string> header_v, const string data, const SSL_TYPE sslType ) : 
    httpsRequest{ header_v, sslType }
{

	// URL
	if (curl_easy_setopt( curlHandle, CURLOPT_URL, url.c_str() ) != CURLE_OK) success = false;
	// JSON data
	if (curl_easy_setopt( curlHandle, CURLOPT_POSTFIELDS, data.c_str() ) != CURLE_OK) success = false;

    performRequest();
}


httpsRequest::httpsRequest( const string url, const vector<string> header_v, const vector<array<string, 2>> getData, const SSL_TYPE sslType )  : 
    httpsRequest{ header_v, sslType }
{

	// URL
	string urlWithGetData{ url };
    if( getData.size() > 0 ) urlWithGetData += "?";				// Append "?"

	// Iteratore all name-value pairs in GET request
	for (vector<array<string, 2>>::const_iterator it = getData.cbegin(); it < getData.cend(); it++) {
		urlWithGetData += it->at( 0 ) + "=" + it->at( 1 );		// Apped name and value
		if( it < getData.cend() - 1) urlWithGetData += "&";		// Append "&" for next name value pair
	}
	
    // Set URL with GET arguments
	if (curl_easy_setopt( curlHandle, CURLOPT_URL, urlWithGetData.c_str() ) != CURLE_OK) success = false;

    
    performRequest();

}


string httpsRequest::getResponseString( void ) const {
    // Conver char-vector to string
    return string{ response.cbegin(), response.cend() };
}