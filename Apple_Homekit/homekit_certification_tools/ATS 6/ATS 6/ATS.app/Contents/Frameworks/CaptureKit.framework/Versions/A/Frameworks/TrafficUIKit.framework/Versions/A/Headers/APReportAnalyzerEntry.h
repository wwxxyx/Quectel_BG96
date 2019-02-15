//
//  APReportAnalyzerEntry.h
//  APTS
//
//  Created by Mark Hamlin on 12/17/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import <TrafficUIKit/TKAttributedString.h>

typedef enum : uint8_t {
    APReportAnalyzerEntryNormalTextTitle,     // Normal text title
    APReportAnalyzerEntryFixedPitchTextTitle, // Fixed pitch font title
    APReportAnalyzerEntryCustomTitle,         // Custom title, such as a non-default cell or view controller
} APReportAnalyzerEntryTitleMode;

@protocol APReportAnalyzerEntryTitleProvider

/*!
 @method        reportAnalyzerEntryTitleMode
 @abstract      The title mode of the entry
 @discussion    Used so the view controller can calculate height without requesting the actual entry
 */
- (APReportAnalyzerEntryTitleMode)reportAnalyzerEntryTitleMode;

@end

@class APReportAnalyzerEntry;
@protocol APReportAnalyzerEntrySource <APReportAnalyzerEntryTitleProvider, NSObject>
@required

/*!
 @macro     AP_LAST_IS_SEPARATOR
 @abstract  Returns whether or not the last menu item is a separator item
 */
#define AP_LAST_IS_SEPARATOR(menu) ((menu.itemArray.count && ((NSMenuItem *)menu.itemArray.lastObject).isSeparatorItem))

/*!
 @method    reportAnalyzerEntryUsingCache
 @abstract  Return a new report analyzer entry, using a cached version if possible
 */
- (APReportAnalyzerEntry *)reportAnalyzerEntryUsingCache;

/*!
 @method    hasCachedReportAnalyzerEntry
 @abstract  Whether or not there is a cached report analyzer entry present
 */
- (BOOL)hasCachedReportAnalyzerEntry;

/*!
 @method    clearCachedReportAnalyzerEntry
 @abstract  Clears the cached entry, if it is present.
 */
- (void)clearCachedReportAnalyzerEntry;

/*!
 The timestamp in nanoseconds
 */
- (UInt64)reportAnalyzerTimestamp;

/*!
 The system timestamp of the event since 1970
 */
- (UInt64)reportAnalyzerTimestampSince1970;

/*!
 The source description string for this entry
 */
- (NSString *)reportAnalyzerSourceString;

/*!
 @method        pasteboardString
 @abstract      Returns the pasteboard string for the event
 */
- (NSString *)pasteboardString;

/*!
 @method        filterShouldIncludeEntryUsingInclusiveWords:exclusiveWords:
 @abstract      Whether or not the filter should include the entry for this given inclusive/exclusive words
 */
- (BOOL)filterShouldIncludeEntryUsingInclusiveWords:(NSArray *)inclusive exclusiveWords:(NSArray *)exclusive;

/*!
 @method        representativeFilterText
 @abstract      Text which should be searched over to implement the filter
 @discussion    This is optional and will only be called once per source. If non-nil, -filterShouldIncludeEntryUsingInclusiveWords:exclusiveWords: will not be used
 */
- (NSString *)representativeFilterText;

/*!
 @property  displayedFilterIndex
 @abstract  The index which this source lives at in the filter's display table
 */
@property (nonatomic, assign) NSInteger displayedFilterIndex;

/*!
 @method        selectedReportAnalyzerEntries:willPresentMenu:
 @abstract      Opportunity for class which sourced these entries to present options in a right click menu
 @discussion    The selected entries get split up by the classes of their source, and then each class will receive this message.
                It is guaranteed that each entry's -source will be of the same class
 */
+ (void)selectedReportAnalyzerEntries:(NSArray *)entries willPresentMenu:(NSMenu *)menu;

/*!
 @method        hasResourcesLoaded
 @abstract      Whether or not resources are currently loaded
 */
- (BOOL)hasResourcesLoaded;

/*!
 @method        swapResourcesOut
 @abstract      Opportunity for resources to be swapped out
 @discussion    This is an indication that source functionality will not be requested
 */
- (void)swapResourcesOut;

/*!
 @method        swapResourcesInWhenAvailable
 @abstract      Indication that resources may be needed soon
 */
