#include <iostream>
#include <vector>
#include <string>
using namespace std;

class BigInteger
{
public:
	friend std::ostream& operator<< (std::ostream& os, const BigInteger& a);
	friend std::istream& operator>> (std::istream& i, BigInteger& a);
	friend bool operator< (const BigInteger& a, const BigInteger& b);
	BigInteger() = default;
	BigInteger(int a);
	BigInteger& operator<< (size_t step);
	BigInteger& operator>> (size_t step);
	BigInteger& operator= (const BigInteger& b);
	BigInteger operator- () const;
	BigInteger operator++ (int);
	BigInteger operator-- (int);
	BigInteger& operator++ ();
	BigInteger& operator-- ();
	BigInteger& operator+= (const BigInteger& b);
	BigInteger& operator-= (const BigInteger& b);
	BigInteger& operator*= (const BigInteger& b);
	BigInteger& operator/= (const BigInteger& b);
	BigInteger& operator%= (const BigInteger& b);
	explicit operator bool() const;
	std::string toString() const;
private:
	BigInteger(int size, char digit);
	BigInteger(bool s, std::vector<char> d);
	BigInteger getSubInteger(size_t start, size_t end) const;
	void setSign(bool sign);
	std::vector<char> digits;
	bool sign = true; // 1 = positive, 0 = negative
};

BigInteger::BigInteger(int a)
{
	sign = (a >= 0);
	if (!sign)
	{
		a *= -1;
	}
	if (a == 0)
	{
		return;
	}
	while (a != 0)
	{
		digits.push_back(a % 10);
		a /= 10;
	}
}

BigInteger::BigInteger(bool s, std::vector<char> d)
{
	sign = s;
	digits = d;
}

BigInteger& BigInteger::operator= (const BigInteger& b)
{
	if (this == &b)
	{
		return *this;
	}
	sign = b.sign;
	digits = b.digits;
	return *this;
}

BigInteger& BigInteger::operator<< (size_t step)
{
	if (digits.size() == 0)
	{
		return *this;
	}
	digits.insert(digits.begin(), step, 0);
	return *this;
}

BigInteger& BigInteger::operator>> (size_t step)
{
	if (digits.size() == 0)
	{
		return *this;
	}
	if (digits.size() < step)
	{
		step = digits.size();
	}
	digits.erase(digits.begin(), digits.begin() + step);
	return *this;
}

void BigInteger::setSign(bool s)
{
	sign = s;
}

bool operator< (const BigInteger& a, const BigInteger& b)
{
	if (a.digits.size() == b.digits.size() && a.digits.size() == 0)
	{
		return false;
	}
	if (a.sign ^ b.sign)
	{
		return b.sign;
	}
	if (a.digits.size() != b.digits.size())
	{
		return a.sign == (b.digits.size() > a.digits.size());
	}
	for (int i = a.digits.size() - 1; i >= 0; i--)
	{
		if (b.digits[i] != a.digits[i])
		{
			return a.sign == (b.digits[i] > a.digits[i]);
		}
	}
	return false;
}

bool operator> (const BigInteger& a, const BigInteger& b)
{
	return (b < a);
}

bool operator>= (const BigInteger& a, const BigInteger& b)
{
	return !(a < b);
}

bool operator<= (const BigInteger& a, const BigInteger& b)
{
	return !(b < a);
}

bool operator== (const BigInteger& a, const BigInteger& b)
{
	return (b >= a) && (a >= b);
}

bool operator!= (const BigInteger& a, const BigInteger& b)
{
	return (b > a) || (a > b);
}

BigInteger BigInteger::operator-() const
{
	return BigInteger(!sign, digits);
}

