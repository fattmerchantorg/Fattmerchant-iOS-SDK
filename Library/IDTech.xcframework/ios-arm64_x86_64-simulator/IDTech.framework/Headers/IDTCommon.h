//
//  IDTCommon.h
//  IDTech
//
//  Created by Randy Palermo on 5/4/15.
//  Copyright (c) 2015 IDTech Products. All rights reserved.
//



/** Structure used to return response from IDT_BTPay::icc_getICCReaderStatus() and IDT_UniPay::icc_getICCReaderStatus()
 */
typedef struct {
    bool iccPower; //!< Determines if ICC has been powered up
    bool cardSeated;  //!< Determines if card is inserted
    bool latchClosed;  //!< Determines if Card Latch is engaged.  If device does not have a latch, value is always FALSE
    bool cardPresent;  //!< If device has a latch, determines if the card is present in device.  If the device does not have a latch, value is always FALSE
    bool magneticDataPresent;  //!< True = Magnetic data present, False = No Magnetic Data
}ICCReaderStatus;


/**
 Structure to set ICC power on options.  Used by IDT_BTPay::icc_powerOnICC:response:() IDT_UniPay::icc_powerOnICC:response:()
 */
typedef struct {
    BOOL sendIFS; //!< Send S(IFS) request if T=1 protocolError: Reference source not found
    BOOL explicitPPS;  //!< Explicit PPSError: Reference source not found
    BOOL disableAutoPPS; //!< No auto pps for negotiate mode
    BOOL disableResponseCheck;  //!< No check on response of S(IFS) request
    unsigned char* pps; //!< pps is used to set the Protocol and Parameters Selection between card and reader, only Di <= 4 are supported. pps must follow the structure specified in ISO 7816-3 as PPS0, [PPS1], [PPS2], and [PPS3]. For more information see ISO 7816-3 section 7.2.
    unsigned char ppsLength; //!< lenght of pps data
}PowerOnStructure;
#ifndef UNIPAY_SHOULD_SKIP_THIS

/**
 Certificate Authority Public Key
 
 Used as parameter in IDT_BTPay::emv_retrieveCAPK:response:(), IDT_BTPay::emv_removeCAPK:(), IDT_BTPay::emv_setCAPK:(), IDT_UniPay::emv_retrieveCAPK:response:(), IDT_UniPay::emv_removeCAPK:(), IDT_UniPay::emv_setCAPK:()
 
 Used as return value in IDT_BTPay::emv_retrieveCAPK:response:() IDT_UniPay::emv_retrieveCAPK:response:()
 */
typedef struct {
    unsigned char hashAlgorithm;       //!< Hash Algorithm  0x01 = SHA-1
    unsigned char encryptionAlgorithm;       //!< Encryption Algorithm 0x01 = RSA
    unsigned char rid[5];  //!< As per payment networks definition
    unsigned char index;  //!< As per payment networks definition
    unsigned char exponentLength;  //!< Length of exponent. 0x01 or 0x03 as per EMV specs
    unsigned char keyLength;  //!< Length of key. max 248 bytes as per EMV specs
    unsigned char exponent[3];  //!< CA Public Key Exponent
    unsigned char key[248];       //!< CA Public Key
    
} CAKey;


/**
 AID Entry - Used to populate array in IDT_BTPay::emv_retrieveAIDList:()  IDT_UniPay::emv_retrieveAIDList:().
 */
typedef struct {
    unsigned char aid[16];        //!< AID value as per payment networks.
    unsigned char aidLen;        //!< AID’s length.
} AIDEntry;

/**
 Mask and Encryption - Used to Set/Retrieve mask and encryption values IDT_BTPay::emv_retrieveAIDList:()  IDT_UniPay::emv_retrieveAIDList:().
 */
typedef struct {
    unsigned char prePANClear;        //!< Leading PAN digits to display. Values '0' - '6'.  Default '4'
    unsigned char postPANClear;        //!< Last PAN digits to display. Values '0' - '4'.  Default '4'
    unsigned char maskChar;        //!< Last PAN digits to display. Values 0x20-0x7E.  Default 0x2A '*'
    unsigned char displayExpDate;        //!< Mask or display expiration date. Values '0' = mask, '1' = don't mask.  Default '1'
    unsigned char baseKeyType;        //!< BTPay Only. Key Type. Values '0' = Data Key,  '1' = Pin Key.  Default '0'
    unsigned char encryptionType;     //!< BTPay Only. Key Type. Values '1' = TDES,  '2' = AES.  Default '1'
    unsigned char encryptionOption;     //!< UniPay II Only.
    //!< Bit 0: T1 force encrypt
    //!< Bit 1 : T2 force encrypt
    //!< Bit 2 : T3 force encrypt
    //!< Bit3 : T3 force encrypt when card type is 0
    unsigned char maskOption;     //!< UniPay II Only.
    //!< Bit0: T1 mask allowed
    //!< Bit1: T2 mask allowed
    //!< Bit2: T3 mask allowed
} MaskAndEncryption;


/**
 device AID File - 571 bytes
 
 Used as parameter in IDT_BTPay::emv_setApplicationData:()
 
 Used as return value of aidResponse in IDT_BTPay::emv_retrieveApplicationData:response:()
 */
typedef struct {
    unsigned char acquirerIdentifier[6];        //!< Indicates  which  acquirer/processor  processes  the corresponding AID. Tag 9F01
    unsigned char aid[16];        //!< AID value as per payment networks.  Tag 9F06
    unsigned char aidLen;        //!< AID’s length.
    unsigned char applicationSelectionIndicator;        //!< Standard parameter.
    unsigned char applicationVersionNumber[2];        //!< EMV application version number.   Tag 9F09
    unsigned char XAmount[3];        //!< Not used by Agnos Framework.
    unsigned char YAmount[3];        //!< Not used by Agnos Framework.
    unsigned char skipTACIACDefault;        //!< Indicates whether or not terminal uses default values for risk management.
    unsigned char tac;        //!< Indicates whether or not terminal uses Terminal Action Code.   0x00 or 0x01
    unsigned char floorlLimitChecking;        //!< Indicates whether  or  not  terminal  uses  Floor  Limit Checking.   0x00 or 0x01
    unsigned char randomTransactionSelection;        //!< Indicates whether  or  not  terminal  uses  Random Transaction Selection.   0x00 or 0x01
    unsigned char velocitiyChecking;        //!< Indicates whether  or  not  terminal  uses  Velocity Checking.   0x00 or 0x01
    unsigned char tACDenial[5];        //!< Terminal Action Code Denial.
    unsigned char tACOnline[5];        //!< Terminal Action Code Online.
    unsigned char tACDefault[5];        //!< Terminal Action Code Default.
    unsigned char terminalFloorLimit[3];        //!< Standard parameter.  Tag 9F1B
    unsigned char targetPercentage;        //!< EMV offline risk management parameter.
    unsigned char thresholdValue[3];        //!< EMV offline risk management parameter.
    unsigned char maxTargetPercentage;        //!< EMV offline risk management parameter.
    unsigned char defaultTDOL;        //!< Standard parameter.
    unsigned char tdolValue[252];        //!< Transaction Data Object List value.
    unsigned char tdolLen;        //!< Transaction Data Object List length.
    unsigned char defaultDDOL;        //!< Standard  parameter..  Tag
    unsigned char ddolValue[252];        //!< Dynamic Data Object List value.
    unsigned char ddolLen;        //!< Dynamic Data Object List length.
    unsigned char transactionCurrencyCode[2];        //!< AID’s currency.   Example: For Canada, {0x01,0x24}.  Tag 5F2A
    unsigned char transactionCurrencyExponent;        //!< Transaction Currency Exponent.   Example: Amount 4.53$ is managed as 453. Tag 5F36
    
} ApplicationID;


/**
 device Terminal Configuration File - 44 bytes
 
 Used as parameter in IDT_BTPay::setTerminalData:()
 
 Used as return value in IDT_BTPay::emv_retrieveTerminalData:()
 */
typedef struct {
    unsigned char terminalCountryCode[2];     //!< Terminal’s location. Tag 9F1A {0x08,0x40}
    unsigned char provideCardholderConfirmation;     //!< Indicates whether or not cardholder may confirm application selection at EMV Selection time. Tag 58 0x00 or 0x01
    unsigned char terminalType;     //!< Standard parameter. Tag 9F35  See EMVCo book IV
    unsigned char emvContact;     //!< Indicates whether terminal supports EMV contact. Tag 9F33, byte 1, bit 6 0x00 or 0x01
    unsigned char terminalCapabilities[3];     //!< Standard parameter.  Tag 9F33 See EMVCo book IV
    unsigned char additionalTerminalCapabilities[5];     //!< Standard parameter.  Tag 9F40 See EMVCo book IV
    unsigned char emvContactless;     //!< Indicates whether  or  not  terminal support scontactless  in EMV mode.  0x00 or 0x01
    unsigned char magstripe;     //!< Indicates whether terminal supports magstripe.  0x00 or 0x01
    unsigned char pinTimeOut;     //!< In seconds. Time allocated to cardholder to enter PIN.  Binary value Example  : 0x0F for 15s
    unsigned char batchManaged;     //!< Indicates whether or not Batch messages are supported by Terminal.  0x00 or 0x01
    unsigned char adviceManaged;     //!< Indicates whether or not Advice messages are supported by Terminal (only if needed by Level3 implementation).  0x00 or 0x01
    unsigned char pse;     //!< Indicates whether or not PSE Selection method is supported by Terminal.  0x00 or 0x01
    unsigned char autoRun;     //!< Indicates whether or not Terminal is configured in AutoRun.  0x00 or 0x01
    unsigned char predefinedAmount[3];     //!< Fixed amount.  Binary value
    unsigned char pinByPass;     //!< Indicates whether or not PIN bypass is supported by Terminal.  0x00 or 0x01
    unsigned char referalManaged;     //!< Indicates whether or not Referal managed are supported by Terminal (only if needed by Level3 implementation)..  0x00 or 0x01
    unsigned char defaultTAC;     //!< Indicates whether or not Default TAC are supported by Terminal.  0x00 or 0x01
    unsigned char defaultTACDenial[5];     //!< Default TAC Denial value.  See EMVCo book IV
    unsigned char defaultTACOnline[5];     //!< Default TAC Online value.  See EMVCo book IV
    unsigned char defaultTACDefault[5];     //!< Default TAC Default value.  See EMVCo book IV
    unsigned char notRTS;     //!< Indicates RTS are not supported by Terminal or not.  0x00 or 0x01
    unsigned char notVelocity;     //!< Indicates Velocity are not supported by Terminal or not.  0x00 or 0x01
    unsigned char cdaType;  //!< Supported CDA type. Value should be 0x02
} TerminalData;

/**
 Certificate Revocation List Entry - 9 bytes
 
 Used as parameter in IDT_BTPay::emv_retrieveCRLForRID:response:(), IDT_BTPay::emv_removeCRL:(), IDT_BTPay::emv_removeCRLUnit:(), IDT_BTPay::emv_setCRL:() IDT_UniPay::emv_retrieveCRLForRID:response:(), IDT_UniPay::emv_removeCRL:(), IDT_UniPay::emv_removeCRLUnit:(), IDT_UniPay::emv_setCRL:()
 
 */
typedef struct {
    unsigned char rid[5];  //!< As per payment networks definition
    unsigned char index;  //!< As per payment networks definition
    unsigned char serialNumber[3];  //!< As per payment networks definition
} CRLEntry;

#endif
//Versioning

//! Capture Encode Types
typedef enum _CAPTURE_ENCODE_TYPE{
    CAPTURE_ENCODE_TYPE_UNKNOWN=0,
    CAPTURE_ENCODE_TYPE_ISOABA=1,
    CAPTURE_ENCODE_TYPE_AAMVA=3,
    CAPTURE_ENCODE_TYPE_Other=4,
    CAPTURE_ENCODE_TYPE_Raw=5,
    CAPTURE_ENCODE_TYPE_JIS_I=6,
    CAPTURE_ENCODE_TYPE_JIS_II,
	CAPTURE_ENCODE_TYPE_CTLS_VISA_K1,
	CAPTURE_ENCODE_TYPE_CTLS_MC,
	CAPTURE_ENCODE_TYPE_CTLS_VISA_K3,
	CAPTURE_ENCODE_TYPE_CTLS_AMEX,
	CAPTURE_ENCODE_TYPE_CTLS_JCB,
	CAPTURE_ENCODE_TYPE_CTLS_DISCOVER,
	CAPTURE_ENCODE_TYPE_CTLS_UNIONPAY,
	CAPTURE_ENCODE_TYPE_CTLS_OTHER,
    CAPTURE_ENCODE_TYPE_MANUAL_ENTRY
} CAPTURE_ENCODE_TYPE;

/** Capture Encrypt Types **/
typedef enum{
    CAPTURE_ENCRYPT_TYPE_UNKNOWN=0,
    CAPTURE_ENCRYPT_TYPE_TDES=1,
    CAPTURE_ENCRYPT_TYPE_AES,
	CAPTURE_ENCRYPT_TYPE_NONE,  CAPTURE_ENCRYPT_TRANS_ARMOR_PKI,  CAPTURE_ENCRYPT_VOLTAGE,  CAPTURE_ENCRYPT_VISA_FPE, CAPTURE_ENCRYPT_VERIFONE_FPE, CAPTURE_ENCRYPT_DESJARDIN, CAPTURE_ENCRYPT_TRANS_ARMOR_TDES,
    CAPTURE_ENCRYPT_TYPE_NO_ENCRYPTION=99
} CAPTURE_ENCRYPT_TYPE;

    typedef enum
    {
        CAPTURE_CARD_TYPE_UNKNOWN, CAPTURE_CARD_TYPE_CONTACT, CAPTURE_CARD_TYPE_CTLS_EMV, CAPTURE_CARD_TYPE_CTLS_MSD, CAPTURE_CARD_TYPE_MSR
	} CAPTURE_CARD_TYPE;

    typedef enum
    {
        KEY_VARIANT_TYPE_UNKNOWN, KEY_VARIANT_TYPE_DATA, KEY_VARIANT_TYPE_PIN
	}KEY_VARIANT_TYPE;

