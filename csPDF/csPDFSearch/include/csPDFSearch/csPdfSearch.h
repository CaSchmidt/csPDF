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

#ifndef __CSPDFSEARCH_H__
#define __CSPDFSEARCH_H__

#include <QObject>

#include <csPDFSearch/cspdfsearch_config.h>
#include <csPDFium/csPDFiumDocument.h>
#include <csPDFSearch/csPdfSearchResult.h>

class CS_PDFSEARCH_EXPORT csPdfSearch : public QObject {
  Q_OBJECT
public:
  csPdfSearch(QObject *parent = 0);
  ~csPdfSearch();

  bool isRunning() const;
  bool start(const csPDFiumDocument& doc, const QStringList& needles,
             const int startIndex = 0,
             const Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchCaseSensitive | Qt::MatchWrap),
             const int context = 0);

public slots:
  void cancel();
  void clear();

signals:
  void canceled();
  void finished();
  void found(const csPdfSearchResults& results);
  void processed(int value);
  void started();

private:
  Q_INVOKABLE void processSearch();
  void initialize(const int start, const int count);
  bool isBlocksFinished() const;
  bool isRemainFinished() const;
  bool isFinished();
  void progressUpdate();
  csPdfSearchResults searchPages(const csPDFiumTextPages& hay,
                                 const QStringList& needles,
                                 const Qt::CaseSensitivity cs,
                                 const int context);

  csPDFiumDocument _doc;
  QStringList _needles;
  Qt::CaseSensitivity _cs;
  bool _wrap;
  int _context;
  volatile bool _cancel;
  volatile bool _running;
  int _startIndex;
  int _numBlocks;
  int _numRemain;
  int _cntBlocks;
  int _cntRemain;
  int _cntIndex;
  int _numToDo;
  int _cntDone;
  int _lastProgress;
};

#endif // __CSPDFSEARCH_H__
