// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "public/fpdf_missing.h"

#include "fpdfsdk/include/fsdk_define.h"
#include "core/fpdfapi/fpdf_page/include/cpdf_form.h"
#include "core/fpdfapi/fpdf_page/include/cpdf_formobject.h"
#include "core/fpdfapi/fpdf_page/include/cpdf_page.h"
#include "core/fpdfapi/fpdf_page/include/cpdf_pageobject.h"
#include "core/fpdfapi/fpdf_page/include/cpdf_pathobject.h"
#include "core/fpdfapi/fpdf_parser/include/cpdf_array.h"

DLLEXPORT FPDF_DOCUMENT STDCALL FPDF_LoadDocumentW(FPDF_WIDESTRING file_path, FPDF_BYTESTRING password)
{
  // NOTE: the creation of the file needs to be by the embedder on the
  // other side of this API.
  IFX_FileRead* pFileAccess = FX_CreateFileRead((const FX_WCHAR*)file_path);
  if( !pFileAccess ) {
    return nullptr;
  }

  CPDF_Parser* pParser = new CPDF_Parser;
  pParser->SetPassword(password);

  CPDF_Parser::Error error = pParser->StartParse(pFileAccess);
  if( error != CPDF_Parser::SUCCESS ) {
    delete pParser;
    ProcessParseError(error);
    return nullptr;
  }
#ifdef PDF_ENABLE_XFA
  CPDF_Document* pPDFDoc = pParser->GetDocument();
  if( !pPDFDoc ) {
    return nullptr;
  }

  CPDFXFA_App* pProvider = CPDFXFA_App::GetInstance();
  return new CPDFXFA_Document(pPDFDoc, pProvider);
#else   // PDF_ENABLE_XFA
  return pParser->GetDocument();
#endif  // PDF_ENABLE_XFA
}

DLLEXPORT int FPDFPageObj_GetType(FPDF_PAGEOBJECT page_object)
{
  CPDF_PageObject *pPageObj = static_cast<CPDF_PageObject*>(page_object);
  if( !pPageObj ) {
    return -1;
  }
  return pPageObj->GetType();
}

DLLEXPORT int FPDFPathObj_CountPoints(FPDF_PAGEOBJECT path_object)
{
  CPDF_PathObject *pPathObj = static_cast<CPDF_PathObject*>(path_object);
  if( !pPathObj ) {
    return -1;
  }

  CPDF_Path& path = pPathObj->m_Path;

  return path.GetPointCount();
}

DLLEXPORT int FPDFPathObj_GetPointType(FPDF_PAGEOBJECT path_object, int point_index)
{
  CPDF_PathObject *pPathObj = static_cast<CPDF_PathObject*>(path_object);
  if( !pPathObj ) {
    return -1;
  }

  CPDF_Path& path = pPathObj->m_Path;
  if( point_index < 0  ||  point_index >= path.GetPointCount() ) {
    return -1;
  }

  return path.GetFlag(point_index);
}

DLLEXPORT double FPDFPathObj_GetPointX(FPDF_PAGEOBJECT path_object, int point_index)
{
  CPDF_PathObject *pPathObj = static_cast<CPDF_PathObject*>(path_object);
  if( !pPathObj ) {
    return 0.0;
  }

  CPDF_Path& path = pPathObj->m_Path;
  if( point_index < 0  ||  point_index >= path.GetPointCount() ) {
    return 0.0;
  }

  return path.GetPointX(point_index);
}

DLLEXPORT double FPDFPathObj_GetPointY(FPDF_PAGEOBJECT path_object, int point_index)
{
  CPDF_PathObject *pPathObj = static_cast<CPDF_PathObject*>(path_object);
  if( !pPathObj ) {
    return 0.0;
  }

  CPDF_Path& path = pPathObj->m_Path;
  if( point_index < 0  ||  point_index >= path.GetPointCount() ) {
    return 0.0;
  }

  return path.GetPointY(point_index);
}