- (void)swapResourcesInWhenAvailable;

/*!
 @method        ensureResourcesSwappedIn
 @abstract      Ensures that resources are swapped in. If not in, block and swap them in.
 */
- (void)ensureResourcesSwappedIn;

@optional

/*!
 @method    updateCellObjectValueTo:
 @abstract  Updates the object value of the cell (if it was editable)
 */
- (void)updateCellObjectValueTo:(id)objectValue;

/*!
 @method    cellObjectValue
 @abstract  Returns the object value of the cell. Used instead of cellObjectValue property on the analyzer entry
 */
- (id)cellObjectValue;

/*!
 @method    rowTextColor
 @abstract  Returns the color of the text for the whole row.
 */
- (NSColor *)rowTextColor;

/*!
@method    relatedEntryColor
@abstract  Returns the highlighting color of the related entries.
*/
- (NSColor *)relatedEntryColor;

/*!
 Returns a single line string representative of this entry, suited for the copy result of a table.
 @return The appropriately formatted string for a copy operation
 */
- (NSString *)simplePasteBoardString;

@end

@interface APReportAnalyzerEntry : NSObject <APReportAnalyzerEntryTitleProvider>

@property (nonatomic, assign) BOOL isRootLevel;

#pragma mark - Display and evaluation

/**
 Often an entry is expressed as Key: Value, this property is thus set. Enables searching entries
 */
@property (nonatomic, strong) NSString *keyString;

/**
 Often an entry is expressed as Key: Value, this property is thus set. Enables turning the entry tree into a dictionary
 */
@property (nonatomic, strong) NSString *valueString;

/**
 The source creating this entry can attach an object for use when turning the entry tree into a dictionary
 */
@property (nonatomic, strong) NSObject *valueObject;

/**
 A short for auxiliary title string providing supplementary information.
 */
@property (nonatomic, strong) NSString *auxTitleString;


/**
 Set to YES to exclude the entry from creating a pasteboard string.
 */
@property (nonatomic) BOOL excludeFromPasteboardString;

#pragma mark -

/*!
 @method    pasteboardStringWithIdentation:
 @abstract  Recursively creates the pasteboard string block with the given indentation string
 */
- (NSString *)pasteboardStringWithIdentation:(NSString *)indentation;

/*!
 @property  title
 @abstract  Title line
 */
@property (nonatomic, strong) TKAttributedString *title;

/*!
 @property  displayUsingFixedPitchFont
 @abstract  Whether or not the title should be displayed using a fixed pitch (monospace) font
 */
@property (nonatomic, assign) BOOL displayUsingFixedPitchFont;

/*!
 @property  useGroupStyle
 @abstract  If the group row style should be used
 */
@property (nonatomic, assign) BOOL useGroupStyle;

/*!
 @property      source
 @abstract      The source of this entry. This may be nil.
 */
@property (nonatomic, weak) id<APReportAnalyzerEntrySource> source;

/*!
    If this entry's source is related to other entries' sources they will be stored here. This may be nil.
 */
@property (nonatomic, strong) NSArray *relatedEntrySources;

/*!
 @method    addEntry:
 @abstract  Add an entry
 */
- (void)addEntry:(APReportAnalyzerEntry *)entry;

/*!
 @method    addEntryLine:
 @abstract   Adds a single-line entry
 */
- (APReportAnalyzerEntry *)addEntryLine:(TKAttributedString *)line;

/*!
 @method    addEntryLineString:
 @abstract   Adds a single-line entry
 */
- (APReportAnalyzerEntry *)addEntryLineString:(NSString *)string;

/*!
 @method    addFixedPitchEntryLine:
 @abstract   Adds a single-line entry displayed using a fixed pitch font
 */
- (APReportAnalyzerEntry *)addFixedPitchEntryLine:(TKAttributedString *)line;

/*!
 @method    addNestedEntry
 @abstract  Creates and returns a nested entry
 */
- (APReportAnalyzerEntry *)addNestedEntry;

/*!
 @method    addNestedEntryToTop
 @abstract  Creates and returns a nested entry above existing ones
 */
- (APReportAnalyzerEntry *)addNestedEntryToTop;

/*!
 @method        addEntryWithData:
 @abstract      Creates and returns a nested entry which displays the data in a hex view
 @discussion    All the caller needs to do for this is assign a title
 */
