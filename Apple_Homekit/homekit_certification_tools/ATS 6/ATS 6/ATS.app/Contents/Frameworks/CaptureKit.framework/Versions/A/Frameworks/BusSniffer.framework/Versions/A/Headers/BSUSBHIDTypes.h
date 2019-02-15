/*
 *  BSUSBHIDTypes.h
 *  BusSniffer
 *
 *  Created by Joe Basu on 6/13/09.
 *  Copyright © 2009 Apple Inc. All Rights Reserved.
 *
 */

enum {
	BSUSBDescriptorHID			= 0x21,
	BSUSBDescriptorHIDReport	= 0x22,
	BSUSBDescriptorHIDPhysical	= 0x23
};

// Taken from HID 1.11 - 7.2.1
enum {
	BSUSBHIDReportInput	    = 0x01,
	BSUSBHIDReportOutput	= 0x02,
	BSUSBHIDReportFeature	= 0x03,
    
    BSUSBHIDReportUnknown   = 0xFF
};

typedef UInt8 BSUSBHIDReportType;

/*!
 @function  BSUSBHIDStringForHIDReportType
 @abstract  Returns the string for the given report type. nil is returned
            if reportType is invalid.
 */
NSString *BSUSBHIDStringForHIDReportType(BSUSBHIDReportType reportType);

enum {
	BSUSBHIDReportStatusOK					= 0x00,
	BSUSBHIDReportStatusUnknownReportID	= 0x01,
	BSUSBHIDReportStatusTooManyBytes		= 0x02,
	BSUSBHIDReportStatusTooFewBytes		= 0x03,
};

typedef UInt8 BSUSBHIDReportStatus;

enum {
    BSUSBHIDRequestGetReport		= 0x01,
    BSUSBHIDRequestGetIdle			= 0x02,
    BSUSBHIDRequestGetProtocol		= 0x03,
    BSUSBHIDRequestSetReport		= 0x09,
    BSUSBHIDRequestSetIdle			= 0x0A,
    BSUSBHIDRequestSetProtocol		= 0x0B,
};

typedef UInt8 BSUSBHIDRequestRequest;

typedef enum : UInt8 {
    BSUSBHIDReportDescriptorItemTypeMain	= 0,
    BSUSBHIDReportDescriptorItemTypeGlobal	= 1,
    BSUSBHIDReportDescriptorItemTypeLocal  = 2,
    BSUSBHIDReportDescriptorItemTypeReserved  = 3,
} BSUSBHIDReportDescriptorItemType;

typedef enum : UInt8 {
    BSUSBHIDReportDescriptorItemTagLongItem		= 0x0F,
    
    // Main items
    BSUSBHIDReportDescriptorItemTagInput			= 0x08,
    BSUSBHIDReportDescriptorItemTagOutput			= 0x09,
    BSUSBHIDReportDescriptorItemTagFeature			= 0x0B,
    BSUSBHIDReportDescriptorItemTagCollection		= 0x0A,
    BSUSBHIDReportDescriptorItemTagEndCollection	= 0x0C,
    
    // Global items
    BSUSBHIDReportDescriptorItemTagUsagePage		= 0x00,
    BSUSBHIDReportDescriptorItemTagLogicalMin		= 0x01,
    BSUSBHIDReportDescriptorItemTagLogicalMax		= 0x02,
    BSUSBHIDReportDescriptorItemTagPhysicalMin		= 0x03,
    BSUSBHIDReportDescriptorItemTagPhysicalMax		= 0x04,
    BSUSBHIDReportDescriptorItemTagUnitExponent	= 0x05,
    BSUSBHIDReportDescriptorItemTagUnit			= 0x06,
    BSUSBHIDReportDescriptorItemTagReportSize		= 0x07,
    BSUSBHIDReportDescriptorItemTagReportID		= 0x08,
    BSUSBHIDReportDescriptorItemTagReportCount		= 0x09,
    BSUSBHIDReportDescriptorItemTagPush			= 0x0A,
    BSUSBHIDReportDescriptorItemTagPop				= 0x0B,
    
    // Local items
    BSUSBHIDReportDescriptorItemTagUsage			= 0x00,
    BSUSBHIDReportDescriptorItemTagUsageMin		= 0x01,
    BSUSBHIDReportDescriptorItemTagUsageMax		= 0x02,
    BSUSBHIDReportDescriptorItemTagDesignatorIndex	= 0x03,
    BSUSBHIDReportDescriptorItemTagDesignatorMin	= 0x04,
    BSUSBHIDReportDescriptorItemTagDesignatorMax	= 0x05,
    BSUSBHIDReportDescriptorItemTagStringIndex		= 0x07,
    BSUSBHIDReportDescriptorItemTagStringMin		= 0x08,
    BSUSBHIDReportDescriptorItemTagStringMax		= 0x09,
    BSUSBHIDReportDescriptorItemTagSetDelimiter	= 0x0A
} BSUSBHIDReportDescriptorItemTag;

