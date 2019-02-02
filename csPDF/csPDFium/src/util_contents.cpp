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

#include "internal/fpdf_util.h"

namespace util {

  void parseContents(const FPDF_BOOKMARK bookmark, const FPDF_DOCUMENT doc,
                     csPDFiumContentsNode *parent)
  {
    if( bookmark == NULL ) {
      return;
    }

    const ulong sz = FPDFBookmark_GetTitle(bookmark, NULL, 0);
    QByteArray buffer(sz, 0);
    if( buffer.size() != sz ) {
      return;
    }
    FPDFBookmark_GetTitle(bookmark, buffer.data(), buffer.size());

    csPDFiumContentsNode *child =
        new csPDFiumContentsNode((const ushort*)buffer.constData(), bookmark,
                                 parent);
    if( child == 0 ) {
      return;
    }
    parent->appendChild(child);

    const FPDF_BOOKMARK down = FPDFBookmark_GetFirstChild(doc, bookmark);
    parseContents(down, doc, child);

    const FPDF_BOOKMARK next = FPDFBookmark_GetNextSibling(doc, bookmark);
    parseContents(next, doc, parent);
  }

} // namespace util
