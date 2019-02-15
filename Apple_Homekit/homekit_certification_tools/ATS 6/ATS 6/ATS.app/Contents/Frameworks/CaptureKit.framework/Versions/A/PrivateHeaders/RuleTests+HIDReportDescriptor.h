//
//  RuleTests+HIDReportDescriptor.h
//  ATSMacApp
//
//  Created by Joe Basu on 11/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>
#import <BusSniffer/BusSniffer.h>

@interface RuleTests (HIDReportDescriptor)

- (NSData *)dataForItemWithTag:(BSUSBHIDReportDescriptorItemTag)tag type:(BSUSBHIDReportDescriptorItemType)type;
- (NSData *)dataForItemWithTag:(BSUSBHIDReportDescriptorItemTag)tag type:(BSUSBHIDReportDescriptorItemType)type signedValue:(SInt32)value;
- (NSData *)dataForItemWithTag:(BSUSBHIDReportDescriptorItemTag)tag type:(BSUSBHIDReportDescriptorItemType)type unsignedValue:(UInt32)value;

- (NSData *)dataForUsagePageItem:(UInt32)usagePage;
- (NSData *)dataForUsageItem:(UInt32)usage;
- (NSData *)dataForUsageMinimumItem:(UInt32)usageMinimum;
- (NSData *)dataForUsageMaximumItem:(UInt32)usageMaximum;
- (NSData *)dataForCollectionItem:(UInt32)collection;
- (NSData *)dataForLogicalMinimumItem:(SInt32)logicalMinimum;
- (NSData *)dataForLogicalMaximumItem:(SInt32)logicalMaximum;
- (NSData *)dataForReportSizeItem:(UInt32)reportSize;
- (NSData *)dataForReportCountItem:(UInt32)reportCount;
- (NSData *)dataForInputItem:(UInt32)inputBitfield;
- (NSData *)dataForOutputItem:(UInt32)outputBitfield;
- (NSData *)dataForEndCollectionItem;
- (NSData *)dataForReportIDItem:(UInt32)reportID;

/*!
 @method    dataForInterfaceDescriptorHIDWithInterfaceNumber:descriptorLength:
 @abstract  Returns default data for a USB interface descriptor for the HID class.
 */
- (NSData *)dataForInterfaceDescriptorHIDWithInterfaceNumber:(UInt8)interfaceNumber descriptorLength:(UInt16)descriptorLength;

/*!
 @method    dataForInterfaceDescriptorHIDWithInterruptInOutAndInterfaceNumber:descriptorLength:
 @abstract  Returns default data for a USB interface descriptor for the HID class with both Interrupt In and Interrupt Out endpoints.
 */
- (NSData *)dataForInterfaceDescriptorHIDWithInterruptInOutAndInterfaceNumber:(UInt8)interfaceNumber descriptorLength:(UInt16)descriptorLength;

/*
 @method    dataForInterfaceDescriptorAudioWithInterfaceNumber:descriptorLength:
 @abstract  Returns default data for a USB interface descriptor for the Audio class.
 */
- (NSData *)dataForInterfaceDescriptorAudioWithInterfaceNumber:(UInt8)interfaceNumber descriptorLength:(UInt16)descriptorLength;

/*!
 @method    dataForInterfaceDescriptorHIDWithInterfaceNumber:descriptorLength:interruptInEndpointAddress:
 @abstract  Returns default data for a USB interface descriptor for the HID class.
 @param     interfaceNumber
            The interface number of the HID descriptor.
 @param     descriptorLength
            The length of the descriptor data.
 @param     endpointAddress
            The Interrupt In endpoint address the HID reports will be sent over.
 */
- (NSData *)dataForInterfaceDescriptorHIDWithInterfaceNumber:(UInt8)interfaceNumber descriptorLength:(UInt16)descriptorLength interruptInEndpointAddress:(UInt8)endpointAddress;

