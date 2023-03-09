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


/*******************************************************************
 * Includes
********************************************************************/

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <array>
using std::array;

#include "curl/curl.h"



/*******************************************************************
 * Definitions
********************************************************************/

/*!
 * @brief Class for a single https request
 * 
 */
class httpsRequest{

	public:

		/*!
		 * @brief Enumeration for SSL verification types
		 * 
		 */
		enum SSL_TYPE{
			NONE,
			VERIFY_PEER,
			VERIFY_HOST,
			VERIFY_PEER_HOST
		};

		/*!
		 * @brief Construct a request with POST method
		 * 
		 * @param url Target URL
		 * @param header_v Vector with header fields. Each element like "Content-Type: application/json"
		 * @param postData Post data as string
		 * @param sslType SSL type
		 */
		httpsRequest( const string url, const vector<string> header_v, const string postData, const SSL_TYPE sslType = NONE );

		/*!
		 * @brief Construct a request with GET method
		 * 
		 * @param url Target URL
		 * @param header_v Vector with header fields. Each element like "Content-Type: application/json"
		 * @param getData Get as vector of key-value pairs
		 * @param sslType SSL type
		 */
		httpsRequest( const string url, const vector<string> header_v, const vector<array<string, 2>> getData, const SSL_TYPE sslType = NONE );

		/*!
		 * @brief Get the response string
		 * 
		 * @return string Resonse string
		 */
		string getResponseString( void ) const;

		/*!
		 * @brief Check if request succeeded
		 * 
		 * @return true When successful
		 * @return false Otherwise
		 */
		bool requestSucceeded ( void ) const;		


	private:

		/*!
		 * @brief Construct a request
		 * 
		 * @param header_v Vector with header fields. Each element like "Content-Type: application/json"
		 * @param type SSL type
		 */
		httpsRequest( const vector<string> header_v, const SSL_TYPE type );

		/*!
		 * @brief Perform request
		 * 
		 */
		void performRequest( void );

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


	private:

		static bool CURL_INITIALISED;		/*!<Flag for CURL initialisation*/

		bool success;						/*!<Flag for request success*/

		CURL* curlHandle;					/*!<CURL handle*/
		curl_slist* header;					/*!<CURL header*/

		vector<char> response;				/*!<Response*/
		
};


#endif