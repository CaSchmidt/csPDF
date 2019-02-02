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

#include <csPDFium/csPDFiumContentsModel.h>

////// public ////////////////////////////////////////////////////////////////

csPDFiumContentsModel::csPDFiumContentsModel(QObject *parent)
  : QAbstractItemModel(parent)
  , _contents(0)
  , _displayed(0)
  , _filtered(0)
{
  _contents = newRootNode();
  _filtered = newRootNode();

  _displayed = _contents;
}

csPDFiumContentsModel::~csPDFiumContentsModel()
{
  delete _contents;
  delete _filtered;
}

csPDFiumContentsNode *csPDFiumContentsModel::newRootNode()
{
  return new csPDFiumContentsNode((const ushort*)L"<Root>", 0, 0);
}

void csPDFiumContentsModel::setRootNode(csPDFiumContentsNode *root)
{
  beginResetModel();

  delete _contents;
  delete _filtered;

  if( root == 0 ) {
    _contents = newRootNode();
  } else {
    _contents = root;
  }
  _filtered = newRootNode();

  _displayed = _contents;

  endResetModel();
}

int csPDFiumContentsModel::columnCount(const QModelIndex& parent) const
{
  if( parent.isValid() ) {
    return static_cast<csPDFiumContentsNode*>(parent.internalPointer())->columnCount();
  }

  return _displayed->columnCount();
}

QVariant csPDFiumContentsModel::data(const QModelIndex& index, int role) const
{
  if( !index.isValid() ) {
    return QVariant();
  }

  if( role != Qt::DisplayRole ) {
    return QVariant();
  }

  csPDFiumContentsNode *item = static_cast<csPDFiumContentsNode*>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags csPDFiumContentsModel::flags(const QModelIndex& index) const
{
  if( !index.isValid() ) {
    return Qt::NoItemFlags;
  }

  return QAbstractItemModel::flags(index);
}

QVariant csPDFiumContentsModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const
{
  if( orientation == Qt::Horizontal  &&  role == Qt::DisplayRole ) {
    return _displayed->data(section);
  }

  return QVariant();
}

QModelIndex csPDFiumContentsModel::index(int row, int column,
                                         const QModelIndex& parent) const
{
  if( !hasIndex(row, column, parent) ) {
    return QModelIndex();
  }

  csPDFiumContentsNode *parentItem;
  if( !parent.isValid() ) {
    parentItem = _displayed;
  } else {
    parentItem = static_cast<csPDFiumContentsNode*>(parent.internalPointer());
  }

  csPDFiumContentsNode *childItem = parentItem->child(row);
  if( childItem != 0 ) {
    return createIndex(row, column, childItem);
  }

  return QModelIndex();
}

QModelIndex csPDFiumContentsModel::parent(const QModelIndex& index) const
{
  if( !index.isValid() ) {
    return QModelIndex();
  }

  csPDFiumContentsNode *childItem  = static_cast<csPDFiumContentsNode*>(index.internalPointer());
  csPDFiumContentsNode *parentItem = childItem->parent();

  if( parentItem == _displayed ) {
    return QModelIndex();
  }

  return createIndex(parentItem->row(), 0, parentItem);
}

int csPDFiumContentsModel::rowCount(const QModelIndex& parent) const
{
  csPDFiumContentsNode *parentItem;
  if( parent.column() > 0 ) {
    return 0;
  }

  if( !parent.isValid() ) {
    parentItem = _displayed;
  } else {
    parentItem = static_cast<csPDFiumContentsNode*>(parent.internalPointer());
  }

  return parentItem->childCount();
}

////// public slots //////////////////////////////////////////////////////////

void csPDFiumContentsModel::filter(const QString& pattern)
{
  beginResetModel();

  delete _filtered;
  _filtered = newRootNode();

  if( pattern.isEmpty() ) {
    _displayed = _contents;
  } else {
    filter(_contents, _filtered, pattern);
    _displayed = _filtered;
  }

  endResetModel();
}

////// private ///////////////////////////////////////////////////////////////

void csPDFiumContentsModel::filter(const csPDFiumContentsNode *node,
                                   csPDFiumContentsNode *filtered,
                                   const QString& pattern)
{
  for(int i = 0; i < node->childCount(); i++) {
    const csPDFiumContentsNode *child = node->constChild(i);
    if( child->title().contains(pattern, Qt::CaseInsensitive) ) {
      filtered->appendChild(child->title().utf16(), child->pointer());
    }
    filter(child, filtered, pattern);
  }
}