BigInteger& BigInteger::operator++ ()
{
	if (digits.size() == 0)
	{
		digits.push_back(1);
		sign = 1;
		return *this;
	}
	if (sign)
	{
		for (size_t i = 0; i < digits.size() - 1; i++)
		{
			++digits[i];
			if (digits[i] > 9)
			{
				digits[i] = 0;
			}
			else
			{
				return *this;
			}
		}
		++digits[digits.size() - 1];
		if (digits[digits.size() - 1] > 9)
		{
			digits[digits.size() - 1] = 0;
			digits.push_back(1);
		}
		return *this;
	}
	else
	{
		for (size_t i = 0; i < digits.size() - 1; i++)
		{
			--digits[i];
			if (digits[i] < 0)
			{
				digits[i] = 9;
			}
			else
			{
				return *this;
			}
		}
		--digits[digits.size() - 1];
		while (digits.size() != 0 && digits[digits.size() - 1] == 0)
		{
			digits.erase(digits.end() - 1);
		}
		return *this;
	}
}

BigInteger& BigInteger::operator-- ()
{
	sign = !sign;
	++* this;
	sign = !sign;
	return *this;
}

BigInteger BigInteger::operator++ (int)
{
	BigInteger a = *this;
	++* this;
	return a;
}

BigInteger BigInteger::operator-- (int)
{
	BigInteger a = *this;
	--* this;
	return a;
}

BigInteger& BigInteger::operator+= (const BigInteger& b)
{
	if (this == &b)
	{
		*this *= 2;
		return *this;
	}
	if (sign != b.sign)
	{
		sign = b.sign;
		*this -= b;
		sign = !sign;
		return *this;
	}
	int excess = 0;
	size_t minSize = (b.digits.size() < digits.size() ? b.digits.size() : digits.size());
	for (size_t i = 0; i < minSize; i++)
	{
		digits[i] += b.digits[i] + excess;
		excess = digits[i] / 10;
		digits[i] %= 10;
	}
	for (size_t i = minSize; excess != 0 && i < digits.size(); i++)
	{
		digits[i] += excess;
		excess = digits[i] / 10;
		digits[i] %= 10;
	}
	for (size_t i = minSize; i < b.digits.size(); i++)
	{
		digits.push_back(b.digits[i] + excess);
		excess = digits[i] / 10;
		digits[i] %= 10;
	}
	if (excess != 0)
	{
		digits.push_back(excess);
	}
	return *this;
}

BigInteger& BigInteger::operator-= (const BigInteger& b)
{
	if (this == &b)
	{
		*this = 0;
		return *this;
	}
	if (sign != b.sign)
	{
		sign = b.sign;
		*this += b;
		sign = !sign;
		return *this;
	}
	int lack = 0;
	if ((*this >= b && sign) || (*this <= b && !sign))
	{
		for (size_t i = 0; i < b.digits.size(); i++)
		{
			digits[i] -= (b.digits[i] + lack);
			lack = 0;
			if (digits[i] < 0)
			{
				lack = 1;
				digits[i] += 10;
			}
		}
		for (size_t i = b.digits.size(); i < digits.size(); i++)
		{
			digits[i] -= lack;
			lack = 0;
			if (digits[i] < 0)
			{
				lack = 1;
				digits[i] += 10;
			}
		}
		while (digits.size() != 0 && digits[digits.size() - 1] == 0)
		{
			digits.erase(digits.end() - 1);
		}
		return *this;
	}
	else
	{
		for (size_t i = 0; i < digits.size(); i++)
		{
			digits[i] = (b.digits[i] - lack - digits[i]);
			lack = 0;
			if (digits[i] < 0)
			{
				lack = 1;
				digits[i] += 10;
			}
		}
		for (size_t i = digits.size(); i < b.digits.size(); i++)
		{
			digits.push_back(b.digits[i] - lack);
			lack = 0;
			if (digits[i] < 0)
			{
				lack = 1;
				digits[i] += 10;
			}
		}
		while (digits.size() != 0 && digits[digits.size() - 1] == 0)
		{
			digits.erase(digits.end() - 1);
		}
		sign = !sign;
		return *this;
	}
}

