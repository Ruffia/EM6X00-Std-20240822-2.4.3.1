/*
 * File:	ximagif.h
 * Purpose:	GIF Image Class Loader and Writer
 */
/* === C R E D I T S  &  D I S C L A I M E R S ==============
 * CxImageICO (c) 07/Aug/2001 <ing.davide.pizzolato@libero.it>
 * Permission is given by the author to freely redistribute and include
 * this code in any program as long as this credit is given where due.
 *
 * CxImage version 4.01 15/May/2002
 * See the file history.htm for the complete bugfix and news report.
 *
 * Special thanks to Troels Knakkergaard for new features, enhancements and bugfixes
 *
 * original CImageGIF  and CImageIterator implementation are:
 * Copyright:	(c) 1995, Alejandro Aguilar Sierra <asierra@servidor.unam.mx>
 *
 * 6/15/97 Randy Spann: Added GIF87a writing support
 *         R.Spann@ConnRiver.net
 *
 * DECODE.C - An LZW decoder for GIF
 * Copyright (C) 1987, by Steven A. Bennett
 * Copyright (C) 1994, C++ version by Alejandro Aguilar Sierra
 *
 * In accordance with the above, I want to credit Steve Wilhite who wrote
 * the code which this is heavily inspired by...
 *
 * GIF and 'Graphics Interchange Format' are trademarks (tm) of
 * Compuserve, Incorporated, an H&R Block Company.
 *
 * Release Notes: This file contains a decoder routine for GIF images
 * which is similar, structurally, to the original routine by Steve Wilhite.
 * It is, however, somewhat noticably faster in most cases.
 *
 * COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
 * OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
 * THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
 * CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
 * THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
 * SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
 * PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
 * THIS DISCLAIMER.
 *
 * Use at your own risk!
 * ==========================================================
 */

#if !defined(__ximaGIF_h)
#define __ximaGIF_h

#include "ximage.h"

#if CXIMAGE_SUPPORT_GIF

typedef short int       code_int;   
typedef long int        count_int;
typedef unsigned char   pixval;
typedef unsigned char   char_type;

/* Various error codes used by decoder */
#define OUT_OF_MEMORY -10
#define BAD_CODE_SIZE -20
#define READ_ERROR -1
#define WRITE_ERROR -2
#define OPEN_ERROR -3
#define CREATE_ERROR -4
#define MAX_CODES   4095
#define GIFBUFTAM 16384
#define TRANSPARENCY_CODE 0xF9

//LZW GIF Image compression
#define MAXBITSCODES    12
#define HSIZE  5003     /* 80% occupancy */
#define MAXCODE(n_bits) (((code_int) 1 << (n_bits)) - 1)
#define HashTabOf(i)    htab[i]
#define CodeTabOf(i)    codetab[i]


class CImageIterator;
class CxImageGIF: public CxImage
{
#pragma pack(1)

typedef struct tag_gifgce{
  BYTE transpcolflag:1;
  BYTE userinputflag:1;
  BYTE dispmeth:3;
  BYTE res:3;
  WORD delaytime;
  BYTE transpcolindex;
} struct_gifgce;

typedef struct tag_dscgif{		/* Logic Screen Descriptor  */
  char header[6];				/* Firma and version */
  USHORT scrwidth;
  USHORT scrheight;
  char pflds;
  char bcindx;
  char pxasrat;
} struct_dscgif;

typedef struct tag_image{      /* Image Descriptor */
  USHORT l;
  USHORT t;
  USHORT w;
  USHORT h;
  BYTE   pf;
} struct_image;

typedef struct tag_TabCol{		/* Tabla de colores */
  SHORT colres;					/* color resolution */
  SHORT sogct;					/* size of global color table */
  rgb_color_struct paleta[256]; /* paleta */
} struct_TabCol;

#pragma pack()

public:
	CxImageGIF(): CxImage(CXIMAGE_FORMAT_GIF) {m_loops=0; m_dispmeth=0; m_comment[0]='\0';}

//	bool Load(const char * imageFileName){ return CxImage::Load(imageFileName,CXIMAGE_FORMAT_GIF);}
//	bool Save(const char * imageFileName){ return CxImage::Save(imageFileName,CXIMAGE_FORMAT_GIF);}
	
