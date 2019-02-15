//
//  RuleTests+HIDReport.h
//  ATSMacApp
//
//  Created by Kip Nicol on 3/12/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>

@interface RuleTests (HIDReport)

/*!
 @method    -dataForZeroedHIDReportWithLength:
 @abstract  Returns data with all zeros and a length of the given length.
 @param     dataLength
            The length of the HID report.
 */
- (NSData *)dataForZeroedHIDReportWithLength:(NSUInteger)dataLength;

/*!
 @method    -dataForZeroedHIDReportWithInvalidPaddingBitAndLength:
 @abstract  Returns data with all zeros and a length of the given length, with a 
            single padding bit set to 1.
 @param     dataLength
            The length of the HID report.
 */
- (NSData *)dataForZeroedHIDReportWithInvalidPaddingBitAndLength:(NSUInteger)dataLength;

/*!
 @method    -dataForZeroedHIDReportWithLength:reportID:
 @abstract  Returns data with all zeros and a length of the given length and with a report ID.
 @param     dataLength
            The length of the HID report (not including the report ID).
 @param     reportID
            The report ID to prepend to the data.
 */
- (NSData *)dataForZeroedHIDReportWithLength:(NSUInteger)dataLength reportID:(UInt8)reportID;

/*!
 @method    -dataForHIDReportWithAllBytesSetTo:length:
 @abstract  Returns HID report data with the bytes all set to the given byte.
 @param     byteValue
            The byte to set all the bytes to.
 @param     dataLength
            The length of the HID report (not including the report ID).
 */
- (NSData *)dataForHIDReportWithAllBytesSetTo:(UInt8)byteValue length:(NSUInteger)dataLength;

/*!
 @method    -dataForKeyboardHIDReportsWithVariableFieldValues
 @abstract  Returns a collection of data objects representing HID reports 
            containing field values for all variable fields for a HID keyboard 
            with all permitted usages.
 */
- (NSArray *)dataForKeyboardHIDReportsWithVariableFieldValues;

/*!
 @method    -dataForKeyboardHIDReportsWithArrayFieldValues
 @abstract  Returns a collection of data objects representing HID reports 
            containing field values for one, two, three, four, and five
            simultaneous array field values.
 */
- (NSArray *)dataForKeyboardHIDReportsWithArrayFieldValues;

/*!
 @method    -dataForKeyboardHIDReportsWithVariableFieldValuesAndReportID:
 @abstract  Returns a collection of data objects representing HID reports
            containing field values for all variable fields for a HID keyboard
            with all permitted usages.
 @param     reportID
            The report ID to use. Pass 0 if there is no report ID.
 */
- (NSArray *)dataForKeyboardHIDReportsWithVariableFieldValuesAndReportID:(UInt8)reportID;

/*!
 @method    -dataForKeyboardHIDReportsWithArrayFieldValuesAndReportID:
 @abstract  Returns a collection of data objects representing HID reports
            containing field values for one, two, three, four, and five
            simultaneous array field values.
 @param     reportID
            The report ID to use. Pass 0 if there is no report ID.
 */
- (NSArray *)dataForKeyboardHIDReportsWithArrayFieldValuesAndReportID:(UInt8)reportID;

/*!
 @method    -dataForKeyboardHIDReportWithValuesForAllFields
 @abstract  Returns a data object representing a HID report containing values 
            for all fields.
 */
- (NSData *)dataForKeyboardHIDReportWithValuesForAllFields;

/*!
    Returns a data object representing a HID report containing values for all fields for the dataForKeyboardHIDReportDescriptorWithPermittedUsagesOne descriptor.
 */
- (NSData *)dataForKeyboardHIDReportWithOneProhibitedUsage;

/*!
 	Returns a data object representing a HID report containing values for all fields for the dataForKeyboardHIDReportDescriptorWithPermittedUsagesOne descriptor without using those prohibited usages.
 */
- (NSData *)dataForKeyboardHIDReportWithOneProhibitedUsageWithoutUsing;

