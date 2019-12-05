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

struct RequestInformation {
  std::string path;
  std::string method;
  std::string body;
  std::map<std::string, std::string, httpserver::http::arg_comparator> args;
};

#endif  // _REQUEST_INFORMATION_H_
