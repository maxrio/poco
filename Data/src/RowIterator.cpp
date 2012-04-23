//
// RowIterator.cpp
//
// $Id: //poco/Main/Data/src/RowIterator.cpp#1 $
//
// Library: Data
// Package: DataCore
// Module:  RowIterator
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Data/RowIterator.h"
#include "Poco/Data/RecordSet.h"
#undef min
#undef max
#include <limits>


namespace Poco {
namespace Data {


const int RowIterator::POSITION_END = std::numeric_limits<std::size_t>::max();


RowIterator::RowIterator(RecordSet* pRecordSet, bool positionEnd): 
	_pRecordSet(pRecordSet),
	_position(positionEnd ? POSITION_END : 0)
{
}


RowIterator::RowIterator(const RowIterator& other):
	_pRecordSet(other._pRecordSet),
	_position(other._position)
{
}


RowIterator::~RowIterator()
{
}


RowIterator& RowIterator::operator = (const RowIterator& other)
{
	RowIterator tmp(other);
	swap(tmp);
	return *this;
}


void RowIterator::swap(RowIterator& other)
{
	using std::swap;
	
	swap(_pRecordSet, other._pRecordSet);
	swap(_position, other._position);
}


void RowIterator::increment() const
{
	if (POSITION_END == _position)
		throw RangeException("End of iterator reached.");

	if (_position < _pRecordSet->subTotalRowCount() - 1)
		++_position;
	else
		_position = POSITION_END;

	if (_pRecordSet->getFilter() && POSITION_END != _position)
	{
		while (!_pRecordSet->isAllowed(_position))
		{
			increment();
			if (POSITION_END == _position) break;
		}
	}
}


void RowIterator::decrement() const
{
	if (0 == _position)
		throw RangeException("Beginning of iterator reached.");
	else if (POSITION_END == _position)
		_position = _pRecordSet->subTotalRowCount() - 1;
	else
		--_position;

	if (_pRecordSet->getFilter() && 0 != _position)
	{
		while (!_pRecordSet->isAllowed(_position))
		{
			decrement();
			if (0 == _position) break;
		}
	}
}


void RowIterator::setPosition(std::size_t pos) const
{
	if (_position == pos) return;

	if (_pRecordSet->getFilter())
	{
		std::size_t start = _position;
		if (_position > pos)
		{
			std::size_t end = _position - pos;
			for (; start > end; --start)
			{
				if (pos) --pos;
				else throw RangeException("Invalid position argument.");
			}
		}
		else
		{
			std::size_t end = pos - _position;
			for (; start < end; ++start)
			{
				if (_pRecordSet->subTotalRowCount() != pos) ++pos;
				else throw RangeException("Invalid position argument.");
			}
		}
	}

	if (pos < _pRecordSet->subTotalRowCount())
		_position = pos;
	else if (pos == _pRecordSet->subTotalRowCount())
		_position = POSITION_END;
	else
		throw RangeException("Invalid position argument.");
}


Row& RowIterator::operator * () const
{
	if (POSITION_END == _position)
		throw InvalidAccessException("End of iterator reached.");

	return _pRecordSet->row(_position);
}


Row* RowIterator::operator -> () const
{
	if (POSITION_END == _position)
		throw InvalidAccessException("End of iterator reached.");

	return &_pRecordSet->row(_position);
}


const RowIterator& RowIterator::operator ++ () const
{
	increment();
	return *this;
}


RowIterator RowIterator::operator ++ (int) const
{
	RowIterator old(*this);
	increment();
	return old;
}


const RowIterator& RowIterator::operator -- () const
{
	decrement();
	return *this;
}


RowIterator RowIterator::operator -- (int) const
{
	RowIterator old(*this);
	decrement();
	return old;
}


RowIterator RowIterator::operator + (std::size_t diff) const
{
	RowIterator ri(*this);
	ri.setPosition(_position + diff);
	return ri;
}


RowIterator RowIterator::operator - (std::size_t diff) const
{
	if (diff > _position) throw RangeException("Invalid position argument.");
	RowIterator ri(*this);
	ri.setPosition(_position - diff);
	return ri;
}


} } // namespace Poco::Data
