//Vicent.Gao Debug BEGIN
#include "../include/telephony/ril_cdma_sms.h"

//Vicent.Gao Debug BEGIN
/* Transport Layer message types
*/
typedef enum
{
  RIL_CDMA_SMS_TL_TYPE_MIN               = 0,

  RIL_CDMA_SMS_TL_TYPE_POINT_TO_POINT    = 0,
  RIL_CDMA_SMS_TL_TYPE_BROADCAST         = 1,
  RIL_CDMA_SMS_TL_TYPE_ACK               = 2,

  RIL_CDMA_SMS_TL_TYPE_MAX               = 2

} RIL_CDMA_SMS_TlMessageTypeEnum;

/* Transport Layer parameter Ids:
*/
typedef enum
{
  RIL_CDMA_SMS_TL_DUMMY          = -1,  /* dummy */
  RIL_CDMA_SMS_TL_TELESERVICE_ID = 0,  /* Teleservice Identifier     */
  RIL_CDMA_SMS_TL_BC_SRV_CATEGORY,     /* Broadcast Service Category */
  RIL_CDMA_SMS_TL_ORIG_ADDRESS,        /* Originating Address        */
  RIL_CDMA_SMS_TL_ORIG_SUBADDRESS,     /* Originating Subaddress     */
  RIL_CDMA_SMS_TL_DEST_ADDRESS,        /* Destination Address        */
  RIL_CDMA_SMS_TL_DEST_SUBADDRESS,     /* Destination Subaddress     */
  RIL_CDMA_SMS_TL_BEARER_REPLY_OPTION, /* Bearer Reply Option        */
  RIL_CDMA_SMS_TL_CAUSE_CODES,         /* Cause Codes                */
  RIL_CDMA_SMS_TL_BEARER_DATA          /* Bearer Data                */

} RIL_CDMA_SMS_TlParmIdEnum;
//Vicent.Gao Debug END

#define CDMASMS_BIT_SIZE(type) (sizeof(type) * 8)

#define CDMASMS_COPY_MASK(type) ((0xffffffff) >> (32 - CDMASMS_BIT_SIZE(type)))

#define CDMASMS_MASK(width, offset, data) \
   (((width) == CDMASMS_BIT_SIZE(data)) ? (data) :   \
   ((((CDMASMS_COPY_MASK(data) << (CDMASMS_BIT_SIZE(data) - ((width) % CDMASMS_BIT_SIZE(data)))) & CDMASMS_COPY_MASK(data)) >>  (offset)) & (data))) 

#define CDMASMS_MASK_AND_SHIFT(width, offset, shift, data)  \
                  ((((signed) (shift)) < 0) ?       \
                    CDMASMS_MASK((width), (offset), (data)) << -(shift) :  \
                    CDMASMS_MASK((width), (offset), (data)) >>  (((unsigned) (shift))))

#define CDMASMS_MASK_B(offset, len) ((0xff >> offset) & (0xff << (8 - (offset + len))))

#define CDMASMS_MIN(x,y) ((x)<(y)?(x):(y))

#define CDMASMS_MAX( x, y ) (((x)>(y)) ? (x) : (y))

#define CDMASMS_SET_PT(p,v) if((p) != NULL) (*(p)) = (v)

/*****************************************************************************
 * FUNCTION
 *  QL_CDMASMS_sPackBit
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <src>
 *  <dst>
 *  <pos>
 *  <len>
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  1. This function is copy from EC20-FUNC: b_packb
 *****************************************************************************/
static void QL_CDMASMS_sPackBit(unsigned char src, unsigned char dst[], unsigned short pos, unsigned short len)
{
   unsigned short   t_pos = pos % 8;
   unsigned short   bits  = 8 - t_pos;

   ASSERT( dst != NULL );
   dst += (pos+len - 1) / 8;

   if ( bits >= len )
   {
       *dst &= (unsigned char) ~(CDMASMS_MASK_B(t_pos, len));
       *dst |= (unsigned char) (CDMASMS_MASK_B(t_pos, len) & (src << (bits - len)));
   }
   else /* len > bits */
   {
       dst--;
       *dst &= (unsigned char) ~(CDMASMS_MASK_B(t_pos, bits));
       *dst |= (unsigned char) (CDMASMS_MASK_B(t_pos, bits) & (src >> (len - bits)));

       dst++;
       *dst &= (unsigned char) ~ CDMASMS_MASK_B(0, (len - bits));
       *dst |= (unsigned char) (CDMASMS_MASK_B(0, (len - bits)) & (src << (8 - (len - bits))));
   }
}