typedef enum{
    POWER_ON_OPTION_IFS_FLAG=1,
    POWER_ON_OPTION_EXPLICIT_PPS_FLAG=2,
    POWER_ON_OPTION_AUTO_PPS_FLAG=64,
    POWER_ON_OPTION_IFS_RESPONSE_CHECK_FLAG=128
}POWER_ON_OPTION;

typedef enum{
    LANGUAGE_TYPE_ENGLISH=1,
    LANGUAGE_TYPE_PORTUGUESE,
    LANGUAGE_TYPE_SPANISH,
    LANGUAGE_TYPE_FRENCH
}LANGUAGE_TYPE;

typedef enum{
    PIN_KEY_TDES_MKSK_extp=0x00, //external plain text
    PIN_KEY_TDES_DUKPT_extp=0x01, //external plain text
    PIN_KEY_TDES_MKSK_intl=0x10,  //internal BTPay
    PIN_KEY_TDES_DUKPT_intl=0x11, //internal BTPay
    PIN_KEY_TDES_MKSK2_intl=0x20,  //internal UniPayII
    PIN_KEY_TDES_DUKPT2_intl=0x21, //internal  UniPayII
}PIN_KEY_Types;

#define EVENT_IDLE  0
#define EVENT_ASYNC_DATA 0x0100
#define EVENT_SDK_BUSY 0x0200

typedef enum{
    EVENT_PINPAD_UNKNOWN = 11,
    EVENT_PINPAD_ENCRYPTED_PIN,
    EVENT_PINPAD_NUMERIC,
    EVENT_PINPAD_AMOUNT,
    EVENT_PINPAD_ACCOUNT,
    EVENT_PINPAD_ENCRYPTED_DATA,
    EVENT_PINPAD_CANCEL,
    EVENT_PINPAD_TIMEOUT,
    EVENT_PINPAD_FUNCTION_KEY,
    EVENT_PINPAD_DATA_ERROR,
    EVENT_PINPAD_PAN_ERROR,
    EVENT_PINPAD_PIN_DUKPT_MISSING,
    EVENT_PINPAD_PIN_DUKPT_EXHAUSTED,
    EVENT_PINPAD_DISPLAY_MESSAGE_ERROR
}EVENT_PINPAD_Types;



typedef enum{
    IDT_DEVICE_BTPAY_IOS = 0,
    IDT_DEVICE_BTPAY_OSX_BT,
    IDT_DEVICE_BTPAY_OSX_USB,
    IDT_DEVICE_UNIPAY_IOS,
    IDT_DEVICE_UNIPAY_OSX_USB,
    IDT_DEVICE_UNIPAYII_IOS,
    IDT_DEVICE_UNIPAYII_OSX_USB,
    IDT_DEVICE_IMAG_IOS,
    IDT_DEVICE_VP3300_IOS,
    IDT_DEVICE_VP3300_OSX_USB,
    IDT_DEVICE_UNIMAG,
    IDT_DEVICE_BTMAG_IOS,
    IDT_DEVICE_BTMAG_OSX_BT,
    IDT_DEVICE_BTMAG_OSX_USB,
    IDT_DEVICE_UNIPAYI_V_IOS,
    IDT_DEVICE_UNIPAYI_V_OSX_USB,
    IDT_DEVICE_NEO2_IOS,
	IDT_DEVICE_VP8800_IOS,
    IDT_DEVICE_UNIMAG_PRO,
	IDT_DEVICE_NONE

	


}IDT_DEVICE_Types;



typedef enum{
    EVENT_MSR_UNKNOWN = 31,
    EVENT_MSR_CARD_DATA,
    EVENT_MSR_CANCEL_KEY,
    EVENT_MSR_BACKSPACE_KEY,
    EVENT_MSR_ENTER_KEY,
    EVENT_MSR_DATA_ERROR,
    EVENT_MSR_ICC_START,
    EVENT_BTPAY_CARD_DATA,
    EVENT_UNIPAYII_EMV_NO_ICC_MSR_DATA,
    EVENT_UNIPAYII_EMV_FALLBACK_DATA,
    EVENT_UNIPAY_KEYLOADING,
    EVENT_MSR_TIMEOUT
}EVENT_MSR_Types;

typedef enum{
    EVENT_ACTIVE_TRANSACTION = 51
}EVENT_CTLS_Types;

typedef enum{
    UNIMAG_COMMAND_DEFAULT_GENERAL_SETTINGS,
    UNIMAG_COMMAND_ENABLE_ERR_NOTIFICATION,
    UNIMAG_COMMAND_DISABLE_ERR_NOTIFICATION,
    UNIMAG_COMMAND_ENABLE_EXP_DATE,
    UNIMAG_COMMAND_DISABLE_EXP_DATE,
    UNIMAG_COMMAND_CLEAR_BUFFER,
    UNIMAG_COMMAND_RESET_BAUD_RATE
}UNIMAG_COMMAND_Types;