/*!
 @method    dataForInterfaceDescriptorHIDWithInterfaceNumber:descriptorLength:interruptInEndpointAddress:interruptOutEndpointAddress:
 @abstract  Returns default data for a USB interface descriptor for the HID class.
 @param     interfaceNumber
            The interface number of the HID descriptor.
 @param     descriptorLength
            The length of the descriptor data.
 @param     endpointInAddress
            The Interrupt In endpoint address the HID reports will be sent over.
 @param     endpointOutAddress
            The Interrupt Out endpoint address the HID reports will be sent over.
 */
- (NSData *)dataForInterfaceDescriptorHIDWithInterfaceNumber:(UInt8)interfaceNumber descriptorLength:(UInt16)descriptorLength interruptInEndpointAddress:(UInt8)endpointInAddress interruptOutEndpointAddress:(UInt8)endpointOutAddress;

/*!
 @method    dataForHIDReportDescriptorUsingBlock:
 @abstract  Returns data for a USB HID report descriptor. The descriptor data is constructed by appending data to 
            the mutable data object passed to the block.
 */
- (NSData *)dataForHIDReportDescriptorUsingBlock:(void (^)(NSMutableData *hidReportDescriptorData))block;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsages
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with all 
            permitted usages.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsages;

/*!
    Convenience method to return data for a HID report descriptor for function Keyboard with some prohibited range usages.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithProhibitedRangeUsages;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndReportID:
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with all
            permitted usages, as well as a single Report ID.
 @param     reportID
            The report ID to use
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndReportID:(UInt32)reportID;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndRepeatedReportID:
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with all
            permitted usages, as well repeated occurrances of a single Report ID.
 @param     reportID
            The report ID to use
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndRepeatedReportID:(UInt32)reportID;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndReportID:additionalInputReportID:
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with all
            permitted usages, as well as a second keyboard report with a different report ID.
 @param     reportID
            The report ID for the first keyboard report
 @param     reportID
            The report ID for the second keyboard report
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndReportID:(UInt32)reportID additionalInputReportID:(UInt32)additionalReportID;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndReportID:additionalOutputReportID:
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with all
            permitted usages, as well the occurrance of two different Report IDs.
 @param     reportID
            The report ID to use
 @param     additionalReportID
            The output report ID to use in addition to the keyboard input 
            report's ID.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndReportID:(UInt32)reportID additionalOutputReportID:(UInt32)additionalReportID;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndAdditionalUsagePage
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with all 
            permitted usages and an additional usage page.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndAdditionalUsagePage;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesBetweenMultipleUsagePages
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with
            permitted usages that are broken up between multiple usage page declarations.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesBetweenMultipleUsagePages;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndNonZeroLogicalMinArray
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with
            permitted usages where the Array field's Logical Min does not start at 0.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndNonZeroLogicalMinArray;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithSingleProhibitedConsumerPageUsage
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with all
            permitted usages and an additional prohibited usage.
 @param     usageID
            The prohibited consumer page usageID.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithSingleProhibitedConsumerPageUsage:(UInt16)usageID;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithProhibitedUsagesOne
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with all
 permitted usages and an additional prohibited usage.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithProhibitedUsagesOne;

/*!
 @method    dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesMutliple
 @abstract  Convenience method to return data for a USB HID report descriptor for function keyboard with all
            permitted usages and multiple prohibited usages.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithProhibitedUsagesMutliple;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithProhibitedUsagesAndAdditionalUsagePage
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with 
            prohibited usages and an additional usage page.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithProhibitedUsagesAndAdditionalUsagePage;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithProhibitedUsagesBetweenMultipleUsagePages
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with 
            prohibited usages that are broken up between multiple usage page declarations.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithProhibitedUsagesBetweenMultipleUsagePages;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithButtonFieldReportCount:usageCount:
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with
            permitted data and Button fields with the given reportCount and usage Count.
 @param     reportCount
            The value to set the Button field's report count to.
 @param     usageCount
            The number of usages to supply for the Button fields.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithButtonFieldReportCount:(UInt32)reportCount usageCount:(UInt32)usageCount;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndReportIDAfterFirstControlItem:
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with
            permitted usages and a Report ID after the first given control item.
 @param     reportType
            The reporttype of the control item that the report ID comes after.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndReportIDAfterFirstControlItem:(BSUSBHIDReportType)reportType;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesWithExtendedUsageMin:extendedUsageMax:
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with
            extended usage min/max depending on the params given.
 @param     hasExtendedUsageMin
            Whether the usage min should be declared extended or not.
 @param     hasExtendedUsageMax
            Whether the usage max should be declared extended or not.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesWithExtendedUsageMin:(BOOL)hasExtendedUsageMin extendedUsageMax:(BOOL)hasExtendedUsageMax;

/*!
 @method    dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndNestedNonApplicationAndTopLevelCollectionApplication:
 @abstract  Convenience method to return data for a USB HID report descriptor for function Keyboard with
            a top level collection that is an Application or not depending on the param.
 @param     hasApplicationCollection
            Whether the top level collection should be an Application or not.
 */
