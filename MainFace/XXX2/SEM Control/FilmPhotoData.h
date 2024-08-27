#if !defined(FilmPhotoData__INCLUDED_)
#define FilmPhotoData__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilmPhotoData.h : header file
//

extern BYTE g_cFilmPhotoData[];

///////////////////////////////////////////////////////////////////////////////

LPBYTE MakeFilmPhotoDataByGraphLay();
LPBYTE MakeFilmPhotoDataByImageLay();

///////////////////////////////////////////////////////////////////////////////

#endif //FilmPhotoData__INCLUDED_