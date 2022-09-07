#include "LN.h"
#include <cstdlib>
#include <iostream>


LN::LN(char* buf) {
	size_ = sizeof(buf);
	if (size_ <= 0)
	{
		isNaN_ = true;
		num_ = nullptr;
		size_ = 0;
		return;
	}
	isNaN_ = false;
	sign_ = 1;
	num_ = buf;
	deleteNulls();
}
LN::LN(char* buf, int size1){
	if (size1 <= 0)
	{
		isNaN_ = true;
		num_ = nullptr;
		size_ = 0;
		return;
	}
	isNaN_ = false;
	sign_ = 1;
	num_ = buf;
	this->size_ = size1;
	deleteNulls();
}
LN::LN(char* buf, int size1, int sign1) {
	if (size_ <= 0)
	{
		isNaN_ = true;
		num_ = nullptr;
		size_ = 0;
		return;
	}
	isNaN_ = false;
	sign_ = sign1;
	num_ = buf;
	size1 = size_;
	deleteNulls();
}
LN::LN(LN&& b) {
	isNaN_ = b.isNaN_;
	sign_ = b.sign_;
	size_ = b.size_;
	num_ = b.num_;
	b.num_ = NULL;
}
LN::LN(const LN& cop) {
	if (cop.isNaN_) {
		isNaN_ = true;
		size_ = 0;
		num_ = nullptr;
		return;
	}
	isNaN_ = false;
	size_ = cop.size_;
	sign_ = cop.sign_;
	num_ = (char*)malloc(size_ * sizeof(char));
	if (num_ == NULL) {
		throw 1;
	}
	for (int i = 0; i < size_; i++)
	{
		num_[i] = cop.num_[i];
	}
}

LN::LN(long long int number) {
	isNaN_ = false;
	if (number < 0) sign_ = -1;
	else sign_ = 1;
	if (number == 0)
	{
		size_ = 1;
		num_ = (char*)malloc(size_ * sizeof(char));
		if (num_ == NULL) {
			throw 1;
		}
		num_[0] = '0';
		return;
	}
	size_ = 0;
	long long int number1 = number;
	while (number1) {
		number1 /= 10;
		size_++;
	}
	num_ = (char*)malloc(size_ * sizeof(char));
	if (num_ == NULL) {
		throw 1;
	}
	for (int i = 0; i < size_; i++)
	{
		num_[i] = number % 10 + 48;
		number /= 10;
	}

}
LN::LN(const std::string_view& str) {
	size_ = 0;
	int size1 = str.length();
	int start = 0;
	sign_ = 1;
	if (str.find('-') != (std::string_view::npos)) {
		sign_ = -1;
		start = 1;
	}
	num_ = (char*)malloc((size1 - start) * sizeof(char));
	if (num_ == nullptr) {
		throw 1;
	}
	int count = size1 - start - 1;
	for (int i = start; i < size1; i++) {
		num_[count--] = str[i];
		size_++;
	}
}


LN::~LN() {
	if (num_ != NULL && num_ != nullptr)
	{
		free(num_);
	}
}
LN LN::createNaN() const {
	LN a;
	a.isNaN_ = true;
	return a;
}

void LN::print(FILE* file) const {
	if (isNaN_)
	{
		fprintf(file, "NaN");
		fprintf(file, "\n");
		return;
	}
	if (sign_ == -1)
	{
		fprintf(file, "-");
	}
	for (size_t i = size_ - 1; i >= 0; i--)
	{
		fprintf(file, "%c", num_[i]);
		if (i == 0) break;
	}
	fprintf(file, "\n");
}
// лишнее
void LN::printInfo() const {
	std::cout << " Number is: ";
	if (isNaN_)
	{
		std::cout << "NaN " << std::endl;
		return;
	}
	for (int i = size_ - 1; i >= 0; i--)
	{
		std::cout << num_[i];
	}
	std::cout << " Size is " << size_ << " Sign is " << sign_;
	std::cout << std::endl;
}
//
int LN::min(const int a, const int b) const {
	if (a < b) return a;
	return b;
}
int LN::max(const int a, const int b) const {
	if (a > b) return a;
	return b;
}
int LN::module(const int a) const {
	if (a < 0)
	{
		return a * -1;
	}
	return a;
}
void LN::deleteNulls() {
	if (isNaN_)
	{
		return;
	}
	for (int i = size_ - 1; i > 0; i--)
	{
		if (num_[i] != '0')
		{
			break;
		}
		else {
			size_--;
		}
	}
}

