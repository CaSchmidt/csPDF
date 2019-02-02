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

#include <cmath>

#include <QtCore/QCoreApplication>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QScrollBar>

#include <csPDFUI/csPdfUiDocumentView.h>

#include <csPDFium/csPDFiumUtil.h>
#include <csPDFSearch/csPdfSearchUtil.h>

////// Macros ////////////////////////////////////////////////////////////////

// Item Data
#define DATA_ID             0
#define DATA_LINKPOINTER    1
#define DATA_SELECTIONTEXT  2
#define DATA_SELECTIONPOS   3

// Zoom

#define ZOOM_INIT  100.0
#define ZOOM_MIN    10.0
#define ZOOM_MAX  1000.0
#define ZOOM_STEP   20.0

#define _SCALE  (_zoom/100.0)

////// Private ///////////////////////////////////////////////////////////////

namespace priv {

  static QGraphicsItem *addHighlight(QGraphicsScene *scene, const QRectF& rect)
  {
    QColor hlColor(Qt::yellow);
    hlColor.setAlphaF(0.4);
    QGraphicsItem *item = scene->addRect(rect,
                                         QPen(Qt::NoPen),
                                         QBrush(hlColor, Qt::SolidPattern));
    item->setZValue(csPdfUiDocumentView::HighlightLayer);
    csPdfUiDocumentView::setItemId(item, csPdfUiDocumentView::HighlightId);

    return item;
  }

  static QGraphicsItem *addLink(QGraphicsScene *scene, const csPDFiumLink& link)
  {
    QGraphicsItem *item = scene->addRect(link.srcRect());
    item->setFlag(QGraphicsItem::ItemHasNoContents, true);
    item->setCursor(Qt::PointingHandCursor);
    item->setZValue(csPdfUiDocumentView::LinkLayer);
    csPdfUiDocumentView::setItemId(item, csPdfUiDocumentView::LinkId);
    item->setData(DATA_LINKPOINTER, QVariant::fromValue(const_cast<void*>(link.pointer())));

    return item;
  }

  static QGraphicsItem *addSelection(QGraphicsScene *scene, const csPDFiumText& pdfText)
  {
    QColor selColor(Qt::blue);
    selColor.setAlphaF(0.4);
    QGraphicsItem *item = scene->addRect(pdfText.rect(),
                                         QPen(Qt::NoPen),
                                         QBrush(selColor, Qt::SolidPattern));
    item->setZValue(csPdfUiDocumentView::SelectionLayer);
    csPdfUiDocumentView::setItemId(item, csPdfUiDocumentView::SelectionId);
    item->setData(DATA_SELECTIONTEXT, pdfText.text());
    item->setData(DATA_SELECTIONPOS,  pdfText.pos());

    return item;
  }

} // namespace priv

////// Static ////////////////////////////////////////////////////////////////

csPdfUiDocumentViewConfig csPdfUiDocumentView::_cfg;

////// public ////////////////////////////////////////////////////////////////

csPdfUiDocumentView::csPdfUiDocumentView(QWidget *parent)
  : QGraphicsView(parent)
  , _scene(0)
  , _doc()
  , _page()
  , _zoom(ZOOM_INIT)
  , _zoomMode(ZoomUser)
  , _keyBounces(0)
  , _wheelBounces(0)
  , _history()
{
  qRegisterMetaType<csPDFiumDest>("csPDFiumDest");

  // Graphics Scene //////////////////////////////////////////////////////////

  _scene = new QGraphicsScene(this);
  _scene->setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
  setScene(_scene);

  // Default Drag Mode ///////////////////////////////////////////////////////

  setDragMode(ScrollHandDrag);

  // Event Filter ////////////////////////////////////////////////////////////

  _scene->installEventFilter(this);

  // Signals & Slots /////////////////////////////////////////////////////////

  connect(this, &csPdfUiDocumentView::rubberBandChanged,
          this, &csPdfUiDocumentView::selectArea);
}

csPdfUiDocumentView::~csPdfUiDocumentView()
{
}

