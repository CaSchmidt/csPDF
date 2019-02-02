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

#ifndef __CSPDFUIDOCUMENTVIEW_H__
#define __CSPDFUIDOCUMENTVIEW_H__

#include <QtCore/QStack>
#include <QtWidgets/QGraphicsView>

#include <csPDFUI/cspdfui_config.h>
#include <csPDFium/csPDFiumDocument.h>

struct csPdfUiDocumentViewConfig {
  csPdfUiDocumentViewConfig()
    : maxKeyBounces(1)
    , maxWheelBounces(1)
  {
  }

  int maxKeyBounces;
  int maxWheelBounces;
};

class  CS_PDFUI_EXPORT csPdfUiDocumentView : public QGraphicsView {
  Q_OBJECT
public:
  enum ItemId {
    PageId = 1,
    HighlightId,
    SelectionId,
    LinkId,
    UserId = 1000
  };

  enum ItemLayer {
    PageLayer      =    0,
    HighlightLayer =   10,
    SelectionLayer =   20,
    LinkLayer      =   30,
    UserLayer      = 1000
  };

  csPdfUiDocumentView(QWidget *parent);
  ~csPdfUiDocumentView();

  QString selectedText() const;

  const csPDFiumDocument& document() const;
  void setDocument(const csPDFiumDocument& doc);

  const csPDFiumPage& page() const;

  static void setItemId(QGraphicsItem *item, const int id);
  static int itemId(const QGraphicsItem *item);

  static void setConfig(const csPdfUiDocumentViewConfig& config);

  qreal zoom() const;

  static qreal zoomMin();
  static qreal zoomMax();
  static qreal zoomStep();

public slots:
  void gotoDestination(const csPDFiumDest& dest);
  void highlightText(const QString& text);
  void removeMarks();
  void reverseHistory();
  void showFirstPage();
  void showLastPage();
  void showNextPage();
  virtual void showPage(int no, bool updateHistory); // [1, _doc.pageCount()]
  void showPreviousPage();
  void setZoom(qreal level); // [%]
  void setZoomBestFit();
  void setZoomFitToPageWidth();
  void zoomIn();
  void zoomOut();

protected:
  bool eventFilter(QObject *watched, QEvent *event);
  void keyPressEvent(QKeyEvent *event);
  QList<QGraphicsItem*> listItems(const int id) const;
  void removeItems(const int id);
  void resizeEvent(QResizeEvent *event);
  void wheelEvent(QWheelEvent *event);

private slots:
  void selectArea(QRect rect, QPointF fromScene, QPointF toScene);

private:
  bool followLink(const QPointF& scenePos);
  bool isBottomTouched() const;
  bool isTopTouched() const;
  bool isVScrollRequired() const;
  void renderPage();
  bool setZoom(const qreal level, const int newMode);

protected:
  QGraphicsScene *_scene;
  csPDFiumDocument _doc;
  csPDFiumPage _page;

private:
  enum ZoomMode {
    ZoomUser = 0,
    ZoomBestFit,
    ZoomFitToPageWidth
  };

  struct PageHistory {
    PageHistory(const int _page = 0, const QPointF& _center = QPointF())
      : page(_page)
      , center(_center)
    {
    }

    int page;
    QPointF center;
  };

  qreal _zoom; // [%]
  int _zoomMode;
  int _keyBounces;
  int _wheelBounces;
  QStack<PageHistory> _history; // [1, _doc.pageCount()]
  static csPdfUiDocumentViewConfig _cfg;

signals:
  void pageChanged(int no); // [1, _doc.pageCount()]
  void remoteDocumentRequested(const QString& srcFilename,
                               const QString& destFilename);
  void zoomChanged(qreal zoom); // [%]
};

#endif // __CSPDFUIDOCUMENTVIEW_H__