void LN::operator=(const LN& b) {
	isNaN_ = b.isNaN_;
	if (b.isNaN_)
	{
		*this = createNaN();
		return;
	}
	if (num_ != NULL && num_ != nullptr) {
		free(num_);
	}
	sign_ = b.sign_;
	size_ = b.size_;
	num_ = (char*)malloc(size_ * sizeof(char));
	if (num_ == NULL) {
		throw 1;
	}
	for (size_t i = 0; i < size_; i++) {
		num_[i] = b.num_[i];
	}
}
LN operator ""_ln(const char* a) {
	int size1 = sizeof(a) / sizeof(char);
	int sign1;
	if (a[0] == '-')
	{
		size1--;
		sign1 = -1;
	}
	else sign1 = 1;
	char* temp = (char*)malloc(size1 * sizeof(a));
	if (temp == NULL) {
		throw 1;
	}
	return LN(temp, size1, sign1);
}

LN& LN::operator=(LN&& b) {
	if (&b == this) {
		return *this;
	}
	free(num_);
	sign_ = b.sign_;
	size_ = b.size_;
	num_ = b.num_;
	b.num_ = NULL;
	return *this;
}

int LN::compare(const LN& b) const {
	int res = 0;
	int sign1 = 1;
	if (size_ == 1 && b.size_ == 1 && num_[0] == '0' && b.num_[0] == '0')
	{
		return 0;
	}
	if (sign_ < b.sign_)
	{
		return -1;
	}
	if (sign_ > b.sign_)
	{
		return 1;
	}
	if (sign_ < 0)
	{
		sign1 = -1;
	}
	if (size_ > b.size_)
	{
		return 1 * sign1;
	}

	if (size_ < b.size_)
	{
		return -1 * sign1;
	}
	if (size_ == b.size_)
	{
		for (size_t i = size_ - 1; i >= 0; i--)
		{

			if (num_[i] > b.num_[i]) {
				res = 1;
				break;
			}
			if (num_[i] < b.num_[i]) {
				res = -1;
				break;
			}
			if (i == 0)
			{
				break;
			}
		}
	}
	return res * sign1;
}
bool LN::operator > (const LN& b) const {
	if (isNaN_ || b.isNaN_) return false;
	return compare(b) > 0;
}
bool LN::operator < (const LN& b) const {
	if (isNaN_ || b.isNaN_) return false;
	return compare(b) < 0;
}
bool LN::operator >=(const LN& b) const {
	if (isNaN_ || b.isNaN_) return false;
	return compare(b) >= 0;
}
bool LN::operator <= (const LN& b) const {
	if (isNaN_ || b.isNaN_) return false;
	return compare(b) <= 0;
}

bool LN::operator == (const LN& b) const {
	if (isNaN_ || b.isNaN_) return false;
	return compare(b) == 0;
}
bool LN::operator != (const LN& b) const {
	if (isNaN_ || b.isNaN_) return true;
	return compare(b) != 0;
}
LN LN::add(const LN& b) const {
	if (isNaN_ || b.isNaN_) {
		return createNaN();
	}
	LN a = *this;
	char dop = 0;
	char* temp = (char*)malloc((max(a.size_, b.size_) + 1) * sizeof(char));
	if (temp == NULL) {
		throw 1;
	}
	LN res(temp);
	if (temp == NULL) {
		throw 1;
	}
	int minSize = min(a.size_, b.size_);
	int maxSize = max(a.size_, b.size_);
	for (size_t i = 0; i < minSize; i++)
	{
		dop = dop + a.num_[i] + b.num_[i] - 96;
		if (dop >= 10)
		{
			temp[i] = dop - 10 + 48;
			dop = 1;
		}
		else {
			temp[i] = dop + 48;
			dop = 0;
		}
	}
	for (int i = minSize; i < maxSize; i++)
	{
		if (a.size_ > b.size_) {
			dop = a.num_[i] + dop - 48;
			temp[i] = dop % 10 + 48;
			dop = dop / 10;
		}
		else {
			dop = b.num_[i] + dop - 48;
			temp[i] = dop % 10 + 48;
			dop = dop / 10;
		}
	}
	if (dop) {
		temp[maxSize] = '1';
		return LN(temp, maxSize + 1);
	}
	res.size_ = maxSize;
	return res;
}

