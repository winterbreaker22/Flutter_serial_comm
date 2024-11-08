
#include "support.h"

unsigned short LCPBuildMessage(unsigned char to, unsigned char status, unsigned char *msg, unsigned char len)
{
    // Local variables.
    unsigned char *LCPtxMsgPtr; // pointer to the LCP transmit message buffer
    unsigned short msgLen; // absolute message length
    unsigned short crc; // CRC of message
    unsigned char i; // loop variable
    // Build the LCP message header.
    LCPtxMsgPtr = &LCPMsg[0]; // initialize pointer to the LCP transmit buffer
    *LCPtxMsgPtr++ = '~'; // insert the first ~
    *LCPtxMsgPtr++ = '~'; // insert the second ~
    msgLen = 2U; // length of LCP message
    crc = 0x7E7E; // seed the CRC with the message header
    // Build the variable part of the message.
    LCPAppendByte(to, &LCPtxMsgPtr, &msgLen, &crc);
    LCPAppendByte(LCPHostNodeAddress, &LCPtxMsgPtr, &msgLen, &crc);
    LCPAppendByte(status, &LCPtxMsgPtr, &msgLen, &crc);
    LCPAppendByte(len, &LCPtxMsgPtr, &msgLen, &crc);
    for (i=0; i < len; i++) // loop to include the message
    {
        LCPAppendByte(msg[i], &LCPtxMsgPtr, &msgLen, &crc);
    }
    LCPAppendByte((unsigned char)(crc%0x0100), &LCPtxMsgPtr, &msgLen, NULL);
    LCPAppendByte((unsigned char)(crc/0x0100), &LCPtxMsgPtr, &msgLen, NULL);
    // LCPBuildMessage exit point.
    return(msgLen); // return the absolute length of the message
}

static void LCPAppendByte(unsigned char byte, unsigned char **LCPMsgPtrPtr, unsigned short *msgLen, unsigned short *crc)
{
    // Check for special characters.
    if (byte == ESC || byte == '~') // check for special characters
    {
        *((*LCPMsgPtrPtr)++) = ESC; // insert the <esc> character
        (*msgLen)++; // update the LCP message length
        LCPUpdateCRC(crc, ESC); // include the <esc> in the CRC
    }
    // Append the byte to the end of the message.
    *((*LCPMsgPtrPtr)++) = byte; // insert the new byte
    (*msgLen)++; // update the LCP message length
    LCPUpdateCRC(crc, byte); // include the byte in the CRC
    // LCPAppendByte exit point.
}

static void LCPUpdateCRC(unsigned short *crc, unsigned char byte)
{
    // Local variables.
    char i; // loop variable
    char XORFlag; // flag indicating the polynomial should be XORed
    // Combine the new data byte with the current CRC.
    if (crc != NULL) // ensure the CRC is wanted
    {
        for (i=7; i >= 0; --i) // loop through each bit in the data byte
        {
            XORFlag = (unsigned char)((*crc & 0x8000) != 0x0000);
            *crc <<= 1; // shift the CRC left
            *crc |= (unsigned short)((byte >> i) & 0x01);
            if (XORFlag) // check for CRC overflow
            *crc ^= 0x1021; // XOR with polynomial
        }
    }
    // LCPUpdateCRC exit point.
}