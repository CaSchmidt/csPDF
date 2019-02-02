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

#ifndef __CSPDFIUMPAGE_H__
#define __CSPDFIUMPAGE_H__

#include <QtCore/QRectF>
#include <QtCore/QSharedPointer>
#include <QtCore/QSizeF>
#include <QtGui/QImage>
#include <QtGui/QPainterPath>

#include <csPDFium/cspdfium_config.h>
#include <csPDFium/csPDFium.h>
#include <csPDFium/csPDFiumLink.h>
#include <csPDFium/csPDFiumText.h>

class csPDFiumPageImpl;

class CS_PDFIUM_EXPORT csPDFiumPage {
public:
  csPDFiumPage();
  ~csPDFiumPage();

  bool isEmpty() const;
  void clear();
  QPointF mapToScene(const QPointF& p) const;
  int number() const;
  QImage renderToImage(const qreal scale = 1.0) const;
  csPDFiumLinks links() const;
  QRectF rect() const;
  QSizeF size() const;

  QString text() const;
  csPDFiumTexts texts(const QRectF& area = QRectF()) const;
  QStringList words() const;

  QList<QPainterPath> extractPaths(const csPDFium::PathExtractionFlags flags = 0) const;

private:
  QSharedPointer<csPDFiumPageImpl> impl;
  friend class csPDFiumDocument;
};

#endif // __CSPDFIUMPAGE_H__
