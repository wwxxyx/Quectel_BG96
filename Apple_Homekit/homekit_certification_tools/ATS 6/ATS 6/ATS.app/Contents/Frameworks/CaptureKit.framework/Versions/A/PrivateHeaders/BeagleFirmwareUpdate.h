//
//  BeagleFirmwareUpdate.h
//  ATSMacApp
//
//  Created by Kip Nicol on 4/2/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/BeagleUSBCaptureDevice.h>

@class BeagleFirmwareUpdate;

@protocol BeagleFirmwareUpdaterDelegate <NSObject>

/*!
 @method    firmwareUpdateDidFinish:
 @abstract  Called when the update has finished successfully.
 @param     update
            The BeagleFirmwareUpdate object.
 */
- (void)beagleFirmwareUpdateDidFinish:(BeagleFirmwareUpdate *)update;

/*!
 @method    firmwareUpdate:didFailWithError:
 @abstract  Called when there was an error during the update process.
 @param     update
            The BeagleFirmwareUpdate object.
 @param     error
            The error object containing the failure reason.
 */
- (void)beagleFirmwareUpdate:(BeagleFirmwareUpdate *)update didFailWithError:(NSError *)error;

/*!
 @method    beagleFirmwareUpdate:didLogOutput:
 @abstract  Called when the updater output logging data.
 @param     update
            The BeagleFirmwareUpdate object.
 @param     output
            The string containing the log output.
 @discussion Only gets sent if enableBeagleUpdateLogging is set to YES.
 */
- (void)beagleFirmwareUpdate:(BeagleFirmwareUpdate *)update didLogOutput:(NSString *)output;

@end

/*!
 @class     BeagleFirmwareUpdate
 @abstract  Used to update the firmware on the Total Phase Beagle 480.
 */
@interface BeagleFirmwareUpdate : NSObject

/*!
 Initialize with the type of the beagle that is being updated
 
 @param beagleType The beagle type to be updated. Only Beagle 480 is supported at the moment.
 
 @return A new BeagleFirmwareUpdate instance
 */

- (instancetype)initWithBeagleType:(ATSUSBBeagleType)beagleType;

/*!
 @method    delegate
 @abstract  Delegate for the BeagleFirmwareUpdate. Must conform to BeagleFirmwareUpdaterDelegate.
 */
@property (nonatomic, assign) id<BeagleFirmwareUpdaterDelegate> delegate;

/*!
 @property  enableBeagleFirmwareUpdateLogging
 @abstract  Set to YES if to enable output sent from the updater to the beagleFirmwareUpdate:idLogOutput:
            delegate method.
 */
@property (nonatomic, assign) BOOL enableBeagleFirmwareUpdateLogging;

/*!
 @method    runFirmwareUpdate
 @abstract  Begins the execution of the firmware update.
 @discussion Due to the limitations of wrapping the update cli tool given
             to us by Total Phase, only one Beagle must be connected. Otherwise
             the update will fail.
             This is an asyncronous call and will not block. Use the BeagleFirmwareUpdaterDelegate
             methods to handle the udpate finish event.
 */

- (void)runFirmwareUpdate;

@end
