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

#include <csPDFSearch/csPdfSearchResultsModel.h>

////// Private ///////////////////////////////////////////////////////////////

enum Columns {
  Col_Page = 0,
  Col_Context,
  Num_Columns
};

////// public ////////////////////////////////////////////////////////////////

csPdfSearchResultsModel::csPdfSearchResultsModel(QObject *parent)
  : QAbstractTableModel(parent)
  , _results()
{
}

csPdfSearchResultsModel::~csPdfSearchResultsModel()
{
}

int csPdfSearchResultsModel::columnCount(const QModelIndex& /*parent*/) const
{
  return Num_Columns;
}

QVariant csPdfSearchResultsModel::data(const QModelIndex& index, int role) const
{
  if( !index.isValid() ) {
    return QVariant();
  }

  if( 0 <= index.row()  &&  index.row() < rowCount() ) {
    if( role == Qt::DisplayRole ) {
      if(        index.column() == Col_Page ) {
        return _results[index.row()].page()+1;
      } else if( index.column() == Col_Context ) {
        return _results[index.row()].contextString();
      }
    }
  }

  return QVariant();
}

Qt::ItemFlags csPdfSearchResultsModel::flags(const QModelIndex& index) const
{
  if( !index.isValid() ) {
    return Qt::NoItemFlags;
  }

  return QAbstractItemModel::flags(index);
}

QVariant csPdfSearchResultsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(        orientation == Qt::Horizontal ) {
    if( role == Qt::DisplayRole ) {
      if(        section == Col_Page ) {
        return trUtf8("Page");
      } else if( section == Col_Context ) {
        return trUtf8("Context");
      }
    }
  } else if( orientation == Qt::Vertical ) {
    if( role == Qt::DisplayRole ) {
      if( 0 <= section  &&  section < rowCount() ) {
        return section+1;
      }
    }
  }

  return QVariant();
}

int csPdfSearchResultsModel::rowCount(const QModelIndex& /*parent*/) const
{
  return _results.size();
}

////// public slots //////////////////////////////////////////////////////////

void csPdfSearchResultsModel::clear()
{
  beginResetModel();
  _results.clear();
  endResetModel();
}

void csPdfSearchResultsModel::insertResults(const csPdfSearchResults& incoming)
{
  if( incoming.isEmpty() ) {
    return;
  }

  beginResetModel();
  _results += incoming;
  qSort(_results);
  endResetModel();
}