BigInteger operator+ (BigInteger a, const BigInteger& b)
{
	BigInteger c = a;
	return c += b;
}

BigInteger operator- (const BigInteger& a, const BigInteger& b)
{
	BigInteger c = a;
	return c -= b;
}

BigInteger BigInteger::getSubInteger(size_t start, size_t end) const
{
	BigInteger result;
	for (size_t i = start; i < end && i < digits.size(); i++)
	{
		result.digits.push_back(digits[i]);
	}
	while (result.digits.size() != 0 && result.digits[result.digits.size() - 1] == 0)
	{
		result.digits.erase(result.digits.end() - 1);
	}
	return result;
}

BigInteger& BigInteger::operator*= (const BigInteger& b)
{
	bool newSign = !(b.sign ^ sign);
	if (b.digits.size() == 1 && b.digits[0] == 1)
	{
		sign = newSign;
		return *this;
	}
	if (digits.size() == 1 && digits[0] == 1)
	{
		*this = b;
		sign = newSign;
		return *this;
	}
	int n = (digits.size() > b.digits.size() ? digits.size() : b.digits.size());
	if (n <= 1)
	{
		if (digits.size() == 0 || b.digits.size() == 0)
		{
			*this = 0;
			return *this;
		}
		*this = BigInteger(sign, BigInteger(digits[0] * b.digits[0]).digits);
		sign = newSign;
		return *this;
	}
	BigInteger thisLow = getSubInteger(0, (n + 1) / 2);
	BigInteger bLow = b.getSubInteger(0, (n + 1) / 2);
	BigInteger thisHigh = getSubInteger((n + 1) / 2, n);
	BigInteger bHigh = b.getSubInteger((n + 1) / 2, n);
	*this = 0;
	*this += ((thisLow + thisHigh) *= (bLow + bHigh)) << ((n + 1) / 2);
	*this += (thisLow *= bLow);
	*this -= (thisLow << ((n + 1) / 2));
	*this -= ((thisHigh *= bHigh) << ((n + 1) / 2));
	*this += (thisHigh << ((n + 1) / 2));
	sign = newSign;
	return *this;
}

BigInteger operator* (const BigInteger& a, const BigInteger& b)
{
	BigInteger c = a;
	return c *= b;
}

BigInteger& BigInteger::operator/= (const BigInteger& b)
{
	if (b.digits.size() == 0)
		throw;
	if (this == &b)
	{
		*this = 1;
		return *this;
	}
	bool newSign = !(b.sign ^ sign);
	sign = true;
	BigInteger r = 0;
	for (int i = digits.size() - 1; i >= 0; i--)
	{
		r << 1;
		r += digits[i];
		digits[i] = 0;
		while (r >= b * (b.sign ? 1 : -1))
		{
			r -= b * (b.sign ? 1 : -1);
			digits[i] += 1;
		}
	}
	while (digits.size() != 0 && digits[digits.size() - 1] == 0)
	{
		digits.erase(digits.end() - 1);
	}
	sign = newSign;
	return *this;
}

BigInteger operator/ (const BigInteger& a, const BigInteger& b)
{
	BigInteger c = a;
	return c /= b;
}

BigInteger& BigInteger::operator%= (const BigInteger& b)
{
	if (b == 0)
		throw;
	if (this == &b)
	{
		*this = 0;
		return *this;
	}
	bool newSign = !(b.sign ^ sign);
	sign = true;
	BigInteger r = 0;
	for (int i = digits.size() - 1; i >= 0; i--)
	{
		r << 1;
		r += digits[i];
		digits[i] = 0;
		while (r >= b * (b.sign ? 1 : -1))
		{
			r -= b * (b.sign ? 1 : -1);
			digits[i] += 1;
		}
	}
	*this = r;
	sign = newSign;
	return *this;
}

BigInteger operator% (const BigInteger& a, const BigInteger& b)
{
	BigInteger c = a;
	return c %= b;
}

BigInteger::operator bool() const
{
	return (digits.size() != 0);
}

