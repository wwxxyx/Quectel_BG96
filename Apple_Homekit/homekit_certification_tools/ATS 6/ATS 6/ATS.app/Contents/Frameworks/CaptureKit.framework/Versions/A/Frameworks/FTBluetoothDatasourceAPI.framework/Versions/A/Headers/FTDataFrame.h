//
//  FTDataFrame.h
//  FTBluetoothDatasourceAPI
//
//  Created by Michael Lance on 10/24/12.
//
//

#import <Foundation/Foundation.h>

/**
 @file FTDataFrame.h
*/

@class FTBdAddress;

/**
 An enumeration of ComProbe data types.
 */
typedef enum ComProbeDataType
{
  DT_NONE, /**< Data type not set.*/
  DT_ACL, /**< ACL event.*/
  DT_HCI_EVENT, /**< HCI event.*/
  DT_HCI_CMD, /**< HCI command.*/
  DT_SNIFFER /**< Sniffer packet.*/
} ComProbeDataType;

/**
 An enumeration of Bluetooth data types.
 */
typedef enum BluetoothDataType
{
  BDT_NONE, /**< Data not Bluetooth.*/
  BDT_PREMIC_CLASSIC, /**< Classic Bluetooth data from older chips. Retained for conversion only.*/
  BDT_CLASSIC, /**< Classic Bluetooth data.*/
  BDT_LOWENERGY, /**< low energy Bluetooth data.*/
  BDT_PRECONNECTION, /**< Preconnection data.*/
} BluetoothDataType;

/**
 An enumeration of device roles.
 */
typedef enum DeviceRole
{
  ROLE_MASTER, /**< Frame from master.*/
  ROLE_SLAVE, /**< Frame from slave.*/
  ROLE_SNIFFER, /**< Frame from sniffer.*/
  ROLE_UNKNOWN /**< Unknown frame producer or invalid data.*/
} DeviceRole;

typedef enum LowEnergyRole
{
  LER_MASTER_SIDE= 1,
  LER_SLAVE_SIDE,
} LowEnergyRole;

typedef enum MICStatus
{
  MIC_IS_PRESENT,
  MIC_NOT_PRESENT
} MICStatus;

typedef enum FHSStatus
{
  FHS_NOT,
  FHS_MISS,
  FHS_PACKET
} FHSStatus;

/**
 FTDataFrame
 
 A class that represents a ComProbe data frame.
 */
@interface FTDataFrame : NSObject

/**
 The serial number of the chip that created the data frame.
 */
@property (readonly) NSString* sourceChipSerialNumber;

/**
 The index of the chip that created the data frame.
 */
@property (readonly) NSUInteger sourceChipIndex;

/**
 The bytes that make up the BDADDR of the Bluetooth device that produced the frame.
 */
@property (readonly) NSData* sourceDeviceBdAddrData;

/**
 The bytes tha make up the BDADDR of the Bluetooth device that produced the frame.
 */
@property (readonly) DeviceRole role;

/**
 The frame data type - ACL event, HCI command or HCI event.
 */
@property (readonly) ComProbeDataType frameDataType;

/**
 The Bkuetooth data type - Classic, low energy or pre-connection.
 */
@property (readonly) BluetoothDataType bluetoothDataType;

/**
 The frame data bytes.
 */
@property (readonly) NSData* frameData;

/**
 The zero-chip tick delta.
 */
@property (readonly) SInt64 chipTickDelta;

/**
 The starting microsecond clock value for the chip.
 */
@property (readonly) UInt64 chipStartingClock;

/**
 The reference starting time interval since first instant of 1 January 2001, GMT, relative to chip 0 ticks.
 */
@property (readonly) NSTimeInterval secondsSince01Jan2001;

/**
 The number of times the chip counter has rolled over.
 */
@property (readonly) NSUInteger chipRolloverCount;

/**
 The reference capture time interval since first instant of 1 January 2001, GMT. Used for chip-clock rollover detection.
 */
@property (readonly) NSTimeInterval captureTimeInterval;

/**
 The access address for the connection associated with the frame.
 */
@property (readonly) uint32_t lowEnergyAccessAddress;

/**
 Is the object valid.
 */
@property (readonly) BOOL frameDataValid;

// Debug source address
@property (readonly) BOOL setMasterBdAddrCalled;
@property FHSStatus fhsPacket;
@property (copy) NSMutableString* extractedBdAddr;
@property (readonly) UInt32 masterSlaveMaskResult;
@property (readonly) Byte globalRoleSwitchInfo;
@property (readonly) Byte dataRoleSwitchInfo;
@property (readonly) NSMutableString* roleSwitchEffector;
- (void) setRoleSwitchData:(UInt32)masterSlave globalRS:(Byte)globalRS dataRS:(Byte)dataRS effector:(NSString*)effector;
// Debug source address