LN LN::subtract(const LN& b1) const {
	if (isNaN_ || b1.isNaN_) {
		return createNaN();
	}
	int dop = 0;
	int resSign = 1;
	LN temp1(1);
	LN a(*this);
	LN b(b1);
	if (a < b)
	{
		resSign = -1;
	}
	if (a.size_ < b.size_)
	{
		temp1 = a;
		a = b;
		b = temp1;
	}
	char* temp = (char*)malloc(max(a.size_, b.size_) * sizeof(char));
	if (temp == NULL) {
		throw 1;
	}
	LN res(temp);
	for (size_t i = 0; i < b.size_; i++)
	{
		dop = a.num_[i] - b.num_[i] - dop;
		if (dop < 0)
		{
			temp[i] = dop + 10 + 48;
			dop = 1;
		}
		else {
			temp[i] = dop + 48;
			dop = 0;
		}
	}
	for (size_t i = b.size_; i < a.size_; i++)
	{
		dop = a.num_[i] - dop - 48;
		if (dop < 0)
		{
			temp[i] = dop + 10 + 48;
			dop = 1;
		}
		else {
			temp[i] = dop + 48;
			dop = 0;
		}
	}
	res.size_ = a.size_;
	res.sign_ = resSign;
	return res;
}

LN LN::operator - (const LN& b1) const {
	if (isNaN_ || b1.isNaN_) {
		return createNaN();
	}
	LN a(*this);
	LN b(b1);
	b.sign_ *= -1;
	if (a.sign_ == b.sign_)
	{
		LN res = add(b);
		res.sign_ = a.sign_;
		return res;
	}
	else {
		if (a.sign_ == 1)
		{
			b.sign_ *= -1;
			return subtract(b);
		}
		else {
			a.sign_ *= -1;
			return b.subtract(a);
		}
	}
}

LN LN::operator + (const LN& b1) const {
	if (isNaN_ || b1.isNaN_) {
		return createNaN();
	}
	LN a(*this);
	LN b(b1);
	if (a.sign_ == b.sign_)
	{
		LN res = add(b);
		res.sign_ = a.sign_;
		return res;
	}
	else {
		if (a.sign_ == 1)
		{
			b.sign_ *= -1;
			return subtract(b);
		}
		else {
			a.sign_ *= -1;
			return b.subtract(a);
		}
	}
}


LN LN::simpleMult(char num1, const int dopNulls) const {
	if (isNaN_) {
		return createNaN();
	}
	LN a = *this;
	int num_ = num1 - 48;
	char dop = 0;
	char* temp = (char*)malloc((a.size_ + 1 + dopNulls) * sizeof(char));
	if (temp == NULL) {
		throw 1;
	}
	LN res(temp);
	int count = 0;
	for (int i = 0; i < dopNulls; i++)
	{
		temp[count++] = '0';
	}
	for (size_t i = 0; i < a.size_; i++)
	{
		int tempN = (int)a.num_[i] - 48;
		dop = dop + num_ * tempN;
		temp[count++] = dop - dop / 10 * 10 + 48;
		dop = dop / 10;
	}
	if (dop > 0)
	{
		temp[count] = dop + 48;
		res.size_ = a.size_ + dopNulls + 1;
		return res;
	}
	res.size_ = a.size_ + dopNulls;
	return res;
}
LN LN::operator * (const LN& b) const {
	if (isNaN_ || b.isNaN_) {
		return createNaN();
	}
	LN a = *this;
	LN next;
	LN res = simpleMult(b.num_[0], 0);
	for (int i = 1; i < b.size_; i++)
	{
		next = simpleMult(b.num_[i], i);
		res = res + next;
	}
	res.deleteNulls();
	res.sign_ = a.sign_ * b.sign_;
	return res;
}

LN LN::operator / (const LN& b) const {
	if (isNaN_ || b.isNaN_ || b == LN(0ll)) {
		return createNaN();
	}
	LN c;
	return divide(b, c);
}

LN LN::operator % (const LN& b) const {
	if (isNaN_ || b.isNaN_ || b == LN(0ll)) {
		return createNaN();
	}
	LN mod;
	divide(b, mod);
	return mod;
}

void LN::reverse(char* a, const int size_) const {

	for (int i = 0; i < size_ / 2; i++)
	{
		char temp = a[i];
		a[i] = a[size_ - i - 1];
		a[size_ - i - 1] = temp;
	}

}
int LN::skipNulls(const LN& a, int left) const {
	if (a.isNaN_) {
		return 0;
	}
	for (int i = left; i < a.size_; i++)
	{
		if (a.num_[i] == '0') left++;
		else {
			break;
		}
	}
	return left;
}

