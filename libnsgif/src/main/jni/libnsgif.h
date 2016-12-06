/*
 * Copyright 2004 Richard Wilson <richard.wilson@netsurf-browser.org>
 * Copyright 2008 Sean Fox <dyntryx@gmail.com>
 *
 * This file is part of NetSurf's libnsgif, http://www.netsurf-browser.org/
 * Licenced under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 */

/** \file
 * Progressive animated GIF file decoding (interface).
 */

#ifndef _LIBNSGIF_H_
#define _LIBNSGIF_H_

#include <stdbool.h>
#include <inttypes.h>


/*	Maximum colour table size
*/
#define GIF_MAX_COLOURS 256

/*	Internal flag that the colour table needs to be processed
*/
#define GIF_PROCESS_COLOURS 0xaa000000

/*	Internal flag that a frame is invalid/unprocessed
*/
#define GIF_INVALID_FRAME -1

/*	Maximum LZW bits available
*/
#define GIF_MAX_LZW 12

/* Transparent colour
*/
#define GIF_TRANSPARENT_COLOUR 0x00

/*	GIF Flags
*/
#define GIF_FRAME_COMBINE 1
#define GIF_FRAME_CLEAR 2
#define GIF_FRAME_RESTORE 3
#define GIF_FRAME_QUIRKS_RESTORE 4
#define GIF_IMAGE_SEPARATOR 0x2c
#define GIF_INTERLACE_MASK 0x40
#define GIF_COLOUR_TABLE_MASK 0x80
#define GIF_COLOUR_TABLE_SIZE_MASK 0x07
#define GIF_EXTENSION_INTRODUCER 0x21
#define GIF_EXTENSION_GRAPHIC_CONTROL 0xf9
#define GIF_DISPOSAL_MASK 0x1c
#define GIF_TRANSPARENCY_MASK 0x01
#define GIF_EXTENSION_COMMENT 0xfe
#define GIF_EXTENSION_PLAIN_TEXT 0x01
#define GIF_EXTENSION_APPLICATION 0xff
#define GIF_BLOCK_TERMINATOR 0x00
#define GIF_TRAILER 0x3b

/*	Error return values
*/
typedef enum {
	GIF_WORKING = 1,
	GIF_OK = 0,
	GIF_INSUFFICIENT_FRAME_DATA = -1,
	GIF_FRAME_DATA_ERROR = -2,
	GIF_INSUFFICIENT_DATA = -3,
	GIF_DATA_ERROR = -4,
	GIF_INSUFFICIENT_MEMORY = -5,
	GIF_FRAME_NO_DISPLAY = -6,
	GIF_END_OF_FRAME = -7
} gif_result;

/*	The GIF frame data
*/
typedef struct gif_frame {
  	bool display;				/**< whether the frame should be displayed/animated */
  	unsigned int frame_delay;		/**< delay (in cs) before animating the frame */
	/**	Internal members are listed below
	*/
  	unsigned int frame_pointer;		/**< offset (in bytes) to the GIF frame data */
  	bool virgin;				/**< whether the frame has previously been used */
	bool opaque;				/**< whether the frame is totally opaque */
	bool redraw_required;			/**< whether a forcable screen redraw is required */
	unsigned char disposal_method;		/**< how the previous frame should be disposed; affects plotting */
	bool transparency;	 		/**< whether we acknoledge transparency */
	unsigned char transparency_index;	/**< the mCurIndex designating a transparent pixel */
	unsigned int redraw_x;			/**< x co-ordinate of redraw rectangle */
	unsigned int redraw_y;			/**< y co-ordinate of redraw rectangle */
	unsigned int redraw_width;		/**< width of redraw rectangle */
	unsigned int redraw_height;		/**< height of redraw rectangle */
} gif_frame;