/**
 Get the "friendly" name for the data type.
 
 @param dataTypeFriendlyName Mutable string to hold returned friendly name.
 @param dataType Data type to get friendly name for.
 @return TRUE if operation succeeded.
 */
+ (BOOL)friendlyName:(NSMutableString*)dataTypeFriendlyName forDataType:(NSUInteger)dataType;

/**
 Interpret the specified packet status.
 
 @param packetStatus Byte that represents the packet status to be interpreted.
 @param interpretedPacketStatus Mutable string to receive the status.
 */
+ (BOOL)interpretPacketStatus:(Byte)packetStatus interpretedPacketStatus:(NSMutableString*)interpretedPacketStatus;

/**
 Initialize an empty data frame.
 
 @return An initialized empty data frame.
 */
- (id)init;

/**
 Initialize a data frame with another data frame.
 
 @param dataFrame Data frame to copy from.
 @return A data frame with the contents of the specified data frame.
 */
- (id)initWithDataFrame:(FTDataFrame*)dataFrame;

/**
 Initialize a data frame by specifying the member values and an array of Byte.
 
 @param theData Data to store in the frame.
 @param dataType Type of data frame. @see ComProbeDataType.
 @param btDataType Type of Bluetooth data carried in frame if dataType is DT_SNIFFER. @see BluetoothDataType.
 @param theChipTickDelta The tick delta between the creating chip and the reference chip.
 @param since01Jan2001 NSTimeInterval that represents the seconds from the first instant of 1 January 2001, GMT to the reference date.
 @param rolloverCount Number of times the chip clock has rolled over.
 @param sourceChip Identifier of the chip that produced the data frame.
 @param chipIndex Index of the chip that produced the data frame.
 @param bdAddrData An NSData object that contains the bytes of the BD ADDR of the Bluetooth device that produced the frame.
 @return A data frame with the specified attributes.
 */
- (id)initWithData:(NSData*)theData type:(ComProbeDataType)dataType bluetoothDataType:(BluetoothDataType)btDataType chipTickDelta:(SInt64)theChipTickDelta chipStartingClock:(UInt64)theChipStartingClock referenceTimeInterval:(NSTimeInterval)since01Jan2001 rolloverCount:(NSUInteger)rolloverCount sourceChipSerialNumber:(NSString*)sourceChip sourceChipIndex:(NSUInteger)chipIndex sourceDeviceBdAddrData:(NSData*)bdAddrData captureTimeInterval:(NSTimeInterval)timeInterval;

/**
 Initialize a data frame by specifying the member values and a Byte pointer.
 
 @param theData Data to store in the frame.
 @param length Valid length of theData.
 @param dataType Type of data frame. @see ComProbeDataType.
 @param btDataType Type of Bluetooth data carried in frame if dataType is DT_SNIFFER. @see BluetoothDataType.
 @param theChipTickDelta The tick delta between the creating chip and the reference chip.
 @param since01Jan2001 NSTimeInterval that represents the seconds from the first instant of 1 January 2001, GMT to the reference date.
 @param rolloverCount Number of times the chip clock has rolled over.
 @param sourceChip Identifier of the chip that produced the data frame.
 @param chipIndex Index of the chip that produced the data frame.
 @param bdAddrData An NSData object that contains the bytes of the BD ADDR of the Bluetooth device that produced the frame.
 @return A data frame with the specified attributes.
 */
- (id)initWithDataBytes:(const Byte*)theData ofLength:(NSUInteger)length type:(ComProbeDataType)dataType bluetoothDataType:(BluetoothDataType)btDataType chipTickDelta:(SInt64)theChipTickDelta chipStartingClock:(UInt64)theChipStartingClock referenceTimeInterval:(NSTimeInterval)since01Jan2001 rolloverCount:(NSUInteger)rolloverCount sourceChipSerialNumber:(NSString*)sourceChip sourceChipIndex:(NSUInteger)chipIndex sourceDeviceBdAddrData:(NSData*)bdAddrData captureTimeInterval:(NSTimeInterval)timeInterval;

/**
 Deallocate a data frame.
 */
- (void)dealloc;

