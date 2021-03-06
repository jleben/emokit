/* Copyright (c) 2010, Daeken and Skadge
 * Copyright (c) 2011, Kyle Machulis <kyle@nonpolynomial.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef LIBEPOC_H_
#define LIBEPOC_H_

#include <stdio.h>
#include <stdint.h>

#define E_NPUTIL_DRIVER_ERROR -1
#define E_NPUTIL_NOT_INITED -2
#define E_NPUTIL_NOT_OPENED -3

#if !defined(WIN32)
# define EPOC_DECLSPEC
#else
# define WIN32_LEAN_AND_MEAN
# define EPOC_DECLSPEC __declspec(dllexport)
#endif

typedef struct epoc_device epoc_device;

/// Vendor ID for all omron health devices
const static uint32_t EPOC_VID = 0x21a1;
/// Product ID for all omron health devices
const static uint32_t EPOC_PID = 0x0001;

/// Out endpoint for all omron health devices
const static uint32_t EPOC_OUT_ENDPT = 0x02;
/// In endpoint for all omron health devices
const static uint32_t EPOC_IN_ENDPT  = 0x82;


struct epoc_contact_quality {
    char F3, FC6, P7, T8, F7, F8, T7, P8, AF4, F4, AF3, O2, O1, FC5;
};

struct epoc_frame {
    int F3, FC6, P7, T8, F7, F8, T7, P8, AF4, F4, AF3, O2, O1, FC5;
    struct epoc_contact_quality cq;
    char gyroX, gyroY;
    char battery;
};

#ifdef __cplusplus
extern "C"
{
#endif
EPOC_DECLSPEC int epoc_init(unsigned char * key, int key_size);
EPOC_DECLSPEC int epoc_deinit();

EPOC_DECLSPEC int epoc_get_next_raw(unsigned char raw_frame[32], unsigned char* raw_data);
EPOC_DECLSPEC int epoc_get_next_frame(struct epoc_frame* frame, unsigned char* raw_data);

EPOC_DECLSPEC epoc_device* epoc_create();
EPOC_DECLSPEC int epoc_get_count(epoc_device* s, int device_vid, int device_pid);
EPOC_DECLSPEC int epoc_open(epoc_device* s, int device_vid, int device_pid, unsigned int device_index);
EPOC_DECLSPEC int epoc_close(epoc_device* s);
EPOC_DECLSPEC void epoc_delete(epoc_device* dev);
EPOC_DECLSPEC int epoc_read_data(epoc_device* dev, uint8_t* input_report);
#ifdef __cplusplus
};
#endif

#define KEY_SIZE 16 /* 128 bits == 16 bytes */

extern EPOC_DECLSPEC unsigned char RESEARCH_KEY[];
extern EPOC_DECLSPEC unsigned char SPECIAL_KEY[];
extern EPOC_DECLSPEC unsigned char CONSUMER_KEY[];
extern EPOC_DECLSPEC unsigned char CONSUMER2_KEY[];
extern EPOC_DECLSPEC unsigned char CONSUMER3_KEY[];
extern EPOC_DECLSPEC unsigned char CONSUMER4_KEY[];

#endif //LIBEPOC_H_
