/*!
*
*	@file httpsRequests.cpp
*	@author Jan Wolzenburg
*	@date 24.04.2023
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
bool HTTPSRequest::CURL_INITIALISED{ false };


size_t HTTPSRequest::write_callback( const char* const ptr, const size_t size, const size_t nmemb, void* const userdata ) {

    // Typecast argument
    vector<char>* data = static_cast<vector<char>*>( userdata );

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


HTTPSRequest::HTTPSRequest( const vector<string> header_v, const SSL_Type type ) : 
    request_success_( false ),
    curl_handle_( nullptr ),
    curl_header_( nullptr )
{

    // Initialise CURL
	if( !CURL_INITIALISED ){
		CURL_INITIALISED = true;
		curl_global_init( CURL_GLOBAL_ALL );
	}

	curl_handle_ = curl_easy_init();
	if (curl_handle_ == NULL) request_success_ = false;

    // Set options for SSL verification
    switch( type ){
        case NONE:
            if (curl_easy_setopt( curl_handle_, CURLOPT_SSL_VERIFYPEER, 0L ) != CURLE_OK) request_success_ = false;
	        if (curl_easy_setopt( curl_handle_, CURLOPT_SSL_VERIFYHOST, 0L ) != CURLE_OK) request_success_ = false;    
            break;

        case VERIFY_PEER:
            if (curl_easy_setopt( curl_handle_, CURLOPT_SSL_VERIFYPEER, 1L ) != CURLE_OK) request_success_ = false;
	        if (curl_easy_setopt( curl_handle_, CURLOPT_SSL_VERIFYHOST, 0L ) != CURLE_OK) request_success_ = false;    
            break;

         case VERIFY_HOST:
            if (curl_easy_setopt( curl_handle_, CURLOPT_SSL_VERIFYPEER, 0L ) != CURLE_OK) request_success_ = false;
	        if (curl_easy_setopt( curl_handle_, CURLOPT_SSL_VERIFYHOST, 1L ) != CURLE_OK) request_success_ = false;    
            break;

         case VERIFY_PEER_HOST:
            if (curl_easy_setopt( curl_handle_, CURLOPT_SSL_VERIFYPEER, 1L ) != CURLE_OK) request_success_ = false;
	        if (curl_easy_setopt( curl_handle_, CURLOPT_SSL_VERIFYHOST, 1L ) != CURLE_OK) request_success_ = false;    
            break;
    }


	// Build curl header
	for (auto const& s : header_v) {
		curl_header_ = curl_slist_append( curl_header_, s.c_str() );
		if (curl_header_ == NULL) request_success_ = false;
	}


	// Build header from list
	if (curl_easy_setopt( curl_handle_, CURLOPT_HTTPHEADER, curl_header_ ) != CURLE_OK) request_success_ = false;

	// Set callback function for response
	if (curl_easy_setopt( curl_handle_, CURLOPT_WRITEFUNCTION, write_callback ) != CURLE_OK) request_success_ = false;

	// Set respone as data destination
	if (curl_easy_setopt( curl_handle_, CURLOPT_WRITEDATA, (void*) &response ) != CURLE_OK) request_success_ = false;

}

HTTPSRequest::~HTTPSRequest( void ){
    if( curl_handle_ != nullptr )
        curl_easy_cleanup( curl_handle_ );
	
    if( curl_header_ != nullptr )
        curl_slist_free_all( curl_header_ );
}

void HTTPSRequest::PerformRequest( void ){

	// Perform request
	CURLcode res = curl_easy_perform( curl_handle_ );
	if (res != CURLE_OK) request_success_ = false;

	// Cleanup
	curl_easy_cleanup( curl_handle_ );
	curl_slist_free_all( curl_header_ );

    curl_handle_ = nullptr;
    curl_header_ = nullptr;

    request_success_ = true;

}


HTTPSRequest::HTTPSRequest( const string url, const vector<string> header_v, const string data, const SSL_Type sslType ) : 
    HTTPSRequest{ header_v, sslType }
{

	// URL
	if (curl_easy_setopt( curl_handle_, CURLOPT_URL, url.c_str() ) != CURLE_OK) request_success_ = false;
	// JSON data
	if (curl_easy_setopt( curl_handle_, CURLOPT_POSTFIELDS, data.c_str() ) != CURLE_OK) request_success_ = false;

    PerformRequest();
}


HTTPSRequest::HTTPSRequest( const string url, const vector<string> header_v, const vector<pair<string, string>> getData, const SSL_Type sslType )  : 
    HTTPSRequest{ header_v, sslType }
{

	// URL
	string urlWithGetData{ url };
    if( getData.size() > 0 ) urlWithGetData += "?";				// Append "?"

	// Iteratore all name-value pairs in GET request
	for ( auto it = getData.cbegin(); it < getData.cend(); it++) {
		urlWithGetData += it->first + "=" + it->second;		    // Apped name and value
		if( it < getData.cend() - 1) urlWithGetData += "&";		// Append "&" for next name value pair
	}
	
    // Set URL with GET arguments
	if (curl_easy_setopt( curl_handle_, CURLOPT_URL, urlWithGetData.c_str() ) != CURLE_OK) request_success_ = false;

    
    PerformRequest();

}


string HTTPSRequest::GetResponseString( void ) const {
    // Conver char-vector to string
    return string{ response.cbegin(), response.cend() };
}