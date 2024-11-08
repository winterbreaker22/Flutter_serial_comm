
#include "stdlib.h"

#define ESC ((char)'\x1B') // ASCII escape
#define LCP_MsgSizeLimit (523U) // maximum size of an LCP message
// Function prototypes taken from Support.h.
unsigned long LCPBuildMessage(unsigned char to, unsigned char status, unsigned char *msg, unsigned char len);
// Static function prototypes.
static void LCPAppendByte(unsigned char byte, unsigned char **LCPMsgPtrPtr, unsigned short *msgLen, unsigned short *crc);
static void LCPUpdateCRC(unsigned short *crc, unsigned char byte);
// Global variables.
unsigned char LCPHostNodeAddress = 255; // host node address
unsigned char LCPMsg[LCP_MsgSizeLimit]; // raw LCP message buffer