typedef enum {
	RETURN_CODE_DO_SUCCESS = 0,             //!< no error, beginning task
	RETURN_CODE_ERR_DISCONNECT_,         //!< no response from reader
	RETURN_CODE_ERR_CMD_RESPONSE_,       //!< invalid response data
	RETURN_CODE_ERR_TIMEDOUT_,           //!< time out for task or CMD
	RETURN_CODE_ERR_INVALID_PARAMETER_,  //!< wrong parameter
	RETURN_CODE_SDK_BUSY_MSR_,           //!< SDK is doing MSR or ICC task
	RETURN_CODE_SDK_BUSY_PINPAD_,        //!< SDK is doing PINPad task
	RETURN_CODE_SDK_BUSY_CTLS_,        //!< SDK is doing CTLS task
	RETURN_CODE_SDK_BUSY_EMV_,        //!< SDK is doing EMV task
	RETURN_CODE_ERR_OTHER_,              //!< SDK is doing Other task
	RETURN_CODE_FAILED_,                 //!< err response or data
	RETURN_CODE_NOT_ATTACHED_,           //!< no reader attached
	RETURN_CODE_MONO_AUDIO_,           //!< mono audio is enabled
	RETURN_CODE_CONNECTED_,           //!< did connection
	RETURN_CODE_LOW_VOLUME_,           //!< audio volume is too low
	RETURN_CODE_CANCELED_,           //!< task or CMD be canceled
	RETURN_CODE_INVALID_STR_,           //!< UF wrong string format
	RETURN_CODE_NO_FILE_,           //!< UF file not found
	RETURN_CODE_INVALID_FILE_,           //!< UF wrong file format
	RETURN_CODE_HOST_UNREACHABLE_,           //!< Attempt to contact online host failed
	RETURN_CODE_RKI_FAILURE_,           //!< Attempt to perform RKI failed
	RETURN_CODE_MISSING_DLL_,           //!< DLL is missing for the target device
	RETURN_CODE_BLOCK_TRANSFER_SUCCESS_,           //!< The current block of the  file was transferred successfully
	RETURN_CODE_SDK_BUSY_FIRMWARE_UPDATE_,           //!< The SDK Busy doing firmware update
	RETURN_CODE_APPLYING_FIRMWARE_UPDATE_,           //!< Applying the firmware update downloaded to memory.
	RETURN_CODE_NO_DATA_AVAILABLE_,           //!< No data available.
	RETURN_CODE_SDK_BUSY_FILE_TRANSFER_,           //!< The SDK Busy doing File Transfer.
	RETURN_CODE_APPLYING_FILE_TRANSFER_,           //!< Applying the File Transfer.
	RETURN_CODE_FILE_TRANSFER_SUCCESSFUL_,           //!< File Transfer Successful.
	RETURN_CODE_NO_DRIVE_SPACE_,           //!< Not enough space available on drive.
	RETURN_CODE_ENTERING_BOOTLOADER_MODE_,           //!< Entering bootloader mode.
	RETURN_CODE_FW_STARTING_UPDATE_,           //!< Starting firmware update.
	RETURN_CODE_FW_PCI_MISMATCH_,           //!< FW PCI Version Mismatch.
	RETURN_CODE_FW_BLOCK_MISMATCH_,           //!< FW Block transfer size mismatch.
	RETURN_CODE_DEVICE_FINALIZING_TRANSACTION_,           //!< Device is busy finalizing transaction.
	RETURN_CODE_SDK_BUSY_RKI_UPDATE_,           //!< The SDK Busy doing RKI update
	RETURN_CODE_BAD_MSR_SWIPE_,          //!< Bad MSR Swipe
	RETURN_CODE_FINANCIAL_CARD_NOT_ALLOWED_,          //!< Financial card not allowed
	RETURN_CODE_SDK_BUSY_GET_EVENT_,        //!< SDK is waiting for input event
	RETURN_CODE_UNSUPPORTED_COMMAND_,        //!< SDK is waiting for input event
    RETURN_CODE_ERR_DISCONNECT = 0xFF01,         //!< no response from reader
    RETURN_CODE_ERR_CMD_RESPONSE = 0xFF02,       //!< invalid response data
    RETURN_CODE_ERR_TIMEDOUT = 0xFF03,           //!< time out for task or CMD
    RETURN_CODE_ERR_INVALID_PARAMETER = 0xFF04,  //!< wrong parameter
    RETURN_CODE_SDK_BUSY_MSR = 0xFF05,           //!< SDK is doing MSR or ICC task
    RETURN_CODE_SDK_BUSY_PINPAD = 0xFF06,        //!< SDK is doing PINPad task
    RETURN_CODE_SDK_BUSY_CTLS = 0xFF07,        //!< SDK is doing CTLS task
    RETURN_CODE_ERR_OTHER = 0xFF08,              //!< SDK is doing Other task
    RETURN_CODE_FAILED = 0xFF09,                 //!< err response or data
    RETURN_CODE_NOT_ATTACHED = 0xFF0A,           //!< no reader attached
    RETURN_CODE_MONO_AUDIO = 0xFF0B,           //!< mono audio is enabled
    RETURN_CODE_CONNECTED = 0xFF0C,           //!< did connection
    RETURN_CODE_LOW_VOLUME = 0xFF0D,           //!< audio volume is too low
    RETURN_CODE_CANCELED = 0xFF0E,           //!< task or CMD be canceled
    RETURN_CODE_INVALID_STR = 0xFF0F,           //!< UF wrong string format
    RETURN_CODE_NO_FILE = 0xFF10,           //!< UF file not found
    RETURN_CODE_INVALID_FILE = 0xFF11,           //!< UF wrong file format
    RETURN_CODE_HOST_UNREACHABLE = 0xFF12,           //!< Attempt to contact online host failed
    RETURN_CODE_RKI_FAILURE = 0xFF13,           //!< Attempt to perform RKI failed
    RETURN_CODE_SDK_BUSY_CMD = 0xFF14,           //!< SDK is busy processing another CMD

    
    
    RETURN_CODE_EMV_AUTHORIZATION_ACCEPTED = 0x0E00,          //!< Authorization Accepted
    RETURN_CODE_EMV_AUTHORIZATION_UNABLE_TO_GO_ONLINE = 0x0E01,   //!< Unable to go online
    RETURN_CODE_EMV_AUTHORIZATION_TECHNICAL_ISSUE = 0x0E02,   //!< Technical Issue
    RETURN_CODE_EMV_AUTHORIZATION_DECLINED = 0x0E03,           //!< Declined
    RETURN_CODE_EMV_AUTHORIZATION_ISSUER_REFERRAL = 0x0E04,           //!< Issuer Referral transaction
    
    RETURN_CODE_EMV_APPROVED = 0x0F00,   //!< Accept the online transaction
    RETURN_CODE_EMV_DECLINED = 0x0F01,   //!< Decline the online transaction
    RETURN_CODE_EMV_GO_ONLINE = 0x0F02,          //!< Request to go online
    RETURN_CODE_EMV_FAILED = 0x0F03,             //!< Transaction is terminated
    RETURN_CODE_EMV_SYSTEM_ERROR = 0x0F05,       //!< Application was not selected by kernel or ICC format error or ICC missing data error
    RETURN_CODE_EMV_NOT_ACCEPTED = 0x0F07,       //!< ICC didn't accept transaction
    RETURN_CODE_EMV_FALLBACK = 0x0F0A,           //!< Application may fallback to magstripe technology
    RETURN_CODE_EMV_CANCEL = 0x0F0C,             //!< Transaction was cancelled
    RETURN_CODE_EMV_TIMEOUT = 0x0F0D,             //!< Timeout
    RETURN_CODE_EMV_OTHER_ERROR = 0x0F0F,        //!< Other EMV Error
    RETURN_CODE_EMV_OFFLINE_APPROVED = 0x0F10,   //!< Accept the offline transaction
    RETURN_CODE_EMV_OFFLINE_DECLINED = 0x0F11,   //!< Decline the offline transaction
    
    
    
    RETURN_CODE_EMV_NEW_SELECTION = 0x0F21,      //!< ICC detected that the conditions of use are not satisfied
    RETURN_CODE_EMV_NO_AVAILABLE_APPS = 0x0F22,   //!< No app were found on card matching terminal configuration
    RETURN_CODE_EMV_NO_TERMINAL_FILE = 0x0F23,   //!< Terminal file does not exist
    RETURN_CODE_EMV_NO_CAPK_FILE = 0x0F24,       //!< CAPK file does not exist
    RETURN_CODE_EMV_NO_CRL_ENTRY = 0x0F25,       //!< CRL Entry does not exist
    RETURN_CODE_BLOCKING_DISABLED = 0x0FFE,        //!< Return code when blocking is disabled
    RETURN_CODE_COMMAND_UNAVAILABLE = 0x0FFF,       //!< Return code when command is unavailable
    
    //IDG Return Codes
    RETURN_CODE_NEO_SUCCESS = 0xEE00,      //!<Command Successful
    RETURN_CODE_NEO_INCORRECT_HEADER_TAG = 0xEE01,      //!<Incorrect Header Tag
    RETURN_CODE_NEO_UNKNOWN_COMMAND = 0xEE02,      //!<Unknown Command
    RETURN_CODE_NEO_UNKNOWN_SUB_COMMAND = 0xEE03,      //!<Unknown Sub-Command
    RETURN_CODE_NEO_CRC_ERROR_IN_FRAME = 0xEE04,      //!<CRC Error in Frame
    RETURN_CODE_NEO_INCORRECT_PARAMETER = 0xEE05,      //!<Incorrect Parameter
    RETURN_CODE_NEO_PARAMETER_NOT_SUPPORTED = 0xEE06,      //!<Parameter Not Supported
    RETURN_CODE_NEO_MAL_FORMATTED_DATA = 0xEE07,      //!<Mal-formatted Data
    RETURN_CODE_NEO_TIMEOUT = 0xEE08,      //!<Timeout
    RETURN_CODE_NEO_FAILED_NAK = 0xEE0A,      //!<Failed / NACK
    RETURN_CODE_NEO_COMMAND_NOT_ALLOWED = 0xEE0B,      //!<Command not Allowed
    RETURN_CODE_NEO_SUB_COMMAND_NOT_ALLOWED = 0xEE0C,      //!<Sub-Command not Allowed
    RETURN_CODE_NEO_BUFFER_OVERFLOW = 0xEE0D,      //!<Buffer Overflow (Data Length too large for reader buffer)
    RETURN_CODE_NEO_USER_INTERFACE_EVENT = 0xEE0E,      //!<User Interface Event
    RETURN_CODE_NEO_COMM_TYPE_NOT_SUPPORTED = 0xEE11,      //!<Communication type not supported, VT-1, burst, etc.
    RETURN_CODE_NEO_SECURE_INTERFACE_NOT_FUNCTIONAL = 0xEE12,      //!<Secure interface is not functional or is in an intermediate state.
    RETURN_CODE_NEO_DATA_FIELD_NOT_MOD8 = 0xEE13,      //!<Data field is not mod 8
    RETURN_CODE_NEO_PADDING_UNEXPECTED = 0xEE14,      //!<Pad 0x80 not found where expected
    RETURN_CODE_NEO_INVALID_KEY_TYPE = 0xEE15,      //!<Specified key type is invalid
    RETURN_CODE_NEO_CANNOT_RETRIEVE_KEY = 0xEE16,      //!<Could not retrieve key from the SAM (InitSecureComm)
    RETURN_CODE_NEO_HASH_CODE_ERROR = 0xEE17,      //!<Hash code problem
    RETURN_CODE_NEO_CANNOT_STORE_KEY = 0xEE18,      //!<Could not store the key into the SAM (InstallKey)
    RETURN_CODE_NEO_FRAME_TOO_LARGE = 0xEE19,      //!<Frame is too large
    RETURN_CODE_NEO_RESEND_COMMAND = 0xEE1A,      //!<Unit powered up in authentication state but POS must resend the InitSecureComm command
    RETURN_CODE_NEO_EEPROM_NOT_INITALIZED = 0xEE1B,      //!<The EEPROM may not be initialized because SecCommInterface does not make sense
    RETURN_CODE_NEO_PROBLEM_ENCODING_APDU = 0xEE1C,      //!<Problem encoding APDU
    RETURN_CODE_NEO_UNSUPPORTED_INDEX = 0xEE20,      //!<Unsupported Index (ILM). SAM Transceiver error – problem communicating with the SAM (Key Mgr)
    RETURN_CODE_NEO_UNEXPECTED_SEQUENCE_COUNTER = 0xEE21,      //!<Unexpected Sequence Counter in multiple frames for single bitmap (ILM). Length error in data returned from the SAM (Key Mgr)
    RETURN_CODE_NEO_IMPROPER_BITMAP = 0xEE22,      //!<Improper bit map (ILM)
    RETURN_CODE_NEO_REQUEST_ONLINE_AUTHORIZATION = 0xEE23,      //!<Request Online Authorization
    RETURN_CODE_NEO_RAW_DATA_READ_SUCCESSFUL = 0xEE24,      //!<ViVOCard3 raw data read successful
    RETURN_CODE_NEO_MESSAGE_NOT_AVAILABLE = 0xEE25,      //!<Message index not available (ILM). ViVOcomm activate transaction card type (ViVOcomm)
    RETURN_CODE_NEO_VERSION_INFORMATION_MISMATCH = 0xEE26,      //!<Version Information Mismatch (ILM)
    RETURN_CODE_NEO_NOT_SENDING_COMMANDS = 0xEE27,      //!<Not sending commands in correct index message index (ILM)
    RETURN_CODE_NEO_TIMEOUT_ILM = 0xEE28,      //!<Time out or next expected message not received (ILM)
    RETURN_CODE_NEO_ILM_NOT_AVAILABLE = 0xEE29,      //!<ILM languages not available for viewing (ILM)
    RETURN_CODE_NEO_OTHER_LANG_NOT_SUPPORTED = 0xEE2A,      //!<Other language not supported (ILM)
	RETURN_CODE_UNKNOWN_ERROR_FROM_SAM = 0XEE41, //!< Unknown Error from SAM
	RETURN_CODE_INVALID_DATA_DETECTED_BY_SAM = 0XEE42, //!< Invalid data detected by SAM
	RETURN_CODE_INCOMPLETE_DATA_DETECTED_BY_SAM = 0XEE43, //!< Incomplete data detected by SAM
	RETURN_CODE_RESERVED = 0XEE44, //!< Reserved
	RETURN_CODE_INVALID_KEY_HASH_ALGORITHM = 0XEE45, //!< Invalid key hash algorithm
	RETURN_CODE_INVALID_KEY_ENCRYPTION_ALRORITHM = 0XEE46, //!< Invalid key encryption algorithm
	RETURN_CODE_INVALID_MODULUS_LENGTH = 0XEE47, //!< Invalid modulus length
	RETURN_CODE_INVALID_EXPONENT = 0XEE48, //!< Invalid exponent
	RETURN_CODE_KEY_ALREADY_EXISTS = 0XEE49, //!< Key already exists
	RETURN_CODE_NO_SPACE_FOR_NEW_RID = 0XEE4A, //!< No space for new RID
	RETURN_CODE_KEY_NOT_FOUND = 0XEE4B, //!< Key not found
	RETURN_CODE_CRYPTO_NOT_RESPONDING = 0XEE4C, //!< Crypto not responding
	RETURN_CODE_CRYPTO_COMMUNICATION_ERROR = 0XEE4D, //!< Crypto communication error
	RETURN_CODE_P2_KEY_MANAGER_ERROR_4E = 0XEE4E, //!< Module-specific error for Key Manager
	RETURN_CODE_ALL_KEY_SLOTS_FULL = 0XEE4F, //!< All key slots are full (maximum number of keys has been installed)
    RETURN_CODE_NEO_AUTO_SWITCH_OK = 0xEE50,      //!Auto-Switch OK
    RETURN_CODE_NEO_AUTO_SWITCH_FAILED = 0xEE51,      //!Auto-Switch failed
    RETURN_CODE_DATA_DOES_NOT_EXIST = 0xEE60,      //!Data not exist
    RETURN_CODE_DATA_FULL = 0xEE61,      //!Data Full
    RETURN_CODE_WRITE_FLASH_ERROR = 0xEE62,      //!Write Flash Error
	RETURN_CODE_OK_NEXT_COMMAND = 0xEE63,      //!Ok and Have Next Command
	RETURN_CODE_KERNEL_ABSENT = 0xEE64,      //!Kernel is absent
	RETURN_CODE_UNEXPECTED_COMMAND_SEQUENCE = 0xEE65,      //!Unexpected Command Sequence
	RETURN_CODE_UNEXPECTED_INDEX = 0xEE66,      //!Unexpected Index
	RETURN_CODE_FILE_SIZE_DOES_NOT_MATCH = 0xEE67,      //!File size does not match
	RETURN_CODE_PLUG_IN_CHARGING_CABLE = 0xEE68,      //!Plug in charging cable

    RETURN_CODE_CANNOT_START_CONTACT_EMV = 0xEE80,      //!Cannot start Contact EMV transaction
    RETURN_CODE_CTLS_MSR_CANCELLED_BY_CARD_INSERT = 0xEE81,      //!CTLS/MSR cancelled due to card insertion

    
    RETURN_CODE_ACCT_DUKPT_KEY_NOT_EXIST = 0xEE90,      //!Account DUKPT Key not exist
    RETURN_CODE_ACCT_DUKPT_KEY_EXHAUSTED = 0xEE91,      //!Account DUKPT Key KSN exhausted
    
    
    
    RETURN_CODE_NO_SERIAL_NUMBER = 0x6200,      //!No Serial Number
    RETURN_CODE_INVALID_COMMAND = 0x6900,      //!Invalid Command
    RETURN_CODE_NO_ADMIN_DUKPT_KEY = 0x5500,      //!No Admin DUKPT Key
    RETURN_CODE_DUKPT_KEY_STOP = 0x5501,      //!Admin DUKPT Key STOP
    RETURN_CODE_DUKPT_KEY_KSN_IS_ERROR = 0x5502,      //!Admin DUKPT Key KSN is Error
    RETURN_CODE_GET_AUTH_CODE1_FAILED = 0x5503,      //!Get Authentication Code1 Failed
    RETURN_CODE_VALIDATE_AUTH_CODE_ERROR = 0x5504,      //!Validate Authentication Code Error
    RETURN_CODE_DECRYPT_DATA_FAILED = 0x5505,      //!Encrypt Or Decrypt data failed
    RETURN_CODE_NOT_SUPPORT_NEW_KEY_TYPE = 0x5506,      //!Not Support the New Key Type
    RETURN_CODE_NEW_KEY_INDEX_IS_ERROR = 0x5507,      //!New Key Index is Error
    RETURN_CODE_STEP_ERROR = 0x5508,      //!Step Error
    RETURN_CODE_TIMED_OUT = 0x5509,      //!Timed out
    RETURN_CODE_MAC_CHECKING_ERROR = 0x550A,      //!MAC checking error
    RETURN_CODE_KEY_USAGE_ERROR = 0x550B,      //!Key Usage Error
    RETURN_CODE_MODE_OF_USE_ERROR = 0x550C,      //!Mode of Use Error
    RETURN_CODE_ALGORITHM_ERROR = 0x550D,      //!Algorithm Error
    RETURN_CODE_OTHER_ERROR = 0x550F,      //!Other Error
    
    RETURN_CODE_CANNOT_AUTHORIZE_RKI = 0x8001,      //!Authorization: Cannot initialize RKI; no customer/key information found
    RETURN_CODE_NO_KEY_INJECTION_ESTABLISHED = 0x8101,      //!Step 1: No key injection established
    RETURN_CODE_FAILED_TO_ENCRYPT_CHALLENGE = 0x8102,      //!Step 1: Failed to encrypt challenge
    RETURN_CODE_CHALLENGE_LENGTH_INCORRECT = 0x8103,      //!Step 1: challenge length is incorrect
    RETURN_CODE_INCORRECT_CHALLENGE_DATA_STEP1 = 0x8104,      //!Step 1: Incorrect challenge data
    RETURN_CODE_RESPONSE_LENGTH_INCORRECT = 0x8105,      //!Step 1: Response length incorrect
    RETURN_CODE_FIRMWARE_RESPOND_NAK_STEP1 = 0x8106,      //!Step 1: Firmware responded NAK for Step 1
    RETURN_CODE_KEY_ID_NOT_FOUND_IN_DB = 0x8201,      //!Step 2: Customer key id could not be found in the DB
    RETURN_CODE_KEY_SLOT_DOES_NOT_EXIST = 0x8202,      //!Step 2: Key Slot does not exist
    RETURN_CODE_NO_FUTURE_KSI_FROM_SERVER = 0x8203,      //!Step 2: Could not get the future KSI from the server
    RETURN_CODE_NO_TR31_DATA_BLOCK = 0x8204,      //!Step 2: Could not get TR31 data block
    RETURN_CODE_TR31_BLOCK_LENGTH_INCORRECT = 0x8205,      //!Step 2: TR31 block length is incorrect
    RETURN_CODE_INCORRECT_CHALLENGE_DATA_STEP2 = 0x8206,      //!Step 2: Incorrect challenge data
    RETURN_CODE_FIRMWARE_RESPOND_NAK_STEP2 = 0x8207,      //!Step 2: Firmware responded NAK for Step 2
    RETURN_CODE_NO_KEY_INJECTION_RECORD = 0x8301,      //!Step 3: No key injection record found
    RETURN_CODE_RKI_FAILED = 0x8302,      //!Step 3: Remote Key Injection failed (NAK)
    RETURN_CODE_INCORRECT_RESPONSE_FORM = 0x8303,      //!Step 3: Incorrect response form
    RETURN_CODE_FIRMWARE_RESPOND_NAK_STEP3 = 0x8304      //!Step 3: Firmware responded NAK for Step 3
    
} RETURN_CODE;

typedef enum {
	PK_STATUS_STARTED,
	PK_STATUS_FAILED,
	PK_STATUS_SENDING_BLOCK,
	PK_STATUS_APPLYING_UPDATE,
	PK_STATUS_COMPLETED
} PK_STATUS_Type;

typedef enum {
    DUKPT_KEY_MSR = 0x00,
    DUKPT_KEY_ICC = 0x01,
    DUKPT_KEY_Admin = 0x10,
    DUKPT_KEY_Pairing_PinPad = 0x20,
} DUKPT_KEY_Type;

typedef enum{
    EMV_PIN_MODE_CANCEL = 0X00,
    EMV_PIN_MODE_ONLINE_PIN_DUKPT = 0X01,
    EMV_PIN_MODE_ONLINE_PIN_MKSK = 0X02,
    EMV_PIN_MODE_OFFLINE_PIN = 0X03,
    EMV_PIN_MODE_BYPASS = 4,
    EMV_PIN_MODE_POG = 0x10,
    EMV_PIN_MODE_MCPOG = 0x11
} EMV_PIN_MODE_Types;