/*****************************************************************************
 * FUNCTION
 *  QL_CDMASMS_sUnpackBit
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <src>
 *  <pos>
 *  <len>
 *
 * RETURNS
 *  'unsigned char' value
 *
 * NOTE
 *  1. This function is copy from EC20-FUNC: b_unpackb
 *****************************************************************************/
static unsigned char QL_CDMASMS_sUnpackBit( unsigned char *src, unsigned short pos, unsigned short len )
{
   unsigned char result = 0;
   int rshift = 0;

   ASSERT( src != NULL );
   src += pos/8;
   pos %= 8;
   
   rshift = CDMASMS_MAX( 8 - (pos + len), 0);

   if ( rshift > 0 ) {

     result = CDMASMS_MASK_AND_SHIFT(len, pos, rshift, *src);
  
   } else {

     result = CDMASMS_MASK(8-pos, pos, *src);
     src++;
     len -= 8 - pos;

      if ( len > 0 ) result = ( result<<len ) | (*src >> (8-len));  // if any bits left
   }

   return result;
} 

/*****************************************************************************
 * FUNCTION
 *  QL_CDMASMS_sPackWord
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <src>
 *  <dst>
 *  <pos>
 *  <len>
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  1. This function is copy from EC20-FUNC: b_packw
 *****************************************************************************/
static void QL_CDMASMS_sPackWord(unsigned short src, unsigned char dst[], unsigned short pos, unsigned short len)
{
   int bits = 0;
   int start = 0;
   unsigned char mask = 0;
 
   ASSERT( dst != NULL );
   dst += (len+pos-1)/8;        /* point to last byte to be written */
   pos  = (len+pos-1)%8;        /* index of last bit to be written */
   
   if (len > pos)  /* if we are filling all of the left part of the byte */
   {
     start = 0;  
   }
   else            /* There are going to be untouched bits at left of 
                   ** destination byte.                                   */
   {
     start = (pos+1) - len;     
   }
   bits = (pos - start) + 1;    /* # of bits to be written in this byte */
   
   *dst &= (unsigned char) ~(CDMASMS_MASK_B(start,bits));  /* clear the bits to be written */
   
   *dst |= (unsigned char) (   ( src << (7 - pos) )    /* left-shift src to line up */
                    & CDMASMS_MASK_B(start, bits) );  /* only touch desired bits */
   
   dst--;                /* back up one byte */
   src >>= bits;         /* get rid of bits we've consumed already */
   
   if(len > bits)        /* if we need to write into other bytes */
   {
     len -= bits;        /* compute remaining length  */
     
     /* for full bytes, we can just overwrite the old value with the new */
     for ( ; len >= 8 ; len -= 8 ) {
       *dst = (unsigned char)( src );
       dst--;                         /* back up one byte */
       src >>= 8;                     /* get rid of used bits */
     }
 
     if (len > 0)     /* if some bits are leftover... */
     {
       mask = (unsigned char) (0xff << len);   
       *dst &= mask;                  /* clear bits on right side of byte */
       *dst |= ( (unsigned char)( src ) & ~mask);        /* set appropriate bits */
     }
 
   }
} 

/*****************************************************************************
 * FUNCTION
 *  QL_CDMASMS_sUnpackWord
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <src>
 *  <pos>
 *  <len>
 *
 * RETURNS
 *  'unsigned short' value
 *
 * NOTE
 *  1. This function is copy from EC20-FUNC: b_unpackw
 *****************************************************************************/
static unsigned short QL_CDMASMS_sUnpackWord(unsigned char src[], unsigned short pos, unsigned short len)
{ 
   unsigned short result = 0;
   int rshift = 0;

   ASSERT( src != NULL );
   src += pos / 8;
   pos %= 8;

   rshift = CDMASMS_MAX( 8 - (pos + len), 0);

   if ( rshift > 0 ) {

     result = CDMASMS_MASK_AND_SHIFT(len, pos, rshift, *src);
  
   } else {

      result = CDMASMS_MASK(8-pos, pos, *src);
      src++;
      len -= 8 - pos;

      for ( ; len >= 8  ; len-=8)
      {
         result = ( result<<8 ) | *src++;
      }

      if ( len > 0 ) result = ( result<<len ) | (*src >> (8-len));  // if any bits left
   }

   return result;
} 

