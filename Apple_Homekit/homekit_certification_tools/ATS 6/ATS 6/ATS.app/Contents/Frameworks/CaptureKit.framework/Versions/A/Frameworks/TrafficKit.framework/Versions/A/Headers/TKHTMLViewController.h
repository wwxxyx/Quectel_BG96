//
//  TKHTMLViewController.h
//  TrafficKit
//
//  Created by Mark Hamlin on 4/29/14.
//  Copyright (c) 2014 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface TKHTMLViewController : NSViewController

/*!
 Restores saved state such as text field values
 @discussion This is useful between application launches
 */
- (void)restorePersistentState:(NSDictionary *)persistentState;

/*!
 Returns persistent state for fields between reloads
 */
- (NSDictionary *)persistentState;

#pragma mark -
#pragma mark Subclassers

/*!
 Returns the path to the css file to use
 @default TKHTMLViewController.css
 */
- (NSString *)cssFilePath;

/*!
 Trigger a reload of the HTML
 */
- (void)reloadHTML;

/*!
 Opportunity to regenerate the document's HTML (subclassers)
 */
- (void)reloadBodyHTML;

#pragma mark -
#pragma mark HTML Generation

/*!
 Defers setting innerText on an element
 @return    The element id
 */
- (NSString *)deferTextContent:(NSString *)text;

/*!
 Generates arbitrary HTML
 */
- (void)generateHTML:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);

/*!
 Generates a centered banner with passed subtext
 */
- (void)generateBannerText:(NSString *)banner subtext:(NSString *)subtext;

/*!
 Open a section
 */
- (void)generateOpenSection:(NSString *)title;

/*!
 Close a section
 */
- (void)generateCloseSection;

/*!
 Add a note below a close section
 */
- (void)generateSectionNote:(NSString *)note;

/*!
 Add a note which can be placed around labels
 */
- (void)generateLabelNote:(NSString *)note;

/*!
 Open a table
 */
- (void)generateOpenTable;

/*!
 Open a table with header titles
 */
- (void)generateOpenTableWithHeaders:(NSArray *)headers;

/*!
 Generates table td values
 */
- (void)generateTableColumns:(NSArray *)columns;

/*!
 Close a table
 */
- (void)generateCloseTable;

/*!
 Add a labeled row to a table
 */
- (void)generateTableRowWithLabel:(NSString *)label value:(NSString *)value;
- (void)generateTableRowWithLabel:(NSString *)label value:(NSString *)value style:(NSString *)style;

/*!
 Add a labeled row to a table, with the value being generated inside the callback
 */
- (void)generateTableRowWithLabel:(NSString *)label valueGenerator:(void (^) (void))block;

/*!
 Add a button to call the passed selector
 Returns the button's identifier
 */
- (NSString *)generateButton:(NSString *)label action:(SEL)action;
- (NSString *)generateButton:(NSString *)label action:(SEL)action context:(id)context;
- (NSString *)generateButton:(NSString *)label action:(SEL)action context:(id)context style:(NSString *)style;
- (NSString *)generateButton:(NSString *)label action:(SEL)action context:(id)context style:(NSString *)style enabled:(BOOL)enabled;

/*!
 Generate a checkbox
 @discussion    changed block will be called once with the initial state, and then
                each time the state changes
 */
- (void)generateCheckbox:(NSString *)label name:(NSString *)name valueChangeBlock:(void (^) (BOOL))changed;

/*!
 Enable/disable a button with the passed identifier
 */
- (void)setEnabled:(BOOL)enabled forButtonWithIdentifier:(NSString *)identifier;

/*!
 Add a progress indicator, returning the id
 */
- (NSString *)generateProgressIndicatorWithWidth:(NSString *)width;

/*!
 Update the progress indicator's percentage (0.0 to 100.0)
 */
- (void)setValue:(float)value forIndicator:(NSString *)identifier;

/*!
 Add a pop up button, which will invoke action with itself each time it changes
 */
- (void)generatePopUpWithOptions:(NSArray *)options defaultValue:(NSString *)defaultValue action:(SEL)action;
- (void)generatePopUpWithOptions:(NSArray *)options defaultValue:(NSString *)defaultValue action:(SEL)action context:(id)context;

/*!
 Add a label
 */
- (NSString *)generateLabel:(NSString *)label;
- (NSString *)generateLabel:(NSString *)label style:(NSString *)style;
- (void)updateLabel:(NSString *)label forLabelWithIdentifier:(NSString *)identifier;

/*!
 Add a text field with the passed style and name to persist values across document reloads
 @param name    A name to give the text field, for value handling
 @param style   CSS style string to apply, for example to set width
 @param action  A selector to invoke when the text field's value changed. It will receive a DOMHTMLInputElement as its argument
 @discussion    It's probably best to use -valueForTextFieldWithName: and -setValue:forTextFieldWithName: to interact with this element
 */
- (NSString *)generateTextFieldWithName:(NSString *)name style:(NSString *)style valueChangedCallback:(SEL)action;
- (NSString *)generateTextFieldWithName:(NSString *)name style:(NSString *)style valueChangedCallback:(SEL)action placeholder:(NSString *)placeholder;

/*!
 Access the value of the text field with the passed name
 @return String value of the text field (user input)
 */
- (NSString *)valueForTextFieldWithName:(NSString *)name;

/*!
 Sets the string value for the text field with the passed name
 @param value   String value
 @param name    The name of the text field, as created in -generateTextFieldWithName:*
 */
- (void)setValue:(NSString *)value forTextFieldWithName:(NSString *)name;

/*!
 Returns the context for an element, such as a button
 @param     element     The element to get the context for, such as the button passed in action
 */
- (id)contextForElement:(id)element;

@end
