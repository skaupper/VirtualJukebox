/*****************************************************************************/
/**
 * @file    LoggingHandler.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   LoggingHandler header file
 */
/*****************************************************************************/

#ifndef _LOGGINGHANDLER_H_
#define _LOGGINGHANDLER_H_

#include <string>

void logInfo(std::string const& msg);
void logWarning(std::string const& msg);
void logError(std::string const& msg);

#endif /* _LOGGINGHANDLER_H_ */