/*****************************************************************************
 * FUNCTION
 *  QL_CDMASMS_sEncodeAddress
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <pAddr>
 *  <pData>
 *
 * RETURNS
 *  Encode address length
 *
 * NOTE
 *  1. This function is copy from EC20-FUNC: wms_ts_cdma_encode_address
 *****************************************************************************/
unsigned char QL_CDMASMS_sEncodeAddress(RIL_CDMA_SMS_Address *pAddr, unsigned char *pData)
{
  unsigned short bit_pos = 0;
  unsigned char digit_size = 0;
  unsigned short i = 0;
  unsigned char uLen = 0;

  /*---- checking ---- */
  if((NULL == pAddr) || (NULL == pData))
  {
      QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! Parameter is INVALID. pAddr:%p,pData:%p",__FUNCTION__,__LINE__,pAddr,pData);
      return 0;
  }

  /* Digit Mode */
  QL_CDMASMS_sPackBit( (unsigned char)(pAddr->digit_mode),pData,bit_pos,1 );
  bit_pos++;

  /* Number mode */
  QL_CDMASMS_sPackBit( (unsigned char)(pAddr->number_mode),pData,bit_pos,1 );
  bit_pos++;

  /* Number type */
  if( RIL_CDMA_SMS_DIGIT_MODE_8_BIT == (pAddr->digit_mode) )
  {
      digit_size = 8;

      QL_CDMASMS_sPackBit( (unsigned char)(pAddr->number_type),pData,bit_pos,3 );
      bit_pos += 3;

      if( RIL_CDMA_SMS_NUMBER_MODE_NOT_DATA_NETWORK == (pAddr->number_mode) )
      {
          QL_CDMASMS_sPackBit( (unsigned char)(pAddr->number_plan),pData,bit_pos,4 );
          bit_pos += 4;
      }
  }
  else
  {
      digit_size = 4;
  }

  /* Address size */
  QL_CDMASMS_sPackBit( pAddr->number_of_digits,pData,bit_pos,8 );
  bit_pos += 8;

  /* pack the digits */
  for( i = 0; i < pAddr->number_of_digits; i++ )
  {
      QL_CDMASMS_sPackBit( pAddr->digits[i],pData,bit_pos,digit_size );
      bit_pos += digit_size;
  }

  if( bit_pos % 8 != 0 )
  {
    /* pad 0 to remaining bits */
    digit_size = (uint8)(8 - bit_pos % 8);
    QL_CDMASMS_sPackBit( 0, pData, bit_pos,  digit_size );
    bit_pos += digit_size;
  }

  uLen = (unsigned char) (bit_pos / 8);

  return uLen;

} 

/*****************************************************************************
 * FUNCTION
 *  QL_CDMASMS_sDecodeAddress
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <pData>
 *  <uParamLen>
 *  <pAddr>
 *
 * RETURNS
 *  1: This function works SUCCESS.
 *  0: This function works FAIL!
 *
 * NOTE
 *  1. This function is copy from EC20-FUNC: wms_ts_cdma_decode_address
 *****************************************************************************/