/**
 Parse the data in a supplied data frame of the specified type into a key/value collection of fields along with a description of the field order and a string representation of the data.
 
 @param frame An array of bytes.
 @param type The type of data represented by the frame.
 @param chip Optional originating chip - can be nil.
 @param sourceAddressData Optional originating device - can be nil.
 @param sourceRole Optional source role - will default to Unknown if not ROLE_MASTER (0), ROLE_SLAVE (1) or ROLE_SNIFFER (2).
 @param parsedFieldsAndValues A key/value collection to describe each data field with the accompanying value.
 @param fieldsInOrder A collection of field names in the order they appear in the frame.
 @param showClassMembers Show the values of class member variables.
 @param frameDescription A mutable string to accept the full description of the data in the frame.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
+ (BOOL)parseFieldsFromData:(NSData*)frame ofType:(ComProbeDataType)type forChip:(NSString*)chip fromSource:(NSData*)sourceAddressData sourceRole:(DeviceRole)sourceRole fieldsAndValues:(NSMutableDictionary*)parsedFieldsAndValues fieldOrder:(NSMutableArray*)fieldsInOrder showClassMembers:(BOOL)showClassMembers frameDescription:(NSMutableString*)frameDescription error:(NSError**)errorPtr;

/**
 Parse the data in a data frame into a key/value collection of fields along with a description of the field order and a string representation of the data.
 
 @param parsedFieldsAndValues A key/value collection to describe each data field with the accompanying value.
 @param fieldsInOrder A collection of field names in the order they appear in the frame.
 @param showClassMembers Show the values of class member variables.
 @param frameDescription A mutable string to accept the full description of the data in the frame.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
*/
- (BOOL)parseFields:(NSMutableDictionary*)parsedFieldsAndValues fieldOrder:(NSMutableArray*)fieldsInOrder showClassMembers:(BOOL)showClassMembers frameDescription:(NSMutableString*)frameDescription error:(NSError**)errorPtr;

/**
 Get the sniffer payload of the data frame if it exists.
 
 @param snifferPayload A mutable data array to accept the sniffer payload bytes.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getSnifferPayload:(NSMutableData*)snifferPayload error:(NSError**)errorPtr;

/**
 Access the ACL payload of the data frame if it exists.
 
 @param lengthPtr Pointer to NSUInteger to accept payload length.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return Pointer to payload if it exists, NULL otherwise.
 */
- (const Byte*)accessSnifferPayloadWithLength:(NSUInteger*)lengthPtr error:(NSError**)errorPtr;

/**
 Get the timestamp of the data frame if it exists. The timestamp will be based on the referenceDate, chipTickDelta and the timestamp from the sniffer header in the data frame.
 
 @param timestampPtr Pointer to an NSDate object to receive the timestamp. The timestamp resolution is microseconds.
 @param fractionalSeconds Pointer to NSTimeInterval to receive the fractional seconds from the timestamp. Not required.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getTimestamp:(NSDate**)timestampPtr fractionalSeconds:(NSTimeInterval*)fractionalSeconds error:(NSError**)errorPtr;

/**
 Get the raw timestamp of the data frame if it exists. The timestamp will be based on the chipTickDelta and the timestamp from the sniffer header in the data frame.
 
 @param timestampPtr Pointer to a UInt64 object to receive the timestamp. The timestamp resolution is microseconds.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getSnifferHeaderTimestamp:(UInt64*)timestampPtr error:(NSError**)errorPtr;

/**
 Generate a string representation of the timestamp.
 
 @param timestamp Mutable string to receive timestamp.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getTimestampString:(NSMutableString*)timestamp error:(NSError**)errorPtr;

/**
 Generate a string representation of the raw Bytes of the data frame with the specified index.
 
 @param index Index of the frame - for display purposes only.
 @param headerOnly Show only the frame header information (frame number, chip of origin, timestamp, etc.) and none of the frame data.
 @param outputString Mutable string to accept frame data description.
 */
- (BOOL)getRawOutputStringForIndex:(NSUInteger)index headerOnly:(BOOL)headerOnly outputString:(NSMutableString*)outputString;

/**
 Get the role and BD_ADDR of the device that produced the frame. If the frame is ACL or Sniffer frame and the address exists, the role will be ROLE_MASTER or ROLE_SLAVE. If the frame is HCI the role will be ROLE_SNIFFER. Otherwise, the role will be ROLE_UNKNOWN.
 
 @param bdAddr FTBdAddress object to hold the returned address.
 @return Role of device.
 */
- (DeviceRole)getSourceDeviceRoleAndBdAddr:(FTBdAddress*)bdAddr;

/**
 Get the signal strength of the data frame if it exists.
 
 @param signalStrength Mutable string to receive formatted signal strength value.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getSignalStrength:(NSMutableString*)signalStrength error:(NSError**)errorPtr;

/**
 Get the Bluetooth clock of the data frame if it exists.
 
 @param bluetoothClock Mutable string to receive formatted Bluetooth clock value.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getBluetoothClock:(NSMutableString*)bluetoothClock error:(NSError**)errorPtr;

- (void)temporarySetNewSourceBdAddrData:(NSData*)newSourceBdAddrData;

@end
