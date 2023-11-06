/**
 *  Card Definitions
 *
 *  @author jylee@suprema.co.kr
 *  @see
 */

/*
 *  Copyright (c) 2014 Suprema Co., Ltd. All Rights Reserved.
 *
 *  This software is the confidential and proprietary information of
 *  Suprema Co., Ltd. ("Confidential Information").  You shall not
 *  disclose such Confidential Information and shall use it only in
 *  accordance with the terms of the license agreement you entered into
 *  with Suprema.
 */

#ifndef __BS2_CSN_CARD_H__
#define __BS2_CSN_CARD_H__

#include "../BS2Types.h"


/**
 *	constants for BS2CSNCard
 */
enum {
	BS2_CARD_DATA_SIZE		= 32,
	BS2_CARD_ID_SIZE		= 24,		// for SecureCredential Card
};

/**
  *  QR code character range
  */
enum {
	BS2_QRCODE_ASCII_MIN		= 0x20,		// ' '		[32~126 in ASCII code]
	BS2_QRCODE_ASCII_MAX		= 0x7E,		// '~'		[32~126 in ASCII code]

	BS2_QR_DATA_SIZE 			= 512,		// BDP-488 1.2.3.1
};

/**
 *  BS2_CARD_TYPE
 */
enum {
	BS2_CARD_TYPE_UNKNOWN		 = 0x00,
	BS2_CARD_TYPE_CSN			 = 0x01,
	BS2_CARD_TYPE_SECURE		 = 0x02,
	BS2_CARD_TYPE_ACCESS		 = 0x03,
	BS2_CARD_TYPE_CSN_MOBILE	 = 0x04,
	BS2_CARD_TYPE_WIEGAND_MOBILE = 0x05,
	BS2_CARD_TYPE_QR		 	 = 0x06,
	BS2_CARD_TYPE_SECURE_QR	 	 = 0x07,
	BS2_CARD_TYPE_WIEGAND		 = 0x0A,		// from Wiegand reader or iClass/HID Card reader
	BS2_CARD_TYPE_CONFIG_CARD	 = 0x0B,	
	BS2_CARD_TYPE_CUSTOM_SMART	 = 0x0D,	
};

typedef uint8_t	BS2_CARD_TYPE;

/**
 *  BS2CSNCard
 */
typedef struct {
	BS2_CARD_TYPE		type;
	uint8_t		size;
	uint8_t		data[BS2_CARD_DATA_SIZE];
} BS2CSNCard;
#endif	// __BS2_CSN_CARD_H__