static unsigned char QL_CDMASMS_sDecodeAddress
(
  unsigned char  *pData,
  unsigned char  uParamLen,
  RIL_CDMA_SMS_Address *pAddr
)
{
  unsigned short bit_pos = 0;
  unsigned char      digit_size = 0;
  unsigned short       i = 0;
  unsigned char iResult = 1; //WARNING! MUST set to 1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*---- checking ---- */
  if (( NULL == pAddr ) || (NULL == pData ) || (0 == uParamLen))
  {
      QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! Parameter is INVALID. pAddr:%p,pData:%p,uParamLen:%d",__FUNCTION__,__LINE__,pAddr,pData,uParamLen);
      return 0;
  }

  do
  {
    /* Digit mode */
    pAddr->digit_mode = QL_CDMASMS_sUnpackBit( pData, bit_pos, 1 );
    bit_pos ++;

    /* Number mode */
    pAddr->number_mode = QL_CDMASMS_sUnpackBit( pData, bit_pos, 1 );
    bit_pos ++;

    /* Number type, and determine digit size */
    if( RIL_CDMA_SMS_DIGIT_MODE_8_BIT == pAddr->digit_mode )
    {
        pAddr->number_type = QL_CDMASMS_sUnpackBit( pData, bit_pos, 3 );
        bit_pos += 3;

        digit_size = 8;
    }
    else
    {
        pAddr->number_type = RIL_CDMA_SMS_NUMBER_TYPE_UNKNOWN;
        digit_size = 4;
    }

    /* Number plan */
    if(    (RIL_CDMA_SMS_DIGIT_MODE_8_BIT == (pAddr->digit_mode))
        && (RIL_CDMA_SMS_NUMBER_MODE_NOT_DATA_NETWORK == (pAddr->number_mode))
      )
    {
      if ((bit_pos + 4) > (unsigned short)(uParamLen * 8))
      {
          QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! decoding failed: address param_len not enough to accomodate number_plan field  %d>%d",__FUNCTION__,__LINE__,bit_pos + 4, uParamLen * 8);
          iResult = 0;

          break;
      }

      pAddr->number_plan = QL_CDMASMS_sUnpackBit( pData, bit_pos, 4);
      bit_pos += 4;
    }
    else
    {
      /* number plan is not used */
      pAddr->number_plan = RIL_CDMA_SMS_NUMBER_PLAN_UNKNOWN;
    }

    if ((bit_pos + 8) > (unsigned short)(uParamLen * 8))
    {
      QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! decoding failed: address param_len not enough to accomodate number_of_address_digits field  %d>%d",__FUNCTION__,__LINE__,bit_pos + 8, uParamLen * 8);
      iResult = 0;
      
      break;
    }
    
    /* Address size */
    pAddr->number_of_digits = QL_CDMASMS_sUnpackBit( pData, bit_pos, 8 );
    bit_pos += 8;

    /* Extract all digits:
        */
    if( pAddr->number_of_digits > RIL_CDMA_SMS_ADDRESS_MAX )
    {
      QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! decoding failed: address digits number too big: %d",__FUNCTION__,__LINE__,pAddr->number_of_digits);
      iResult = 0;
      
      break;
    }

    for( i=0; i < pAddr->number_of_digits; i++ )
    {
        if ((bit_pos + digit_size) > (unsigned short)(uParamLen * 8))
        {
          QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! decoding failed: number_of_address_digits is crossing address param_len: %d>%d",__FUNCTION__,__LINE__,bit_pos + digit_size, uParamLen * 8);
          iResult = 0;
          
          break;
        }  
        
        pAddr->digits[i] = QL_CDMASMS_sUnpackBit( pData, bit_pos, digit_size );
        bit_pos += digit_size;
    }
  } while(0);

  return iResult;
} /* wms_ts_cdma_decode_address() */

/*****************************************************************************
 * FUNCTION
 *  QL_CDMASMS_sEncodeSubaddress
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <pAddr>
 * <pData>
 *
 * RETURNS
 *  Encode subaddress length
 *
 * NOTE
 *  1. This function is copy from EC20-FUNC: wms_ts_cdma_encode_subaddress
 *****************************************************************************/
unsigned char QL_CDMASMS_sEncodeSubaddress
(
  RIL_CDMA_SMS_Subaddress *pAddr,
  unsigned char *pData
)
{
  unsigned short  bit_pos = 0;
  unsigned char   digit_size = 0;
  unsigned short  i = 0;
  unsigned char uLen = 0;

  /*---- checking ---- */
  if ( ( NULL == pAddr ) || (NULL == pData) )
  {
      QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! Parameter is INVALID. pAddr:%p,pData:%p",__FUNCTION__,__LINE__,pAddr,pData);
      return 0;
  }

  /* subaddress type */
  QL_CDMASMS_sPackBit( (unsigned char)(pAddr->subaddressType),pData,bit_pos,3 );
  bit_pos += 3;

  /* Odd flag */
  QL_CDMASMS_sPackBit( pAddr->odd,pData,bit_pos,1 );
  bit_pos += 1;

  /* Address size */
  QL_CDMASMS_sPackBit( pAddr->number_of_digits,pData,bit_pos,8 );
  bit_pos += 8;

  /* Digit size */
  digit_size = 8;  /* always 8 */

  /* Pack the digits */
  for( i = 0; i < pAddr->number_of_digits; i++ )
  {
      QL_CDMASMS_sPackBit( pAddr->digits[i],pData,bit_pos,digit_size );
      bit_pos += digit_size;
  }

  /* pad last 4 bits with 0 */
  QL_CDMASMS_sPackBit( 0,pData,bit_pos,4 );
  bit_pos += 4;

  /* return len */
  uLen = (uint8) (bit_pos / 8);

  return uLen;

} 

