/*****************************************************************************/
/**
 * @file    Utils/LoggingHandler.h
 * @author  Michael Wurm <wurm.michael95@gmail.com>
 * @brief   LoggingHandler header file
 */
/*****************************************************************************/

#ifndef _LOGGINGHANDLER_H_
#define _LOGGINGHANDLER_H_

#include <string>

void initLoggingHandler(std::string const& exe);

/* Usage for Google Log messages:
 *
 * DLOG(INFO) << "Logging a DEBUG message!";
 * LOG(INFO) << "Logging an INFO message!";
 * LOG(WARNING) << "Logging a WARNING message!";
 * LOG(ERROR) << "Logging an ERROR message!";
 *
 */

#endif /* _LOGGINGHANDLER_H_ */