std::string BigInteger::toString() const
{
	if (digits.size() == 0)
	{
		return std::string("0");
	}
	std::string str;
	if (!sign)
	{
		str.push_back('-');
	}
	for (int i = digits.size() - 1; i >= 0; i--)
	{
		str.push_back(digits[i] + '0');
	}
	return str;
}

std::ostream& operator<< (std::ostream& os, const BigInteger& a)
{
	if (a.digits.size() == 0)
	{
		os << 0;
		return os;
	}
	if (!a.sign)
	{
		os << "-";
	}
	for (int k = a.digits.size() - 1; k >= 0; k--)
	{
		os << int(a.digits[k]);
	}
	return os;
}

std::istream& operator>> (std::istream& i, BigInteger& a)
{
	std::string s;
	i >> s;
	a.sign = 1;
	a.digits.clear();
	int t = 0;
	if (s[0] == '-')
	{
		a.sign = 0;
		++t;
	}
	while (t < int(s.size()) && s[t] == '0')
	{
		++t;
	}
	for (int k = s.size() - 1; k >= t; k--)
	{
		if (s[k] < '0' || s[k] > '9')
		{
			throw;
		}
		a.digits.push_back(s[k] - '0');
	}
	return i;
}

class Rational
{
public:
	Rational(const BigInteger& b);
	Rational(const int& b);
	Rational() = default;
	Rational& operator= (const Rational& b);
	Rational operator- () const;
	Rational operator++ (int);
	Rational operator-- (int);
	Rational& operator++ ();
	Rational& operator-- ();
	Rational& operator+= (const Rational& b);
	Rational& operator-= (const Rational& b);
	friend Rational operator+ (const Rational& a, const Rational& b);
	friend Rational operator- (const Rational& a, const Rational& b);
	Rational& operator*= (const Rational& b);
	friend Rational operator* (const Rational& a, const Rational& b);
	Rational& operator/= (const Rational& b);
	friend Rational operator/ (const Rational& a, const Rational& b);
	friend bool operator< (const Rational& a, const Rational& b);
	friend bool operator== (const Rational& a, const Rational& b);
	explicit operator bool() const;
	explicit operator double() const;
	explicit operator BigInteger() const;
	std::string toString() const;
	std::string asDecimal(size_t precision = 0) const;
private:
	Rational(const BigInteger& n, const BigInteger& d, bool s = 1);
	BigInteger GCD(BigInteger a, BigInteger b);
	BigInteger num;
	BigInteger denum = 1;
};

Rational::Rational(const BigInteger& b)
{
	num = b;
}

Rational::Rational(const int& b)
{
	num = b;
}

Rational::Rational(const BigInteger& n, const BigInteger& d, bool s)
{
	num = n;
	denum = d;
	if (!s)
	{
		num *= -1;
	}
}

Rational& Rational::operator= (const Rational& b)
{
	if (&b != this)
	{
		num = b.num;
		denum = b.denum;
	}
	return *this;
}

BigInteger Rational::GCD(BigInteger a, BigInteger b)
{
	if (a < 0)
	{
		a *= -1;
	}
	if (b < 0)
	{
		b *= -1;
	}
	while (a != 0 && b != 0)
	{
		a %= b;
		if (a != 0)
		{
			b %= a;
		}
	}
	if (a != 0)
		return a;
	else
		return b;
}

Rational Rational::operator- () const
{
	return Rational(num, denum, 0);
}

Rational Rational::operator++ (int)
{
	Rational result = *this;
	num += denum;
	return result;
}

Rational Rational::operator-- (int)
{
	Rational result = *this;
	num -= denum;
	return result;
}

Rational& Rational::operator++ ()
{
	num += denum;
	return *this;
}

Rational& Rational::operator-- ()
{
	num -= denum;
	return *this;
}

