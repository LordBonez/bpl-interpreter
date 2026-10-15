#include "val.h"
#include <limits>

/*
 * CS280 - Fall 2025
 * Programming Assignment 3
 * Anthony Vorontsov
*/

bool IsDigit(const string& Word)
{
	size_t Length = Word.length();
	if (Length == 0)
		return false;

	int Dots = 0;
	//iterate for each character of the Word
	for (size_t i = 0; i < Length; i++)
	{
		char ch = Word[i];

		if (ch == '.')
		{
			Dots++; //count Dots
			if (Dots > 1 || i == 0 || i == Length - 1)  //Check for only One Dot, not first, not last
				return false;
		}

		if (!isdigit(ch) && ch != '.')
			return false;
	}
	return true;
}

Value Value::operator+(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;
	else if (IsString() && IsDigit(GetString()))
	{
		valueL.SetType(VNUM);
		valueL.SetNum(atof(GetString().c_str()));
	}

	if (op.IsNum())
		valueR = op;
	else if (op.IsString() && IsDigit(op.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(op.GetString().c_str()));
	}

	if (valueL.IsNum() && valueR.IsNum())
	{
		value.SetType(VNUM);
		value.SetNum(valueL.GetNum() + valueR.GetNum());
	}

	return value;
}

// numeric overloaded subtract op from this
Value Value::operator-(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;
	else if (IsString() && IsDigit(GetString()))
	{
		valueL.SetType(VNUM);
		valueL.SetNum(atof(GetString().c_str()));
	}

	if (op.IsNum())
		valueR = op;
	else if (op.IsString() && IsDigit(op.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(op.GetString().c_str()));
	}

	if (valueL.IsNum() && valueR.IsNum())
	{
		value.SetType(VNUM);
		value.SetNum(valueL.GetNum() - valueR.GetNum());
	}

	return value;
}

// numeric overloaded multiply this by op
Value Value::operator*(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;
	else if (IsString() && IsDigit(GetString()))
	{
		valueL.SetType(VNUM);
		valueL.SetNum(atof(GetString().c_str()));
	}

	if (op.IsNum())
		valueR = op;
	else if (op.IsString() && IsDigit(op.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(op.GetString().c_str()));
	}

	if (valueL.IsNum() && valueR.IsNum())
	{
		value.SetType(VNUM);
		value.SetNum(valueL.GetNum() * valueR.GetNum());
	}

	return value;
}

// numeric overloaded divide this by op
Value Value::operator/(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;
	else if (IsString() && IsDigit(GetString()))
	{
		valueL.SetType(VNUM);
		valueL.SetNum(atof(GetString().c_str()));
	}

	if (op.IsNum())
		valueR = op;
	else if (op.IsString() && IsDigit(op.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(op.GetString().c_str()));
	}

	if (valueL.IsNum() && valueR.IsNum())
	{
		if (valueR.GetNum() != 0)
		{
			value.SetType(VNUM);
			value.SetNum(valueL.GetNum() / valueR.GetNum());
		}
	}

	return value;
}

//numeric remaider of integer division of this by op
Value Value::operator%(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;
	else if (IsString() && IsDigit(GetString()))
	{
		valueL.SetType(VNUM);
		valueL.SetNum(atof(GetString().c_str()));
	}

	if (op.IsNum())
		valueR = op;
	else if (op.IsString() && IsDigit(op.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(op.GetString().c_str()));
	}

	if (valueL.IsNum() && valueR.IsNum())
	{
		// BPL defines % as integer remainder, including numeric-string operands.
		const double left = valueL.GetNum();
		const double right = valueR.GetNum();
		const double minInteger = static_cast<double>(std::numeric_limits<long long>::min());
		const double maxInteger = static_cast<double>(std::numeric_limits<long long>::max());
		if (!std::isfinite(left) || !std::isfinite(right) ||
			left < minInteger || left >= maxInteger || right < minInteger || right >= maxInteger)
			return value;
		const long long dividend = static_cast<long long>(left);
		const long long divisor = static_cast<long long>(right);
		if (divisor == 0 || (dividend == std::numeric_limits<long long>::min() && divisor == -1))
			return value;
		value.SetType(VNUM);
		value.SetNum(static_cast<double>(dividend % divisor));
	}

	return value;
}