QString csPdfUiDocumentView::selectedText() const
{
  csPDFiumTexts texts;
  foreach(QGraphicsItem *item, _scene->items()) {
    if( itemId(item) == SelectionId ) {
      QGraphicsRectItem *ri = dynamic_cast<QGraphicsRectItem*>(item);
      texts.push_back(csPDFiumText(ri->rect(),
                                   item->data(DATA_SELECTIONTEXT).toString(),
                                   item->data(DATA_SELECTIONPOS).toInt()));
    }
  }

  if( texts.isEmpty() ) {
    return QString();
  }

  qSort(texts);

  QString text(texts[0].text());
  for(int i = 1; i < texts.size(); i++) {
    if( csPDFium::overlapsV(texts[i-1].rect(), texts[i].rect()) ) {
      text += _L1C(' ');
    } else {
      text += _L1C('\n');
    }
    text += texts[i].text();
  }

  return text;
}

const csPDFiumDocument& csPdfUiDocumentView::document() const
{
  return _doc;
}

void csPdfUiDocumentView::setDocument(const csPDFiumDocument& doc)
{
  _scene->clear();
  _doc.clear();
  _page.clear();
  _history.clear();

  _doc = doc;

  resetTransform();
  _zoom = ZOOM_INIT;
  emit zoomChanged(_zoom);

  showFirstPage();
}

const csPDFiumPage& csPdfUiDocumentView::page() const
{
  return _page;
}

void csPdfUiDocumentView::setItemId(QGraphicsItem *item, const int id)
{
  item->setData(DATA_ID, id);
}

int csPdfUiDocumentView::itemId(const QGraphicsItem *item)
{
  return item->data(DATA_ID).toInt();
}

void csPdfUiDocumentView::setConfig(const csPdfUiDocumentViewConfig& config)
{
  _cfg = config;
}

qreal csPdfUiDocumentView::zoom() const
{
  return _zoom;
}

qreal csPdfUiDocumentView::zoomMin()
{
  return ZOOM_MIN;
}

qreal csPdfUiDocumentView::zoomMax()
{
  return ZOOM_MAX;
}

qreal csPdfUiDocumentView::zoomStep()
{
  return ZOOM_STEP;
}

////// public slots //////////////////////////////////////////////////////////

void csPdfUiDocumentView::gotoDestination(const csPDFiumDest& dest)
{
  if( !dest.isValid() ) {
    return;
  }

  if(        dest.type() == csPDFiumDest::Goto ) {
    showPage(dest.pageIndex+1, true);
    if( dest.focusPosPage.isNull() ) {
      centerOn(_page.rect().center().x(), _page.rect().top());
    } else {
      centerOn(_page.mapToScene(dest.focusPosPage));
    }
  } else if( dest.type() == csPDFiumDest::RemoteGoto ) {
    if( !dest.srcFilename.isEmpty()  &&  !dest.destFilename.isEmpty() ) {
      emit remoteDocumentRequested(dest.srcFilename, dest.destFilename);
    }
  }
}

void csPdfUiDocumentView::highlightText(const QString& text)
{
  removeItems(HighlightId);

  if( _page.isEmpty() ) {
    return;
  }

  const csPDFiumTexts texts = _page.texts();
  const QStringList needles = csPdfPrepareSearch(text);
  if( needles.size() == 1 ) {
    foreach(const int pos,
            csPdfFindAll(texts, needles.front(), Qt::CaseInsensitive)) {
      priv::addHighlight(_scene, texts[pos].rect());
    }
  } else if( needles.size() > 1 ) {
    QList<QRectF> hlBoxes;
    foreach(const int pos,
            csPdfFindAll(texts, needles, Qt::CaseInsensitive)) {
      for(int i = 0; i < needles.size(); i++) {
        if( !hlBoxes.contains(texts[pos+i].rect()) ) {
          hlBoxes.push_back(texts[pos+i].rect());
        }
      }
    }
    foreach(const QRectF hlBox, hlBoxes) {
      priv::addHighlight(_scene, hlBox);
    }
  }
}

void csPdfUiDocumentView::removeMarks()
{
  removeItems(HighlightId);
  removeItems(SelectionId);
}

