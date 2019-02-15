//
//  CaptureDocumentVideoPlayer.h
//  ATSMacApp
//
//  Created by Natália Cabral on 2/5/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <AVKit/AVKit.h>

/*!
    Defines the delegate protocol used by the CaptureVideoPlayer during the capture video playback.
 */
@protocol CaptureVideoPlayerDelegate <NSObject>

@required
/*!
    Expands the capture video playback to a separate window
 */
- (void)openVideoWindow;
@end

@interface CaptureVideoPlayer : AVPlayerView

/*!
    The delegate of the CaptureVideoPlayer object. Must conform to CaptureVideoPlayerDelegate protocol.
 */
@property (strong) id<CaptureVideoPlayerDelegate> delegate;

@end