typedef enum{
    EMV_RESULT_CODE_NO_RESPONSE = -1,
    EMV_RESULT_CODE_OFFLINE_APPROVED = 0X10,
    EMV_RESULT_CODE_OFFLINE_DECLINED = 0X11,
    EMV_RESULT_CODE_APPROVED = 0X00,
    EMV_RESULT_CODE_DECLINED = 0X01,
    EMV_RESULT_CODE_GO_ONLINE = 0X02,
    EMV_RESULT_CODE_FAILED = 0X03,
    EMV_RESULT_CODE_SYSTEM_ERROR = 0X05,
    EMV_RESULT_CODE_NOT_ACCEPT = 0X07,
    EMV_RESULT_CODE_FALLBACK = 0X0A,
    EMV_RESULT_CODE_CANCEL = 0X0C,
    EMV_RESULT_CODE_OTHER_ERROR = 0X0F,
    EMV_RESULT_CODE_TIME_OUT = 0X0D,
    EMV_RESULT_CODE_REFERRAL_PROCESSING = 0X12,
    EMV_RESULT_CODE_ERROR_APP_PROCESSING = 0X13,
    EMV_RESULT_CODE_ERROR_APP_READING = 0X14,
    EMV_RESULT_CODE_ERROR_DATA_AUTH = 0X15,
    EMV_RESULT_CODE_ERROR_PROCESSING_RESTRICTIONS = 0X16,
    EMV_RESULT_CODE_ERROR_CVM_PROCESSING = 0X17,
    EMV_RESULT_CODE_ERROR_RISK_MGMT = 0X18,
    EMV_RESULT_CODE_ERROR_TERM_ACTION_ANALYSIS = 0X19,
    EMV_RESULT_CODE_ERROR_CARD_ACTION_ANALYSIS = 0X1A,
    EMV_RESULT_CODE_ERROR_APP_SELECTION_TIMEOUT = 0X1B,
    EMV_RESULT_CODE_ERROR_NO_CARD_INSERTED = 0X1C,
    EMV_RESULT_CODE_ERROR_APP_SELECTING = 0X1D,
    EMV_RESULT_CODE_ERROR_READING_CARD_APP = 0X1E,
    EMV_RESULT_CODE_ERROR_POWER_CARD_ERROR = 0X1F,
    EMV_RESULT_CODE_ERROR_NO_RESULT_CODE_PROVIDED_FOR_COMPLETION = 0X20,
    EMV_RESULT_CODE_APPROVED_WITH_ADVISE_NO_REASON = 0X21,
    EMV_RESULT_CODE_APPROVED_WITH_ADVISE_IA_FAILED = 0X22,
    EMV_RESULT_CODE_ERROR_AMOUNT_NOT_SPECIFIED = 0X23,
    EMV_RESULT_CODE_ERROR_CARD_COMPLETION = 0X24,
    EMV_RESULT_CODE_ERROR_DATA_LEN_INCORRECT = 0X25,
    EMV_RESULT_CODE_CALL_YOUR_BANK = 0X26,
    EMV_RESULT_CODE_NO_ICC_ON_CARD = 0X27,
    EMV_RESULT_CODE_NEW_SELECTION = 0X28,
    EMV_RESULT_CODE_START_TRANSACTION_SUCCESS = 0X29
} EMV_RESULT_CODE_Types;


typedef enum{
    EMV_RESULT_CODE_V2_NO_RESPONSE = -1,
    EMV_RESULT_CODE_V2_APPROVED_OFFLINE = 0x0000,
    EMV_RESULT_CODE_V2_DECLINED_OFFLINE = 0x0001,
    EMV_RESULT_CODE_V2_APPROVED = 0x0002,
    EMV_RESULT_CODE_V2_DECLINED = 0x0003,
    EMV_RESULT_CODE_V2_GO_ONLINE = 0x0004,
    EMV_RESULT_CODE_V2_CALL_YOUR_BANK = 0x0005,
    EMV_RESULT_CODE_V2_NOT_ACCEPTED = 0x0006,
    EMV_RESULT_CODE_V2_USE_MAGSTRIPE = 0x0007,
    EMV_RESULT_CODE_V2_TIME_OUT = 0x0008,
    EMV_RESULT_CODE_V2_START_TRANS_SUCCESS = 0x0010,
	EMV_RESULT_CODE_V2_SWIPE_NON_ICC = 0x0011,
	EMV_RESULT_CODE_V2_TRANSACTION_CANCELLED = 0x0012,
    EMV_RESULT_CODE_V2_GO_ONLINE_CTLS = 0x0023,
	EMV_RESULT_CODE_CTLS_TWO_CARDS = 0x7A,
	EMV_RESULT_CODE_CTLS_TERMINATE = 0x7E,
	EMV_RESULT_CODE_CTLS_TERMINATE_TRY_ANOTHER = 0x7D,
	EMV_RESULT_CODE_MSR_SWIPE_CAPTURED = 0x80,
	EMV_RESULT_CODE_REQUEST_ONLINE_PIN = 0x81,
	EMV_RESULT_CODE_REQUEST_SIGNATURE = 0x82,
	EMV_RESULT_CODE_FALLBACK_TO_CONTACT = 0x83,
	EMV_RESULT_CODE_FALLBACK_TO_OTHER = 0x84,
	EMV_RESULT_CODE_REVERSAL_REQUIRED = 0x85,
	EMV_RESULT_CODE_ADVISE_REQUIRED = 0x86,
	EMV_RESULT_CODE_ADVISE_REVERSAL_REQUIRED = 0x87,
	EMV_RESULT_CODE_NO_ADVISE_REVERSAL_REQUIRED = 0x88,
	EMV_RESULT_CODE_UNABLE_TO_REACH_HOST = 0xFF,
    EMV_RESULT_CODE_V2_FILE_ARG_INVALID = 0x1001,
    EMV_RESULT_CODE_V2_FILE_OPEN_FAILED = 0x1002,
    EMV_RESULT_CODE_V2_FILE_OPERATION_FAILED = 0X1003,
    EMV_RESULT_CODE_V2_MEMORY_NOT_ENOUGH = 0x2001,
    EMV_RESULT_CODE_V2_SMARTCARD_FAIL = 0x3001,
    EMV_RESULT_CODE_V2_SMARTCARD_INIT_FAILED = 0x3003,
    EMV_RESULT_CODE_V2_FALLBACK_SITUATION = 0x3004,
    EMV_RESULT_CODE_V2_SMARTCARD_ABSENT = 0x3005,
    EMV_RESULT_CODE_V2_SMARTCARD_TIMEOUT = 0x3006,
	EMV_RESULT_CODE_V2_MSR_CARD_ERROR = 0x3007,
	EMV_RESULT_CODE_V2_MSR_CARD_READ_ERROR = 0x3012,
	EMV_RESULT_CODE_V2_TIMEOUT_INSERT_OR_FALLBACK = 0x3013,
    EMV_RESULT_CODE_V2_PARSING_TAGS_FAILED= 0X5001,
    EMV_RESULT_CODE_V2_CARD_DATA_ELEMENT_DUPLICATE = 0X5002,
    EMV_RESULT_CODE_V2_DATA_FORMAT_INCORRECT = 0X5003,
    EMV_RESULT_CODE_V2_APP_NO_TERM = 0X5004,
    EMV_RESULT_CODE_V2_APP_NO_MATCHING = 0X5005,
    EMV_RESULT_CODE_V2_AMANDATORY_OBJECT_MISSING = 0X5006,
    EMV_RESULT_CODE_V2_APP_SELECTION_RETRY = 0X5007,
    EMV_RESULT_CODE_V2_AMOUNT_ERROR_GET = 0X5008,
    EMV_RESULT_CODE_V2_CARD_REJECTED = 0X5009,
    EMV_RESULT_CODE_V2_AIP_NOT_RECEIVED = 0X5010,
    EMV_RESULT_CODE_V2_AFL_NOT_RECEIVEDE = 0X5011,
    EMV_RESULT_CODE_V2_AFL_LEN_OUT_OF_RANGE = 0X5012,
    EMV_RESULT_CODE_V2_SFI_OUT_OF_RANGE = 0X5013,
    EMV_RESULT_CODE_V2_AFL_INCORRECT = 0X5014,
    EMV_RESULT_CODE_V2_EXP_DATE_INCORRECT = 0X5015,
    EMV_RESULT_CODE_V2_EFF_DATE_INCORRECT = 0X5016,
    EMV_RESULT_CODE_V2_ISS_COD_TBL_OUT_OF_RANGE = 0X5017,
    EMV_RESULT_CODE_V2_CRYPTOGRAM_TYPE_INCORRECT = 0X5018,
    EMV_RESULT_CODE_V2_PSE_BY_CARD_NOT_SUPPORTED = 0X5019,
    EMV_RESULT_CODE_V2_USER_LANGUAGE_SELECTED = 0X5020,
    EMV_RESULT_CODE_V2_SERVICE_NOT_ALLOWED = 0X5021,
    EMV_RESULT_CODE_V2_NO_TAG_FOUND = 0X5022,
    EMV_RESULT_CODE_V2_CARD_BLOCKED = 0X5023,
    EMV_RESULT_CODE_V2_LEN_INCORRECT = 0X5024,
    EMV_RESULT_CODE_V2_CARD_COM_ERROR = 0X5025,
    EMV_RESULT_CODE_V2_TSC_NOT_INCREASED = 0X5026,
    EMV_RESULT_CODE_V2_HASH_INCORRECT = 0X5027,
    EMV_RESULT_CODE_V2_ARC_NOT_PRESENCED = 0X5028,
    EMV_RESULT_CODE_V2_ARC_INVALID = 0X5029,
    EMV_RESULT_CODE_V2_COMM_NO_ONLINE = 0X5030,
    EMV_RESULT_CODE_V2_TRAN_TYPE_INCORRECT = 0X5031,
    EMV_RESULT_CODE_V2_APP_NO_SUPPORT = 0X5032,
    EMV_RESULT_CODE_V2_APP_NOT_SELECT = 0X5033,
    EMV_RESULT_CODE_V2_LANG_NOT_SELECT = 0X5034,
    EMV_RESULT_CODE_V2_TERM_DATA_NOT_PRESENCED = 0X5035,
    EMV_RESULT_CODE_V2_CVM_TYPE_UNKNOWN = 0X6001,
    EMV_RESULT_CODE_V2_CVM_AIP_NOT_SUPPORTED = 0X6002,
    EMV_RESULT_CODE_V2_CVM_TAG_8E_MISSING = 0X6003,
    EMV_RESULT_CODE_V2_CVM_TAG_8E_FORMAT_ERROR = 0X6004,
    EMV_RESULT_CODE_V2_CVM_CODE_IS_NOT_SUPPORTED = 0X6005,
    EMV_RESULT_CODE_V2_CVM_COND_CODE_IS_NOT_SUPPORTED = 0X6006,
    EMV_RESULT_CODE_V2_CVM_NO_MORE = 0X6007,
    EMV_RESULT_CODE_V2_PIN_BYPASSED_BEFORE = 0X6008,
    EMV_RESULT_CODE_V2_GO_ONLINE_CL = 0xEE23
} EMV_RESULT_CODE_V2_Types;




typedef enum{
    EMV_AUTHORIZATION_RESULT_ACCEPTED = 0X00,
    EMV_AUTHORIZATION_RESULT_UNABLE_TO_GO_ONLINE = 0X01,
    EMV_AUTHORIZATION_RESULT_TECHNICAL_ISSUE = 0X02,
    EMV_AUTHORIZATION_RESULT_DECLINED = 0X03,
    EMV_AUTHORIZATION_RESULT_ISSUER_REFERAL = 0X04
} EMV_AUTHORIZATION_RESULT;


typedef enum{
    FIRMWARE_TYPE_UNKNOWN = -1, //!< UNKNOWN FIRMWARE TYPE.
    FIRMWARE_TYPE_External_EMV_CTL2 = 0x00, //!< External EMV CTL2 Kernel.
    FIRMWARE_TYPE_EMV_CLL2_PPSE = 0x01, //!< EMV CLL2 PPSE Kernel.
    FIRMWARE_TYPE_EMV_CLL2_VCPS = 0x02, //!< EMV CLL2 VCPS Kernel (Visa).
    FIRMWARE_TYPE_EMV_CLL2_MChip = 0x03, //!< EMV CLL2 MChip Kernel.
    FIRMWARE_TYPE_EMV_CLL2_DPass = 0x04, //!< EMV CLL2 DPass Kernel.
    FIRMWARE_TYPE_EMV_CLL2_Amex = 0x05, //!< EMV CLL2 Amex Kernel.
    FIRMWARE_TYPE_EMV_CLL2_Interac = 0x06, //!< EMV CLL2 Interac Kernel.
    FIRMWARE_TYPE_EMV_CLL2_CUP = 0x07, //!< EMV CLL2 CUP Kernel.
    FIRMWARE_TYPE_EMV_CLL2_JCB = 0x08, //!< EMV CLL2 JCB Kernel.
    FIRMWARE_TYPE_APVAS = 0x09, //!< APVAS.
    FIRMWARE_TYPE_SmartTap = 0x0A, //!< SmartTap.
    FIRMWARE_TYPE_SCRP = 0x0B, //!< SCRP.
    FIRMWARE_TYPE_ADF_SDK = 0x0C, //!< ADF SDK.
    FIRMWARE_TYPE_ADF_App = 0x0D, //!< ADF App.
    FIRMWARE_TYPE_1050 = 0x10, //!< 1050 Firmware Type.
    FIRMWARE_TYPE_DEVICE_TREE_PRJ = 0x31, //!< Device Tree PRJ.
    FIRMWARE_TYPE_NEO3_BOOTLOADER = 0xFC, //!< NEO3 Bootloader.
    FIRMWARE_TYPE_K81_BOOTLOADER_A = 0xFD, //!< K81 Bootloader A.
    FIRMWARE_TYPE_K81_BOOTLOADER_B = 0xFE, //!< K81 Bootloader B.
    FIRMWARE_TYPE_K81 = 0xFF, //!< K81 Firmware Type.
} FIRMWARE_TYPE;