- (NSData *)dataForKeyboardHIDReportDescriptorWithPermittedUsagesAndNestedNonApplicationAndTopLevelCollectionApplication:(BOOL)hasApplicationCollection;

/*!
 @method    dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedUsages
 @abstract  Convenience method to return data for a USB HID report descriptor for function Media Playback Remote
            with all permitted usages.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedUsages;

/*!
    Convenience method to return data for a HID report descriptor for function Media Playback Remote
    with all permitted usages as array fields.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedRangeUsages;

/*!
 	Convenience method to return data for a HID report descriptor for function Media Playback Remote
 	with all prohibited usages as array fields.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedRangeUsages;

/*!
 @method    dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedUsagesAndAdditionalUsagePage  
 @abstract  Convenience method to return data for a USB HID report descriptor for function Media Playback Remote
            with permitted usages and an additional usage page.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedUsagesAndAdditionalUsagePage;

/*!
 @method    dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedUsagesBetweenMultipleUsagePages
 @abstract  Convenience method to return data for a USB HID report descriptor for function Media Playback Remote
            with permitted usages that are broken up between multiple usage page declarations.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithPermittedUsagesBetweenMultipleUsagePages;

/*!
 @method    dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesOne
 @abstract  Convenience method to return data for a USB HID report descriptor for function Media Playback Remote
            with one prohibited usage.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesOne;

/*!
 @method    dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesMutliple
 @abstract  Convenience method to return data for a USB HID report descriptor for function Media Playback Remote
            with multiple prohibited usages.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesMutliple;

/*!
 @method    dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesAndAdditionalUsagePage
 @abstract  Convenience method to return data for a USB HID report descriptor for function Media Playback Remote
            with prohibited usages and an additional usage page.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesAndAdditionalUsagePage;

/*!
 @method    dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesBetweenMultipleUsagePages
 @abstract  Convenience method to return data for a USB HID report descriptor for function Media Playback Remote
            with prohibited usages that are broken up between multiple usage page declarations.
 */
- (NSData *)dataForMediaPlaybackRemoteHIDReportDescriptorWithProhibitedUsagesBetweenMultipleUsagePages;

/*!
 @method    dataForAssistiveTouchPointerHIDReportDescriptorWithRequiredUsagePageAndRequiredUsage
 @abstract  Convenience method to return data for a USB HID report descriptor for function AssistiveTouch 
            Pointer that declares support for both the HID Generic Desktop usage page and the Mouse usage.
 */
- (NSData *)dataForAssistiveTouchPointerHIDReportDescriptorWithRequiredUsagePageAndRequiredUsage;

/*!
 @method    dataForAssistiveTouchPointerHIDReportDescriptorWithRequiredUsagePageAndRequiredUsageAndAdditionalUsagePage
 @abstract  Convenience method to return data for a USB HID report descriptor for function AssistiveTouch 
            Pointer that declares support for both the HID Generic Desktop usage page and the Mouse usage, as 
            well as an additional usage page.
 */
- (NSData *)dataForAssistiveTouchPointerHIDReportDescriptorWithRequiredUsagePageAndRequiredUsageAndAdditionalUsagePage;

/*!
 @method    dataForAssistiveTouchPointerHIDReportDescriptorWithRequiredUsagePageWithoutRequiredUsage
 @abstract  Convenience method to return data for a USB HID report descriptor for function AssistiveTouch 
            Pointer that declares support for the HID Generic Desktop usage page but not the Mouse usage.
 */
