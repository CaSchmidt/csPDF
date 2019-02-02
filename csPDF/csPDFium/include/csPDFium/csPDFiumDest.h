/****************************************************************************
** Copyright (c) 2016, Carsten Schmidt. All rights reserved.
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

#ifndef __CSPDFIUMDEST_H__
#define __CSPDFIUMDEST_H__

#include <QtCore/QPointF>

struct csPDFiumDest {
  enum Type {
    InvalidDest = 0,
    Goto,
    RemoteGoto
  };

  csPDFiumDest()
    : _type(InvalidDest)
    , pageIndex()
    , focusPosPage()
    , srcFilename()
    , destFilename()
  {
  }

  explicit csPDFiumDest(const int _pageIndex,
                        const QPointF& _focusPosPage)
    : _type(Goto)
    , pageIndex(_pageIndex)
    , focusPosPage(_focusPosPage)
    , srcFilename()
    , destFilename()
  {
  }

  explicit csPDFiumDest(const QString& _srcFilename,
                        const QString& _destFilename)
    : _type(RemoteGoto)
    , pageIndex()
    , focusPosPage()
    , srcFilename(_srcFilename)
    , destFilename(_destFilename)
  {
  }

  inline bool isValid() const
  {
    return _type != InvalidDest;
  }

  inline Type type() const
  {
    return _type;
  }

  // Goto
  int     pageIndex;
  QPointF focusPosPage;
  // RemoteGoto
  QString srcFilename;
  QString destFilename;

private:
  Type _type;
};

#endif // __CSPDFIUMDEST_H__