/*****************************************************************************
 * FUNCTION
 *  QL_CDMASMS_sDecodeSubaddress
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <pData>
 *  <uParamLen>
 *  <pAddr>
 *
 * RETURNS
 *  1: This function works SUCCESS.
 *  0: This function works FAIL!
 *
 * NOTE
 *  1. This function is copy from EC20-FUNC: wms_ts_cdma_decode_subaddress
 *****************************************************************************/
static unsigned char QL_CDMASMS_sDecodeSubaddress
(
  unsigned char *pData,
  unsigned char uParamLen,
  RIL_CDMA_SMS_Subaddress *pAddr
)
{
  unsigned short     bit_pos = 0;
  unsigned char      digit_size = 0;
  unsigned short       i;
  unsigned char iResult = 1; //WARNING! MUST set to 1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*---- checking ---- */
  if ( (NULL == pAddr) || (NULL == pData) || (0 == uParamLen) )
  {
      QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! Parameter is INVALID. pAddr:%p,pData:%p,uParamLen:%d",__FUNCTION__,__LINE__,pAddr,pData,uParamLen);
      return 0;
  }

  do
  {
    /* address type */
    pAddr->subaddressType = QL_CDMASMS_sUnpackBit( pData, bit_pos, 3 );
    bit_pos += 3;

    /* Odd flag */
    pAddr->odd = QL_CDMASMS_sUnpackBit( pData, bit_pos, 1 );
    bit_pos += 1;

    /* Address size */
    if ((bit_pos + 8) > (unsigned short)(uParamLen * 8))
    {
      QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! decoding failed: subaddress param_len not enough to accomodate number_of_subaddress_digits field  %d>%d",__FUNCTION__,__LINE__,bit_pos, uParamLen * 8);
      iResult = 0;
      
      break;
    }
     
    pAddr->number_of_digits = QL_CDMASMS_sUnpackBit( pData, bit_pos, 8 );
    bit_pos += 8;

    /* Extract all digits:
        */
    if( pAddr->number_of_digits > RIL_CDMA_SMS_SUBADDRESS_MAX )
    {
      QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! decoding: subaddress digits number too big: %d",__FUNCTION__,__LINE__,pAddr->number_of_digits);
      iResult = 0;

      break;
    }
    
    digit_size = 8;  /* always 8 bits so far */

    for( i=0; i < pAddr->number_of_digits; i++ )
    {
        if ((bit_pos + digit_size) > (unsigned short)(uParamLen * 8))
        {
          QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! decoding failed: number_of_subaddress_digits is crossing subaddress param_len: %d>%d",__FUNCTION__,__LINE__,bit_pos, uParamLen * 8);
          iResult = 0;
          
          break;
        }

        pAddr->digits[i] = QL_CDMASMS_sUnpackBit( pData,bit_pos,digit_size );
        bit_pos += digit_size;
    }
  } while(0);

  /* Done */
  return iResult;

} /* wms_ts_cdma_decode_subaddress() */

/*****************************************************************************
 * FUNCTION
 *  QL_RIL_CDMASMS_sEncodePDUP2P
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <bIsMo>
 *  <pInfo>
 *  <pLen>   [Out]
 *
 * RETURNS
 *  The pointer of encode PDU data
 *
 * NOTE
 *  1. This function is refer to EC20-FUNC: wms_ts_encode_CDMA_tl
 *****************************************************************************/
