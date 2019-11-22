#ifndef NETWORK_LISTENER_H
#define NETWORK_LISTENER_H

#include <variant>
#include <memory>
#include <string>


//
// Forward declarations
//
class NetworkListenerError;


//
// Type definitions
//
template <class GOOD>
using NetworkResult = std::variant<GOOD, std::unique_ptr<NetworkListenerError>>;


//
// Class definitions
//
class NetworkListenerError {
public:
    virtual ~NetworkListenerError() {}

    virtual int getErrorCode() = 0;
    virtual std::string getErrorMessage() = 0;
};


class NetworkListener {
public:
    virtual ~NetworkListener() {}

    virtual NetworkResult<std::string> generateSession() = 0;
};

#endif