/*	API for Bitmap callbacks
*/
typedef void* (*gif_bitmap_cb_create)(int width, int height);
typedef void (*gif_bitmap_cb_destroy)(void *bitmap);
typedef unsigned char* (*gif_bitmap_cb_get_buffer)(void *bitmap);
typedef void (*gif_bitmap_cb_set_opaque)(void *bitmap, bool opaque);
typedef bool (*gif_bitmap_cb_test_opaque)(void *bitmap);
typedef void (*gif_bitmap_cb_modified)(void *bitmap);

/*	The Bitmap callbacks function table
*/
typedef struct gif_bitmap_callback_vt {
	gif_bitmap_cb_create bitmap_create;		/**< Create a bitmap. */
	gif_bitmap_cb_destroy bitmap_destroy;		/**< Free a bitmap. */
	gif_bitmap_cb_get_buffer bitmap_get_buffer;	/**< Return a pointer to the pixel data in a bitmap. */
	/**	Members below are optional
	*/
	gif_bitmap_cb_set_opaque bitmap_set_opaque;	/**< Sets whether a bitmap should be plotted opaque. */
	gif_bitmap_cb_test_opaque bitmap_test_opaque;	/**< Tests whether a bitmap has an opaque alpha channel. */
	gif_bitmap_cb_modified bitmap_modified;	/**< The bitmap image has changed, so flush any persistant cache. */
} gif_bitmap_callback_vt;

/*	The GIF animation data
*/
typedef struct gif_animation {
	gif_bitmap_callback_vt bitmap_callbacks;	/**< callbacks for bitmap functions */
	unsigned char *gif_data;			/**< pointer to GIF data */
	unsigned int width;				/**< width of GIF (may increase during decoding) */
	unsigned int height;				/**< heigth of GIF (may increase during decoding) */
	unsigned int frame_count;			/**< number of frames decoded */
	unsigned int frame_count_partial;		/**< number of frames partially decoded */
	gif_frame *frames;				/**< decoded frames */
	int decoded_frame;				/**< current frame decoded to bitmap */
	void *frame_image;				/**< currently decoded image; stored as bitmap from bitmap_create callback */
	int loop_count;					/**< number of times to loop animation */
	gif_result current_error;			/**< current error type, or 0 for none*/
	/**	Internal members are listed below
	*/
	unsigned int buffer_position;			/**< current mCurIndex into GIF data */
	unsigned int buffer_size;			/**< total number of bytes of GIF data available */
	unsigned int frame_holders;			/**< current number of frame holders */
	unsigned int background_index;			/**< mCurIndex in the colour table for the background colour */
	unsigned int aspect_ratio;			/**< image aspect ratio (ignored) */
	unsigned int colour_table_size;		/**< size of colour table (in entries) */
	bool global_colours;				/**< whether the GIF has a global colour table */
	unsigned int *global_colour_table;		/**< global colour table */
	unsigned int *local_colour_table;		/**< local colour table */

	/*	General LZW values. They are shared for all GIFs being decoded, and
	thus we can't handle progressive decoding efficiently without having
	the data for each image which would use an extra 10Kb or so per GIF.
	*/
	unsigned char buf[4];
	unsigned char *direct;
	int table[2][(1 << GIF_MAX_LZW)];
	unsigned char stack[(1 << GIF_MAX_LZW) * 2];
	unsigned char *stack_pointer;
	int code_size, set_code_size;
	int max_code, max_code_size;
	int clear_code, end_code;
	int curbit, lastbit, last_byte;
	int firstcode, oldcode;
	bool zero_data_block;
	bool get_done;

	/*	Whether to clear the decoded image rather than plot
	*/
	bool clear_image;
} gif_animation;

void gif_create(gif_animation *gif, gif_bitmap_callback_vt *bitmap_callbacks);
gif_result gif_initialise(gif_animation *gif, size_t size, unsigned char *data);
gif_result gif_decode_frame(gif_animation *gif, unsigned int frame);
void gif_finalise(gif_animation *gif);

#endif
