#include "CurlHttpClient.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	string data((const char*)ptr, (size_t)size * nmemb);

	*((stringstream*)stream) << data << endl;

	return size * nmemb;
}

string CurlHttpClient::SendGetReq(const char* url)
{
	CURL *curl;
	CURLcode res;
	stringstream out;
	curl = curl_easy_init();
	string response = "";
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(curl, CURLOPT_URL, url);

		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		// 设置接收数据的处理函数和存放变量
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		if (res == CURLE_OK)
			response = out.str();
		/* always cleanup */
		curl_easy_cleanup(curl);
		int index = response.find("\n");
		while (index != string::npos)
		{
			response.replace(index, 1, "");
			index = response.find("\n");
		}
		return response;
	}
}

string CurlHttpClient::SendPostReq(const char* url, const char* postdata)
{
	CURL *curl;
	CURLcode res;
	stringstream out;
	curl = curl_easy_init();
	string response = "";
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(curl, CURLOPT_URL, url);

		/* specify we want to POST data */
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		/* Set the expected POST size */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postdata));
		/* Set the expected POST data */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);

		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		// 设置接收数据的处理函数和存放变量
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		if (res == CURLE_OK)
			response = out.str();
		/* always cleanup */
		curl_easy_cleanup(curl);
		int index = response.find("\n");
		while (index != string::npos)
		{
			response.replace(index, 1, "");
			index = response.find("\n");
		}
		return response;
	}
}
