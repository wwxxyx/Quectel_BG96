/* Copyright 200-2010, Apple Inc.  All rights reserved. */

#ifndef __AFC_SERVER_H__
#define __AFC_SERVER_H__

#include <stdio.h>
#include <sys/types.h>

#include <MobileDevice/AFCTypes.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	/* Always allow a minimum number of file descriptors to be opened */
#define kAFCMinOpenFileCount  16
	/* Reserve some file descriptors for use by the server */
#define kAFCOpenFileReserveCount  16
    /* Divide maximum vnodes available by this number to get max files open */
    /* E.g., if kern.maxvnodes = 750, then (750 / 16) = 46 */
#define kAFCMaxVNodeScaleFactor 16
	/* Maximum number of in-flight IOs */
#define kAFCMaxIORequests	 3


// Create a new server context.
AFC_EXPORT AFCServerContext *
AFCCreateServerContext( void );
AFC_EXPORT void	AFCInitServerConnection(AFCConnectionRef conn);

// Free buffers contained in the server context.
AFC_EXPORT void AFCFlushServerContext( AFCServerContext *ctx );
AFC_EXPORT void AFCFreeServerContext( AFCServerContext *ctx );

AFC_EXPORT AFCDomainError AFCServe(AFCConnectionRef conn); // deprecated
AFC_EXPORT AFCDomainError AFCServeWithRoot(AFCConnectionRef conn, CFStringRef rootPath);

#define kAFCDefaultDirectoryMode 0755
#define kAFCDefaultFileMode 0644

/* Other symbols used on the server */
AFC_EXPORT size_t gAFCFSBlocksizeMax;
AFC_EXPORT size_t gAFCFSBlocksizeDefault;
AFC_EXPORT bool gAllowSpecialFiles;
	
AFC_EXPORT int gAFCIOPriority; // dispatch queue priority for IO requests
AFC_EXPORT size_t gAFCMaxIORequests; // maximum number of outstanding IO requests
    
AFC_EXPORT void AFCServerConnectionSetFileCoordinationEnabled(AFCConnectionRef conn, bool enabled);
AFC_EXPORT bool AFCServerConnectionGetFileCoordinationEnabled(AFCConnectionRef conn);
AFC_EXPORT void AFCServerConnectionSetRespectDataProtectionClass(AFCConnectionRef conn, bool enabled);
AFC_EXPORT bool AFCServerConnectionGetRespectDataProtectionClass(AFCConnectionRef conn);
    
#ifdef __cplusplus
}
#endif
	
#endif /*! __AFC_SERVER_H__ */