- (NSData *)dataForAssistiveTouchPointerHIDReportDescriptorWithRequiredUsagePageWithoutRequiredUsage;

/*!
 @method    dataForAssistiveSwitchControlHIDReportDescriptor
 @abstract  Convenience method to return data for a USB HID report descriptor for function Assistive Switch
 Control
 */
- (NSData *)dataForAssistiveSwitchControlHIDReportDescriptor;

/*!
 @method    dataForAssistiveSwitchControlHIDReportDescriptorWithNumberOfButtons:usesUsageSeparated:usesAdditionalUsages
 @param     numberOfButtons     The number of buttons that shoulb be declared at the descriptor.
 @param     usesUsageSeparated  A boolean that represents if the usages will be tested using usage range approach or separated usage approach.
 @param     usesAdditionalUsages  A boolean that represents if the test will use only button usages or uses another type of usage too.
 @abstract  Convenience method to return data for a USB HID report descriptor for function Assistive Switch
 control that declares the specified number of buttons;
 */
- (NSData *)dataForAssistiveSwitchControlHIDReportDescriptorWithNumberOfButtons:(UInt32)numberOfButtons usesUsageSeparated:(BOOL)usesUsageSeparated usesAdditionalUsages:(BOOL)usesAdditionalUsages;

/*!
 @method    dataForHIDReportDescriptorWithLEDUsagePage
 @abstract  Convenience method to return data for a USB HID report descriptor that only declares support for the 
            LED usage page.
 */
- (NSData *)dataForHIDReportDescriptorWithLEDUsagePage;

/*!
 @method    dataForHIDReportDescriptorWithInput:usage:usagePage:logicalMin:logicalMax:reportSize:reportCount:;
 @abstract  Convenience method to return data for a USB HID report descriptor that has the items denoted by the params.
 @param     hasInput Whether the Input item is declared.
 @param     hasUsage Whether the usage item is declared.
 @param     hasUsagePage Whether the usage page item is declared.
 @param     hasLogicalMin Whether the logical min item is declared.
 @param     hasLogicalMax Whether the logical max item is declared.
 @param     hasReportSize Whether the report size item is declared.
 @param     hasReportCount Whether the report count item is declared.
 */
- (NSData *)dataForHIDReportDescriptorWithInput:(BOOL)hasInput
                                          usage:(BOOL)hasUsage
                                      usagePage:(BOOL)hasUsagePage
                                     logicalMin:(BOOL)hasLogicalMin
                                     logicalMax:(BOOL)hasLogicalMax
                                     reportSize:(BOOL)hasReportSize
                                    reportCount:(BOOL)hasReportCount;

/*!
 @method    dataForHIDReportDescriptorWithFirstReportSize:secondReportSize:
 @abstract  Convenience method to return data for a USB HID report descriptor that has two report sizes with the given params.
 @param     reportSize1 The value for the first report size.
 @param     reportSize2 The value for the second report size.
 */
- (NSData *)dataForHIDReportDescriptorWithFirstReportSize:(UInt32)reportSize1 secondReportSize:(UInt32)reportSize2;

/*!
 @method    dataForGameControllerHIDReportDescriptor:
 @abstract  Returns HID report descriptor data for Game Controller
 @param     hasLEDArray
            Whether the LED Array is present in the HID report descriptor.
 @param     pauseButtonFieldsAsArray
            Whether the pause button should be Array or Variable.
 @param     lowerShoulderButtonFieldsAsArray
            Whether the lower shoulder button fields should be Array or Variable.
*/
- (NSData *)dataForGameControllerHIDReportDescriptorHasLEDArray:(BOOL)hasLEDArray
                                       pauseButtonFieldsAsArray:(BOOL)pauseButtonFieldsAsArray
                               lowerShoulderButtonFieldsAsArray:(BOOL)lowerShoulderButtonFieldsAsArray;

