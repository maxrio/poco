//
// Time.h
//
// Library: SQL
// Package: SQLCore
// Module:  Time
//
// Definition of the Time class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_Time_INCLUDED
#define SQL_Time_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/Dynamic/VarHolder.h"
#include "Poco/Exception.h"


namespace Poco {

namespace Dynamic {

class Var;

}

class DateTime;

namespace SQL {


class Date;


class Poco_SQL_API Time
	/// Time class wraps a DateTime and exposes time related interface.
	/// The purpose of this class is binding/extraction support for time fields.
{
public:
	Time();
		/// Creates the Time

	Time(int hour, int minute, int second);
		/// Creates the Time

	Time(const DateTime& dt);
		/// Creates the Time from DateTime

	~Time();
		/// Destroys the Time.

	int hour() const;
		/// Returns the hour.

	int minute() const;
		/// Returns the minute.

	int second() const;
		/// Returns the second.

	void assign(int hour, int minute, int second);
		/// Assigns time.

	Time& operator = (const Time& t);
		/// Assignment operator for Time.

	Time& operator = (const DateTime& dt);
		/// Assignment operator for DateTime.

	Time& operator = (const Poco::Dynamic::Var& var);
		/// Assignment operator for Var.

	bool operator == (const Time& time) const;
		/// Equality operator.

	bool operator != (const Time& time) const;
		/// Inequality operator.

	bool operator < (const Time& time) const;
		/// Less then operator.

	bool operator > (const Time& time) const;
		/// Greater then operator.

private:
	int _hour;
	int _minute;
	int _second;
};


//
// inlines
//
inline int Time::hour() const
{
	return _hour;
}


inline int Time::minute() const
{
	return _minute;
}


inline  int Time::second() const
{
	return _second;
}


inline Time& Time::operator = (const Time& t)
{
	assign(t.hour(), t.minute(), t.second());
	return *this;
}


inline Time& Time::operator = (const DateTime& dt)
{
	assign(dt.hour(), dt.minute(), dt.second());
	return *this;
}


inline bool Time::operator == (const Time& time) const
{
	return _hour == time.hour() &&
		_minute == time.minute() &&
		_second == time.second();
}


inline bool Time::operator != (const Time& time) const
{
	return !(*this == time);
}


inline bool Time::operator > (const Time& time) const
{
	return !(*this == time) && !(*this < time);
}


} } // namespace Poco::SQL


//
// VarHolderImpl<Time>
//


namespace Poco {
namespace Dynamic {


template <>
class VarHolderImpl<Poco::SQL::Time>: public VarHolder
{
public:
	VarHolderImpl(const Poco::SQL::Time& val): _val(val)
	{
	}

	~VarHolderImpl()
	{
	}
	
	const std::type_info& type() const
	{
		return typeid(Poco::SQL::Time);
	}

	void convert(Poco::Timestamp& val) const
	{
		Poco::DateTime dt;
		dt.assign(dt.year(), dt.month(), dt.day(), _val.hour(), _val.minute(), _val.second());
		val = dt.timestamp();
	}

	void convert(Poco::DateTime& val) const
	{
		Poco::DateTime dt;
		dt.assign(dt.year(), dt.month(), dt.day(), _val.hour(), _val.minute(), _val.second());
		val = dt;
	}

	void convert(Poco::LocalDateTime& val) const
	{
		Poco::LocalDateTime ldt;
		ldt.assign(ldt.year(), ldt.month(), ldt.day(), _val.hour(), _val.minute(), _val.second());
		val = ldt;
	}

	void convert(std::string& val) const
	{
		DateTime dt(0, 1, 1, _val.hour(), _val.minute(), _val.second());
		val = DateTimeFormatter::format(dt, "%H:%M:%S");
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = 0) const
	{
		return cloneHolder(pVarHolder, _val);
	}
	
	const Poco::SQL::Time& value() const
	{
		return _val;
	}

	bool isDate() const
	{
		return false;
	}

	bool isTime() const
	{
		return true;
	}

	bool isDateTime() const
	{
		return false;
	}

private:
	VarHolderImpl();
	Poco::SQL::Time _val;
};


} } // namespace Poco::Dynamic


#endif // Data_Time_INCLUDED