//numeric overloaded equality operator of this with op
Value Value::operator==(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;
	else if (IsString() && IsDigit(GetString()))
	{
		valueL.SetType(VNUM);
		valueL.SetNum(atof(GetString().c_str()));
	}

	if (op.IsNum())
		valueR = op;
	else if (op.IsString() && IsDigit(op.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(op.GetString().c_str()));
	}

	if (valueL.IsNum() && valueR.IsNum())
	{
		value.SetType(VBOOL);
		value.SetBool((int)valueL.GetNum() == (int)valueR.GetNum());
	}

	return value;
}

//numeric overloaded greater than operator of this with op
Value Value::operator>=(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;
	else if (IsString() && IsDigit(GetString()))
	{
		valueL.SetType(VNUM);
		valueL.SetNum(atof(GetString().c_str()));
	}

	if (op.IsNum())
		valueR = op;
	else if (op.IsString() && IsDigit(op.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(op.GetString().c_str()));
	}

	if (valueL.IsNum() && valueR.IsNum())
	{
		value.SetType(VBOOL);
		value.SetBool((int)valueL.GetNum() >= (int)valueR.GetNum());
	}

	return value;
}

//numeric overloaded less than operator of this with op
Value Value::operator<(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;
	else if (IsString() && IsDigit(GetString()))
	{
		valueL.SetType(VNUM);
		valueL.SetNum(atof(GetString().c_str()));
	}

	if (op.IsNum())
		valueR = op;
	else if (op.IsString() && IsDigit(op.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(op.GetString().c_str()));
	}

	if (valueL.IsNum() && valueR.IsNum())
	{
		value.SetType(VBOOL);
		value.SetBool(valueL.GetNum() < valueR.GetNum());
	}

	return value;
}

//Numeric exponentiation operation this raised to the power of op 
Value Value::Expon(const Value& oper) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsNum())
		valueL = *this;

	if (oper.IsNum())
		valueR = oper;

	if (valueL.IsNum() && valueR.IsNum())
	{
		value.SetType(VNUM);
		value.SetNum(pow(valueL.GetNum(), (int)valueR.GetNum()));
	}

	return value;
}

//string concatenation operation of this with op
Value Value::Catenate(const Value& oper) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsString())
		valueL = *this;
	else if (IsNum())
	{
		valueL.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", GetNum());
		valueL.SetString(buf);
	}

	if (oper.IsString())
		valueR = oper;
	else if (oper.IsNum())
	{
		valueR.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", oper.GetNum());
		valueR.SetString(buf);
	}

	if (valueL.IsString() && valueR.IsString())
	{
		value.SetType(VSTRING);
		value.SetString(valueL.GetString() + valueR.GetString());
	}

	return value;
}

//string repetition operation of this with op
Value Value::Repeat(const Value& oper) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsString())
		valueL = *this;
	else if (IsNum())
	{
		valueL.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", GetNum());
		valueL.SetString(buf);
	}

	if (oper.IsNum())
		valueR = oper;
	else if (oper.IsString() && IsDigit(oper.GetString()))
	{
		valueR.SetType(VNUM);
		valueR.SetNum(atof(oper.GetString().c_str()));
	}

	if (valueL.IsString() && valueR.IsNum())
	{
		value.SetType(VSTRING);
		for (int i = 0; i < (int)valueR.GetNum(); i++)
			value.SetString(value.GetString() + valueL.GetString());
	}

	return value;
}

//string equality (-eq) operator of this with op
Value Value::SEQ(const Value& oper) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsString())
		valueL = *this;
	else if (IsNum())
	{
		valueL.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", GetNum());
		valueL.SetString(buf);
	}

	if (oper.IsString())
		valueR = oper;
	else if (oper.IsNum())
	{
		valueR.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", oper.GetNum());
		valueR.SetString(buf);
	}

	if (valueL.IsString() && valueR.IsString())
	{
		value.SetType(VBOOL);
		value.SetBool(valueL.GetString() == valueR.GetString());
	}

	return value;
}

