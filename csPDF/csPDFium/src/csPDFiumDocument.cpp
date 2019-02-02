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

#include <QtCore/QFile>

#include <csPDFium/csPDFiumDocument.h>

#include <csPDFium/csPDFiumContentsModel.h>

#include "internal/csPDFiumDocumentImpl.h"
#include "internal/csPDFiumPageImpl.h"
#include "internal/fpdf_util.h"

csPDFiumDocument::csPDFiumDocument()
  : impl()
{
}

csPDFiumDocument::~csPDFiumDocument()
{
}

bool csPDFiumDocument::isEmpty() const
{
  return impl.isNull();
}

bool csPDFiumDocument::isMemory() const
{
  if( isEmpty() ) {
    return false;
  }

  CSPDFIUM_DOCIMPL();

  return !impl->data.isEmpty();
}

void csPDFiumDocument::clear()
{
  if( isEmpty() ) {
    return;
  }

  // CAUTION: Upon on a successful lock we may delete 'mutex'!
  impl->mutex.lock();
  impl->mutex.unlock();

  impl.clear();
}

QString csPDFiumDocument::fileName() const
{
  if( isEmpty() ) {
    return QString();
  }

  CSPDFIUM_DOCIMPL();

  return impl->fileName;
}

int csPDFiumDocument::pageCount() const
{
  if( isEmpty() ) {
    return -1;
  }

  CSPDFIUM_DOCIMPL();

  return FPDF_GetPageCount(impl->document);
}

csPDFiumPage csPDFiumDocument::page(const int no) const
{
  if( isEmpty() ) {
    return csPDFiumPage();
  }

  CSPDFIUM_DOCIMPL();

  if( no < 0  ||  no >= FPDF_GetPageCount(impl->document) ) {
    return csPDFiumPage();
  }

  csPDFiumPageImpl *pimpl = new csPDFiumPageImpl();
  if( pimpl == 0 ) {
    return csPDFiumPage();
  }

  pimpl->page = FPDF_LoadPage(impl->document, no);
  if( pimpl->page == NULL ) {
    delete pimpl;
    return csPDFiumPage();
  }

  pimpl->ctm = util::getPageCTM(pimpl->page);
  pimpl->doc = impl;
  pimpl->no  = no;

  csPDFiumPage page;
  page.impl = QSharedPointer<csPDFiumPageImpl>(pimpl);

  return page;
}

csPDFiumContentsNode *csPDFiumDocument::tableOfContents() const
{
  if( isEmpty() ) {
    return 0;
  }

  CSPDFIUM_DOCIMPL();

  csPDFiumContentsNode *root = csPDFiumContentsModel::newRootNode();
  if( root != 0 ) {
    util::parseContents(FPDFBookmark_GetFirstChild(impl->document, NULL),
                        impl->document, root);
  }

  return root;
}

csPDFiumTextPage csPDFiumDocument::textPage(const int no) const
{
  if( isEmpty() ) {
    return csPDFiumTextPage();
  }

  CSPDFIUM_DOCIMPL();

  if( no < 0  ||  no >= FPDF_GetPageCount(impl->document) ) {
    return csPDFiumTextPage();
  }

  const FPDF_PAGE page = FPDF_LoadPage(impl->document, no);
  if( page == NULL ) {
    return csPDFiumTextPage();
  }

  const QMatrix ctm = util::getPageCTM(page);

  const csPDFiumTextPage result(no, util::extractTexts(page, ctm));

  FPDF_ClosePage(page);

  return result;
}

csPDFiumTextPages csPDFiumDocument::textPages(const int first, const int count) const
{
  if( isEmpty() ) {
    return csPDFiumTextPages();
  }

  CSPDFIUM_DOCIMPL();

  const int pageCount = FPDF_GetPageCount(impl->document);
  if( first < 0  ||  first >= pageCount ) {
    return csPDFiumTextPages();
  }

  const int last = count <= 0
      ? pageCount-1
      : qBound(0, first+count-1, pageCount-1);

  csPDFiumTextPages results;
  for(int pageNo = first; pageNo <= last; pageNo++) {
    const FPDF_PAGE page = FPDF_LoadPage(impl->document, pageNo);
    if( page == NULL ) {
      continue;
    }

    const QMatrix ctm = util::getPageCTM(page);

    results.push_back(csPDFiumTextPage(pageNo,
                                       util::extractTexts(page, ctm)));

    FPDF_ClosePage(page);
  }

  return results;
}

csPDFiumDest csPDFiumDocument::resolveBookmark(const void *pointer) const
{
  if( isEmpty() ) {
    return csPDFiumDest();
  }

  CSPDFIUM_DOCIMPL();

  if( pointer == 0 ) {
    return csPDFiumDest();
  }

  const FPDF_BOOKMARK bookmark = (const FPDF_BOOKMARK)pointer;
  const FPDF_DEST         dest = FPDFBookmark_GetDest(impl->document, bookmark);

  return createDest(dest, FPDFBookmark_GetAction(bookmark));
}