typedef enum{
    RKI_KEY_TYPE_NONE = 0,
    RKI_KEY_TYPE_REV_A_PROD = 1,
    RKI_KEY_TYPE_REV_B_PROD = 2,
    RKI_KEY_TYPE_PKI_PROD = 3,
    RKI_KEY_TYPE_REV_A_DEMO = 4,
    RKI_KEY_TYPE_REV_B_DEMO = 5,
    RKI_KEY_TYPE_PKI_DEMO = 6,
    RKI_KEY_TYPE_AUTO = 7
} RKI_KEY_TYPE;

typedef enum{
    ADF_TYPE_SDK,
    ADF_TYPE_APP
} ADF_TYPE;

typedef struct{
    int type;
    int address;
    int size;
} ADF_Info;

typedef enum
{
    EVENT_TRANSACTION_DATA_UNKNOWN,
    EVENT_TRANSACTION_DATA_CARD_DATA,
    EVENT_TRANSACTION_DATA_EMV_DATA,
    EVENT_TRANSACTION_DATA_MSR_CANCEL_KEY,
    EVENT_TRANSACTION_DATA_MSR_BACKSPACE_KEY,
    EVENT_TRANSACTION_DATA_MSR_ENTER_KEY,
    EVENT_TRANSACTION_DATA_MSR_DATA_ERROR,
    EVENT_TRANSACTION_PIN_DATA
} EVENT_TRANSACTION_DATA_Types;

typedef enum
{
    KEY_MANAGEMENT_UNKNOWN,
    KEY_MANAGEMENT_TDES_DUKPT,
    KEY_MANAGEMENT_TDES_DUKPT_AES_112,
    KEY_MANAGEMENT_AES_DUKPT_DATA_KEY_ENCRYPT_DECRYPT,
    KEY_MANAGEMENT_AES_DUKPT_DATA_KEY_ENCRYPT,
    KEY_MANAGEMENT_AES_DUKPT_DATA_KEY_DECRYPT,
    KEY_MANAGEMENT_AES_DUKPT_MAC_KEY_ENCRYPT_DECRYPT,
    KEY_MANAGEMENT_AES_DUKPT_MAC_KEY_ENCRYPT,
    KEY_MANAGEMENT_AES_DUKPT_MAC_KEY_DECRYPT
} KEY_MANAGEMENT;

typedef enum
{
    EVENT_NOTIFICATION_UNKNOWN,
    EVENT_NOTIFICATION_Card_Not_Seated,
    EVENT_NOTIFICATION_Card_Seated,
    EVENT_NOTIFICATION_Swipe_Card,
    EVENT_NOTIFICATION_Insert_Card,
    EVENT_NOTIFICATION_Remove_Card,
    EVENT_NOTIFICATION_Card_Insert_Timeout,
    EVENT_NOTIFICATION_Transaction_Timeout,
    EVENT_NOTIFICATION_Card_Remove_Timeout,
    EVENT_NOTIFICATION_Insert_Tap_Card,
    EVENT_NOTIFICATION_Device_Unresponsive,
    EVENT_NOTIFICATION_Device_Soft_Reboot_Complete,
    EVENT_NOTIFICATION_Unable_To_Soft_Reboot,
    EVENT_NOTIFICATION_Card_Seated_Data_Captured,
    EVENT_NOTIFICATION_Card_Seated_Data_Not_Captured,
    EVENT_NOTIFICATION_Card_Seated_No_MSR_Detected,
    EVENT_NOTIFICATION_Card_Removed_Data_Captured,
    EVENT_NOTIFICATION_Card_Removed_Data_Not_Captured,
    EVENT_NOTIFICATION_Card_Removed_No_MSR_Detected,
    EVENT_NOTIFICATION_Card_Inserted_Not_Seated,
    EVENT_NOTIFICATION_Card_Removed,
    EVENT_NOTIFICATION_Message
} EVENT_NOTIFICATION_Types;

typedef enum
{
    CTLS_APPLICATION_NONE = 0,
    CTLS_APPLICATION_MASTERCARD = 1,
    CTLS_APPLICATION_VISA = 2,
    CTLS_APPLICATION_AMEX = 3,
    CTLS_APPLICATION_DISCOVER = 4,
    CTLS_APPLICATION_SPEEDPASS = 5,
    CTLS_APPLICATION_GIFT_CARD = 6,
    CTLS_APPLICATION_DINERS_CLUB = 7,
    CTLS_APPLICATION_EN_ROUTE = 8,
    CTLS_APPLICATION_JCB = 9,
    CTLS_APPLICATION_VIVO_DIAGNOSTIC = 10,
    CTLS_APPLICATION_HID = 11,
    CTLS_APPLICATION_MSR_SWIPE = 12,
    CTLS_APPLICATION_RESERVED = 13,
    CTLS_APPLICATION_DES_FIRE_TRACK_DATA = 14,
    CTLS_APPLICATION_DES_FIRE_RAW_DATA = 15,
    CTLS_APPLICATION_RBS = 17,
    CTLS_APPLICATION_VIVO_COMM = 20
} CTLS_APPLICATION;

//Added from IDT_Transactions
typedef enum
{
    TRANS_ERROR_CODE_OUT_OF_SEQ = 0x01, //!< Out of Sequence Command
    TRANS_ERROR_CODE_USE_CONTACT = 0x02, //!< Go to Contact Interface
    TRANS_ERROR_CODE_TRANS_AMOUNT_ZERO = 0x03, //!< Transaction Amount is Zero
    TRANS_ERROR_CODE_USE_OTHER_INF = 0x04, //!< Go to Other Interface
    TRANS_ERROR_CODE_USE_NEARBY_CONTACT_INF = 0x05, //!< Go to Nearby Contact Interface
    TRANS_ERROR_CODE_USE_MAG_INF = 0x06, //!< Go to Magstripe Interface
    TRANS_ERROR_CODE_CARD_RETURNED_ERROR = 0x20, //!< Card returned Error Status
    TRANS_ERROR_CODE_COLLISION = 0x21, //!< Collision Error
    TRANS_ERROR_CODE_MAX_AMT_EXCEEDED = 0x22, //!< Amount Over Maximum Limit
    TRANS_ERROR_CODE_GO_ONLINE = 0x23, //!< Request Online Authorization
    TRANS_ERROR_CODE_CARD_BLOCKED = 0x25, //!< Card Blocked
    TRANS_ERROR_CODE_CARD_EXPIRED = 0x26, //!< Card Expired
    TRANS_ERROR_CODE_UNSUPPORTED_CARD = 0x27, //!< Unsupported Card
    TRANS_ERROR_CODE_CARD_NO_RESPONSE = 0x30, //!< Card did not respond
    TRANS_ERROR_CODE_UNKNOWN_DATA_ELEMENT = 0x40, //!< Unknown Data Element
    TRANS_ERROR_CODE_REQ_DATA_MISSING = 0x41, //!< Required Data Element(s) Missing
    TRANS_ERROR_CODE_AAC = 0x42, //!< Card Generated AAC
    TRANS_ERROR_CODE_ARQC = 0x43, //!< Card Generated ARQC
    TRANS_ERROR_CODE_DATA_AUTH_FAILED_NOT_SUP = 0x44, //!< SDA/DDA Failed (Not Supported by Card)
    TRANS_ERROR_CODE_DATA_AUTH_FAILED_CAPK = 0x50, //!< SDA/DDA/CDDA Failed (CA Public Key)
    TRANS_ERROR_CODE_DATA_AUTH_FAILED_ISS_PK = 0x51, //!< SDA/DDA/CDDA Failed (Issuer Public Key)
    TRANS_ERROR_CODE_DATA_AUTH_FAILED_SSAD = 0x52, //!< SDA Failed (SSAD)
    TRANS_ERROR_CODE_DATA_AUTH_FAILED_ICC_PK = 0x53, //!< DDA/CDDA Failed (ICC Public Key)
    TRANS_ERROR_CODE_DATA_AUTH_FAILED_SIG_VER = 0x54, //!< DDA/CDDA Failed (Dynamic Signature Verification)
    TRANS_ERROR_CODE_PROC_REST_FAILED = 0x55, //!< Processing Restrictions Failed
    TRANS_ERROR_CODE_TRM_FAILED = 0x56, //!< Terminal Risk Management (TRM) Failed
    TRANS_ERROR_CODE_CV_FAILED = 0x57, //!< Cardholder Verification Failed
    TRANS_ERROR_CODE_TAA_FAILED = 0x58, //!< Terminal Action Analysis (TAA) Failed
    TRANS_ERROR_CODE_SD_MEMORY = 0x61, //!< SD Memory Error
    TRANS_ERROR_CODE_CONTACT_EMV = 0x70, //!< Contact EMV Error
    TRANS_ERROR_CODE_NO_MERCHANTS = 0x80, //!< No Merchants
    TRANS_ERROR_CODE_TLV_PARSE = 0x81, //!< TLV Parse Error
    TRANS_ERROR_CODE_MERCHANT_DATA = 0x82, //!< Merchant Data Error
    TRANS_ERROR_CODE_SYSTEM_MEMORY = 0x83, //!< System Memory Error
    TRANS_ERROR_CODE_APP_SKIP = 0x84, //!< Application Skip Error
    TRANS_ERROR_CODE_APP_VER = 0x85 //!< Application Version Error
} TRANS_ERROR_CODE;

typedef enum
{
    RF_STATE_NONE = 0x00,        //!< RF State Code not available
    RF_STATE_PPSE = 0x01,        //!< Error occurred during PPSE command
    RF_STATE_SELECT = 0x02,        //!< Error occurred during SELECT command
    RF_STATE_GPO = 0x03,        //!< Error occurred during GET PROCESSING OPTIONS command
    RF_STATE_READ_RECORD = 0x04,        //!< Error occurred during READ RECORD command
    RF_STATE_GEN_AC = 0x05,        //!< Error occurred during GEN AC command
    RF_STATE_CCC = 0x06,        //!< Error occurred during CCC command
    RF_STATE_IA = 0x07,        //!< Error occurred during IA command
    RF_STATE_SDA = 0x08,        //!< Error occurred during SDA processing
    RF_STATE_DDA = 0x09,        //!< Error occurred during DDA processing
    RF_STATE_CDA = 0x0A,        //!< Error occurred during CDA processing
    RF_STATE_TAA = 0x0B,        //!< Error occurred during TAA processing
    RF_STATE_GET_DATA = 0x0C,        //!< Error occurred during GET DATA command
    RF_STATE_CARD_READ_COMPLETE = 0x21,        //!< Error occurred during CARD READ COMPLETE processing
    RF_STATE_PROCESSING_RESTRICTIONS = 0x22,        //!< Error occurred during PROCESSING RESTRICTIONS processing
    RF_STATE_ODA = 0x23,        //!< Error occurred during ODA processing
    RF_STATE_CARDHOLDER_VERIFICATION = 0x24,        //!< Error occurred during CARDHOLDER VERIFICATION processing
    RF_STATE_ONLINE_PROCESSING = 0x25,        //!< Error occurred during ONLINE PROCESSING processing
    RF_STATE_COMPLETION = 0x26,        //!< Error occurred during COMPLETION processing
} RF_STATE;