static unsigned char* QL_RIL_CDMASMS_sEncodePDUP2P(unsigned char bIsMo,RIL_CDMA_SMS_Message *pInfo,unsigned short *pLen)
{
    #define CDMA_SMS_PDU_MAX_LEN   (512)

    static unsigned char s_aPDU[CDMA_SMS_PDU_MAX_LEN] = {0,};
    unsigned char *pTmp = s_aPDU;
    unsigned char *pParamLen = NULL;

    if((NULL == pInfo) || (NULL == pLen))
    {
        QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! Parameter is INVALID. pInfo:%p,pLen:%p",__FUNCTION__,__LINE__,pInfo,pLen);
        return NULL;
    }

    //Initialize
    memset(s_aPDU,0x00,sizeof(s_aPDU));

    //SMS_MSG_TYPE
    *(pTmp++) = RIL_CDMA_SMS_TL_TYPE_POINT_TO_POINT; //SMS Point-to-Point

    //Teleservice Identifier
    *(pTmp++) = RIL_CDMA_SMS_TL_TELESERVICE_ID; //PARAMETER_ID
    *(pTmp++) = 0x02; //PARAMETER_LEN
    QL_CDMASMS_sPackWord( (unsigned short)(pInfo->uTeleserviceID),s_aPDU,((pTmp - s_aPDU) * 8),0x02 * 8 ); pTmp += 2; //IDENTIFIER

    //Service Category
    if(pInfo->bIsServicePresent)
    {
        *(pTmp++) = RIL_CDMA_SMS_TL_BC_SRV_CATEGORY; //PARAMETER_ID
        *(pTmp++) = 0x02; //PARAMETER_LEN
        QL_CDMASMS_sPackWord( (unsigned short)(pInfo->uServicecategory),s_aPDU,((pTmp - s_aPDU) * 8),0x02 * 8 ); pTmp += 2; //CATEGORY
    }

    //Destination Address or Originating Address
    if(bIsMo)
    {
        *(pTmp++) = RIL_CDMA_SMS_TL_DEST_ADDRESS; //PARAMETER_ID
    }
    else
    {
        *(pTmp++) = RIL_CDMA_SMS_TL_ORIG_ADDRESS; //PARAMETER_ID
    }
    pParamLen = pTmp;
    pTmp++; //Skip PARAMETER_LEN
    *pParamLen = QL_CDMASMS_sEncodeAddress(&(pInfo->sAddress),pTmp); pTmp += *pParamLen;//Set PARAMETER_LEN and others

    //Destination Subaddress or Originating Subaddress
    if(pInfo->sSubAddress.number_of_digits != 0)
    {
        if(bIsMo)
        {
            *(pTmp++) = RIL_CDMA_SMS_TL_DEST_SUBADDRESS; //PARAMETER_ID
        }
        else
        {
            *(pTmp++) = RIL_CDMA_SMS_TL_ORIG_SUBADDRESS; //PARAMETER_ID
        }
        pParamLen = pTmp;
        pTmp++; //Skip PARAMETER_LEN
        *pParamLen = QL_CDMASMS_sEncodeSubaddress(&(pInfo->sSubAddress),pTmp); pTmp += *pParamLen;//Set PARAMETER_LEN and others
    }

    //Bearer Data
    if(pInfo->uBearerDataLen != 0)
    {
        *(pTmp++) = RIL_CDMA_SMS_TL_BEARER_DATA; //PARAMETER_ID
        pParamLen = pTmp;
        pTmp++; //Skip PARAMETER_LEN
        *pParamLen = CDMASMS_MIN(pInfo->uBearerDataLen,CDMA_SMS_PDU_MAX_LEN - (pTmp - s_aPDU));
        memcpy(pTmp,pInfo->aBearerData,*pParamLen); pTmp += *pParamLen;
    }

    *pLen = pTmp - s_aPDU;

    return s_aPDU;
}

/*****************************************************************************
 * FUNCTION
 *  QL_RIL_CDMASMS_sDecodePDUP2P
 *
 * DESCRIPTION
 *  NUL
 *  
 * PARAMETERS
 *  <pIsMo>  [Out]
 *  <pInfo>  [Out]
 *  <pPDU>  [In]
 *  <uLen>   [In]
 *
 * RETURNS
 *  1: This function works SUCCESS.
 *  0: This function works FAIL!
 *
 * NOTE
 *  1. This function is refer to EC20-FUNC: wms_ts_decode_CDMA_tl
 *****************************************************************************/
