/****************************************************************************
** Copyright (c) 2013-2014, Carsten Schmidt. All rights reserved.
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

#ifndef __CSPDFUISEARCHWIDGET_H__
#define __CSPDFUISEARCHWIDGET_H__

#include <QtWidgets/QWidget>

#include <csPDFUI/cspdfui_config.h>
#include <csPDFium/csPDFiumDocument.h>

namespace Ui {
  class csPdfUiSearchWidget;
};

class CS_PDFUI_EXPORT csPdfUiSearchWidget : public QWidget {
  Q_OBJECT
public:
  csPdfUiSearchWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
  ~csPdfUiSearchWidget();

  void setDocument(const csPDFiumDocument& doc);

public slots:
  void cancel();
  void clear();
  void selectSearchText();
  void setStartPage(int no);
  void start();

private slots:
  void activateResult(const QModelIndex& index);

protected:
  bool event(QEvent *event);

signals:
  void highlightRequested(const QString& text);
  void pageRequested(int no, bool history = true);

private:
  int helpPosition(const QPoint& globalPos);

  Ui::csPdfUiSearchWidget *ui;
  class csHighlightingDelegate *_delegate;
  csPDFiumDocument _doc;
  class csPdfSearchResultsModel *_results;
  class csPdfSearch *_search;
  class QThread *_thread;
  int _startIndex;
};

#endif // __CSPDFUISEARCHWIDGET_H__