typedef enum
{
    ESC_ERR_APPMGR_CLIENT_CREATE = 0x000A0000,   //!< Error creating AppMgr Client
    ESC_ERR_APPMGR_CLIENT_INVALID = 0x000A0001,   //!< Invalid client type for this operation
    ESC_ERR_APPMGR_SERVICE_UNAVAILABLE = 0x000A0002,   //!< Service is not available
    ESC_ERR_APPMGR_BAD_RESPONSE = 0x000A0003,   //!< Received bad response from Daemon
    ESC_ERR_APPMGR_CMD_CANCELED = 0x000A0004,   //!< Command was canceled
    ESC_ERR_APPMGR_CMD_DEFERRED = 0x000A0005,   //!< Command was Deferred
    ESC_ERR_APPMGR_MODULE_CORRUPT = 0x000A0006,   //!< Module failed to load
    ESC_ERR_APPMGR_MODULE_MISSING_INIT = 0x000A0007,   //!< Module missing Init interface
    ESC_ERR_APPMGR_MODULE_MISSING_EXIT = 0x000A0008,   //!< Module missing Exit interface
    ESC_ERR_APPMGR_MODULE_MISSING_INSTALL = 0x000A0009,   //!< Module missing Install interface
    ESC_ERR_APPMGR_MODULE_MISSING_PROCESS = 0x000A000A,   //!< Module missing Process interface
    ESC_ERR_APPMGR_MODULE_INSTALL_FAILED = 0x000A000B,   //!< Failed to install module
    ESC_ERR_APPMGR_PROCOTOL_VIOLATION = 0x000A000C,   //!< Protocol violation
    ESC_ERR_APPMGR_CL_BAD_NUM_AIDS = 0x000A000D,   //!< Bad number of AIDs for the module
    ESC_ERR_APPMGR_CL_NO_EPS_MODULE = 0x000A000E,   //!< No EPS module for the CL card
    ESC_ERR_APPMGR_CL_NO_AID_MODULE = 0x000A000F,   //!< No AID module for the application selected
    ESC_ERR_APPMGR_CL_NO_MIFARE_MODULE = 0x000A0010,   //!< No MIFARE module for the CL card
    ESC_ERR_APPMGR_CL_MIFARE_FAILED_AUTH = 0x000A0011,   //!< Failed to authenticate MIFARE sector
    ESC_ERR_APPMGR_CL_MIFARE_FAILED_BLOCK_READ = 0x000A0012,   //!< Failed to read MIFARE block
    ESC_ERR_APPMGR_CL_FALLBACK_TO_MAGSTRIPE = 0x000A0013,   //!< Fallback from contactless interface to magstripe
    ESC_ERR_APPMGR_CL_FALLBACK_TO_CONTACT = 0x000A0014,   //!< Fallback from contactless interface to contact
    ESC_ERR_APPMGR_CL_FALLBACK_TO_OTHER = 0x000A0015,   //!< Fallback from contactless interface to other
    ESC_ERR_APPMGR_CL_FALLBACK_TO_MIFARE = 0x000A0016,   //!< Fallback from contactless interface to Mifare
    ESC_ERR_APPMGR_CL_REQUEST_MID_PROCESSING = 0x000A0018,   //!< Fallback from AID Module to Mifare to process specified MIDs
    ESC_ERR_APPMGR_CL_DESFIRE_RAW_DATA_SUCCESS = 0x000A0019,   //!< ERR_APPMGR_CL_DESFIRE_RAW_DATA_SUCCESS
    ESC_ERR_APPMGR_CL_VIVOCOMM_RAW_DATA_SUCCESS = 0x000A001A,   //!< ERR_APPMGR_CL_VIVOCOMM_RAW_DATA_SUCCESS
    ESC_ERR_APPMGR_CL_VIVOCOMM_RAW_WRITE_SUCCESS = 0x000A001B,   //!< ERR_APPMGR_CL_VIVOCOMM_RAW_WRITE_SUCCESS
    ESC_ERR_APPMGR_CT_BAD_NUM_AIDS = 0x000A001C,   //!< Bad number of AIDs for the module
    ESC_ERR_APPMGR_CT_NO_APPSEL_MODULE = 0x000A001D,   //!< No module for the CT Application Selection
    ESC_ERR_APPMGR_CT_NO_AID_MODULE = 0x000A001E,   //!< No AID Module to handle selected AID
    ESC_ERR_APPMGR_CT_FALLBACK_TO_MAGSTRIPE = 0x000A001F,   //!< Fallback from contact interface to magstripe
    ESC_ERR_APPMGR_CL_NO_MSR_MODULE = 0x000A0020,   //!< No module registered for MSR cards
    ESC_ERR_APPMGR_UI_PLATFORM_MISMATCH = 0x000A0021,   //!< Module platform and system platform mismatch
    ESC_ERR_APPMGR_DECLINE = 0x000A0022,   //!< ERR_APPMGR_DECLINE
    ESC_ERR_APPMGR_NO_ADVICE_OR_REVERSAL_REQUIRED = 0x000A0023,   //!< ERR_APPMGR_NO_ADVICE_OR_REVERSAL_REQUIRED
    ESC_ERR_APPMGR_ADVICE_REQUIRED = 0x000A0024,   //!< ERR_APPMGR_ADVICE_REQUIRED
    ESC_ERR_APPMGR_REVERSAL_REQUIRED = 0x000A0025,   //!< ERR_APPMGR_REVERSAL_REQUIRED
    ESC_ERR_APPMGR_ADVICE_AND_REVERSAL_REQUIRED = 0x000A0026,   //!< ERR_APPMGR_ADVICE_AND_REVERSAL_REQUIRED
    ESC_ERR_APPMGR_APP_SELECTION_RETRY = 0x000A0027,   //!< ERR_APPMGR_APP_SELECTION_RETRY
    ESC_ERR_APPMGR_REQUEST_ONLINE_AUTH = 0x000A0028,   //!< ERR_APPMGR_REQUEST_ONLINE_AUTH
    ESC_ERR_APPMGR_CL_REQUEST_ONLINE_AUTH = 0x000A0029,   //!< ERR_APPMGR_CL_REQUEST_ONLINE_AUTH
    ESC_ERR_APPMGR_REQUEST_ONLINE_PIN = 0x000A002A,   //!< ERR_APPMGR_REQUEST_ONLINE_PIN
    ESC_ERR_APPMGR_REQUEST_SIGNATURE = 0x000A002B,   //!< ERR_APPMGR_REQUEST_SIGNATURE
    ESC_CEMV_STATUS_CARD_L1_ERROR = 0x60000,   //!< An EMV Level 1 Error Occurred while accessing the Card
    ESC_CEMV_STATUS_CARD_NOT_FOUND = 0x60001,   //!< There is no card inserted in the reader
    ESC_CEMV_STATUS_BAD_RESPONSE = 0x60002,   //!< EMVD Response was incorrect
    ESC_CEMV_STATUS_EMVK_ERROR = 0x60003,   //!< EMV Kernel Returned an unknown Error (or an error that has not  been mapped to a CEMV Error yet)
    ESC_CEMV_STATUS_CARD_REJECTED = 0x60004,   //!< Card rejected due to application level error
    ESC_CEMV_STATUS_CARD_TIMEOUT = 0x60005,   //!< Card Failed to respond in given time
    ESC_CEMV_STATUS_CARD_BLOCKED = 0x60006,   //!< The card is blocked
    ESC_CEMV_STATUS_SERVICE_NOT_ALLOWED_BY_CARD = 0x60007,   //!< Service not allowed by Card
    ESC_CEMV_STATUS_CARD_FALLBACK = 0x60008,   //!< Smartcard transaction failed but application can fallback to  MagStripe
    ESC_CEMV_STATUS_TLV_FORMAT_ERROR = 0x60009,   //!< Mal-formatted TLV Data
    ESC_CEMV_STATUS_TLV_LENGTH_EXCEEDS_MAX_SPECIFIED = 0x0006000A,   //!< Length of one or more Tags requested via the CEMV_GetDataElements
    ESC_CEMV_STATUS_CARD_DATA_FORMAT_INCORRECT = 0x0006000B,   //!< Data read from card is not formatted correctly
    ESC_CEMV_STATUS_MANDATORY_OBJECT_MISSING = 0x0006000C,   //!< Data read from card is missing one or more mandatory data items
    ESC_CEMV_STATUS_DUPLICATE_DATA = 0x0006000D,   //!< Duplicate Data Element(s) read from Card
    ESC_CEMV_STATUS_PSE_NOT_SUPPORTED_BY_CARD = 0x0006000E,   //!< CEMV_STATUS_PSE_NOT_SUPPORTED_BY_CARD
    ESC_CEMV_STATUS_NO_APPS_ON_CARD = 0x0006000F,   //!< There are no applications on the card
    ESC_CEMV_STATUS_NO_SUPPORTED_APPS_ON_CARD = 0x60010,   //!< No matching application between terminal and card
    ESC_CEMV_STATUS_APPLIST_PARSE_ERROR = 0x60011,   //!< Application list returned in the selection of application has  a wrong data format
    ESC_CEMV_STATUS_APP_NAME_NOT_FOUND = 0x60012,   //!< Application Name was not Found
    ESC_CEMV_STATUS_ISS_COD_TBL_OUT_OF_RANGE = 0x60013,   //!< Issuer code table index received from card is not EMV compliant
    ESC_CEMV_STATUS_AIP_NOT_RECEIVED = 0x60014,   //!< Card didn’t return AIP parameter, which is mandatory in Initiate  Application Processing step
    ESC_CEMV_STATUS_AFL_NOT_RECEIVED = 0x60015,   //!< Card didn’t return AFL parameter, which is mandatory in Initiate  Application Processing step
    ESC_CEMV_STATUS_AFL_LEN_OUT_OF_RANGE = 0x60016,   //!< AFL parameter length is wrong
    ESC_CEMV_STATUS_AFL_INCORRECT = 0x60017,   //!< AFL parameter is wrong
    ESC_CEMV_STATUS_EXP_DATE_INCORRECT = 0x60018,   //!< Expiry date format read from the card is incorrect
    ESC_CEMV_STATUS_EFF_DATE_INCORRECT = 0x60019,   //!< Effective date format read from the card is incorrect
    ESC_CEMV_STATUS_SFI_NOT_CORRECT = 0x0006001A,   //!< SFI (Short File Identifier) returned by card has a wrong value
    ESC_CEMV_STATUS_INCORRECT_SFI = 0x0006001B,   //!< Short file identifier of the record which is read is not correct Note: This seems very similar to the CEMV_STATUS_SFI_NOT_CORRECT error code and it seems that it is spurious. It is being included for now since the EMV Kernel Specs define them are two separate error codes.
    ESC_CEMV_STATUS_SFI_OUT_OF_RANGE = 0x0006001C,   //!< SFI Out of Range
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_NOT_FOUND = 0x0006001D,   //!< CA Public Key not found for Data Authentication
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_FAILED_HASH_CHECK = 0x0006001E,   //!< CA Public Key failed hash check. Key data is not valid.
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_INVALID_HASH_ALGO = 0x0006001F,   //!< CA Public Key Hash Algorithm Indicator has an invalid value.
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_INVALID_KEY_ALGO = 0x60020,   //!< CA Public Key Algorithm Indicator has an invalid value.
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_INVALID_MOD_LEN = 0x60021,   //!< CA Public Key Modulus length is invalid.
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_INVALID_EXPONENT = 0x60022,   //!< CA Public Key Exponent has an invalid value.
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_ALREADY_EXISTS = 0x60023,   //!< CA Public Key already exists.
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_NO_SPACE = 0x60024,   //!< There is no more space to store the CA Public Key.
    ESC_CEMV_STATUS_CA_PUBLIC_KEY_BAD_DATA = 0x60025,   //!< CA Public Key data is bad or corrupt.
    ESC_CEMV_STATUS_TERMINAL_PARAM_NOT_FOUND = 0x60026,   //!< A required terminal parameter is missing
    ESC_CEMV_STATUS_CVMLIST_LEN_INCORRECT = 0x60027,   //!< Length of CVM List is not correct
    ESC_CEMV_STATUS_CRYPTOGRAM_TYPE_INCORRECT = 0x60028,   //!< Cryptogram types requested by terminal and returned by card in EMV_CardActionAnalysis and EMV_Completion functions are inconsistent
    ESC_CEMV_STATUS_TAG_NOT_FOUND = 0x60029,   //!< One or More Tags were not found
    ESC_CEMV_STATUS_GET_AMOUNT_ERROR = 0x0006002A,   //!<  Amount parameter format is wrong
    ESC_CEMV_STATUS_USER_SELECTED_LANGUAGE = 0x0006002B,   //!< CEMV_STATUS_USER_SELECTED_LANGUAGE
    ESC_CEMV_STATUS_REQUEST_ONLINE_PIN = 0x0006002C,   //!< CEMV_STATUS_REQUEST_ONLINE_PIN
    ESC_CEMV_STATUS_REQUEST_SIGNATURE = 0x0006002D,   //!< CEMV_STATUS_REQUEST_SIGNATURE
    ESC_CEMV_STATUS_APP_SELECTION_RETRY = 0x0006002E,   //!< Application Selection Retry (for example if GPO returns SW=6985)
    ESC_CEMV_STATUS_TSC_NOT_INCREASED = 0x0006002F,   //!< CEMV_STATUS_TSC_NOT_INCREASED
    ESC_CEMV_STATUS_AUTH_RESP_CODE_MISSING = 0x60030,   //!< Authorization Response Code is missing
    ESC_CEMV_STATUS_AUTH_RESP_CODE_INVALID = 0x60031,   //!< Authorization Response Code is invalid
    ESC_CEMV_STATUS_EMVK_NO_ONLINE_COMM = 0x60032,   //!< EMV Kernel returned "No Online Comm" Error
    ESC_CEMV_STATUS_CVM_TYPE_UNKNOWN = 0x60033,   //!< CEMV_STATUS_CVM_TYPE_UNKNOWN
    ESC_CEMV_STATUS_CVM_AIP_NOT_SUPPORTED = 0x60034,   //!< CEMV_STATUS_CVM_AIP_NOT_SUPPORTED
    ESC_CEMV_STATUS_CVM_LIST_MISSING = 0x60035,   //!< CEMV_STATUS_CVM_LIST_MISSING
    ESC_CEMV_STATUS_CVM_LIST_FORMAT_ERROR = 0x60036,   //!< CEMV_STATUS_CVM_LIST_FORMAT_ERROR
    ESC_CEMV_STATUS_CVM_CODE_NOT_SUPPORTED = 0x60037,   //!< CEMV_STATUS_CVM_CODE_NOT_SUPPORTED
    ESC_CEMV_STATUS_CVM_COND_CODE_NOT_SUPPORTED = 0x60038,   //!< CEMV_STATUS_CVM_COND_CODE_NOT_SUPPORTED
    ESC_CEMV_STATUS_CVM_NO_MORE_CVM = 0x60039,   //!< CEMV_STATUS_CVM_NO_MORE_CVM
    ESC_CEMV_STATUS_CVM_PIN_BYPASSED_BEFOFRE = 0x0006003A,   //!< CEMV_STATUS_CVM_PIN_BYPASSED_BEFOFRE
    ESC_ERR_CFG_CLIENT_CREATE = 0x70000,   //!< Error creating CFG Client
    ESC_ERR_CFG_SERVICE_UNAVAILABLE = 0x70001,   //!< CFG Service is not available
    ESC_ERR_CFG_BAD_RESPONSE = 0x70002,   //!< Received bad response from Daemon
    ESC_EFAILED = 0x1,   //!< General failure for the procedure
    ESC_EOUTOFMEMORY = 0x2,   //!< Out of memory
    ESC_EBADPARAM = 0x3,   //!< Bad parameter (s) supplied
    ESC_ENOTSUPPORTED = 0x4,   //!< Procedure is not supported
    ESC_ENOTFOUND = 0x5,   //!< Item was not found
    ESC_EALREADYEXISTS = 0x6,   //!< Item already exists
    ESC_EOUTOFBOUNDS = 0x7,   //!< Out of bounds error
    ESC_ENOTAUTH = 0x8,   //!< Not authorized
    ESC_EQFAILURE = 0x9,   //!< Q error
    ESC_ETIMEOUT = 0x0000000A,   //!< Timeout
    ESC_ENODEVICE = 0x0000000B,   //!< Device not found
    ESC_EFOPEN = 0x0000000C,   //!< File open error
    ESC_EFOPERATION = 0x0000000D,   //!< File operation error
    ESC_ECANCEL = 0x0000000E,   //!< Operation canceled
    ESC_ERESOURCEBUSY = 0x0000000F,   //!< Resource or device is busy
    ESC_ENOTADIRECTORY = 0x10,   //!< Specified directory not found
    ESC_EFILETOOBIG = 0x11,   //!< Specified file is too big
    ESC_ENOSPACE = 0x12,   //!< Not enough memory to complete command
    ESC_ERR_ICC_BAD_RESPONSE = 0x50000,   //!< ICCD Response was Incorrect
    ESC_ERR_ICC_SLOT_IN_USE = 0x50001,   //!< The slot is already in use for a complex command such as  polling or Card Removal
    ESC_ERR_ICC_DRIVER_RESOURCE_BUSY = 0x50002,   //!< The driver resources are dedicated for processing a command on another slot.
    ESC_ERR_ICC_CARD_NOT_PRESENT = 0x50003,   //!< Error - Card was not present
    ESC_ERR_ICC_CARD_NOT_READY = 0x50004,   //!< The card is not ready for application level transactions since  a TTL session has not been established with the card yet
    ESC_ERR_ICC_L1_ERROR_CARD_DEACTIVATED = 0x50005,   //!< Level 1 error occurred. Card was deactivated by the ICC Service  or the smart card driver
    ESC_ERR_ICC_L1_ERROR_CARD_NOT_DEACTIVATED = 0x50006,   //!< Level 1 error occurred. Card was not deactivated by the ICC Service or the smart card driver. Decision to deactivate card  left up to the client application
    ESC_ERR_ICC_CARD_DID_NOT_RESPOND = 0x50007,   //!< The card failed to respond to the APDU command
    ESC_ERR_ICC_ATR_NOT_AVAILABLE = 0x50008,   //!< Card is present but ATR data not yet available. Wait until Card is Ready before calling this function.
    ESC_ERR_ICC_UNKNOWN_MODE = 0x50009,   //!< ATR was returned but the Mode is not known. This condition will occur if a non-standard ATR is returned by the card.
    ESC_ERR_ICC_COMMAND_NOT_ALLOWED = 0x0005000A,   //!< Command not allowed (Command used out of sequence)
    ESC_ERR_MSR_BAD_RESPONSE = 0x90000,   //!< MSRD Response was Incorrect
    ESC_ERR_MSR_BAD_SWIPE = 0x90001,   //!< A card was swiped but an error occurred
    ESC_ERR_MSR_COMMAND_NOT_ALLOWED = 0x90002,   //!< Command not allowed (Command used out of sequence)
    ESC_ERR_PCI_CLIENT_CREATE = 0x10000,   //!< Error creating PCI Client
    ESC_ERR_PCI_SERVICE_UNAVAILABLE = 0x10001,   //!< PCI Service is not available
    ESC_ERR_PCI_BAD_RESPONSE = 0x10002,   //!< Received bad response from Daemon
    ESC_ERR_PCI_KEY_NOT_FOUND = 0x10003,   //!< No key was found for PIN request
    ESC_ERR_PCI_CEASE_OPERATION = 0x10004,   //!< Key Generation (DUKPT) has reached its end of life. Cease unit operation.
    ESC_ERR_PCI_PIN_ENTRY_CANCELLED = 0x10005,   //!< PIN entry cancelled
    ESC_ERR_PCI_BAD_KEY = 0x10006,   //!< Key is incorrectly formatted
    ESC_ERR_PCI_RNG_DEVICE_FAILURE = 0x10007,   //!< Random number generator device failure
    ESC_ERR_PCI_ILLEGAL_NUMERIC_CHAR = 0x10008,   //!< Illegal char passed in place of a number
    ESC_ERR_PCI_ILLEGAL_CURRENCY_CHAR = 0x10009,   //!< Illegal currency character given in command string
    ESC_ERR_PCI_CMD_CANCELLED = 0x0001000A,   //!< Command was cancelled via signal
    ESC_ERR_PCI_KEYPAD_INTRUSION_DETECTED = 0x0001000B,   //!<  Intrusion was detected on the keypad
    ESC_ERR_PCI_KEYPAD_BLOCKED = 0x0001000C,   //!< Attempt to read keypad while in non-secure display mode
    ESC_ERR_PCI_NO_DATA = 0x0001000D,   //!< No data to return
    ESC_ERR_PICC_CLIENT_CREATE_ERROR = 0x40000,   //!< Error creating PICC Client
    ESC_ERR_PICC_SERVICE_UNAVAILABLE = 0x40001,   //!< PICC Service is not available
    ESC_ERR_PICC_BAD_RESPONSE = 0x40002,   //!< Received bad response from Daemon
    ESC_ERR_PICC_CMD_CANCELLED = 0x40003,   //!< Command was cancelled via signal
    ESC_ERR_PICC_POLL_TIMEOUT = 0x40004,   //!< ERR_PICC_POLL_TIMEOUT
    ESC_ERR_PICC_CARD_NOT_FOUND = 0x40005,   //!< ERR_PICC_CARD_NOT_FOUND
    ESC_ERR_PICC_COLLISION = 0x40006,   //!< ERR_PICC_COLLISION
    ESC_ERR_PICC_TYPE_NOT_SUPPORTED = 0x40007,   //!< ERR_PICC_TYPE_NOT_SUPPORTED
    ESC_ERR_PICC_CRC_ERR = 0x40008,   //!< ERR_PICC_CRC_ERR
    ESC_ERR_PICC_PARITY_ERR = 0x40009,   //!< ERR_PICC_PARITY_ERR
    ESC_ERR_PICC_FRAMING_ERR = 0x0004000A,   //!< ERR_PICC_FRAMING_ERR
    ESC_ERR_PICC_BIT_COUNT_ERR = 0x0004000B,   //!< ERR_PICC_BIT_COUNT_ERR
    ESC_ERR_PICC_TRANSMISSION_ERR = 0x0004000C,   //!< ERR_PICC_TRANSMISSION_ERR
    ESC_ERR_PICC_BYTE_COUNT_ERR = 0x0004000D,   //!< ERR_PICC_BYTE_COUNT_ERR
    ESC_ERR_PICC_AUTHENTICATION_FAILED = 0x0004000E,   //!< ERR_PICC_AUTHENTICATION_FAILED
    ESC_ERR_PICC_ACCESS_TIMEOUT = 0x0004000F,   //!< ERR_PICC_ACCESS_TIMEOUT
    ESC_ERR_PICC_READ_ERR = 0x40010,   //!< ERR_PICC_READ_ERR
    ESC_ERR_PICC_WRITE_ERR = 0x40011,   //!< ERR_PICC_WRITE_ERR
    ESC_ERR_PICC_NOT_AUTHORIZED = 0x40012,   //!< ERR_PICC_NOT_AUTHORIZED
    ESC_ERR_PICC_RETURN_CODE_ERR = 0x40013,   //!< ERR_PICC_RETURN_CODE_ERR
    ESC_ERR_PICC_VALUE_ERR = 0x40014,   //!< ERR_PICC_VALUE_ERR
    ESC_ERR_PICC_KEY_ERR = 0x40015,   //!< ERR_PICC_KEY_ERR
    ESC_ERR_PICC_FIFO_OVERFLOW = 0x40016,   //!< ERR_PICC_FIFO_OVERFLOW
    ESC_ERR_PICC_TX_BUFFER_OVERFLOW = 0x40017,   //!< ERR_PICC_TX_BUFFER_OVERFLOW
    ESC_ERR_PICC_RX_BUFFER_OVERFLOW = 0x40018,   //!< ERR_PICC_RX_BUFFER_OVERFLOW
    ESC_ERR_PICC_DATA_CODING_ERR = 0x40019,   //!< ERR_PICC_DATA_CODING_ERR
    ESC_ERR_PICC_GEN_PROTOCOL_ERR = 0x0004001A,   //!< ERR_PICC_GEN_PROTOCOL_ERR
    ESC_ERR_PICC_EXCEEDED_RETRIES = 0x0004001B,   //!< ERR_PICC_EXCEEDED_RETRIES
    ESC_ERR_PICC_BLOCK_PROTOCOL_ERR = 0x0004001C,   //!< ERR_PICC_BLOCK_PROTOCOL_ERR
    ESC_ERR_PICC_UNK_COMMAND = 0x0004001D,   //!< ERR_PICC_UNK_COMMAND
    ESC_ERR_PICC_COLLISION_INTERPRETED = 0x0004001E,   //!< ERR_PICC_COLLISION_INTERPRETED
    ESC_ERR_PICC_TIMEOUT_WITH_ERR = 0x0004001F,   //!< ERR_PICC_TIMEOUT_WITH_ERR
    ESC_ERR_PICC_ALREADY_POLLING = 0x40020,   //!< ERR_PICC_ALREADY_POLLING
    ESC_ERR_PICC_CARD_NOT_REMOVED = 0x40021,   //!< ERR_PICC_CARD_NOT_REMOVED
    ESC_ERR_PICC_CODE_END = 0x00040022,   //!< ERR_PICC_CODE_END
    ESC_ERR_SOFTSAM_BAD_RESPONSE = 0x000B0000,   //!< SoftSAM Response was Incorrect
    ESC_ERR_SOFTSAM_CA_KEY_NOT_FOUND = 0x000B0001,   //!< ERR_SOFTSAM_CA_KEY_NOT_FOUND
    ESC_ERR_SOFTSAM_BAD_MODULUS_LENGTH = 0x000B0002,   //!< ERR_SOFTSAM_BAD_MODULUS_LENGTH
    ESC_ERR_SOFTSAM_BAD_EXPONENT_LENGTH = 0x000B0003,   //!< ERR_SOFTSAM_BAD_EXPONENT_LENGTH
    ESC_ERR_SOFTSAM_BAD_CERTIFICATE_LENGTH = 0x000B0004,   //!< ERR_SOFTSAM_BAD_CERTIFICATE_LENGTH
    ESC_ERR_SOFTSAM_MALFORMATTED_CERTIFICATE = 0x000B0005,   //!< ERR_SOFTSAM_MALFORMATTED_CERTIFICATE
    ESC_ERR_SOFTSAM_RECOVERY_FN_FAILED = 0x000B0006,   //!< ERR_SOFTSAM_RECOVERY_FN_FAILED
    ESC_ERR_SOFTSAM_BUFFER_OVF = 0x000B0007,   //!< ERR_SOFTSAM_BUFFER_OVF
    ESC_ERR_SOFTSAM_ALG_NOT_SUPPORTED = 0x000B0008,   //!< ERR_SOFTSAM_ALG_NOT_SUPPORTED
    ESC_ERR_SOFTSAM_HASH_ALG_NOT_SUPPORTED = 0x000B0009,   //!< ERR_SOFTSAM_HASH_ALG_NOT_SUPPORTED
    ESC_ERR_SOFTSAM_CRYPT_ALG_NOT_SUPPORTED = 0x000B000A,   //!< ERR_SOFTSAM_CRYPT_ALG_NOT_SUPPORTED
    ESC_ERR_SOFTSAM_HASH_FN_FAILED = 0x000B000B,   //!< ERR_SOFTSAM_HASH_FN_FAILED
    ESC_ERR_SOFTSAM_RID_KEY_SLOTS_FULL = 0x000B000C,   //!< ERR_SOFTSAM_RID_KEY_SLOTS_FULL
    ESC_ERR_SOFTSAM_KEY_ALREADY_EXISTS = 0x000B000D,   //!< ERR_SOFTSAM_KEY_ALREADY_EXISTS
    ESC_ERR_SOFTSAM_NO_SPACE_FOR_NEW_RID = 0x000B000E,   //!< ERR_SOFTSAM_NO_SPACE_FOR_NEW_RID
    ESC_ERR_SOFTSAM_NO_FREE_KEY_SLOTS = 0x000B000F,   //!< ERR_SOFTSAM_NO_FREE_KEY_SLOTS
    ESC_ERR_SOFTSAM_BAD_PKT_DATA_LEN = 0x000B0010,   //!< ERR_SOFTSAM_BAD_PKT_DATA_LEN
    ESC_ERR_SOFTSAM_BAD_PACKET_NUMBERING = 0x000B0011,   //!< ERR_SOFTSAM_BAD_PACKET_NUMBERING
    ESC_ERR_SOFTSAM_INVALID_CRYPTO_ALGORITHM = 0x000B0012,   //!< ERR_SOFTSAM_INVALID_CRYPTO_ALGORITHM
    ESC_ERR_SOFTSAM_INVALID_CRYPTO_CMD = 0x000B0013,   //!< ERR_SOFTSAM_INVALID_CRYPTO_CMD
    ESC_ERR_SOFTSAM_INVALID_CRYPTO_KEYTYPE = 0x000B0014,   //!< ERR_SOFTSAM_INVALID_CRYPTO_KEYTYPE
    ESC_ERR_SOFTSAM_INVALID_CRYPTO_DATA_LENGTH = 0x000B0015,   //!< ERR_SOFTSAM_INVALID_CRYPTO_DATA_LENGTH
    ESC_ERR_SOFTSAM_RESP_EMV_FAILURE = 0x000B0016,   //!< ERR_SOFTSAM_RESP_EMV_FAILURE
    ESC_ERR_SOFTSAM_RESP_EMV_BAD_PKT_DATA_LEN = 0x000B0017,   //!< ERR_SOFTSAM_RESP_EMV_BAD_PKT_DATA_LEN
    ESC_ERR_SOFTSAM_RESP_EMV_CA_KEY_NOT_FOUND = 0x000B0018,   //!< ERR_SOFTSAM_RESP_EMV_CA_KEY_NOT_FOUND
    ESC_ERR_SOFTSAM_RESP_EMV_BAD_CERTIFICATE_LENGTH = 0x000B0019,   //!< ERR_SOFTSAM_RESP_EMV_BAD_CERTIFICATE_LENGTH
    ESC_ERR_SOFTSAM_RESP_EMV_RECOVERY_FN_FAILED = 0x000B001A,   //!< ERR_SOFTSAM_RESP_EMV_RECOVERY_FN_FAILED
    ESC_ERR_SOFTSAM_RESP_EMV_MALFORMATTED_CERTIFICATE = 0x000B001B,   //!< ERR_SOFTSAM_RESP_EMV_MALFORMATTED_CERTIFICATE
    ESC_ERR_SOFTSAM_RESP_EMV_HASH_ALG_NOT_SUPPORTED = 0x000B001C,   //!< ERR_SOFTSAM_RESP_EMV_HASH_ALG_NOT_SUPPORTED
    ESC_ERR_SOFTSAM_RESP_EMV_HASH_FN_FAILED = 0x000B001D,   //!< ERR_SOFTSAM_RESP_EMV_HASH_FN_FAILED
    ESC_ERR_SOFTSAM_RESP_EMV_CRYPT_ALG_NOT_SUPPORTED = 0x000B001E,   //!< ERR_SOFTSAM_RESP_EMV_CRYPT_ALG_NOT_SUPPORTED
    ESC_ERR_SOFTSAM_RESP_EMV_BAD_EXPONENT_LENGTH = 0x000B001F,   //!< ERR_SOFTSAM_RESP_EMV_BAD_EXPONENT_LENGTH
    ESC_ERR_SOFTSAM_UNKNOWN_ERROR = 0x000B0020,   //!< ERR_SOFTSAM_UNKNOWN_ERROR
    ESC_ERR_TLV_BAD_TAG = 0x20000,   //!< TLV Tag provided was badly formatted
    ESC_ERR_TLV_BAD_LENGTH = 0x20001,   //!< TLV Length was badly formatted
    ESC_ERR_TLV_ILLEGAL_SCOPE = 0x20002,   //!< TLV tag was found under an incorrect constructed tag.
    ESC_ERR_TLV_BAD_FORMAT = 0x20003,   //!< TLV was badly formatted. This error is usually thrown  when some bytes are left in the end that did not get  processed. Check if SW word is also being passed in.
    ESC_ERR_TLV_ILLEGAL_NC_TAG = 0x20004,   //!< Non-conforming tag callback has returned illegal value  for the tag lengths
    ESC_ERR_UI_CLIENT_CREATE = 0x30000,   //!< Error creating UI Client
    ESC_ERR_UI_SERVICE_UNAVAILABLE = 0x30001,   //!< UI Service is not available
    ESC_ERR_UI_BAD_RESPONSE = 0x30002,   //!< Received bad response from Daemon
    ESC_ERR_UI_NOT_ENOUGH_ROOM = 0x30003,   //!< Buffer isn't large enough to hold signature
    ESC_ERR_UI_UNSUPPORTED_FONT = 0x30004,   //!< Font not supported (either font size or the font)
    ESC_ERR_UI_NO_BUTTONS = 0x30005,   //!< No touchscreen buttons currently defined
    ESC_ERR_UI_CMD_CANCELLED = 0x30006,   //!< Command cancelled by signal
    ESC_ERR_UI_TEXT_AREAS_OVERLAP = 0x30007,   //!< Text areas overlap on screen - not allowed
    ESC_ERR_UI_BUTTON_AREAS_OVERLAP = 0x30008,   //!< Text areas overlap on screen - not allowed
    ESC_ERR_UI_LINE_ITEM_AREAS_OVERLAP = 0x30009,   //!< Line item areas overlap on screen - not allowed
    ESC_ERR_UI_CHECKBOX_AREAS_OVERLAP = 0x0003000A,   //!< Checkbox areas overlap on screen - not allowed
    ESC_ERR_UI_NO_CHECKBOXES = 0x0003000B,   //!< No touchscreen checkboxes currently defined
    ESC_ERR_UI_SIGCAP_AREAS_OVERLAP = 0x0003000C,   //!< Signature capture area overlaps
    ESC_ERR_UI_INPUT_FIELD_AREAS_OVERLAP = 0x0003000D,   //!< New screen item overlaps existing input field
    ESC_ERR_UI_NO_DATA = 0x0003000E,   //!< Request for read with zero timeout and no data
    ESC_ERR_UI_EXCEEDED_ITEM_LIMIT = 0x0003000F,   //!< Exceeded limit for allowed items on the screen
    ESC_ERR_UI_TOUCH_AREAS_OVERLAP = 0x30010,   //!< Text areas overlap on screen - not allowed
    ESC_ERR_UI_NO_TOUCH_AREAS = 0x30011,   //!< No touch sensitive areas are defined
    ESC_ERR_UI_CUSTOM_MODE_REQUIRED = 0x30012,   //!< Command requires custom mode
    ESC_ERR_UI_SLIDESHOW_ALREADY_ACTIVE = 0x30013,   //!< Slideshow is currently in progress
    ESC_ERR_UI_TOUCHSCREEN_UNAVAILABLE = 0x30014,   //!< Touchscreen hardware not present
    
    ESC_ERR_UI_GRAPHIC_DISPLAY_UNAVAILABLE = 0x30015,   //!< No graphic display hardware present
    ESC_ERR_UI_RECTANGLE_AREAS_OVERLAP = 0x30016,   //!< Rectangle areas overlap on screen display
    ERR_UI_PARAGRAPH_AREAS_OVERLAP = 0x00030017,  //!< Paragraph areas overlap on screen - not allowed
    ERR_UI_WORD_TOO_WIDE_FOR_RECTANGLE = 0x00030018,  //!< A word in a paragraph is too wide for the rectangle
    ERR_UI_PARAGRAPH_VIOLATES_MIN_DIMENSIONS = 0x00030019,  //!< Paragraph areas violate the minimum dimensions
    ERR_UI_PAN_ENTRY_CANCELLED = 0x0003001A,  //!< Get PAN is cancelled
    ERR_FIO_RECORD_TOO_LARGE_FOR_BUFFER = 0x00080000,  //!< Given buffer won't contain a record
    ERR_FIO_FILE_SIZE_INVALID = 0x00080001,  //!< File size is not a multiple of record size
    ERR_FIO_FILE_EMPTY = 0x00080002,  //!< No records in file
    ERR_FIO_NO_MATCHING_RECORDS = 0x00080003,  //!< No records matched the search criteria
    ERR_FIO_INVALID_RECORD_LENGTH = 0x00080004,  //!< Length of input record does not match record size
    ERR_FIO_FAILED_TO_READ_RECORD = 0x00080005,  //!< Failed to read expected record length
    ERR_FIO_DUPLICATE_RECORD = 0x00080006,  //!< Record already exists
    ERR_FIO_FAILED_TO_WRITE_RECORD = 0x00080007  //!< Failed to write expected number of bytes
} EXTENDED_STATUS_CODES;

