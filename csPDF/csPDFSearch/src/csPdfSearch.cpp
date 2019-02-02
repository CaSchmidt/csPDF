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

#include <csPDFSearch/csPdfSearch.h>

#include "internal/config.h"
#include <csPDFSearch/csPdfSearchUtil.h>

////// public ////////////////////////////////////////////////////////////////

csPdfSearch::csPdfSearch(QObject *parent)
  : QObject(parent)
  , _doc()
  , _needles()
  , _cs()
  , _wrap()
  , _context()
  , _cancel()
  , _running()
  , _startIndex()
  , _numBlocks()
  , _numRemain()
  , _cntBlocks()
  , _cntRemain()
  , _cntIndex()
  , _numToDo()
  , _cntDone()
  , _lastProgress()
{
}

csPdfSearch::~csPdfSearch()
{
}

bool csPdfSearch::isRunning() const
{
  return _running;
}

bool csPdfSearch::start(const csPDFiumDocument& doc, const QStringList& needles,
                        const int startIndex, const Qt::MatchFlags flags,
                        const int context)
{
  if( _running  ||
      doc.isEmpty()  ||  doc.pageCount() < 1  ||
      needles.isEmpty()  ||
      startIndex < 0  ||  startIndex >= doc.pageCount()  ||
      context < 0 ) {
    return false;
  }

  _doc     = doc;
  _needles = needles;
  _context = context;
  _cs      = flags.testFlag(Qt::MatchCaseSensitive)
      ? Qt::CaseSensitive
      : Qt::CaseInsensitive;
  _wrap    = flags.testFlag(Qt::MatchWrap);
  _cancel  = false;

  if( _wrap ) {
    _numToDo = _doc.pageCount();
  } else {
    _numToDo = _doc.pageCount() - startIndex;
  }
  _cntDone = 0;
  _lastProgress = -1;
  progressUpdate();

  initialize(startIndex, _doc.pageCount());
  _running = true;
  QMetaObject::invokeMethod(this, "processSearch", Qt::QueuedConnection);
  emit started();

  return true;
}

////// public slots //////////////////////////////////////////////////////////

void csPdfSearch::cancel()
{
  _cancel = true;
}

void csPdfSearch::clear()
{
  if( !_running ) {
    _doc.clear();
  }
}

////// private ///////////////////////////////////////////////////////////////

void csPdfSearch::processSearch()
{
  if( !_running ) {
    return;
  }

  if( _cancel ) {
    _running = false;
    emit canceled();
    return;
  }

  int blockSize = 0;
  if(        !isBlocksFinished() ) {
    // Do CSPDF_SEARCH_BLOCKSIZE's Work...
    blockSize = CSPDF_SEARCH_BLOCKSIZE;
    _cntBlocks++;
  } else if(  isBlocksFinished()  &&  !isRemainFinished() ) {
    // Do _numRemain's Work...
    blockSize = _numRemain;
    _cntRemain += _numRemain;
  }

  if( blockSize != 0 ) {
    const csPdfSearchResults results =
        searchPages(_doc.textPages(_cntIndex, blockSize),
                    _needles, _cs, _context);
    if( !results.isEmpty() ) {
      emit found(results);
    }
    _cntIndex += blockSize;
    _cntDone  += blockSize;
    progressUpdate();
  }

  if( isFinished()  &&  _startIndex != 0  &&  _wrap ) {
    initialize(0, _startIndex);
    _startIndex = 0;
  }

  if( !isFinished() ) {
    QMetaObject::invokeMethod(this, "processSearch", Qt::QueuedConnection);
  } else {
    _running = false;
    emit finished();
  }
}

void csPdfSearch::initialize(const int start, const int count)
{
  _startIndex = start;
  _numBlocks  = (count - start) / CSPDF_SEARCH_BLOCKSIZE;
  _numRemain  = (count - start) % CSPDF_SEARCH_BLOCKSIZE;
  _cntBlocks  = 0;
  _cntRemain  = 0;
  _cntIndex   = start;
}

bool csPdfSearch::isBlocksFinished() const
{
  return _cntBlocks == _numBlocks;
}

bool csPdfSearch::isRemainFinished() const
{
  return _cntRemain == _numRemain;
}

bool csPdfSearch::isFinished()
{
  return isBlocksFinished()  &&  isRemainFinished();
}

void csPdfSearch::progressUpdate()
{
  const int p = qBound(0, _cntDone * 100 / _numToDo, 100);
  if( p != _lastProgress ) {
    emit processed(p);
  }
  _lastProgress = p;
}

csPdfSearchResults csPdfSearch::searchPages(const csPDFiumTextPages& hay,
                                            const QStringList& needles,
                                            const Qt::CaseSensitivity cs,
                                            const int context)
{
  csPdfSearchResults results;
  foreach(const csPDFiumTextPage textPage, hay) {
    const csPdfFindResults found = needles.size() == 1
        ? csPdfFindAll(textPage.texts(), needles.front(), cs)
        : csPdfFindAll(textPage.texts(), needles, cs);
    foreach(const int index, found) {
      int pos = index-context;
      int n   = needles.size()+2*context;
      if( pos < 0 ) {
        pos = 0;
        n  += index-context; // index-context < 0 !!!
      }
      QStringList sl;
      foreach(const csPDFiumText t, textPage.texts().mid(pos, n)) {
        sl.push_back(t.text());
      }
      results.push_back(csPdfSearchResult(textPage.pageNo(), index, sl));
    }
  } // For Each Text Page

  return results;
}
