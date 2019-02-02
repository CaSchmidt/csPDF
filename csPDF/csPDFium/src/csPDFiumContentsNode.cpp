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

#include <csPDFium/csPDFiumContentsNode.h>

////// Private ///////////////////////////////////////////////////////////////

namespace priv {

  int utf16len(const ushort *str)
  {
    if( str == 0 ) {
      return 0;
    }

    ushort *ptr = const_cast<ushort*>(str);
    int len(0);
    while( *ptr++ ) {
      len++;
    }
    return len;
  }

  ushort *utf16dup(const ushort *str)
  {
    const int len = utf16len(str);
    if( len == 0 ) {
      return 0;
    }

    ushort *copy = new ushort[len+1];
    if( copy == 0 ) {
      return 0;
    }

    for(int i = 0; i < len; i++) {
      copy[i] = str[i];
    }
    copy[len] = 0;

    return copy;
  }

} // namespace priv

////// public ////////////////////////////////////////////////////////////////

csPDFiumContentsNode::csPDFiumContentsNode(const ushort *title,
                                           const void *pointer,
                                           csPDFiumContentsNode *parent)
  : _title(0)
  , _parent(parent)
  , _children(0)
  , _pointer(pointer)
  , _numChildren(0)
{
  _title = priv::utf16dup(title);
}

csPDFiumContentsNode::~csPDFiumContentsNode()
{
  delete[] _title;

  for(int i = 0; i < _numChildren; i++) {
    delete _children[i];
  }
  delete[] _children;
}

QString csPDFiumContentsNode::title() const
{
  return QString::fromUtf16(_title);
}

const void *csPDFiumContentsNode::pointer() const
{
  return _pointer;
}

void csPDFiumContentsNode::appendChild(const ushort *title, const void *pointer)
{
  appendChild(new csPDFiumContentsNode(title, pointer, this));
}

void csPDFiumContentsNode::appendChild(const csPDFiumContentsNode *child)
{
  if( child == 0 ) {
    return;
  }

  csPDFiumContentsNode **old = _children;

  _children = new csPDFiumContentsNode*[_numChildren+1];
  if( _children == 0 ) {
    _children = old;
    return;
  }

  for(int i = 0; i < _numChildren; i++) {
    _children[i] = old[i];
  }
  _children[_numChildren] = const_cast<csPDFiumContentsNode*>(child);

  delete[] old;
  _numChildren++;
}

csPDFiumContentsNode *csPDFiumContentsNode::child(int row)
{
  if( row < 0  ||  row >= _numChildren ) {
    return 0;
  }
  return _children[row];
}

int csPDFiumContentsNode::childCount() const
{
  return _numChildren;
}

int csPDFiumContentsNode::columnCount() const
{
  return 1;
}

const csPDFiumContentsNode *csPDFiumContentsNode::constChild(int row) const
{
  if( row < 0  ||  row >= _numChildren ) {
    return 0;
  }
  return _children[row];
}

QVariant csPDFiumContentsNode::data(int /*column*/) const
{
  return QString::fromUtf16(_title);
}

csPDFiumContentsNode *csPDFiumContentsNode::parent()
{
  return _parent;
}

int csPDFiumContentsNode::row() const
{
  if( _parent != 0 ) {
    for(int i = 0; i < _parent->_numChildren; i++) {
      if( _parent->_children[i] == this ) {
        return i;
      }
    }
  }
  return 0;
}
