//
//  BSHIDUsageTables.h
//  BusSniffer
//
//  Created by Kip Nicol on 8/8/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

//
// IOKit's IOHIDUsageTables defines some, but not all valid HID Usage ID's and Usage Pages. This header
// file is used to add additional Usage ID's that are needed in our code, but aren't defined in IOHIDUsageTables.h.
//

#ifndef BusSniffer_BSHIDUsageTables_h
#define BusSniffer_BSHIDUsageTables_h

#import <Kernel/Availability.h>

// Example:
//
// /* Sensor Page (0x20) */
// enum {
//     kHIDUsage_Snsr_Data_Biometric_HumanPresence = 0x04B1
// };

/* Usage Pages */
enum {
    kHIDPage_BrailleDisplay = 0x41,
};

/* Braille Display Page (0x41) */
enum {
    kHIDUsage_BrailleDisplay_BrailleDisplay = 0x01,
    kHIDUsage_BrailleDisplay_BrailleRow = 0x02,
    kHIDUsage_BrailleDisplay_8DotBrailleCell = 0x03,
    kHIDUsage_BrailleDisplay_6DotBrailleCell = 0x04,
    kHIDUsage_BrailleDisplay_NumberOfBrailleCells = 0x05,
    kHIDUsage_BrailleDisplay_ScreenReaderControl = 0x06,
    kHIDUsage_BrailleDisplay_ScreenReaderIdentifier = 0x07,
    /* 0x08 - 0xF9 Reserved */
    kHIDUsage_BrailleDisplay_RouterSet1 = 0xFA,
    kHIDUsage_BrailleDisplay_RouterSet2 = 0xFB,
    kHIDUsage_BrailleDisplay_RouterSet3 = 0xFC,
    kHIDUsage_BrailleDisplay_RouterButton = 0x100,
    kHIDUsage_BrailleDisplay_BrailleButtons = 0x200,
    kHIDUsage_BrailleDisplay_BrailleKeyboardDot1 = 0x201,
    kHIDUsage_BrailleDisplay_BrailleKeyboardDot2 = 0x202,
    kHIDUsage_BrailleDisplay_BrailleKeyboardDot3 = 0x203,
    kHIDUsage_BrailleDisplay_BrailleKeyboardDot4 = 0x204,
    kHIDUsage_BrailleDisplay_BrailleKeyboardDot5 = 0x205,
    kHIDUsage_BrailleDisplay_BrailleKeyboardDot6 = 0x206,
    kHIDUsage_BrailleDisplay_BrailleKeyboardDot7 = 0x207,
    kHIDUsage_BrailleDisplay_BrailleKeyboardDot8 = 0x208,
    kHIDUsage_BrailleDisplay_BrailleKeyboardSpace = 0x209,
    kHIDUsage_BrailleDisplay_BrailleKeyboardLeftSpace = 0x20A,
    kHIDUsage_BrailleDisplay_BrailleKeyboardRightSpace = 0x20B,
    kHIDUsage_BrailleDisplay_BrailleFaceControls = 0x20C,
    kHIDUsage_BrailleDisplay_BrailleLeftControls = 0x20D,
    kHIDUsage_BrailleDisplay_BrailleRightControls = 0x20E,
    kHIDUsage_BrailleDisplay_BrailleTopControls = 0x20F,
    kHIDUsage_BrailleDisplay_BrailleJoystickCenter = 0x210,
    kHIDUsage_BrailleDisplay_BrailleJoystickUp = 0x211,
    kHIDUsage_BrailleDisplay_BrailleJoystickDown = 0x212,
    kHIDUsage_BrailleDisplay_BrailleJoystickLeft = 0x213,
    kHIDUsage_BrailleDisplay_BrailleJoystickRight = 0x224,
    kHIDUsage_BrailleDisplay_BrailleDPadCenter = 0x225,
    kHIDUsage_BrailleDisplay_BrailleDPadUp = 0x226,
    kHIDUsage_BrailleDisplay_BrailleDPadDown = 0x217,
    kHIDUsage_BrailleDisplay_BrailleDPadLeft = 0x218,
    kHIDUsage_BrailleDisplay_BrailleDPadRight = 0x219,
    kHIDUsage_BrailleDisplay_BraillePanLeft = 0x21A,
    kHIDUsage_BrailleDisplay_BraillePanRight = 0x21B,
    kHIDUsage_BrailleDisplay_BrailleRockerUp = 0x21C,
    kHIDUsage_BrailleDisplay_BrailleRockerDown = 0x21D,
    kHIDUsage_BrailleDisplay_BrailleRockerPress = 0x21E,
    /* 0x21F - 0x2FF Reserved */
    kHIDUsage_BrailleDisplay_Reserved = 0xFFFF,
};

#endif