void csPdfUiDocumentView::reverseHistory()
{
  if( !_history.isEmpty() ) {
    const PageHistory back = _history.pop();
    showPage(back.page, false);
    centerOn(back.center);
  }
}

void csPdfUiDocumentView::showFirstPage()
{
  showPage(1, false);
}

void csPdfUiDocumentView::showLastPage()
{
  showPage(_doc.pageCount(), false);
}

void csPdfUiDocumentView::showNextPage()
{
  if( _doc.isEmpty()  ||  _page.isEmpty() ) {
    emit pageChanged(1);
    return;
  }

  const int oldNo = _page.number();
  const int newNo = qBound(0, oldNo+1, _doc.pageCount()-1);
  showPage(newNo+1, false);

  if( oldNo != newNo ) {
    centerOn(_scene->sceneRect().center().x(), _scene->sceneRect().top());
  }
}

void csPdfUiDocumentView::showPage(int no, bool updateHistory)
{
  _keyBounces = 0;
  _wheelBounces = 0;

  if( _doc.isEmpty() ) {
    emit pageChanged(1);
    return;
  }

  if( !_page.isEmpty()  &&  updateHistory ) {
    _history.push(PageHistory(_page.number()+1,
                              mapToScene(viewport()->rect().center())));
  }

  if( !_page.isEmpty()  &&  _page.number() == no-1 ) {
    return;
  }

  _scene->clear();

  const int pageNo = qBound(0, no-1, _doc.pageCount()-1); // 0-based
  _page = _doc.page(pageNo);
  setZoom(_zoom, _zoomMode);
  renderPage();

  foreach(const csPDFiumLink link, _page.links()) {
    if( !link.isEmpty() ) {
      priv::addLink(_scene, link);
    }
  }

  emit pageChanged(pageNo+1);
}

void csPdfUiDocumentView::showPreviousPage()
{
  if( _doc.isEmpty()  ||  _page.isEmpty() ) {
    emit pageChanged(1);
    return;
  }

  const int oldNo = _page.number();
  const int newNo = qBound(0, oldNo-1, _doc.pageCount()-1);
  showPage(newNo+1, false);

  if( oldNo != newNo ) {
    centerOn(_scene->sceneRect().center().x(), _scene->sceneRect().bottom());
  }
}

void csPdfUiDocumentView::setZoom(qreal level)
{
  if( setZoom(level, ZoomUser) ) {
    renderPage();
  }
}

void csPdfUiDocumentView::setZoomBestFit()
{
  if( setZoom(_zoom, ZoomBestFit) ) {
    renderPage();
  }
}

void csPdfUiDocumentView::setZoomFitToPageWidth()
{
  if( setZoom(_zoom, ZoomFitToPageWidth) ) {
    renderPage();
  }
}

void csPdfUiDocumentView::zoomIn()
{
  setZoom(_zoom+ZOOM_STEP);
}

void csPdfUiDocumentView::zoomOut()
{
  setZoom(_zoom-ZOOM_STEP);
}

////// protected /////////////////////////////////////////////////////////////

bool csPdfUiDocumentView::eventFilter(QObject *watched, QEvent *event)
{
  if(        event->type() == QEvent::GraphicsSceneMousePress ) {
    QGraphicsSceneMouseEvent *mev =
        dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if( mev->modifiers() == Qt::NoModifier  &&
        mev->buttons()   ==  Qt::LeftButton &&
        followLink(mev->scenePos()) ) {
      return true;
    }
  }

  return QGraphicsView::eventFilter(watched, event);
}