// Collection constants
typedef enum : UInt8 {
    BSUSBHIDReportDescriptorCollectionItemValuePhysical	= 0x00,
    BSUSBHIDReportDescriptorCollectionItemValueApplication = 0x01,
    BSUSBHIDReportDescriptorCollectionItemValueLogical		= 0x02
} BSUSBHIDReportDescriptorCollectionItemValue;

/*!
 @define    BSUSBHIDReportDescriptorCollectionType
 @abstract  Defines the collection type.
 */
#define BSUSBHIDReportDescriptorCollectionType BSUSBHIDReportDescriptorCollectionItemValue
#define BSUSBHIDReportDescriptorCollectionTypePhysical BSUSBHIDReportDescriptorCollectionItemValuePhysical
#define BSUSBHIDReportDescriptorCollectionTypeApplication BSUSBHIDReportDescriptorCollectionItemValueApplication
#define BSUSBHIDReportDescriptorCollectionTypeLogical BSUSBHIDReportDescriptorCollectionItemValueLogical

// I/O constants (used for Input/Output/Feature tags)
enum
    {
    BSUSBHIDReportDescriptorIOItemBitDataOrConstant                 = 0x0001,
    BSUSBHIDReportDescriptorIOItemBitArrayOrVariable				= 0x0002,
    BSUSBHIDReportDescriptorIOItemBitAbsoluteOrRelative             = 0x0004,
    BSUSBHIDReportDescriptorIOItemBitNoWrapOrWrap					= 0x0008,
    BSUSBHIDReportDescriptorIOItemBitLinearOrNonLinear				= 0x0010,
    BSUSBHIDReportDescriptorIOItemBitPreferredStateOrNoPreferred	= 0x0020,
    BSUSBHIDReportDescriptorIOItemBitNoNullPositionOrNullState		= 0x0040,
    BSUSBHIDReportDescriptorIOItemBitNonVolatileOrVolatile			= 0x0080,		// reserved for Input
    BSUSBHIDReportDescriptorIOItemBitBitFieldOrBufferedBytes		= 0x0100
};

/*!
 @const     BSUSBHIDUsagePageUndefined
 @abstract  Defines the undefined (reserved) usage page.
 */
extern const UInt16 BSUSBHIDUsagePageUndefined;

/*!
 @const     BSUSBHIDReportIDReserved
 @abstract  Defines the reserved report ID.
 */
extern const UInt8 BSUSBHIDReportIDReserved;

/*!
 @const     BSUSBHIDReportIDUndefined
 @abstract  Defines the report ID for a HID report that doesn't have a report ID.
 */
#define BSUSBHIDReportIDUndefined BSUSBHIDReportIDReserved

/*!
 @const     BSUSBHIDUsageIDUndefined
 @abstract  Defines the value for an undefined usage ID.
 */
extern const UInt16 BSUSBHIDUsageIDUndefined;

struct BSUSBHIDReportForiAPContentStructure {
	BOOL partialLeading;
	NSUInteger fullCount;
	BOOL partialTrailing;
};
