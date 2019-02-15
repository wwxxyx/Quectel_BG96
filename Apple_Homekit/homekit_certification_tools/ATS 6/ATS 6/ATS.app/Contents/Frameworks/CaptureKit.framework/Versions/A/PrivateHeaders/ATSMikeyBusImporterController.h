//
//  ATSMikeyBusImporterController.h
//  
//
//  Created by Wesley McCloy on 12/2/15.
//
//

#import <Foundation/Foundation.h>

/*!
 This class serves as an intermediary for the AppDelegate to create a MikeyBusCaptureImporter and open a Capture Document to accompany it.
 */
@interface ATSMikeyBusImporterController : NSObject

/*!
 This method start the import process and parsing process to create a Capture Document for MikeyBus
 
 @param url A NSURL corresponding to the file imported to be parsed
 */
+ (void)importFromURL:(NSURL *)url;

@end
