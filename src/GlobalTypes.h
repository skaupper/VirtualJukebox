/** ----------------------------------------------------------------------------
 * @file    GlobalTypes.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Defines types and constants that are used globally
 * ---------------------------------------------------------------------------*/

#ifndef _GLOBALTYPES_H_
#define _GLOBALTYPES_H_

#include <string>

/**
 * @brief Queue type enumerator
 */
enum class QueueType { Admin, Normal };

/**
 * @brief Player action type enumerator
 */
enum class PlayerAction { Play, Pause, Stop, Skip, VolumeUp, VolumeDown };

/**
 * @brief Vote type
 */
using TVote = bool;

/**
 * @brief Session ID type
 */
using TSessionID = std::string;

/**
 * @brief Track ID type
 */
using TTrackID = std::string;

#endif /* _GLOBALTYPES_H_ */
