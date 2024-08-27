/*
 * File:	ximajpg.h
 * Purpose:	JPG Image Class Loader and Writer
 */
/* === C R E D I T S  &  D I S C L A I M E R S ==============
 * CxImageJPG (c) 07/Aug/2001 <ing.davide.pizzolato@libero.it>
 * Permission is given by the author to freely redistribute and include
 * this code in any program as long as this credit is given where due.
 *
 * CxImage version 4.01 15/May/2002
 * See the file history.htm for the complete bugfix and news report.
 *
 * Special thanks to Troels Knakkergaard for new features, enhancements and bugfixes
 *
 * Special thanks to Chris Shearer Cooper for CxFileJpg tips & code
 *
 * original CImageJPG  and CImageIterator implementation are:
 * Copyright:	(c) 1995, Alejandro Aguilar Sierra <asierra@servidor.unam.mx>
 *
 * This software is based in part on the work of the Independent JPEG Group.
 * Copyright (C) 1991-1998, Thomas G. Lane.
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
#if !defined(__ximaJPEG_h)
#define __ixmaJPEG_h

#include "ximage.h"

#if CXIMAGE_SUPPORT_JPG

extern "C" {
 #include "jpeglib.h"
 #include "jerror.h"
};

class CxImageJPG: public CxImage
{
public:
	CxImageJPG(): CxImage(CXIMAGE_FORMAT_JPG) {}

//	bool Load(const char * imageFileName){ return CxImage::Load(imageFileName,CXIMAGE_FORMAT_JPG);}
//	bool Save(const char * imageFileName){ return CxImage::Save(imageFileName,CXIMAGE_FORMAT_JPG);}
	bool Decode(CxFile * hFile);
	bool Encode(CxFile * hFile);

	bool Decode(FILE *hFile) { CxFile file(hFile); return Decode(&file); }
	bool Encode(FILE *hFile) { CxFile file(hFile); return Encode(&file); }

// thanks to Chris Shearer Cooper <cscooper@frii.com>
class CxFileJpg : public jpeg_destination_mgr, public jpeg_source_mgr, public CxFile
	{
public:
	enum { eBufSize = 4096 };

	CxFileJpg(CxFile* pFile)
	{
		Transfer(*pFile);

		init_destination = InitDestination;
		empty_output_buffer = EmptyOutputBuffer;
		term_destination = TermDestination;

		init_source = InitSource;
		fill_input_buffer = FillInputBuffer;
		skip_input_data = SkipInputData;
		resync_to_restart = jpeg_resync_to_restart; // use default method
		term_source = TermSource;
		next_input_byte = NULL; //* => next byte to read from buffer 
		bytes_in_buffer = 0;	//* # of bytes remaining in buffer 

		m_pBuffer = new unsigned char[eBufSize];
	}
	~CxFileJpg()
	{
		delete [] m_pBuffer;
	}

	static void InitDestination(j_compress_ptr cinfo)
	{
		CxFileJpg* pDest = (CxFileJpg*)cinfo->dest;
		pDest->next_output_byte = pDest->m_pBuffer;
		pDest->free_in_buffer = eBufSize;
	}

	static boolean EmptyOutputBuffer(j_compress_ptr cinfo)
	{
		CxFileJpg* pDest = (CxFileJpg*)cinfo->dest;
		if (pDest->Write(pDest->m_pBuffer,1,eBufSize)!=(size_t)eBufSize)
			ERREXIT(cinfo, JERR_FILE_WRITE);
		pDest->next_output_byte = pDest->m_pBuffer;
		pDest->free_in_buffer = eBufSize;
		return TRUE;
	}

	static void TermDestination(j_compress_ptr cinfo)
	{
		CxFileJpg* pDest = (CxFileJpg*)cinfo->dest;
		size_t datacount = eBufSize - pDest->free_in_buffer;
		/* Write any data remaining in the buffer */
		if (datacount > 0) {
			if (!pDest->Write(pDest->m_pBuffer,1,datacount))
				ERREXIT(cinfo, JERR_FILE_WRITE);
		}
		pDest->Flush();
		/* Make sure we wrote the output file OK */
		if (pDest->Error()) ERREXIT(cinfo, JERR_FILE_WRITE);
		return;
	}

	static void InitSource(j_decompress_ptr cinfo)
	{
		CxFileJpg* pSource = (CxFileJpg*)cinfo->src;
		pSource->m_bStartOfFile = TRUE;
	}

	static boolean FillInputBuffer(j_decompress_ptr cinfo)
	{
		size_t nbytes;
		CxFileJpg* pSource = (CxFileJpg*)cinfo->src;
		nbytes = pSource->Read(pSource->m_pBuffer,1,eBufSize);
		if (nbytes <= 0){
			if (pSource->m_bStartOfFile)	//* Treat empty input file as fatal error 
				ERREXIT(cinfo, JERR_INPUT_EMPTY);
			WARNMS(cinfo, JWRN_JPEG_EOF);
			// Insert a fake EOI marker 
			pSource->m_pBuffer[0] = (JOCTET) 0xFF;
			pSource->m_pBuffer[1] = (JOCTET) JPEG_EOI;
			nbytes = 2;
		}
		pSource->next_input_byte = pSource->m_pBuffer;
		pSource->bytes_in_buffer = nbytes;
		pSource->m_bStartOfFile = FALSE;
		return TRUE;
	}

	static void SkipInputData(j_decompress_ptr cinfo, long num_bytes)
	{
		CxFileJpg* pSource = (CxFileJpg*)cinfo->src;
		if (num_bytes > 0){
			while (num_bytes > (long)pSource->bytes_in_buffer){
				num_bytes -= (long)pSource->bytes_in_buffer;
				FillInputBuffer(cinfo);
				// note we assume that fill_input_buffer will never return FALSE,
				// so suspension need not be handled.
			}
			pSource->next_input_byte += (size_t) num_bytes;
			pSource->bytes_in_buffer -= (size_t) num_bytes;
		}
	}

	static void TermSource(j_decompress_ptr cinfo)
	{
		return;
	}
protected:
	unsigned char *m_pBuffer;
	bool m_bStartOfFile;
};

};

#endif

#endif
