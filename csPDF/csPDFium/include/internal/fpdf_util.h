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

#ifndef __FPDF_UTIL_H__
#define __FPDF_UTIL_H__

#include <fpdfsdk/fpdfview.h>
#include <fpdfsdk/fpdf_doc.h>
#include <fpdfsdk/fpdf_edit.h>
#include <fpdfsdk/fpdf_missing.h>
#include <fpdfsdk/fpdf_text.h>

#include <QtGui/QMatrix>

#include <csPDFium/csPDFium.h>
#include <csPDFium/csPDFiumContentsNode.h>
#include <csPDFium/csPDFiumText.h>

// cf. "fx_ge.h" for FXPT_* aka. FPDF_PATH_* Definitions
// cf. "fx_ge.h" for FXFILL_* aka. FPDF_FILL_* Definitions
// cf. "fx_agg_driver.cpp" for CAgg_PathData::BuildPath() aka. Path Construction
// cf. "PDF Reference v1.7", "4.2.3 Transformation Matrices" for CTM Concatenation
// cf. "fx_agg_path_storage.[cpp|h]" for Path Construction
// cf. "fpdf_page[obj].h" for Page Objects

namespace util {

  csPDFiumTexts extractTexts(const FPDF_PAGE page, const QMatrix& ctm);

  QStringList extractWords(const FPDF_PAGE page);

  void parseContents(const FPDF_BOOKMARK bookmark, const FPDF_DOCUMENT doc,
                     csPDFiumContentsNode *parent);

  QList<QPainterPath> extractPaths(const FPDF_PAGE page, const QMatrix& ctm,
                                   const csPDFium::PathExtractionFlags flags);

  QMatrix getPageCTM(const FPDF_PAGE page);

} // namespace util

#endif // __FPDF_UTIL_H__
