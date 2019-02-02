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

#ifndef __CSPDFIUMTEXTPAGE_H__
#define __CSPDFIUMTEXTPAGE_H__

#include <csPDFium/csPDFiumText.h>

class csPDFiumTextPage {
public:
  csPDFiumTextPage(const int no = -1,
                   const csPDFiumTexts& texts = csPDFiumTexts())
    : _pageNo(no)
    , _texts(texts)
  {
  }

  ~csPDFiumTextPage()
  {
  }

  inline bool isEmpty() const
  {
    return _pageNo < 0  ||  _texts.isEmpty();
  }

  inline int pageNo() const
  {
    return _pageNo;
  }

  inline const csPDFiumTexts& texts() const
  {
    return _texts;
  }

private:
  int _pageNo;
  csPDFiumTexts _texts;
};

typedef QList<csPDFiumTextPage> csPDFiumTextPages;

#endif // __CSPDFIUMTEXTPAGE_H__
