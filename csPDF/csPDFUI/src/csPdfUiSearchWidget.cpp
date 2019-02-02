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

#include <QtCore/QThread>
#include <QtGui/QHelpEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMainWindow>

#include <csPDFSearch/csPdfSearch.h>
#include <csPDFSearch/csPdfSearchResultsModel.h>
#include <csPDFSearch/csPdfSearchUtil.h>
#include <csPDFium/csPDFiumUtil.h>
#include <csQt/csHighlightingDelegate.h>
#include <csQt/csImageTip.h>
#include <csQt/csQtUtil.h>

#include <csPDFUI/csPdfUiSearchWidget.h>
#include "ui_csPdfUiSearchWidget.h"

////// public ////////////////////////////////////////////////////////////////

csPdfUiSearchWidget::csPdfUiSearchWidget(QWidget *parent, Qt::WindowFlags f)
  : QWidget(parent, f)
  , ui(new Ui::csPdfUiSearchWidget)
  , _delegate(0)
  , _doc()
  , _results(0)
  , _search(0)
  , _thread(0)
  , _startIndex()
{
  qRegisterMetaType<csPdfSearchResults>("csPdfSearchResults");

  // Initialize UI ///////////////////////////////////////////////////////////

  ui->setupUi(this);
  ui->progressBar->setVisible(false);
  ui->cancelButton->setVisible(false);

  connect(ui->searchEdit, &QLineEdit::returnPressed, this, &csPdfUiSearchWidget::start);
  connect(ui->startButton, &QPushButton::clicked, this, &csPdfUiSearchWidget::start);
  connect(ui->cancelButton, &QPushButton::clicked, this, &csPdfUiSearchWidget::cancel);

  connect(ui->resultsView, &QTableView::activated,
          this, &csPdfUiSearchWidget::activateResult);
  connect(ui->resultsView, &QTableView::clicked,
          this, &csPdfUiSearchWidget::activateResult);

  // Highlighting Delegate ///////////////////////////////////////////////////

  _delegate = new csHighlightingDelegate(this);
  ui->resultsView->setItemDelegateForColumn(1, _delegate);

  // Initialize Search Utilities /////////////////////////////////////////////

  _results = new csPdfSearchResultsModel(this);
  ui->resultsView->setModel(_results);
  _thread  = new QThread(this);
  _search  = new csPdfSearch();
  _search->moveToThread(_thread);

  connect(_search, &csPdfSearch::finished, this, &csPdfUiSearchWidget::cancel);
  connect(_search, &csPdfSearch::found,
          _results, &csPdfSearchResultsModel::insertResults);
  connect(_search, &csPdfSearch::processed,
          ui->progressBar, &QProgressBar::setValue);
}

csPdfUiSearchWidget::~csPdfUiSearchWidget()
{
  cancel();
  delete _search;
  delete ui;
}

void csPdfUiSearchWidget::setDocument(const csPDFiumDocument& doc)
{
  cancel();
  clear();
  _doc = doc;
  _startIndex = 0;
}

////// public slots //////////////////////////////////////////////////////////

void csPdfUiSearchWidget::cancel()
{
  if( !_thread->isRunning() ) {
    return;
  }

  _search->cancel();
  while( _search->isRunning() );
  _thread->quit();
  _thread->wait();

  ui->startButton->setVisible(true);
  ui->cancelButton->setVisible(false);
  ui->searchEdit->setVisible(true);
  ui->progressBar->setVisible(false);
}

void csPdfUiSearchWidget::clear()
{
  _results->clear();
}

void csPdfUiSearchWidget::selectSearchText()
{
  ui->searchEdit->selectAll();
  ui->searchEdit->setFocus(Qt::OtherFocusReason);
}

void csPdfUiSearchWidget::setStartPage(int no)
{
  _startIndex = no-1;
}

void csPdfUiSearchWidget::start()
{
  if( _thread->isRunning() ) {
    return;
  }

  const QStringList needles = csPdfPrepareSearch(ui->searchEdit->text());
  if( needles.isEmpty()  ||  _doc.isEmpty() ) {
    return;
  }

  ui->startButton->setVisible(false);
  ui->cancelButton->setVisible(true);
  ui->searchEdit->setVisible(false);
  ui->progressBar->setVisible(true);
  clear();
  _delegate->setSubstring(ui->searchEdit->text());

  _thread->start();
  _search->start(_doc, needles, _startIndex, Qt::MatchWrap, 2);
}

////// private slots /////////////////////////////////////////////////////////

void csPdfUiSearchWidget::activateResult(const QModelIndex& index)
{
  if( !index.isValid() ) {
    return;
  }

  const QModelIndex pageIndex = _results->index(index.row(), 0);
  const int pageNo = _results->data(pageIndex, Qt::DisplayRole).toInt();
  if( pageNo > 0 ) {
    emit pageRequested(pageNo); // 1-based!
    emit highlightRequested(ui->searchEdit->text());
  }
}

////// protected /////////////////////////////////////////////////////////////

bool csPdfUiSearchWidget::event(QEvent *event)
{
  if( event->type() == QEvent::ToolTip ) {
    QHelpEvent *help = dynamic_cast<QHelpEvent*>(event);

    if( ui->resultsView->geometry().contains(help->pos()) ) {
      const QPoint pos = ui->resultsView->viewport()->mapFromGlobal(help->globalPos());
      const QModelIndex index = ui->resultsView->indexAt(pos);
      const QModelIndex pageIndex = _results->index(index.row(), 0);
      const int pageNo = _results->data(pageIndex, Qt::DisplayRole).toInt(); // 1-based!
      if( pageNo > 0 ) {
        // Image of Page
        const csPDFiumPage helpPage = _doc.page(pageNo-1);
        const qreal s = qMin((qreal)height() / helpPage.size().width(),
                             (qreal)height() / helpPage.size().height());
        const QImage helpImage = helpPage.renderToImage(s);
        // Flags
        csImageTip::Flags flags = csImageTip::DrawBorder;
        // Position
        QPoint tipPos = help->globalPos();
        const int helpPos = helpPosition(help->globalPos());
        if(        helpPos > 0 ) { // Right
          tipPos = mapToGlobal(QPoint()-QPoint(helpImage.width(), 0));
        } else if( helpPos < 0 ) { // Left
          tipPos = mapToGlobal(QPoint(width(), 0));
        }
        if( helpPos != 0 ) {
          flags |= csImageTip::ForcePosition;
        }
        // Tool Tip
        csImageTip::showImage(tipPos, helpImage, 0, flags);

        help->accept();
        return true;
      }
    }
  }

  return QWidget::event(event);
}

////// private ///////////////////////////////////////////////////////////////

int csPdfUiSearchWidget::helpPosition(const QPoint& globalPos)
{
  const QRect screenRect = csScreenGeometry(globalPos, this);
  if( !screenRect.isNull() ) {
    if( globalPos.x() < screenRect.center().x() ) {
      return -1; // Left
    } else {
      return 1; // Right
    }
  }
  return 0;
}
