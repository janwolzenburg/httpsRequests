#ifndef HTTPSREQUESTS_H
#define HTTPSREQUESTS_H

/*!
*
*	@file httpsRequests.h
*	@author Jan Wolzenburg
*	@date 24.04.2023
*
*/


/*******************************************************************
 * Includes
********************************************************************/

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <tuple>
using std::pair;

#include "curl/curl.h"



/*******************************************************************
 * Definitions
********************************************************************/

/*!
 * @brief Class for a single https request
 * 
 */
class HTTPSRequest{

public:

    /*!
     * @brief Enumeration for SSL verification types
     * 
     */
    enum SSL_Type{
        NONE,
        VERIFY_PEER,
        VERIFY_HOST,
        VERIFY_PEER_HOST
    };

    /*!
     * @brief Construct a request with POST method
     * 
     * @param url Target URL
     * @param header Vector with header fields. Each element like "Content-Type: application/json"
     * @param post_data Post data as string
     * @param ssl_type SSL type
     */
    HTTPSRequest( const string url, const vector<string> header, const string post_data, const SSL_Type ssl_type = NONE );

    /*!
     * @brief Construct a request with GET method
     * 
     * @param url Target URL
     * @param header Vector with header fields. Each element like "Content-Type: application/json"
     * @param get_arguments Get as vector of key-value pairs
     * @param ssl_type SSL type
     */
    HTTPSRequest( const string url, const vector<string> header, const vector<pair<string, string>> get_arguments, const SSL_Type ssl_type = NONE );

    /*!
     * @brief Destructor
     * 
     */
    ~HTTPSRequest( void );

    /*!
     * @brief Get the response string
     * 
     * @return string Resonse string
     */
    string GetResponseString( void ) const;

    /*!
     * @brief Check if request succeeded
     * 
     * @return true When successful
     * @return false Otherwise
     */
    bool request_success( void ) const{ return request_success_; };


private:
    
    static bool CURL_INITIALISED;		/*!<Flag for CURL initialisation*/

    bool request_success_;				/*!<Flag for request success*/

    CURL* curl_handle_;					/*!<CURL handle*/
    curl_slist* curl_header_;		    /*!<CURL header*/

    vector<char> response;				/*!<Response*/

    /*!
     * @brief Construct a request
     * 
     * @param header Vector with header fields. Each element like "Content-Type: application/json"
     * @param ssl_type SSL type
     */
    HTTPSRequest( const vector<string> header, const SSL_Type ssl_type );

    /*!
     * @brief Perform request
     * 
     */
    void PerformRequest( void );

    /*!
     * @brief Write callback for curl. Gets called when received data needs to be stored
     * @details https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
     * 
     * @param ptr Pointer to the received data
     * @param size Always 1
     * @param nmemb Size of data
     * @param userdata Pointer to a vector<char> object
     * @return size_t Bytes taken care of
     */
    static size_t write_callback( const char* const ptr, const size_t size, const size_t nmemb, void* const userdata );

		
};


#endif