static unsigned char QL_RIL_CDMASMS_sDecodePDUP2P(unsigned char *pIsMo,RIL_CDMA_SMS_Message *pInfo,unsigned char *pPDU,unsigned short uLen)
{
    unsigned char *pTmp = pPDU;
    unsigned char uParamId = RIL_CDMA_SMS_TL_DUMMY;
    unsigned char uParamLen = 0;
    unsigned char uLimitParamLen = 0;
    unsigned char bResult = 1; //WARNING! MUST set to 1

    if((NULL == pInfo) || (NULL == pPDU) || (0 == uLen))
    {
        QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! Parameter is INVALID. pInfo:%p,pPDU:%p,uLen:%d",__FUNCTION__,__LINE__,pInfo,pPDU,uLen);
        return 0;
    }

    if(pPDU[0] != RIL_CDMA_SMS_TL_TYPE_POINT_TO_POINT)
    {
        QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_FAIL! type:%d NOT Point-to-Point.",__FUNCTION__,__LINE__,pPDU[0]);
        return 0;
    }

    //Initialize
    memset(pInfo,0x00,sizeof(RIL_CDMA_SMS_Message));

    pTmp++; //Skip SMS_MSG_TYPE

    while((pTmp - pPDU) < uLen)
    {
        //Get 'Teleservice Identifier'
        uParamId = *(pTmp++); //PARAMETER_ID
        uParamLen = *(pTmp++); //PARAMETER_LEN

        switch(uParamId)
        {
            case RIL_CDMA_SMS_TL_TELESERVICE_ID:
            {
                pInfo->uTeleserviceID = QL_CDMASMS_sUnpackWord(pPDU,((pTmp - pPDU) * 8),uParamLen * 8);

                pTmp += uParamLen; //Skip param data
            }
            break;

            case RIL_CDMA_SMS_TL_BC_SRV_CATEGORY:
            {
                pInfo->bIsServicePresent = 1;
                pInfo->uServicecategory = QL_CDMASMS_sUnpackWord( pPDU, ((pTmp - pPDU) * 8),uParamLen * 8);
                
                pTmp += uParamLen; //Skip param data
            }
            break;

            case RIL_CDMA_SMS_TL_DEST_ADDRESS:
            case RIL_CDMA_SMS_TL_ORIG_ADDRESS:
            {
                if(RIL_CDMA_SMS_TL_DEST_ADDRESS == uParamId)
                {
                    CDMASMS_SET_PT(pIsMo,1);
                }
                else
                {
                    CDMASMS_SET_PT(pIsMo,0);
                }

                bResult = QL_CDMASMS_sDecodeAddress( pTmp, uParamLen, &(pInfo->sAddress) );
                if(!bResult)
                {
                    QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_WARNING! QL_CDMASMS_sDecodeAddress FAIL! uParamId:%d",__FUNCTION__,__LINE__,uParamId);
                }
                
                pTmp += uParamLen; //Skip param data
            }
            break;

            case RIL_CDMA_SMS_TL_DEST_SUBADDRESS:
            case RIL_CDMA_SMS_TL_ORIG_SUBADDRESS:
            {
                bResult = QL_CDMASMS_sDecodeSubaddress( pTmp, uParamLen, &(pInfo->sSubAddress) );
                if(!bResult)
                {
                    QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_WARNING! QL_CDMASMS_sDecodeSubaddress FAIL! uParamId:%d",__FUNCTION__,__LINE__,uParamId);
                }

                pTmp += uParamLen; //Skip param data
            }
            break;

            case RIL_CDMA_SMS_TL_BEARER_DATA:
            {
                uLimitParamLen = CDMASMS_MIN(uParamLen, RIL_CDMA_SMS_BEARER_DATA_MAX);
                memcpy(pInfo->aBearerData,pTmp,uLimitParamLen);
                pInfo->uBearerDataLen = uLimitParamLen;
                
                pTmp += uParamLen; //Skip param data
            }
            break;

            default:
            {
                QL_CDMASMS_TRACE(QL_CDMASMS_TRACE_BUF,"Enter %s(%d),CDMASMS_WARNING! uParamId:%d ignore.",__FUNCTION__,__LINE__,uParamId);
                pTmp += uParamLen; //Skip param data
            }
            break;
        }
    }

    return 1;
}
//Vicent.Gao Debug END