//string greater than (-gt) operator of this with op
Value Value::SGT(const Value& oper) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsString())
		valueL = *this;
	else if (IsNum())
	{
		valueL.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", GetNum());
		valueL.SetString(buf);
	}

	if (oper.IsString())
		valueR = oper;
	else if (oper.IsNum())
	{
		valueR.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", oper.GetNum());
		valueR.SetString(buf);
	}

	if (valueL.IsString() && valueR.IsString())
	{
		value.SetType(VBOOL);
		value.SetBool(valueL.GetString() > valueR.GetString());
	}

	return value;
}

//string less than operator of this with op
Value Value::SLE(const Value& oper) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsString())
		valueL = *this;
	else if (IsNum())
	{
		valueL.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", GetNum());
		valueL.SetString(buf);
	}

	if (oper.IsString())
		valueR = oper;
	else if (oper.IsNum())
	{
		valueR.SetType(VSTRING);
		char buf[50];
		snprintf(buf, 50, "%g", oper.GetNum());
		valueR.SetString(buf);
	}

	if (valueL.IsString() && valueR.IsString())
	{
		value.SetType(VBOOL);
		value.SetBool(valueL.GetString() < valueR.GetString());
	}

	return value;
}

//Logic overloaded Anding of this and op
Value Value::operator&&(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsBool())
	{
		valueL = *this;
	}
	else if (IsNum())
	{
		valueL.SetType(VBOOL);
		if (GetNum() == 0)
			valueL.SetBool(false);
		else
			valueL.SetBool(true);
	}
	else
	{
		valueL.SetType(VBOOL);
		if (GetString().empty() || GetString()[0] == '0')
			valueL.SetBool(false);
		else
			valueL.SetBool(true);
	}

	if (op.IsBool())
		valueR = op;
	else if (op.IsNum())
	{
		valueR.SetType(VBOOL);
		if (op.GetNum() == 0)
			valueR.SetBool(false);
		else
			valueR.SetBool(true);
	}
	else
	{
		valueR.SetType(VBOOL);
		if (op.GetString().empty() || op.GetString()[0] == '0')
			valueR.SetBool(false);
		else
			valueR.SetBool(true);
	}

	if (valueL.IsBool() && valueR.IsBool())
	{
		value.SetType(VBOOL);
		value.SetBool(valueL.GetBool() && valueR.GetBool());
	}

	return value;
}

//Logic overloaded Oring of this and op
Value Value::operator||(const Value& op) const
{
	Value value;
	Value valueL;
	Value valueR;

	if (IsBool())
		valueL = *this;
	else if (IsNum())
	{
		valueL.SetType(VBOOL);
		if (GetNum() == 0)
			valueL.SetBool(false);
		else
			valueL.SetBool(true);
	}
	else
	{
		valueL.SetType(VBOOL);
		if (GetString().empty() || GetString()[0] == '0')
			valueL.SetBool(false);
		else
			valueL.SetBool(true);
	}

	if (op.IsNum() || op.IsBool())
		valueR = op;
	else if (op.IsNum())
	{
		valueR.SetType(VBOOL);
		if (op.GetNum() == 0)
			valueR.SetBool(false);
		else
			valueR.SetBool(true);
	}
	else
	{
		valueR.SetType(VBOOL);
		if (op.GetString().empty() || op.GetString()[0] == '0')
			valueR.SetBool(false);
		else
			valueR.SetBool(true);
	}

	if (valueL.IsBool() && valueR.IsBool())
	{
		value.SetType(VBOOL);
		value.SetBool(valueL.GetBool() || valueR.GetBool());
	}

	return value;
}

//Logic overloaded Not of this 
Value Value::operator!() const
{
	Value value;
	Value valueL;

	if (IsBool())
		valueL = *this;
	else if (IsNum())
	{
		valueL.SetType(VBOOL);
		if (GetNum() == 0)
			valueL.SetBool(false);
		else
			valueL.SetBool(true);
	}
	else
	{
		valueL.SetType(VBOOL);
		if (GetString().empty() || GetString()[0] == '0')
			valueL.SetBool(false);
		else
			valueL.SetBool(true);
	}

	if (valueL.IsBool())
	{
		value.SetType(VBOOL);
		value.SetBool(!(bool)valueL.GetBool());
	}

	return value;
}