typedef enum
{
    CEMV_APP_ERROR_FN_NONE = 0x00,  //!< CEMV_APP_ERROR_FN_NONE
    CEMV_APP_ERROR_FN_CEMV_INITIALIZE = 0x01,  //!< CEMV_APP_ERROR_FN_CEMV_INITIALIZE
    CEMV_APP_ERROR_FN_CEMV_INITIATE_TRANSACTION = 0x02,  //!< CEMV_APP_ERROR_FN_CEMV_INITIATE_TRANSACTION
    CEMV_APP_ERROR_FN_CEMV_BUILD_CANDIDATE_LIST = 0x03,  //!< CEMV_APP_ERROR_FN_CEMV_BUILD_CANDIDATE_LIST
    CEMV_APP_ERROR_FN_CEMV_FINAL_APPLICATION_SELECTION = 0x04,  //!< CEMV_APP_ERROR_FN_CEMV_FINAL_APPLICATION_SELECTION
    CEMV_APP_ERROR_FN_CEMV_INITIATE_APPLICATION = 0x05,  //!< CEMV_APP_ERROR_FN_CEMV_INITIATE_APPLICATION
    CEMV_APP_ERROR_FN_CEMV_READ_APPLICATION_DATA = 0x06,  //!< CEMV_APP_ERROR_FN_CEMV_READ_APPLICATION_DATA
    CEMV_APP_ERROR_FN_CEMV_OFFLINE_DATA_AUTHENTICATION = 0x07,  //!< CEMV_APP_ERROR_FN_CEMV_OFFLINE_DATA_AUTHENTICATION
    CEMV_APP_ERROR_FN_CEMV_PROCESS_RESTRICTIONS = 0x08,  //!< CEMV_APP_ERROR_FN_CEMV_PROCESS_RESTRICTIONS
    CEMV_APP_ERROR_FN_CEMV_CARDHOLDER_VERIFICATION = 0x09,  //!< CEMV_APP_ERROR_FN_CEMV_CARDHOLDER_VERIFICATION
    CEMV_APP_ERROR_FN_CEMV_CARDHOLDER_VERIFICATION_CONTINUE = 0x0a,  //!< CEMV_APP_ERROR_FN_CEMV_CARDHOLDER_VERIFICATION_CONTINUE
    CEMV_APP_ERROR_FN_CEMV_TERMINAL_RISK_MANAGEMENT = 0x0b,  //!< CEMV_APP_ERROR_FN_CEMV_TERMINAL_RISK_MANAGEMENT
    CEMV_APP_ERROR_FN_CEMV_TERMINAL_ACTION_ANALYSIS = 0x0c,  //!< CEMV_APP_ERROR_FN_CEMV_TERMINAL_ACTION_ANALYSIS
    CEMV_APP_ERROR_FN_CEMV_CARD_ACTION_ANALYSIS = 0x0d,  //!< CEMV_APP_ERROR_FN_CEMV_CARD_ACTION_ANALYSIS
    CEMV_APP_ERROR_FN_CEMV_COMPLETION = 0x0e,  //!< CEMV_APP_ERROR_FN_CEMV_COMPLETION
    CEMV_APP_ERROR_FN_CEMV_TRANSACTION_MODE = 0x0f,  //!< CEMV_APP_ERROR_FN_CEMV_TRANSACTION_MODE
    CEMV_APP_ERROR_FN_CEMV_GET_APPLICATION_NAME = 0x10,  //!< CEMV_APP_ERROR_FN_CEMV_GET_APPLICATION_NAME
    CEMV_APP_ERROR_FN_CEMV_SET_DATA_ELEMENTS = 0x11,  //!< CEMV_APP_ERROR_FN_CEMV_SET_DATA_ELEMENTS
    CEMV_APP_ERROR_FN_CEMV_GET_DATA_ELEMENTS = 0x12,  //!< CEMV_APP_ERROR_FN_CEMV_GET_DATA_ELEMENTS
    CEMV_APP_ERROR_FN_CEMV_UPDATE_EMV_KERNEL_CONFIG_FILE = 0x13,  //!< CEMV_APP_ERROR_FN_CEMV_UPDATE_EMV_KERNEL_CONFIG_FILE
    CEMV_APP_ERROR_FN_UI_DISPLAY_CLEAR = 0x20,  //!< CEMV_APP_ERROR_FN_UI_DISPLAY_CLEAR
    CEMV_APP_ERROR_FN_UI_DISLAY_TEXT = 0x21,  //!< CEMV_APP_ERROR_FN_UI_DISLAY_TEXT
    CEMV_APP_ERROR_FN_UI_DISPLAY_BUTTON = 0x22,  //!< CEMV_APP_ERROR_FN_UI_DISPLAY_BUTTON
    CEMV_APP_ERROR_FN_UI_LIST_CREATE = 0x23,  //!< CEMV_APP_ERROR_FN_UI_LIST_CREATE
    CEMV_APP_ERROR_FN_UI_LIST_ADD_ITEM = 0x24,  //!< CEMV_APP_ERROR_FN_UI_LIST_ADD_ITEM
    CEMV_APP_ERROR_FN_UI_LIST_READ_BUTTON = 0x25,    //!< CEMV_APP_ERROR_FN_UI_LIST_READ_BUTTON
    CEMV_APP_ERROR_FN_UI_LIST_GET_SELECTED_ITEM = 0x26,  //!< CEMV_APP_ERROR_FN_UI_LIST_GET_SELECTED_ITEM
    CEMV_APP_ERROR_FN_PCI_START_CUSTOM_DISPLAY_MODE = 0x30  //!< CEMV_APP_ERROR_FN_PCI_START_CUSTOM_DISPLAY_MODE
} CEMV_APP_ERROR_FN;

