/*****************************************************************************/
/**
 * @file    LoggingHandler.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   Class LoggingHandler definition
 */
/*****************************************************************************/

#ifndef _LOGGINGHANDLER_H_
#define _LOGGINGHANDLER_H_

#include <string>

static void logInfo(std::string msg);
static void logWarning(std::string msg);
static void logError(std::string msg);

#endif /* _LOGGINGHANDLER_H_ */
