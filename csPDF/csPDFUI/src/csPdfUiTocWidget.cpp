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

#include <csPDFium/csPDFiumDocument.h>
#include <csPDFium/csPDFiumContentsModel.h>

#include <csPDFUI/csPdfUiTocWidget.h>
#include "ui_csPdfUiTocWidget.h"

////// public ////////////////////////////////////////////////////////////////

csPdfUiTocWidget::csPdfUiTocWidget(QWidget *parent, Qt::WindowFlags f)
  : QWidget(parent, f)
  , ui(new Ui::csPdfUiTocWidget)
  , _contentsModel(0)
  , _doc()
{
  ui->setupUi(this);

  // Contents Model //////////////////////////////////////////////////////////

  _contentsModel = new csPDFiumContentsModel(ui->contentsView);
  ui->contentsView->setModel(_contentsModel);

  connect(ui->contentsView, &QTreeView::activated,
          this, &csPdfUiTocWidget::activateTocItem);
  connect(ui->contentsView, &QTreeView::clicked,
          this, &csPdfUiTocWidget::activateTocItem);

  // Filter //////////////////////////////////////////////////////////////////

  connect(ui->filterEdit, &QLineEdit::textChanged,
          _contentsModel, static_cast<void (csPDFiumContentsModel::*)(const QString&)>(&csPDFiumContentsModel::filter));
}

csPdfUiTocWidget::~csPdfUiTocWidget()
{
  delete ui;
}

void csPdfUiTocWidget::setDocument(const class csPDFiumDocument& doc)
{
  ui->filterEdit->clear();
  _contentsModel->setRootNode(doc.tableOfContents());
  _doc = doc;
}

////// private slots /////////////////////////////////////////////////////////

void csPdfUiTocWidget::activateTocItem(const QModelIndex& index)
{
  csPDFiumContentsNode *node = static_cast<csPDFiumContentsNode*>(index.internalPointer());
  if( node == 0 ) {
    return;
  }
  const csPDFiumDest dest = _doc.resolveBookmark(node->pointer());
  if( dest.isValid() ) {
    emit destinationRequested(dest);
  }
}