Rational& Rational::operator+= (const Rational& b)
{
	if (this == &b)
	{
		*this *= 2;
		return *this;
	}
	num *= b.denum;
	num += (b.num * denum);
	denum *= b.denum;
	BigInteger fractionGCD = GCD(denum, num);
	num /= fractionGCD;
	denum /= fractionGCD;
	return *this;
}

Rational& Rational::operator-= (const Rational& b)
{
	if (this == &b)
	{
		*this = 0;
		return *this;
	}
	num *= b.denum;
	num -= (b.num * denum);
	denum *= b.denum;
	BigInteger fractionGCD = GCD(denum, num);
	num /= fractionGCD;
	denum /= fractionGCD;
	return *this;
}

Rational operator+ (const Rational& a, const Rational& b)
{
	Rational c = a;
	return c += b;
}

Rational operator- (const Rational& a, const Rational& b)
{
	Rational c = a;
	return c -= b;
}

Rational& Rational::operator*= (const Rational& b)
{
	if (*this == 0)
		return *this;
	num *= b.num;
	denum *= b.denum;
	BigInteger fractionGCD = GCD(denum, num);
	num /= fractionGCD;
	denum /= fractionGCD;
	return *this;
}

Rational operator* (const Rational& a, const Rational& b)
{
	Rational c = a;
	return c *= b;
}

Rational& Rational::operator/= (const Rational& b)
{
	if (b == 0)
		throw;
	if (*this == 0)
		return *this;
	if (this == &b)
	{
		*this = 1;
		return *this;
	}
	num *= (b.denum * (b.num > 0 ? 1 : -1));
	denum *= b.num * (b.num > 0 ? 1 : -1);
	BigInteger fractionGCD = GCD(denum, num);
	num /= fractionGCD;
	denum /= fractionGCD;
	return *this;
}

Rational operator/ (const Rational& a, const Rational& b)
{
	Rational c = a;
	return c /= b;
}

bool operator< (const Rational& a, const Rational& b)
{
	return (a.num * b.denum < b.num * a.denum);
}

bool operator> (const Rational& a, const Rational& b)
{
	return b < a;
}

bool operator<= (const Rational& a, const Rational& b)
{
	return !(b < a);
}

bool operator>= (const Rational& a, const Rational& b)
{
	return !(a < b);
}

bool operator== (const Rational& a, const Rational& b)
{
	return a.num == b.num && a.denum == b.denum;
}

bool operator!= (const Rational& a, const Rational& b)
{
	return !(a == b);
}

Rational::operator bool() const
{
	return (num != 0);
}

Rational::operator BigInteger() const
{
	return (num / denum);
}

Rational::operator double() const
{
	bool sign = (*this >= 0);
	std::string integerPart = (BigInteger(*this) *= (sign ? 1 : -1)).toString();
	std::string fractionalPart = ((*this - BigInteger(*this)) *= (sign ? 1 : -1)).asDecimal(308 - integerPart.size());
	double result = 0;
	double digit = 1;
	for (int i = fractionalPart.size() - 1; i >= 2; i--)
	{
		result += (fractionalPart[i] - '0');
		result /= 10;
	}
	for (int i = integerPart.size() - 1; i >= 0; i--)
	{
		result += ((integerPart[i] - '0') * digit);
		digit *= 10;
	}
	if (!sign)
		result *= -1;
	return result;
}

std::string Rational::toString() const
{
	std::string str = num.toString();
	if (denum != 1)
	{
		str += "/";
		str += denum.toString();
	}
	return str;
}

std::string Rational::asDecimal(size_t precision) const
{
	std::string str = ((BigInteger(num) << precision) / denum).toString();
	bool sign = 1;
	if (str[0] == '-')
	{
		sign = 0;
		str.erase(str.begin(), str.begin() + 1);
	}
	if (precision >= str.size())
		str.insert(str.begin(), 1 + precision - str.size(), '0');
	if (precision != 0)
		str.insert(str.end() - precision, 1, '.');
	if (!sign)
		str.insert(str.begin(), 1, '-');
	return str;
}