LN LN::divide(const LN& b1, LN& mod)const {
	if (isNaN_ || b1.isNaN_) {
		return createNaN();
	}
	if (*this < b1)
	{
		mod = LN(*this);
		return LN(0ll);
	}
	LN a(*this);
	LN b(b1);
	a.sign_ = 1;
	b.sign_ = 1;
	int last = 0;
	int multN = 0;
	int nuls = 0;
	char* resBuf = (char*)malloc((size_) * sizeof(char));
	if (resBuf == NULL) {
		throw 1;
	}
	int left = 0;
	int right = b.size_;
	int resSize = 0;
	int lefted = b.size_;
	int dop = b.size_;
	while (a >= b)
	{
		LN part(a.detach(dop));
		if (part < b || dop == 0)
		{
			a.insertBack(part);
			dop++;
			part = a.detach(dop);
		}
		int dops = 0;
		while (part < b)
		{
			a.insertBack(part);
			dop++;
			part = a.detach(dop);
			dops++;
		}
		for (int i = 0; i < dops; i++)
		{
			resBuf[resSize++] = '0';
		}
		LN per;
		for (int i = 1; i < 10; i++)
		{
			LN temp(b.simpleMult(i + 48, 0));
			if (part >= temp) {
				multN = i;
				per = temp;
			}
			else {
				break;
			}
		}
		resBuf[resSize++] = multN + 48;
		LN inserted(part - per);
		a.insertBack(inserted);
		size_t dopNulls = a.countNulls();
		if (inserted == LN(0ll))
		{
			dopNulls--;
			dop = 0;
		}
		else dop = inserted.size_;
		for (size_t i = 0; i < dopNulls; i++)
		{
			resBuf[resSize++] = '0';
		}
		a.deleteNulls();
	}
	if (a != LN(0ll))
	{
		for (size_t i = dop; i < a.size_; i++)
		{
			resBuf[resSize++] = '0';
		}
	}
	reverse(resBuf, resSize);
	LN res = LN(resBuf, resSize);
	a.deleteNulls();
	res.deleteNulls();
	res.sign_ = sign_ * b1.sign_;
	mod = *this - (res * b1);
	return res;
}
LN LN::detach(const int kol) {
	if (isNaN_) {
		return createNaN();
	}
	char* temp = (char*)malloc((kol) * sizeof(char));
	if (temp == NULL) {
		throw 1;
	}
	LN res(temp);
	int sizeD = kol - 1;
	size_t size1 = size_;
	for (int i = size1 - 1; i >= size1 - kol; i--)
	{
		temp[sizeD--] = num_[i];
		num_[i] = '0';
		size_--;
		if (i == 0)
		{
			break;
		}
	}
	res.size_ = kol;
	res.deleteNulls();
	return res;
}
void LN::insertBack(const LN& b) {
	if (isNaN_ || b.isNaN_) {
		return;
	}
	for (int i = size_; i < size_ + b.size_; i++)
	{
		num_[i] = b.num_[i - size_];
	}
	size_ += b.size_;
}
size_t LN::countNulls() const {
	if (isNaN_) {
		return 0;
	}
	size_t res = 0;
	for (size_t i = size_ - 1; i >= 0; i--)
	{
		if (num_[i] == '0') {
			res++;
		}
		else return res;
		if (i == 0)
		{
			return res;
		}
	}
}
void LN:: operator+= (const LN& b) {
	*this = *this + b;
}
void LN:: operator-= (const LN& b) {
	*this = *this - b;
}
void LN:: operator*= (const LN& b) {
	*this = *this * b;
}
void LN:: operator/= (const LN& b) {
	*this = *this / b;
}
void LN:: operator%= (const LN& b) {
	*this = *this % b;
}

LN:: operator long long int() const{
	if (isNaN_) throw 3;
	if (*this < LN(9223372036854775808ll))
	{
		throw 3;
	}
	if (*this > (9223372036854775807ll))
	{
		throw 3;
	}
	long long int res = 0;
	long long int mult = 1;
	try {
		for (long long int i = 0; i < size_; i++)
		{
			res += mult * (num_[i] - 48);
			mult *= 10;
		}
	}
	catch(int e){}
	return res * sign_;
}

LN:: operator bool() const {
	if (*this != LN(0ll))
	{
		return true;
	}
	else {
		return false;
	}
}


LN LN::operator ~() const {
	if (isNaN_ || *this < LN(0ll)) {
		return createNaN();
	}
	LN a(*this);
	LN left = LN(0ll);
	LN right = LN(a);
	LN res;
	LN temp1(1);
	LN temp2(2);
	while (left <= right)
	{
		LN mid1 = left + right;
		LN mid = mid1 / temp2;
		LN midX2 = mid * mid;
		if (midX2 <= a)
		{
			res = mid;
			left = left + temp1;
		}
		else right = mid - temp1;
	}
	return res;
}

void LN::operator - () {
	if (isNaN_) {
		return;
	}
	sign_ *= -1;
}

