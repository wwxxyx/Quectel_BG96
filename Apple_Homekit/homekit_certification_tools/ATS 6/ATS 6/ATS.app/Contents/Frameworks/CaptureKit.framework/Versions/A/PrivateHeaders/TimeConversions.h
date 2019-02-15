/*
 *  TimeConversions.h
 *  ATSMacApp
 *
 *  Created by Bob Burrough on 9/27/10.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */

// conversion utils to convert to nanoseconds
#define NANOSECOND    1ULL
#define MICROSECOND  (1000ULL * NANOSECOND)
#define MILLISECOND  (1000ULL * MICROSECOND)
#define SECOND       (1000ULL * MILLISECOND)
#define MINUTE       (60ULL * SECOND)

