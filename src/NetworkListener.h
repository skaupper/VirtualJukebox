/** \file NetworkListener.h
 *  @brief The interface used by the core logic to get notified of incoming requests.
 *
 *  This file contains the interface for incoming requests as well as an interface which can be used to report errors
 *  back to the client.
 *  For easier handling a typedef has been defined for the result type of requests.
 */

#ifndef NETWORK_LISTENER_H
#define NETWORK_LISTENER_H

#include <variant>
#include <string>
#include <vector>


//
// Temporary Structs used for the interface specification
// FIXME: use the real structs defined by the data store here!
//
/** @cond TEMP */
struct Track {};
struct Queues {};
struct User {};
enum class PlayerAction {};
/** @endcond */


//
// Forward declarations
//
class NetworkListenerError;


//
// Type definitions
//
/**
 * @typedef NetworkResult
 * @brief Used as result type of all request handlers in `NetworkListener`.
 *
 * Abstracts away the need to specify `std::variant` and the alternative type `NetworkListenerError`.
 */
template <class GOOD>
using NetworkResult = std::variant<GOOD, NetworkListenerError>;


//
// Class definitions
//
/**
 * @brief The error type for the request handlers in `NetworkListener`
 * @sa NetworkResult
 */
class NetworkListenerError {
public:
    virtual ~NetworkListenerError() {}

    virtual int getErrorCode() = 0;
    virtual std::string getErrorMessage() = 0;
};


/**
 * @brief Interface for all available client requests.
 *
 * The core logic may implement this interface in order to get notified of incoming client requests. \n
 * For a specific description of how this endpoints should be used, see @ref rest_interface "here".
 *
 * @sa NetworkAPI
 */
class NetworkListener {
public:
    virtual ~NetworkListener() {}

    /**
     * @brief Generate a new session for the given user.
     * @param user User to generate the session for.
     * @return The generated session as a string, an object of type `NetworkListenerError` on error.
     */
    virtual NetworkResult<std::string> generateSession(User user) = 0;
    virtual NetworkResult<std::vector<Track>> queryMusic(const std::string &pattern) = 0;
    virtual NetworkResult<Queues> getCurrentQueues() = 0;
    virtual NetworkResult<void> addTrackToQueue(User user, Track track) = 0;
    virtual NetworkResult<void> addTrackToAdminQueue(User user, Track track) = 0;
    virtual NetworkResult<void> voteTrack(User user, Track track) = 0;
    virtual NetworkResult<void> controlPlayer(User user, PlayerAction action) = 0;
};

#endif