typedef enum
{
    CEMV_APP_ERROR_STATE_NONE = 0x00,   //!< CEMV_APP_ERROR_STATE_NONE
    CEMV_APP_ERROR_STATE_SERIAL_COMM = 0x01,   //!< CEMV_APP_ERROR_STATE_SERIAL_COMM
    CEMV_APP_ERROR_STATE_VALIDATE_COMMAND = 0x02,   //!< CEMV_APP_ERROR_STATE_VALIDATE_COMMAND
    CEMV_APP_ERROR_STATE_TRANSACTION = 0x03,   //!< CEMV_APP_ERROR_STATE_TRANSACTION
    CEMV_APP_ERROR_STATE_CANDIDATE_SELECTION = 0x04,   //!< CEMV_APP_ERROR_STATE_CANDIDATE_SELECTION
    CEMV_APP_ERROR_STATE_DISPLAY_APP_MENU = 0x05,   //!< CEMV_APP_ERROR_STATE_DISPLAY_APP_MENU
    CEMV_APP_ERROR_STATE_LOAD_CONFIG_GROUP = 0x06,   //!< CEMV_APP_ERROR_STATE_LOAD_CONFIG_GROUP
    CEMV_APP_ERROR_STATE_CARD_REMOVAL = 0x07,   //!< CEMV_APP_ERROR_STATE_CARD_REMOVAL
    CEMV_APP_ERROR_STATE_UNKNOWN = 0xff   //!< CEMV_APP_ERROR_STATE_UNKNOWN
} CEMV_APP_ERROR_STATE;

typedef enum
{
    EMV_LCD_DISPLAY_MODE_CANCEL = 0,
    EMV_LCD_DISPLAY_MODE_MENU = 1,
    EMV_LCD_DISPLAY_MODE_PROMPT = 2,
    EMV_LCD_DISPLAY_MODE_MESSAGE = 3,
    EMV_LCD_DISPLAY_MODE_LANGUAGE_SELECT = 8,
    EMV_LCD_DISPLAY_MODE_CLEAR_SCREEN = 16,
    EMV_LCD_DISPLAY_MODE_AR_MESSAGE2 = 0Xf2,
    EMV_LCD_DISPLAY_MODE_AR_MESSAGE3 = 0Xf3,
    EMV_LCD_DISPLAY_MODE_AR_MESSAGE4 = 0Xf4
} EMV_LCD_DISPLAY_MODE;
