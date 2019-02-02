/****************************************************************************
** Copyright (c) 2013-2015, Carsten Schmidt. All rights reserved.
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

#ifndef __CSPDFSEARCHRESULT_H__
#define __CSPDFSEARCHRESULT_H__

#include <QStringList>

#include <csPDFium/csPDFiumUtil.h>

class csPdfSearchResult {
public:
  inline csPdfSearchResult(const int pg = -1, const int idx = -1,
                           const QStringList& ctx = QStringList())
    : _page(pg)
    , _index(idx)
    , _context(ctx)
  {
  }

  inline ~csPdfSearchResult()
  {
  }

  inline bool isEmpty() const
  {
    return _page < 0  ||  _index < 0  ||  _context.isEmpty();
  }

  inline int page() const
  {
    return _page;
  }

  inline int index() const
  {
    return _index;
  }

  inline const QStringList context() const
  {
    return _context;
  }

  inline const QString contextString() const
  {
    return _context.join(_L1C(' '));
  }

  inline bool operator<(const csPdfSearchResult& other) const
  {
    return
        _page <  other._page  ||
        (_page == other._page  &&  _index < other._index);
  }

private:
  int _page;
  int _index;
  QStringList _context;
};

typedef QList<csPdfSearchResult>                       csPdfSearchResults;
typedef QList<csPdfSearchResult>::iterator             csPdfSearchResultIter;
typedef QList<csPdfSearchResult>::const_iterator  csConstPdfSearchResultIter;

Q_DECLARE_METATYPE(csPdfSearchResults)

#endif // __CSPDFSEARCHRESULT_H__
