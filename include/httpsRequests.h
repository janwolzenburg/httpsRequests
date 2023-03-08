#ifndef HTTPSREQUESTS_H
#define HTTPSREQUESTS_H

/*!
*
*	@file httpsRequests.h
*	@brief
*	@author Jan Wolzenburg
*	@date 08.03.2023
*
*/

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <exception>
using std::exception;


#include "curl/curl.h"

class EXC_CURL : public std::exception {
	public:
	virtual const char* what() { return "CURL: Something went wrong!"; }
};

class CURL_INIT : public EXC_CURL {
	public:
	const char* what() override { return "CURL: Initialisation failed!"; }
};

class CURL_CHAR_WRITE : public EXC_CURL {
	public:
	const char* what() override { return "CURL: Write callback during curl_easy_perform() failed!"; }
};

class CURL_OPT_SET : public EXC_CURL {
	public:
	const char* what() override { return "CURL: Setting options failed!"; }
};

class CURL_PERFORM : public EXC_CURL {
	public:
	const char* what() override { return "CURL: Perform failed!"; }
};

class CURL_OTHER : public EXC_CURL {
	public:
	const char* what() override { return "CURL: Something went wrong!"; }
};

static bool CURL_INITIALISED = false;


static size_t write_callback( char* ptr, size_t size, size_t nmemb, void* userdata ) {

	try {
		// Typecast argument
		vector<char>* data = (vector<char>*) userdata;

		// Iterate through data pointer ptr
		size_t i = 0;

		for (i = 0; i < size * nmemb; i++) { data->push_back( ptr[i] ); }

		return i;
	}
	catch ( ... ) {
		throw CURL_CHAR_WRITE();
	}
}

vector<char> request_POST( const string url, const vector<string> v_header, const string data ) {

	if( !CURL_INITIALISED ){
		CURL_INITIALISED = true;
		curl_global_init( CURL_GLOBAL_ALL );
	}

	// Initialize CURL
	CURL* curlHandle = curl_easy_init();
	if (curlHandle == NULL) throw CURL_INIT();

	// Linked list for header
	struct curl_slist* header = NULL;


	/*
		Set up request
	*/

	// URL
	if (curl_easy_setopt( curlHandle, CURLOPT_URL, url.c_str() ) != CURLE_OK) throw CURL_OPT_SET();

	// Skip SSL verifications
	if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYPEER, 0L ) != CURLE_OK) throw CURL_OPT_SET();
	if (curl_easy_setopt( curlHandle, CURLOPT_SSL_VERIFYHOST, 0L ) != CURLE_OK) throw CURL_OPT_SET();

	// Header
	for (auto const& s : v_header) {
		header = curl_slist_append( header, s.c_str() );
		if (header == NULL) throw CURL_OTHER();
	}

	// Build header from list
	if (curl_easy_setopt( curlHandle, CURLOPT_HTTPHEADER, header ) != CURLE_OK) throw CURL_OPT_SET();

	// JSON data
	if (curl_easy_setopt( curlHandle, CURLOPT_POSTFIELDS, data.c_str() ) != CURLE_OK) throw CURL_OPT_SET();

	// Set callback function for response
	if (curl_easy_setopt( curlHandle, CURLOPT_WRITEFUNCTION, write_callback ) != CURLE_OK) throw CURL_OPT_SET();

	// Set respone as data destination
	vector<char> response;
	if (curl_easy_setopt( curlHandle, CURLOPT_WRITEDATA, (void*) &response ) != CURLE_OK) throw CURL_OPT_SET();

	// Perform request
	CURLcode res = curl_easy_perform( curlHandle );
	if (res != CURLE_OK) throw CURL_PERFORM();

	// Cleanup
	curl_easy_cleanup( curlHandle );
	curl_slist_free_all( header );

	return response;
}

#endif