/*!
 @method    dataForGameControllerHIDReportDescriptor:
 @abstract  Returns HID report descriptor data for Game Controller
 @param     hasLEDArray
            Whether the LED Array is present in the HID report descriptor.
 @param     pauseButtonFieldsAsArray
            Whether the pause button should be Array or Variable.
 @param     lowerShoulderButtonFieldsAsArray
            Whether the lower shoulder button fields should be Array or Variable.
 @param     additionalButtonUsages
            Array of additional button usage IDs (NSNumber)
 */
- (NSData *)dataForGameControllerHIDReportDescriptorHasLEDArray:(BOOL)hasLEDArray
                                       pauseButtonFieldsAsArray:(BOOL)pauseButtonFieldsAsArray
                               lowerShoulderButtonFieldsAsArray:(BOOL)lowerShoulderButtonFieldsAsArray
                                         additionalButtonUsages:(NSArray *)additionalButtonUsages;

/*!
 @method    dataForGameControllerHIDReportDescriptorMissingUsagePage:missingUsageID
 @abstract  Returns HID report descriptor data for with the missing usage missing.
 @param     missingUsagePage
            The usage page for the missing usage.
 @param     missingUsageID
            The usage ID for the missing usage.
*/
- (NSData *)dataForGameControllerHIDReportDescriptorMissingUsagePage:(UInt16)missingUsagePage missingUsageID:(UInt16)missingUsageID;

/*!
 @method    dataForAISpecExampleHIDReportDescriptorKeyboard
 @abstract  Returns HID report descriptor data for the Keyboard example in AISpec.
 */
- (NSData *)dataForAISpecExampleHIDReportDescriptorKeyboard;

/*!
 @method    dataForAISpecExampleHIDReportDescriptorMediaPlaybackRemote
 @abstract  Returns HID report descriptor data for the Media Playback Remote example in AISpec.
 */
- (NSData *)dataForAISpecExampleHIDReportDescriptorMediaPlaybackRemote;

/*!
 @method    dataForAISpecExampleHIDReportDescriptorAssistiveTouchPointer
 @abstract  Returns HID report descriptor data for the AssitiveTouch Pointer example in AISpec.
 */
- (NSData *)dataForAISpecExampleHIDReportDescriptorAssistiveTouchPointer;

/*!
 @method    dataForAISpecExampleHIDReportDescriptorGameController
 @abstract  Returns HID report descriptor data for the Game Controller example in AISpec.
 */
- (NSData *)dataForAISpecExampleHIDReportDescriptorGameController;

/*!
 @method    dataForAISpecExampleHIDReportDescriptorAssistiveSwitchControl
 @abstract  Returns HID report descriptor data for the Switch Control example in AISpec.
 */
- (NSData *)dataForAISpecExampleHIDReportDescriptorAssistiveSwitchControl;

/*!
 @method    dataForAISpecExampleHIDReportDescriptorHeadphone
 @abstract  Returns HID report descriptor data for the Headphone example in AISpec.
 */
- (NSData *)dataForAISpecExampleHIDReportDescriptorHeadphone;

/*!
 @method    - (NSData *)dataForGameControllerHIDReportDescriptorPauseButtonFieldsAsArray:withPauseButtonReportSize:logicalMax:logicalMin:
 @abstract  Returns HID report descriptor for a GameController with the given params.
 @param     pauseButtonFieldsAsArray
            Whether the pause button should be defined as Array type or Variable.
 @param     pauseButtonReportSize
            The Report Size for the Pause button's field.
 @param     pauseButtonLogicalMax
            The Logical Max for the Pause button's field.
 @param     pauseButtonLogicalMin
            The Logical Min for the Pause button's field.
 */
- (NSData *)dataForGameControllerHIDReportDescriptorPauseButtonFieldsAsArray:(BOOL)pauseButtonFieldsAsArray
                                      withPauseButtonReportSize:(UInt32)pauseButtonReportSize
                                                     logicalMax:(SInt32)pauseButtonLogicalMax
                                                     logicalMin:(SInt32)pauseButtonLogicalMin;