DLLEXPORT int FPDFPathObj_GetFillMode(FPDF_PAGEOBJECT path_object)
{
  CPDF_PathObject *pPathObj = static_cast<CPDF_PathObject*>(path_object);
  if( !pPathObj ) {
    return -1;
  }

  return pPathObj->m_FillType;
}

DLLEXPORT FPDF_BOOL FPDFPathObj_GetMatrix(FPDF_PAGEOBJECT path_object,
                                          double* a, double* b, double* c,
                                          double* d, double* e, double* f)
{
  CPDF_PathObject *pPathObj = static_cast<CPDF_PathObject*>(path_object);
  if( !pPathObj  ||  !a  ||  !b  ||  !c  ||  !d  ||  !e  ||  !f ) {
    return FALSE;
  }

  *a = pPathObj->m_Matrix.a;
  *b = pPathObj->m_Matrix.b;
  *c = pPathObj->m_Matrix.c;
  *d = pPathObj->m_Matrix.d;
  *e = pPathObj->m_Matrix.e;
  *f = pPathObj->m_Matrix.f;

  return TRUE;
}

DLLEXPORT int FPDFFormObj_CountSubObjects(FPDF_PAGEOBJECT form_object)
{
  CPDF_FormObject *pFormObj = static_cast<CPDF_FormObject*>(form_object);
  if( !pFormObj ) {
    return -1;
  }

  CPDF_Form *pForm = pFormObj->m_pForm;
  if( !pForm ) {
    return -1;
  }

  return pForm->GetPageObjectList()->size();
}

DLLEXPORT FPDF_PAGEOBJECT FPDFFormObj_GetSubObject(FPDF_PAGEOBJECT form_object, int index)
{
  CPDF_FormObject *pFormObj = static_cast<CPDF_FormObject*>(form_object);
  if( !pFormObj ) {
    return NULL;
  }

  CPDF_Form *pForm = pFormObj->m_pForm;
  if( !pForm  ||  index < 0  ||  index >= pForm->GetPageObjectList()->size() ) {
    return NULL;
  }

  return (FPDF_PAGEOBJECT)pForm->GetPageObjectList()->operator[](index).get();
}

DLLEXPORT FPDF_BOOL FPDFFormObj_GetMatrix(FPDF_PAGEOBJECT form_object,
                                          double* a, double* b, double* c,
                                          double* d, double* e, double* f)
{
  CPDF_FormObject *pFormObj = static_cast<CPDF_FormObject*>(form_object);
  if( !pFormObj  ||  !a  ||  !b  ||  !c  ||  !d  ||  !e  ||  !f ) {
    return FALSE;
  }

  *a = pFormObj->m_FormMatrix.a;
  *b = pFormObj->m_FormMatrix.b;
  *c = pFormObj->m_FormMatrix.c;
  *d = pFormObj->m_FormMatrix.d;
  *e = pFormObj->m_FormMatrix.e;
  *f = pFormObj->m_FormMatrix.f;

  return TRUE;
}

DLLEXPORT unsigned long FPDFDest_GetZoomMode(FPDF_DEST pDest)
{
  if( !pDest ) {
    return 0;
  }
  CPDF_Dest dest(static_cast<CPDF_Array*>(pDest));
  return dest.GetZoomMode();
}

DLLEXPORT double FPDFDest_GetZoomParam(FPDF_DEST pDest, int param)
{
  if( !pDest ) {
    return 0;
  }
  CPDF_Dest dest(static_cast<CPDF_Array*>(pDest));
  return dest.GetParam(param);
}

DLLEXPORT void STDCALL FPDF_GetPageMatrix(FPDF_PAGE page,
                                          double *a, double *b, double *c,
                                          double *d, double *e, double *f)
{
  if( !page  ||  !a  ||  !b  ||  !c  ||  !d  ||  !e  ||  !f ) {
    return;
  }
  const CFX_Matrix& m = CPDFPageFromFPDFPage(page)->GetPageMatrix();
  *a = m.a;
  *b = m.b;
  *c = m.c;
  *d = m.d;
  *e = m.e;
  *f = m.f;
}