	bool Decode(CxFile * fp);
	bool Encode(CxFile * fp);
	bool Encode(CxFile * fp, CxImage ** pImages, int pagecount);

	bool Decode(FILE *fp) { CxFile file(fp); return Decode(&file); }
	bool Encode(FILE *fp) { CxFile file(fp); return Encode(&file); }
	bool Encode(FILE *fp, CxImage ** pImages, int pagecount)
				{ CxFile file(fp); return Encode(&file, pImages, pagecount); }

	void SetLoops(int loops);
	long GetLoops();
	void SetComment(const char* sz_comment_in);
	void GetComment(char* sz_comment_out);
	void SetDisposalMethod(int dm);
	long GetDisposalMethod();

protected:
	bool DecodeExtension(CxFile *fp);
	void EncodeHeader(CxFile *fp);
	void EncodeLoopExtension(CxFile *fp);
	void EncodeExtension(CxFile *fp);
	void EncodeBody(CxFile *fp);
	void EncodeComment(CxFile *fp);
	void GifMix(CxImage & imgsrc2, long lxOffset, long lyOffset);
	
	struct_gifgce gifgce;

	int             curx, cury;
	long             CountDown;
	unsigned long    cur_accum;
	int              cur_bits;
	int interlaced, iypos, istep, iheight, ipass;
	int ibf;
	int ibfmax;
	BYTE buf[GIFBUFTAM + 1];
// Implementation
	int GifNextPixel ();
	void Putword (int w, CxFile* fp );
	void compress (int init_bits, CxFile* outfile);
	void output (code_int code );
	void cl_hash (count_int hsize);
	void char_out (int c);
	void flush_char ();
	short init_exp(short size);
	short get_next_code(CxFile*);
	short decoder(CxFile*, CImageIterator* iter, short linewidth, INT& bad_code_count);
	int get_byte(CxFile*);
	int out_line(CImageIterator* iter, unsigned char *pixels, int linelen);
	int get_num_frames(CxFile *f);

	short curr_size;                     /* The current code size */
	short clear;                         /* Value for a clear code */
	short ending;                        /* Value for a ending code */
	short newcodes;                      /* First available code */
	short top_slot;                      /* Highest code for current size */
	short slot;                          /* Last read code */

	/* The following static variables are used
	* for seperating out codes */
	short navail_bytes;              /* # bytes left in block */
	short nbits_left;                /* # bits left in current BYTE */
	BYTE b1;                           /* Current BYTE */
	BYTE byte_buff[257];               /* Current block */
	BYTE *pbytes;                      /* Pointer to next BYTE in block */
	/* The reason we have these seperated like this instead of using
	* a structure like the original Wilhite code did, is because this
	* stuff generally produces significantly faster code when compiled...
	* This code is full of similar speedups...  (For a good book on writing
	* C for speed or for space optomisation, see Efficient C by Tom Plum,
	* published by Plum-Hall Associates...)
	*/
	BYTE stack[MAX_CODES + 1];            /* Stack for storing pixels */
	BYTE suffix[MAX_CODES + 1];           /* Suffix table */
	USHORT prefix[MAX_CODES + 1];           /* Prefix linked list */

//LZW GIF Image compression routines
	count_int htab [HSIZE];
	unsigned short codetab [HSIZE];
	int n_bits;				/* number of bits/code */
	code_int maxcode;		/* maximum code, given n_bits */
	code_int free_ent;		/* first unused entry */
	int clear_flg;
	int g_init_bits;
	CxFile* g_outfile;
	int ClearCode;
	int EOFCode;

	int a_count;
	char accum[256];

	char m_comment[256];
	int m_loops;
	int m_dispmeth;
};

#endif

#endif
