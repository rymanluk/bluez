/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2011-2014  Intel Corporation
 *  Copyright (C) 2002-2010  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <inttypes.h>

#include "lib/bluetooth.h"
#include "lib/hci.h"

#include "src/shared/util.h"
#include "display.h"
#include "packet.h"
#include "ll.h"
#include "vendor.h"
#include "mynewt.h"

static const struct vendor_ocf vendor_ocf_table[] = {
	{ }
};

const struct vendor_ocf *mynewt_vendor_ocf(uint16_t ocf)
{
	int i;

	for (i = 0; vendor_ocf_table[i].str; i++) {
		if (vendor_ocf_table[i].ocf == ocf)
			return &vendor_ocf_table[i];
	}

	return NULL;
}

static void llcp_pdu_trace_evt(const void *data, uint8_t size)
{
	uint8_t type;
	uint16_t handle;
	const char *str;

	type = get_u8(data);
	handle = get_le16(data + 1);

	switch (type) {
	case 0x03:
		str = "LLCP RX";
		break;
	case 0x04:
		str = "LLCP TX";
		break;
	default:
		str = "Unknown";
		break;
	}

	print_field("Type: %s (0x%2.2x)", str, type);
	print_field("Handle: %u", handle);

	switch (type) {
	case 0x03:
		llcp_packet(data + 3, size - 3, false);
		break;
	case 0x04:
	    llcp_packet(data + 3, size - 3, false);
		break;
	default:
		packet_hexdump(data + 3, size - 3);
		break;
	}
}

static const struct vendor_evt vendor_evt_table[] = {
	{ 0x03, "LLCP PDU Trace",
			llcp_pdu_trace_evt, 3, false },
	{ }
};

const struct vendor_evt *mynewt_vendor_evt(uint8_t evt)
{
	int i;

	for (i = 0; vendor_evt_table[i].str; i++) {
		if (vendor_evt_table[i].evt == evt)
			return &vendor_evt_table[i];
	}

	return NULL;
}
