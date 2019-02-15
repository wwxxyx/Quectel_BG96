#ifndef _APPLEMOBILELOG_
#define _APPLEMOBILELOG_

#include <CoreFoundation/CoreFoundation.h>

/*!
 * @header MobileDeviceLog.h
 *
 * Logging.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @function AMDAddLogFileDescriptor
 * @abstract Add a file descriptor to the set of descriptors logged to.
 * @param fd A valid file descriptor that can be used with write().
 * @result 0 on success, non-zero on failure.
 * @discussion The file descriptor is managed by the caller. Will begin writing
 * MobileDevice logs to this fd.
 */
int
AMDAddLogFileDescriptor(
	int fd);

/*!
 * @function AMDRemoveLogFileDescriptor
 * @abstract Remove a file descriptore from the set that is being logged to.
 * @result 0 on success, non-zero on failure.
 * @result Will cause MobileDevice to stop logging to the given file descriptor.
 */
int
AMDRemoveLogFileDescriptor(
	int fd);

/*!
 * @function AMDSetLogLevel
 * @abstract Set the log level.
 * @param level The level to set logging at.
 * @discussion Works like setlogmask(LOG_UPTO(level));
 */
void
AMDSetLogLevel(
	int level);

#ifdef __cplusplus
}
#endif

#endif /* _APPLEMOBILELOG_ */