/*!
    Returns a data object representing a HID report containing values for all fields for the dataForKeyboardHIDReportDescriptorWithValuesForRangeUsages descriptor.
 */
- (NSData *)dataForKeyboardHIDReportWithRangeUsages;

/*!
    Returns a data object representing a HID report containing values for all fields for the dataForKeyboardHIDReportDescriptorWithValuesForRangeUsages descriptor without using those prohibited usages.
 */
- (NSData *)dataForKeyboardHIDReportWithRangeUsagesWithoutUsing;

/*!
    Returns a data object representing a HID report containing values for all fields (multiple prohibited usages) for the dataForKeyboardHIDReportDescriptorWithProhibitedUsagesMutliple descriptor.
 */
- (NSData *)dataForKeyboardHIDReportWithMultipleProhibitedUsages;

/*!
 	Returns a data object representing a HID report containing values for all fields (multiple prohibited usages) for the dataForKeyboardHIDReportDescriptorWithProhibitedUsagesMutliple descriptor without using those prohibited usages.
 */
- (NSData *)dataForKeyboardHIDReportWithMultipleProhibitedUsagesWithoutUsing;

/*!
    Returns a data object representing a HID report containing values for all fields (adding an extra usage page) for the dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndAdditionalUsagePage descriptor.
 */
- (NSData *)dataForKeyboardHIDReportWithAdditionalUsagePage;

/*!
 	Returns a data object representing a HID report containing values for all fields (adding an extra usage page) for the dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndAdditionalUsagePage descriptor without using those prohibited usages.
 */
- (NSData *)dataForKeyboardHIDReportWithAdditionalUsagePageWithoutUsing;

/*!
    Returns a data object representing a HID report containing values for all fields (adding an extra usage page) for the dataForKeyboardHIDReportDescriptorWithProhibitedUsagesBetweenMultipleUsagePages descriptor.
 */
- (NSData *)dataForKeyboardHIDReportWithProhibitedUsagesBetweenMultipleUsagePages;

/*!
 	Returns a data object representing a HID report containing values for all fields (adding an extra usage page) for the dataForKeyboardHIDReportDescriptorWithProhibitedUsagesBetweenMultipleUsagePages descriptor without using those prohibited usages.
 */
- (NSData *)dataForKeyboardHIDReportWithProhibitedUsagesBetweenMultipleUsagePagesWithoutUsing;

/*!
    Returns a data object representing a HID report containing values for all fields (adding an extra usage page) for the dataForKeyboardHIDReportDescriptorWithPermittedUsagesBetweenMultipleUsagePages descriptor.
 */
- (NSData *)dataForKeyboardHIDReportWithPermittedUsagesBetweenMultipleUsagePages;

/*!
    Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithValuesForAllUsages descriptor.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithValuesForAllFields;

/*!
    Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesOne descriptor.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithOneProhibitedUsage;

/*!
 	Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedRangeUsages descriptor.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithRangeUsages;

/*!
    Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesOne descriptor without using those prohibited usages.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithOneProhibitedUsageWithoutUsing;

/*!
    Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesMutliple descriptor.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithMultipleProhibitedUsages;

/*!
 	Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesMutliple descriptor without using those prohibited usages.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithMultipleProhibitedUsagesWithoutUsing;

/*!
    Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedUsagesAndAdditionalUsagePage descriptor.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithAdditionalUsagePage;

/*!
 	Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedUsagesAndAdditionalUsagePage descriptor without using those prohibited usages.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithAdditionalUsagePageWithoutUsing;

/*!
    Returns a data object representing a media playback remote HID report containing values for all permitted fields for the dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesBetweenMultipleUsagePages descriptor.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportWithProhibitedUsagesBetweenMultipleUsagePages;

/*!
 @method    -dataForKeyboardHIDReportWithValuesForAllFieldsAndReportID:
 @abstract  Returns a data object representing a HID report with a report ID 
            prefix and values for all fields.
 @param     reportID
            The report ID to use as a prefix. Pass 0 if there is no report ID.
 */
