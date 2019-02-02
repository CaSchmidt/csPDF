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

#ifndef __CSPDFIUMTEXT_H__
#define __CSPDFIUMTEXT_H__

#include <QtCore/QList>
#include <QtCore/QRectF>
#include <QtCore/QString>

#include <csPDFium/csPDFiumUtil.h>

class csPDFiumText {
public:
  csPDFiumText(const QRectF& rect = QRectF(), const QString& text = QString(),
               const int pos = -1)
    : _rect(rect)
    , _text(text)
    , _pos(pos)
  {
  }

  ~csPDFiumText()
  {
  }

  csPDFiumText& operator=(const QRectF& r)
  {
    _rect = r;
    return *this;
  }

  csPDFiumText& operator=(const QString& t)
  {
    _text = t;
    return *this;
  }

  inline bool isEmpty() const
  {
    // NOTE:
    // QRectF::isEmpty() is TRUE if ONE  dimension  is  0
    // QRectF::isNull()  is TRUE if BOTH dimensions are 0
    return _rect.isNull()  ||  _text.isEmpty();
  }

  inline void clear()
  {
    _pos  = -1;
    _rect = QRectF();
    _text.clear();
  }

  inline int pos() const
  {
    return _pos;
  }

  inline void setPos(const int pos)
  {
    _pos = pos;
  }

  inline const QRectF& rect() const
  {
    return _rect;
  }

  inline const QString& text() const
  {
    return _text;
  }

  inline void merge(const QRectF& r, const QString& t)
  {
    _rect |= r;
    _text += t;
  }

  inline bool operator<(const csPDFiumText& other) const
  {
    return _pos < other._pos;
  }

private:
  int     _pos;
  QRectF  _rect;
  QString _text;
};

typedef QList<csPDFiumText> csPDFiumTexts;

#endif // __CSPDFIUMTEXT_H__