csPDFiumDest csPDFiumDocument::resolveLink(const void *pointer) const
{
  if( isEmpty() ) {
    return csPDFiumDest();
  }

  CSPDFIUM_DOCIMPL();

  if( pointer == 0 ) {
    return csPDFiumDest();
  }

  const FPDF_LINK link = (const FPDF_LINK)pointer;
  const FPDF_DEST dest = FPDFLink_GetDest(impl->document, link);

  return createDest(dest, FPDFLink_GetAction(link));
}

csPDFiumWordsPages csPDFiumDocument::wordsPages(const int firstIndex,
                                                const int count) const
{
  if( isEmpty() ) {
    return csPDFiumWordsPages();
  }

  CSPDFIUM_DOCIMPL();

  const int pageCount = FPDF_GetPageCount(impl->document);
  if( firstIndex < 0  ||  firstIndex >= pageCount ) {
    csPDFiumWordsPages();
  }

  const int lastIndex = count <= 0
      ? pageCount-1
      : qBound(0, firstIndex+count-1, pageCount-1);

  csPDFiumWordsPages result;
  for(int index = firstIndex; index <= lastIndex; index++) {
    const FPDF_PAGE page = FPDF_LoadPage(impl->document, index);
    if( page == NULL ) {
      continue;
    }

    const csPDFiumWordsPage wordsPage(index, util::extractWords(page));
    if( !wordsPage.second.isEmpty() ) {
      result.push_back(wordsPage);
    }

    FPDF_ClosePage(page);
  }

  return result;
}

csPDFiumDocument csPDFiumDocument::load(const QString& filename,
                                        const bool memory,
                                        const QByteArray& password,
                                        bool *pw_required)
{
  csPDFiumDocumentImpl *impl = new csPDFiumDocumentImpl();
  if( impl == 0 ) {
    return csPDFiumDocument();
  }

  if( pw_required != nullptr ) {
    *pw_required = false;
  }
  const char *pdf_password = password.isEmpty()
      ? NULL
      : password.constData();

  impl->fileName = filename;
  if( memory ) {
    QFile file(filename);
    if( !file.open(QIODevice::ReadOnly) ) {
      delete impl;
      return csPDFiumDocument();
    }

    impl->data = file.readAll();
    if( impl->data.size() != file.size() ) {
      file.close();
      delete impl;
      return csPDFiumDocument();
    }
    file.close();

    impl->document = FPDF_LoadMemDocument(impl->data.constData(),
                                          impl->data.size(), pdf_password);

  } else {
#ifndef Q_OS_WIN // ASSUMPTION: All other OSes treat paths as UTF-8...
    impl->document = FPDF_LoadDocument(filename.toUtf8().constData(), pdf_password);
#else
    // NOTE: PDFium uses UTF-16LE encoding!
    impl->document = FPDF_LoadDocumentW(filename.utf16(), pdf_password);
#endif
  }

  if( impl->document == NULL ) {
    if( pw_required != nullptr ) {
      *pw_required = FPDF_GetLastError() == FPDF_ERR_PASSWORD;
    }

    delete impl;
    return csPDFiumDocument();
  }

  csPDFiumDocument doc;
  doc.impl = QSharedPointer<csPDFiumDocumentImpl>(impl);

  return doc;
}

////// private ///////////////////////////////////////////////////////////////

csPDFiumDest csPDFiumDocument::createDest(const void *_dest, const void *_action) const
{
  FPDF_DEST           dest = (FPDF_DEST)_dest;
  const FPDF_ACTION action = (const FPDF_ACTION)_action;

  if( action != NULL ) {
    if(        FPDFAction_GetType(action) == PDFACTION_GOTO  &&  dest == NULL ) {
      dest = FPDFAction_GetDest(impl->document, action);
    } else if( FPDFAction_GetType(action) == PDFACTION_REMOTEGOTO ) {
      const int size = FPDFAction_GetFilePath(action, NULL, 0);
      if( size < 1 ) {
        return csPDFiumDest();
      }
      QByteArray destFilename(size, '\0');
      FPDFAction_GetFilePath(action, destFilename.data(), destFilename.size());
      return csPDFiumDest(impl->fileName, QString::fromUtf8(destFilename));
    }
  }

  if( dest == NULL ) {
    return csPDFiumDest();
  }

  return csPDFiumDest(FPDFDest_GetPageIndex(impl->document, dest),
                      FPDFDest_GetZoomMode(dest) == FPDF_ZOOM_XYZ
                      ? QPointF(FPDFDest_GetZoomParam(dest, 0),
                                FPDFDest_GetZoomParam(dest, 1))
                      : QPointF());
}
