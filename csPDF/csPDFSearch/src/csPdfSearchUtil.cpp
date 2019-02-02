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

#include <QStringList>

#include <csPDFSearch/csPdfSearchUtil.h>

#include <csPDFium/csPDFiumUtil.h>

CS_PDFSEARCH_EXPORT int csPdfFind(const csPDFiumTexts& hay,
                                  const QString& needle,
                                  const int position,
                                  const Qt::CaseSensitivity cs)
{
  if( hay.isEmpty()  ||  needle.isEmpty()  ||
      position < 0  ||  position >= hay.size() ) {
    return -1;
  }

  for(int i = position; i < hay.size(); i++) {
    if( hay[i].text().contains(needle, cs) ) {
      return i;
    }
  }

  return -1;
}

CS_PDFSEARCH_EXPORT csPdfFindResults csPdfFindAll(const csPDFiumTexts& hay,
                                                  const QString& needle,
                                                  const Qt::CaseSensitivity cs)
{
  if( hay.isEmpty()  ||  needle.isEmpty() ) {
    return csPdfFindResults();
  }

  csPdfFindResults results;
  for(int i = 0; i < hay.size(); i++) {
    if( hay[i].text().contains(needle, cs) ) {
      results.push_back(i);
    }
  }

  return results;
}

CS_PDFSEARCH_EXPORT int csPdfFind(const csPDFiumTexts& hay,
                                  const QStringList& needles,
                                  const int position,
                                  const Qt::CaseSensitivity cs)
{
  if( hay.isEmpty()  ||  needles.isEmpty()  ||
      needles.size() < 2  ||  needles.size() > hay.size()  ||
      position < 0  ||  position > (hay.size()-needles.size()) ) {
    return -1;
  }

  for(int i = position; i <= hay.size()-needles.size(); i++) {
    int hits(0);

    if( hay[i].text().endsWith(needles.front(), cs) ) {
      hits++;
    } else {
      continue;
    }

    for(int j = 1; j < needles.size()-1; j++) {
      if( hay[i+j].text().compare(needles[j], cs) == 0 ) {
        hits++;
      } else {
        break;
      }
    }

    if( hay[i+needles.size()-1].text().startsWith(needles.back(), cs) ) {
      hits++;
    } else {
      continue;
    }

    if( hits == needles.size() ) {
      return i;
    }
  }

  return -1;
}

CS_PDFSEARCH_EXPORT csPdfFindResults csPdfFindAll(const csPDFiumTexts& hay,
                                                  const QStringList& needles,
                                                  const Qt::CaseSensitivity cs)
{
  if( hay.isEmpty()  ||  needles.isEmpty()  ||
      needles.size() < 2  ||  needles.size() > hay.size() ) {
    return csPdfFindResults();
  }

  csPdfFindResults results;
  for(int i = 0; i <= hay.size()-needles.size(); i++) {
    int hits(0);

    if( hay[i].text().endsWith(needles.front(), cs) ) {
      hits++;
    } else {
      continue;
    }

    for(int j = 1; j < needles.size()-1; j++) {
      if( hay[i+j].text().compare(needles[j], cs) == 0 ) {
        hits++;
      } else {
        break;
      }
    }

    if( hay[i+needles.size()-1].text().startsWith(needles.back(), cs) ) {
      hits++;
    } else {
      continue;
    }

    if( hits == needles.size() ) {
      results.push_back(i);
    }
  }

  return results;
}

CS_PDFSEARCH_EXPORT QStringList csPdfPrepareSearch(const QString& text)
{
  return text.split(QRegExp(_L1("\\s+")), QString::SkipEmptyParts);
}
