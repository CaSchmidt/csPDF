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

#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QActionGroup>
#include <QtWidgets/QFileDialog>

#include <csPDFium/csPDFiumDocument.h>
#include <csPDFium/csPDFiumContentsModel.h>

#include "wmainwindow.h"
#include "ui_wmainwindow.h"

////// public ////////////////////////////////////////////////////////////////

WMainWindow::WMainWindow(QWidget *parent, Qt::WindowFlags flags)
  : QMainWindow(parent, flags),
    ui(new Ui::WMainWindow)
{
  ui->setupUi(this);

  // Drag & Drop /////////////////////////////////////////////////////////////

  setAcceptDrops(true);
  ui->pdfView->setAcceptDrops(true);
  ui->pdfView->installEventFilter(this);

  // Document View ///////////////////////////////////////////////////////////

  connect(ui->pdfView, &csPdfUiDocumentView::pageChanged,
          ui->searchWidget, &csPdfUiSearchWidget::setStartPage);

  // Quick Search ////////////////////////////////////////////////////////////

  ui->pdfView->installEventFilter(ui->quickSearchWidget);
  ui->quickSearchWidget->hide();

  connect(ui->quickSearchWidget, &WQuickSearch::searchTextEdited,
          ui->pdfView, &csPdfUiDocumentView::highlightText);

  // Table of Contents ///////////////////////////////////////////////////////

  connect(ui->contentsWidget, &csPdfUiTocWidget::destinationRequested,
          ui->pdfView, &csPdfUiDocumentView::gotoDestination);

  connect(ui->showTocAction, &QAction::triggered,
          ui->contentsDock, &QDockWidget::show);

  // Document Search /////////////////////////////////////////////////////////

  connect(ui->showSearchAction, &QAction::triggered,
          ui->searchDock, &QDockWidget::show);
  connect(ui->showSearchAction, &QAction::triggered,
          ui->searchWidget, &csPdfUiSearchWidget::selectSearchText);
  connect(ui->searchWidget, &csPdfUiSearchWidget::pageRequested,
          ui->pdfView, &csPdfUiDocumentView::showPage);
  connect(ui->searchWidget, &csPdfUiSearchWidget::highlightRequested,
          ui->pdfView, &csPdfUiDocumentView::highlightText);

  // Edit Mode ///////////////////////////////////////////////////////////////

  QActionGroup *modeGroup = new QActionGroup(this);
  modeGroup->addAction(ui->handToolAction);
  modeGroup->addAction(ui->rectangleToolAction);
  ui->handToolAction->setChecked(true);
  setEditMode(false);

  connect(ui->handToolAction, &QAction::triggered,
          this, &WMainWindow::setEditMode);
  connect(ui->rectangleToolAction, &QAction::triggered,
          this, &WMainWindow::setEditMode);


  // Signals & Slots /////////////////////////////////////////////////////////

  connect(ui->openAction, &QAction::triggered, this, static_cast<void (WMainWindow::*)()>(&WMainWindow::openFile));
  connect(ui->quitAction, &QAction::triggered, this, &WMainWindow::close);

  connect(ui->copyAction, &QAction::triggered, this, &WMainWindow::copySelection);

  connect(ui->zoomBestFitAction, &QAction::triggered,
          ui->pdfView, &csPdfUiDocumentView::setZoomBestFit);
  connect(ui->zoomFitToPageWidthAction, &QAction::triggered,
          ui->pdfView, &csPdfUiDocumentView::setZoomFitToPageWidth);
}

WMainWindow::~WMainWindow()
{
  delete ui;
}

////// private slots /////////////////////////////////////////////////////////

void WMainWindow::copySelection()
{
  const QString s = ui->pdfView->selectedText();
  if( s.isEmpty() ) {
    QApplication::clipboard()->clear();
    return;
  }

  QApplication::clipboard()->setText(s);
}

void WMainWindow::openFile()
{
  const QString filename =
      QFileDialog::getOpenFileName(this,
                                   tr("Open"),
                                   QString(),
                                   tr("Adobe PDF files (*.pdf)"));
  if( filename.isEmpty() ) {
    return;
  }

  openFile(filename);
}

void WMainWindow::setEditMode(bool)
{
  if( ui->handToolAction->isChecked() ) {
    ui->pdfView->setDragMode(QGraphicsView::ScrollHandDrag);
  } else if( ui->rectangleToolAction->isChecked() ) {
    ui->pdfView->setDragMode(QGraphicsView::RubberBandDrag);
  }
}

////// protected /////////////////////////////////////////////////////////////

void WMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
  if( event->mimeData()->hasUrls()  &&
      event->mimeData()->urls().size() == 1  &&
      event->mimeData()->urls().front().toLocalFile().endsWith(QStringLiteral(".pdf"), Qt::CaseInsensitive) ) {
    event->setDropAction(Qt::CopyAction);
    event->acceptProposedAction();
  }
}

void WMainWindow::dropEvent(QDropEvent *event)
{
  openFile(event->mimeData()->urls().front().toLocalFile());
  event->acceptProposedAction();
}

bool WMainWindow::eventFilter(QObject *watched, QEvent *event)
{
  if( watched == ui->pdfView ) {
    if(        event->type() == QEvent::DragEnter ) {
      dragEnterEvent(dynamic_cast<QDragEnterEvent*>(event));
      return event->isAccepted();
    } else if( event->type() == QEvent::Drop ) {
      dropEvent(dynamic_cast<QDropEvent*>(event));
      return event->isAccepted();
    }
  }

  return QMainWindow::eventFilter(watched, event);
}

void WMainWindow::keyPressEvent(QKeyEvent *event)
{
  if( event->key() == Qt::Key_Escape ) {
    if( ui->quickSearchWidget->isVisible() ) {
      ui->quickSearchWidget->hide();
    } else {
      ui->pdfView->removeMarks();
    }
    // setFocus(Qt::OtherFocusReason);
    event->accept();
  }
}

////// private ///////////////////////////////////////////////////////////////

void WMainWindow::openFile(const QString& filename)
{
  csPDFiumDocument doc = csPDFiumDocument::load(filename);
  ui->pdfView->setDocument(doc);
  ui->contentsWidget->setDocument(doc);
  ui->searchWidget->setDocument(doc);
}