void csPdfUiDocumentView::keyPressEvent(QKeyEvent *event)
{
  // Conditions & State
  const bool condVScrollRequired = isVScrollRequired();

  // Events
  if(        event->matches(QKeySequence::MoveToStartOfDocument) ) {
    showFirstPage();
    event->accept();
    return;
  } else if( event->matches(QKeySequence::MoveToEndOfDocument) ) {
    showLastPage();
    event->accept();
    return;
  } else if( event->matches(QKeySequence::MoveToNextPage) ) {
    showNextPage();
    event->accept();
    return;
  } else if( event->matches(QKeySequence::MoveToPreviousPage) ) {
    showPreviousPage();
    event->accept();
    return;
  } else if( event->matches(QKeySequence::ZoomIn)  ||
             (event->modifiers() == Qt::ControlModifier  &&
              event->key() == Qt::Key_BracketRight) ) {
    zoomIn();
    event->accept();
    return;
  } else if( event->matches(QKeySequence::ZoomOut) ) {
    zoomOut();
    event->accept();
    return;
  } else if( event->matches(QKeySequence::Back) ) {
    reverseHistory();
    event->accept();
    return;
  } else if( event->matches(QKeySequence::MoveToPreviousLine) ) {
    if( !condVScrollRequired ) {
      showPreviousPage();
    } else {
      if( isTopTouched() ) {
        if( _keyBounces > 0 ) {
          _keyBounces = -1;
        } else {
          _keyBounces--;
        }
        if( qAbs(_keyBounces) > _cfg.maxKeyBounces ) {
          showPreviousPage();
          _keyBounces = 0;
          return;
        }
      } else {
        _keyBounces = 0;
      }
    }
    // NOTE: We do not return to let the event be processed by QGraphicsView!
  } else if( event->matches(QKeySequence::MoveToNextLine) ) {
    if( !condVScrollRequired ) {
      showNextPage();
    } else {
      if( isBottomTouched() ) {
        if( _keyBounces < 0 ) {
          _keyBounces = 1;
        } else {
          _keyBounces++;
        }
        if( qAbs(_keyBounces) > _cfg.maxKeyBounces ) {
          showNextPage();
          _keyBounces = 0;
          return;
        }
      } else {
        _keyBounces = 0;
      }
    }
    // NOTE: We do not return to let the event be processed by QGraphicsView!
  }

  QGraphicsView::keyPressEvent(event);
}

QList<QGraphicsItem*> csPdfUiDocumentView::listItems(const int id) const
{
  QList<QGraphicsItem*> found;
  foreach(QGraphicsItem *item, _scene->items()) {
    if( itemId(item) == id ) {
      found.push_back(item);
    }
  }
  return found;
}

void csPdfUiDocumentView::removeItems(const int id)
{
  foreach(QGraphicsItem *item, _scene->items()) {
    if( itemId(item) == id ) {
      _scene->removeItem(item);
      delete item;
    }
  }
}

void csPdfUiDocumentView::resizeEvent(QResizeEvent *event)
{
  if( _zoomMode == ZoomBestFit  ||  _zoomMode == ZoomFitToPageWidth ) {
    if( setZoom(_zoom, _zoomMode) ) {
      renderPage();
    }
  }

  QGraphicsView::resizeEvent(event);
}

void csPdfUiDocumentView::wheelEvent(QWheelEvent *event)
{
  // Conditions
  const bool condVScrollRequired = isVScrollRequired();
  const bool condPageChange
      = (event->modifiers() == Qt::NoModifier  &&  !condVScrollRequired)
      || event->modifiers() == Qt::ShiftModifier;
  const bool condZoom
      = event->modifiers() == Qt::ControlModifier;
  const bool condPageBounce
      = event->modifiers() == Qt::NoModifier  &&  condVScrollRequired;

  // dy > 0: Away From User; dy < 0: Toward User
  const int dy = event->angleDelta().y() / 8; // [Degree]

  if(        condPageChange ) {
    if(        dy < 0 ) {
      showNextPage();
      event->accept();
      return;
    } else if( dy > 0 ) {
      showPreviousPage();
      event->accept();
      return;
    }

  } else if( condZoom ) {
    if(        dy < 0 ) {
      zoomOut();
      event->accept();
      return;
    } else if( dy > 0 ) {
      zoomIn();
      event->accept();
      return;
    }

  } else if( condPageBounce ) {
    if(        isTopTouched()  &&  dy > 0 ) {
      if( _wheelBounces > 0 ) {
        _wheelBounces = -1;
      } else {
        _wheelBounces--;
      }
      if( qAbs(_wheelBounces) > _cfg.maxWheelBounces ) {
        showPreviousPage();
        _wheelBounces = 0;
        return;
      }

    } else if( isBottomTouched()  &&  dy < 0 ) {
      if( _wheelBounces < 0 ) {
        _wheelBounces = 1;
      } else {
        _wheelBounces++;
      }
      if( qAbs(_wheelBounces) > _cfg.maxWheelBounces ) {
        showNextPage();
        _wheelBounces = 0;
        return;
      }

    } else {
      _wheelBounces = 0;
    }
  }

  QGraphicsView::wheelEvent(event);
}