/*!
 @method    dataForGameControllerHIDReportDescriptorJoystickFieldsAsArray:withJoystickReportSize:logicalMax:logicalMin:
 @abstract  Returns HID report descriptor for a GameController with the given params.
 @param     joystickFieldAsArray
            Whether the joystick fields should be defined as Array or Variable.
 @param     joystickReportSize
            The Report Size for the Joystick fields.
 @param     joystickLogicalMax
            The Logical Max for the Joystick fields.
 @param     joystickLogicalMin
            The Logical Min for the Joystick fields.
 */
- (NSData *)dataForGameControllerHIDReportDescriptorJoystickFieldsAsArray:(BOOL)joystickFieldsAsArray
                                         withJoystickReportSize:(UInt32)joystickReportSize
                                                     logicalMax:(SInt32)joystickLogicalMax
                                                     logicalMin:(SInt32)joystickLogicalMin;

/*!
 @method    dataForGameControllerHIDReportDescriptor...:
 @abstract  Returns HID report descriptor for a GameController with the given params.
 @param     allInvalid
            If YES, all pressure sensitive fields are invalid.
 @param     pressureSensitiveUsagePage
            The usage page for the pressure sensitive control.
 @param     pressureSensitiveUsageID
            The usage ID for the pressure sensitive control.
 @param     reportSize
            The Report Size for the pressure sensitive field.
 @param     logicalMax
            The Logical Max for the pressure sensitive field.
 @param     logicalMin
            The Logical Min for the pressure sensitive field.
 */
- (NSData *)dataForGameControllerHIDReportDescriptorAllInvalid:(BOOL)allInvalid
                                     pressureSensitiveUsagePage:(UInt16)pressureSensitiveUsagePage
                                       pressureSensitiveUsageID:(UInt16)pressureSensitiveUsageID
                                                     reportSize:(UInt32)reportSize
                                                     logicalMax:(SInt32)logicalMax
                                                     logicalMin:(SInt32)logicalMin;
/*!
 @method    dataForGameControllerHIDReportDescriptor...
 @abstract  Returns HID report descriptor for a GameController with the given params.
 @param     badJoystickUsageID
            The joystick usage ID to apply the bad ReportSize to.
 @param     badJoystickReportSize
            The Report Size for the bad Joystick field.
 @param     badJoystickLogicalMax, badJoystickLogicalMin
            The Logical Max/Min for the bad Joystick field.
 @param     joystickReportSize
            The Report Size for the good fields.
 @param     joystickLogicalMax, joystickLogicalMin
            The Logical Max/Min for the good Joystick field.
 */
- (NSData *)dataForGameControllerHIDReportDescriptorBadJoystickUsageID:(UInt8)badJoystickUsageID
                                      withBadJoystickReportSize:(UInt32)badJoystickReportSize
                                                     logicalMax:(SInt32)badJoystickLogicalMax
                                                     logicalMin:(SInt32)badJoystickLogicalMin
                                         goodJoystickReportSize:(UInt32)joystickReportSize
                                                     logicalMax:(SInt32)joystickLogicalMax
                                                     logicalMin:(SInt32)joystickLogicalMin;

/*!
 @method    dataForGameControllerHIDReportDescriptor...
 @abstract  Returns HID report descriptor for a GameController with the given params.
 @param     ledReportType
            The LED field report type.
 @param     includeLEDUsages
            Whether the LED fields should be included or not.
 @param     ledFieldsAsArray
            Whether the field should be of Array type or Variable.
 @param     ledDefinedAsSingleUsages
            Whether single usages should be used or a usage min/max range.
 */
- (NSData *)dataForGameControllerHIDReportDescriptorForLEDReportType:(BSUSBHIDReportType)ledReportType
                                               includeLEDUsages:(BOOL)includeLEDUsages
                                               ledFieldsAsArray:(BOOL)ledFieldsAsArray
                                       ledDefinedAsSingleUsages:(BOOL)ledDefinedAsSingleUsages;

/*!
    Returns HID report descriptor for a GameController with the given report Size for LED array field.
    @param     reportSize
    The LED array field report size.
    @return    HID report descriptor data
 */
- (NSData *)dataForGameControllerHIDReportDescriptorWithLEDArrayReportSize:(UInt8)reportSize;

@end