- (NSData *)dataForKeyboardHIDReportWithValuesForAllFieldsAndReportID:(UInt8)reportID;

/*!
 @method    -dataForKeyboardHIDReportWithAllValuesForAllFields
 @abstract  Returns a data object representing a HID report containing all values
 for all fields.
 */
- (NSArray *)dataForKeyboardHIDReportWithAllValuesForAllFields;

/*!
 @method    -dataForKeyboardHIDReportsWithOneFieldValuePerReport
 @abstract  Returns a collection of data objects representing a series of HID 
            reports in which a value is provided for each report field in turn.
 */
- (NSArray *)dataForKeyboardHIDReportsWithOneFieldValuePerReport;

/*!
 @method    -dataForKeyboardHIDReportsWithOneFieldValuePerReportAndReportID:
 @abstract  Returns a collection of data objects representing a series of HID
            reports in which a value is provided for each report field in turn.
 @param     reportID
            The report ID to use as a prefix. Pass 0 if there is no report ID.
 */
- (NSArray *)dataForKeyboardHIDReportsWithOneFieldValuePerReportAndReportID:(UInt8)reportID;

/*!
 @method    -dataForKeyboardWithNonZeroLogicalMinArrayHIDReportWithValuesForVariableFields
 @abstract  Returns a data object representing a HID report containing values for variable fields.
 */
- (NSData *)dataForKeyboardWithNonZeroLogicalMinArrayHIDReportWithValuesForVariableFields;

/*!
 @method    -dataForKeyboardWithNonZeroLogicalMinArrayHIDReportWithValuesForAllFields
 @abstract  Returns a collection of data objects representing a series of HID
            reports in which a value is provided for each report field in turn.
 */
- (NSArray *)dataForKeyboardWithNonZeroLogicalMinArrayHIDReportWithValuesForAllFields;

/*!
 @method    -dataForAssistiveTouchPointerHIDReportWithAllValuesForAllFields
 @abstract  Returns a data object representing a HID report containing all values for all fields.
 */
- (NSData *)dataForAssistiveTouchPointerHIDReportWithAllValuesForAllFields;

/*!
 @method    -dataForAssistiveTouchPointerHIDReportWithAllValuesForAllFieldsAndReportID:
 @abstract  Returns a data object representing a HID report containing all values for all fields.
 @param     reportID
            The report ID to use as a prefix. Pass 0 if there is no report ID.
 */
- (NSData *)dataForAssistiveTouchPointerHIDReportWithAllValuesForAllFieldsAndReportID:(UInt8)reportID;

/*!
 @method    -dataForGameControllerHIDReportWithDPadUp...
 @abstract  Returns a data object representing a HID report containing Game Controller
            data for the given parameters.
 @param     Each parameter refers to the value for each control on a Game Controller.
 */
- (NSData *)dataForGameControllerHIDReportWithDPadUp:(UInt8)dPadUp
                                                    dPadDown:(UInt8)dPadDown
                                                    dPadLeft:(UInt8)dPadLeft
                                                   dPadRight:(UInt8)dPadRight
                                                     aButton:(UInt8)aButton
                                                     bButton:(UInt8)bButton
                                                     xButton:(UInt8)xButton
                                                     yButton:(UInt8)yButton
                                         leftShoulderButton1:(UInt8)leftShoulderButton1
                                        rightShoulderButton1:(UInt8)rightShoulderButton1
                                         leftShoulderButton2:(UInt8)leftShoulderButton2
                                        rightShoulderButton2:(UInt8)rightShoulderButton2
                                                 pauseButton:(BOOL)pauseButton
                                              joystickRightX:(SInt8)joystickRightX
                                              joystickRightY:(SInt8)joystickRightY
                                               joystickLeftX:(SInt8)joystickLeftX
                                               joystickLeftY:(SInt8)joystickLeftY;

@end