////// private slots /////////////////////////////////////////////////////////

void csPdfUiDocumentView::selectArea(QRect rect, QPointF fromScene, QPointF toScene)
{
  if( _page.isEmpty()  ||  rect.isEmpty() ) {
    return;
  }

  const qreal x = qMin(fromScene.x(), toScene.x());
  const qreal y = qMin(fromScene.y(), toScene.y());
  const qreal w = qAbs(fromScene.x() - toScene.x());
  const qreal h = qAbs(fromScene.y() - toScene.y());

  removeItems(SelectionId);
  foreach(const csPDFiumText t, _page.texts(QRectF(x, y, w, h))) {
    priv::addSelection(_scene, t);
  }
}

////// private ///////////////////////////////////////////////////////////////

bool csPdfUiDocumentView::followLink(const QPointF& scenePos)
{
  foreach (QGraphicsItem *item, _scene->items(scenePos)) {
    if( item == 0  ||  itemId(item) != LinkId ) {
      continue;
    }

    const void     *pointer = item->data(DATA_LINKPOINTER).value<void*>();
    const csPDFiumDest dest = _doc.resolveLink(pointer);
    if( !dest.isValid() ) {
      return false;
    }

    gotoDestination(dest);

    return true;
  }

  return false;
}

bool csPdfUiDocumentView::isBottomTouched() const
{
  const QRectF view = mapToScene(viewport()->rect()).boundingRect();
  return std::ceil(view.bottom()) >= std::floor(sceneRect().bottom());
}

bool csPdfUiDocumentView::isTopTouched() const
{
  const QRectF view = mapToScene(viewport()->rect()).boundingRect();
  return std::floor(view.top()) <= std::ceil(sceneRect().top());
}

bool csPdfUiDocumentView::isVScrollRequired() const
{
  const int height = mapFromScene(sceneRect()).boundingRect().height();
  return height > viewport()->height();
}

void csPdfUiDocumentView::renderPage()
{
  removeItems(PageId);

  if( _page.isEmpty() ) {
    return;
  }

  const QImage image = _page.renderToImage(_SCALE);
  QGraphicsItem *item = _scene->addPixmap(QPixmap::fromImage(image));
  item->setTransform(QTransform::fromScale(1.0/_SCALE, 1.0/_SCALE));
  item->setZValue(PageLayer);
  setItemId(item, PageId);

  setSceneRect(_page.rect());
}

bool csPdfUiDocumentView::setZoom(const qreal level, const int newMode)
{
  const qreal oldZoom = _zoom;

  _zoomMode = newMode;
  if(        _zoomMode == ZoomBestFit         &&  !_page.isEmpty() ) {
    const QSizeF   pageSize = _page.size();
    const QSizeF screenSize = viewport()->size();

    const qreal scaleH = screenSize.width()  / pageSize.width();
    const qreal scaleV = screenSize.height() / pageSize.height();

    const qreal ratio = std::floor(pageSize.height()*scaleH) > screenSize.height()
        ? scaleV
        : scaleH;

    _zoom = qBound(ZOOM_MIN, std::floor(ratio*100.0), ZOOM_MAX);

  } else if( _zoomMode == ZoomFitToPageWidth  &&  !_page.isEmpty() ) {
    const qreal   pageWidth = _page.size().width();
    const qreal screenWidth = viewport()->width();
    _zoom = qBound(ZOOM_MIN, std::floor(screenWidth / pageWidth * 100.0), ZOOM_MAX);

  } else { // Default: User defined
    _zoom = qBound(ZOOM_MIN, std::floor(level), ZOOM_MAX);

  }
  setTransform(QTransform::fromScale(_SCALE, _SCALE));

  if( oldZoom != _zoom ) {
    emit zoomChanged(_zoom);
  }

  return oldZoom != _zoom;
}
