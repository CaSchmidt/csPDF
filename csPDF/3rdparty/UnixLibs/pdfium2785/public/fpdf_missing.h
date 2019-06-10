// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _FPDFMISSING_H_
#define _FPDFMISSING_H_

#include "fpdfview.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT FPDF_DOCUMENT STDCALL FPDF_LoadDocumentW(FPDF_WIDESTRING file_path, FPDF_BYTESTRING password);

DLLEXPORT int FPDFPageObj_GetType(FPDF_PAGEOBJECT page_object);

DLLEXPORT int FPDFPathObj_CountPoints(FPDF_PAGEOBJECT path_object);

#define FPDF_PATH_CLOSEFIGURE  1 // Flag
#define FPDF_PATH_LINETO       2
#define FPDF_PATH_BEZIERTO     4
#define FPDF_PATH_MOVETO       6
#define FPDF_PATH_TYPE         6 // Mask

DLLEXPORT int FPDFPathObj_GetPointType(FPDF_PAGEOBJECT path_object, int point_index);

DLLEXPORT double FPDFPathObj_GetPointX(FPDF_PAGEOBJECT path_object, int point_index);

DLLEXPORT double FPDFPathObj_GetPointY(FPDF_PAGEOBJECT path_object, int point_index);

#define FPDF_FILL_NULL       0
#define FPDF_FILL_ALTERNATE  1
#define FPDF_FILL_WINDING    2

DLLEXPORT int FPDFPathObj_GetFillMode(FPDF_PAGEOBJECT path_object);

DLLEXPORT FPDF_BOOL FPDFPathObj_GetMatrix(FPDF_PAGEOBJECT path_object, 
                                          double* a, double* b, double* c,
                                          double* d, double* e, double* f);

DLLEXPORT int FPDFFormObj_CountSubObjects(FPDF_PAGEOBJECT form_object);

DLLEXPORT FPDF_PAGEOBJECT FPDFFormObj_GetSubObject(FPDF_PAGEOBJECT form_object, int index);

DLLEXPORT FPDF_BOOL FPDFFormObj_GetMatrix(FPDF_PAGEOBJECT form_object,
                                          double* a, double* b, double* c,
                                          double* d, double* e, double* f);

#define FPDF_ZOOM_XYZ      1
#define FPDF_ZOOM_FITPAGE  2
#define FPDF_ZOOM_FITHORZ  3
#define FPDF_ZOOM_FITVERT  4
#define FPDF_ZOOM_FITRECT  5

DLLEXPORT unsigned long FPDFDest_GetZoomMode(FPDF_DEST dest);

DLLEXPORT double FPDFDest_GetZoomParam(FPDF_DEST dest, int param);

DLLEXPORT void STDCALL FPDF_GetPageMatrix(FPDF_PAGE page,
                                          double *a, double *b, double *c,
                                          double *d, double *e, double *f);

#ifdef __cplusplus
}
#endif
#endif // _FPDFMISSING_H_
