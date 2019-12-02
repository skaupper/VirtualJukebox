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

void logInfo(std::string msg);
void logWarning(std::string msg);
void logError(std::string msg);

#endif /* _LOGGINGHANDLER_H_ */
