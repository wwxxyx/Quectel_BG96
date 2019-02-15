//
//  ATSMikeyBusCaptureImporter.h
//  
//
//  Created by Wesley McCloy on 12/2/15.
//
//

#import <CaptureKit/ATSCaptureImporter.h>

// This is used when launching the import. It is the name of the imported file within the .ats (.zip)
extern NSString *const ATSMikeyBusImportFilename;

/*!
 This class handles the importing of a file parsed as MikeyBus data
 */
@interface ATSMikeyBusCaptureImporter : ATSCaptureImporter

@end
