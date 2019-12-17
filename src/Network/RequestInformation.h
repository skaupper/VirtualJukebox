/*****************************************************************************/
/**
 * @file    RequestInformation.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of class RequestInformation
 */
/*****************************************************************************/

#ifndef _REQUEST_INFORMATION_H_
#define _REQUEST_INFORMATION_H_

#include <httpserver.hpp>
#include <map>
#include <string>

/**
 * @brief Wraps all relevant pieces of information provided by a HTTP request.
 */
struct RequestInformation {
  std::string path;
  std::string method;
  std::string body;
  std::map<std::string, std::string, httpserver::http::arg_comparator> args;
};

/**
 * @brief Wraps all needed pieces of information to form a proper HTTP response.
 */
struct ResponseInformation {
  std::string body;
  int code = 200;
};

#endif  // _REQUEST_INFORMATION_H_
