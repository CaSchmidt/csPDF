/****************************************************************************
** Copyright (c) 2015, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <QtGui/QPainterPath>

#include "internal/fpdf_util.h"

namespace util {

  namespace priv {

    void toPainterPath(QList<QPainterPath>& paths,
                       const FPDF_PAGEOBJECT object,
                       const QMatrix& ctmStack,
                       const csPDFium::PathExtractionFlags flags)
    {
      if( FPDFPageObj_GetType(object) != FPDF_PAGEOBJ_PATH ) {
        return;
      }

      if( FPDFPathObj_GetFillMode(object) > 0  &&
          !flags.testFlag(csPDFium::ExtractFilled) ) {
        return;
      }

      double a, b, c, d, e, f;
      FPDFPathObj_GetMatrix(object, &a, &b, &c, &d, &e, &f);
      const QMatrix M(a, b, c, d, e, f);

      const QMatrix ctm(M*ctmStack);

      bool hasClosed(false);
      QPainterPath path;
      for(int i = 0; i < FPDFPathObj_CountPoints(object); i++) {
        const int  type = FPDFPathObj_GetPointType(object, i) & FPDF_PATH_TYPE;

        if(        type == FPDF_PATH_MOVETO ) {
          const QPointF p(FPDFPathObj_GetPointX(object, i),
                          FPDFPathObj_GetPointY(object, i));
          path.moveTo(p*ctm);

        } else if( type == FPDF_PATH_LINETO ) {
          const QPointF p(FPDFPathObj_GetPointX(object, i),
                          FPDFPathObj_GetPointY(object, i));
          path.lineTo(p*ctm);

        } else if( type == FPDF_PATH_BEZIERTO ) {
          const QPointF c1(FPDFPathObj_GetPointX(object, i),
                           FPDFPathObj_GetPointY(object, i));
          const QPointF c2(FPDFPathObj_GetPointX(object, i+1),
                           FPDFPathObj_GetPointY(object, i+1));
          const QPointF  p(FPDFPathObj_GetPointX(object, i+2),
                           FPDFPathObj_GetPointY(object, i+2));
          path.cubicTo(c1*ctm, c2*ctm, p*ctm);
          i += 2;
        }

        const bool isClosed =
            (FPDFPathObj_GetPointType(object, i) & FPDF_PATH_CLOSEFIGURE) != 0;
        if( isClosed ) {
          path.closeSubpath();
          hasClosed = true;
        }
      } // For Each Point

      if( hasClosed ) {
        if( flags.testFlag(csPDFium::ExtractClosed) ) {
          paths.push_back(path);
        }
      } else {
        paths.push_back(path);
      }
    }

    void parseForm(QList<QPainterPath>& paths,
                   const FPDF_PAGEOBJECT object,
                   const QMatrix& ctmStack,
                   const csPDFium::PathExtractionFlags flags)
    {
      if( FPDFPageObj_GetType(object) != FPDF_PAGEOBJ_FORM ) {
        return;
      }

      double a, b, c, d, e, f;
      FPDFFormObj_GetMatrix(object, &a, &b, &c, &d, &e, &f);
      const QMatrix M(a, b, c, d, e, f);

      const QMatrix ctm(M*ctmStack);

      for(int i = 0; i < FPDFFormObj_CountSubObjects(object); i++) {
        const FPDF_PAGEOBJECT subObject = FPDFFormObj_GetSubObject(object, i);
        const int subType = FPDFPageObj_GetType(subObject);

        if(        subType == FPDF_PAGEOBJ_PATH ) {
          toPainterPath(paths, subObject, ctm, flags);
        } else if( subType == FPDF_PAGEOBJ_FORM ) {
          parseForm(paths, subObject, ctm, flags);
        }
      }
    }

  } // namespace priv

  QList<QPainterPath> extractPaths(const FPDF_PAGE page, const QMatrix& ctm,
                                   const csPDFium::PathExtractionFlags flags)
  {
    QList<QPainterPath> paths;

    for(int i = 0; i < FPDFPage_CountObject(page); i++) {
      const FPDF_PAGEOBJECT object = FPDFPage_GetObject(page, i);
      const int type = FPDFPageObj_GetType(object);

      if(        type == FPDF_PAGEOBJ_PATH ) {
        priv::toPainterPath(paths, object, ctm, flags);
      } else if( type == FPDF_PAGEOBJ_FORM ) {
        priv::parseForm(paths, object, ctm, flags);
      }
    } // For Each Object

    return paths;
  }

} // namespace util