- (APReportAnalyzerEntry *)addEntryWithData:(NSData *)data;

/*!
 @method        addEntriesToTopFromOther:
 @abstract      Steals the other entry's entries and makes them our own
 */
- (void)addEntriesToTopFromOther:(APReportAnalyzerEntry *)other;

/**
 The more efficient way of checking if the entry has children. This avoids loading the children
 */
- (BOOL)hasChildren;

/*!
 How many child entries there are.
 
 WARNING: Loads the children by calling `childLoaderBlock`
 */
- (NSUInteger)numChildren;

/*!
 @method    children
 @abstract  Children of this object (may be nil)
 */
- (NSArray *)children;

/**
 The entire tree of children and descendants, recurses.

 @return An array containing all descendants, nil if there are no descendants.
 */
- (NSArray *)allDescendants;

/*!
 For deferred loading of children, specify a block to be invoked when the children are needed
 @discussion    This dramatically helps performance. Use it for events which happen frequently
 */
@property (nonatomic, copy) void (^childLoaderBlock)(APReportAnalyzerEntry *);

/*!
 For deferred loading of previous layer's event-entries, specify a block to be invoked when the children are needed. In order to append previous layer's event-entries, this block is always invoked after loading the childLoaderBlock.
 @discussion    This dramatically helps performance. Use it for events which happen frequently
 
 @note Currently this block is implicitly set in CaptureEvent base class when there are previous layer events (A.K.A when the PreviousLayerEvents property has a non-zero count).
 */
@property (nonatomic, copy) void (^previousLayerEntryLoaderBlock)(APReportAnalyzerEntry *);

/*!
 For deferred loading of next layer's event-entries, specify a block to be invoked when the children are needed. In order to append next layer's event-entries, this block is always invoked after loading the childLoaderBlock.
 @discussion    This dramatically helps performance. Use it for events which happen frequently
 
 @note Currently this block is implicitly set in CaptureEvent base class when there are next layer events (A.K.A when the NextLayerEvents property has a non-zero count).
 */
@property (nonatomic, copy) void (^nextLayerEntryLoaderBlock)(APReportAnalyzerEntry *);


#pragma mark Dependencies

/*!
 @property dependencies
 @abstract Set of ids which this entry is dependent upon
 */
@property (nonatomic, strong) NSSet *dependencies;

/*!
 @property  dependencyIdentifier
 @abstract  An (arbitrary) identifier used for determining dependency relationships
 */
@property (nonatomic, assign) NSUInteger dependencyIdentifier;

#pragma mark -
#pragma mark Custom Views

// *** This may be set if title is nil

/*!
 @property      viewControllerBlock
 @abstract      Block responsible for creating a view controller for this entry
 @discussion    If this entry should have 
 */
@property (nonatomic, copy) NSViewController * (^viewControllerBlock)(APReportAnalyzerEntry *);

#pragma mark -
#pragma mark Custom Cell Drawing

// *** These may be set if title is nil and viewControllerBlock is nil

/*!
 @property  cellClass
 @abstract  The type of cell to create to render this entry. If using this, you must not use title
 */
@property (nonatomic, strong) Class cellClass; // Defaults to NSCell

/*!
 @property  cellInitializer
 @abstract  Specify a block to get called back with the custom cell after initialization
 */
@property (nonatomic, copy) void (^cellInitializer)(id);

/*!
 @property  cellObjectValue
 @abstract  The cell's object value
 */
@property (nonatomic, strong) id cellObjectValue;

/*!
 @property  preferredCellHeight
 @abstract  The cell height which the custom cell should be rendered in
 @default   This may be 0.0, in which case the outline view's default row height will be used
 */
@property (nonatomic, assign) CGFloat preferredCellHeight;

#pragma mark - 
#pragma mark Title Interactivity

/*!
 @property isTitleInteractive
 @abstract Whether the text field that displays the title should intercept clicks.
 */
@property (nonatomic, assign) BOOL isTitleInteractive;

/*!
 @property      linkHandler
 @abstract      Code to be run when a link within this entry's title is clicked.
 */
@property (nonatomic, strong) BOOL (^linkHandler)(NSString *);

/*!
 @method        handleTitleLinkClicked:
 @abstract      Allows the entry object to handle a link-click interaction with 
                its title.
 @param         destination
                The URL string to which the link points.
 */
-(BOOL)handleTitleLinkClicked:(NSString *)destination;

@end
