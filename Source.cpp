#include <ostream>
#include <istream>
#include <iostream>
#include <exception>
#include <fstream>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <sstream>
using namespace std;

enum class ETipToken {

	parenteza,
	numar,
	operatie,
	sfarsit,
};
enum class ETipOperator {
	plus,
	minus,
	steluta,
	caret,
	slash,
	hash
};

enum class ETipParanteza {
	deschisa,
	inchisa,
	pDeschisa,
	pInchisa
};

class Token
{
private:
	ETipToken tip;
	union {
		ETipOperator  op;
		ETipParanteza paranteza;
		double numar;
	} val;
public:
	Token();
	explicit Token(ETipParanteza y);
	explicit Token(ETipOperator x);
	explicit Token(double numar);

	static const char kParantezaRDeschisa;

	explicit operator ETipOperator() const;
	explicit operator ETipParanteza() const;
	explicit operator double() const;
	ETipToken getTip() const;

	friend std::ostream& operator<<(std::ostream& out, const Token& t);
	friend std::istream& operator>>(std::istream& in, Token& t);
};

const char Token::kParantezaRDeschisa = '(';

Token::Token() :
	tip(ETipToken::sfarsit)
{
	val.numar = 0;
}
Token::Token(ETipParanteza y)
{
	tip = ETipToken::parenteza;
	val.paranteza = y;
}
Token::Token(ETipOperator x)
{
	tip = ETipToken::operatie;
	val.op = x;
}

Token::Token(double numar)
{
	tip = ETipToken::numar;
	val.numar = numar;
}

Token:: operator ETipOperator() const
{
	return val.op;
}
Token:: operator ETipParanteza() const
{
	return val.paranteza;
}

Token:: operator double() const
{
	return val.numar;
}

ETipToken Token::getTip() const
{
	return tip;
}


std::ostream& operator<<(std::ostream& out, ETipOperator op)
{
	switch (op)
	{
	case ETipOperator::plus:
		out << "+";
		break;
	case ETipOperator::minus:
		out << "-";
		break;
	case ETipOperator::steluta:
		out << "*";
		break;
	case ETipOperator::caret:
		out << "^";
		break;
	case ETipOperator::slash:
		out << "/";
		break;
	case ETipOperator::hash:
		out << "#";
		break;
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, ETipParanteza paranteza)
{
	switch (paranteza)
	{
	case ETipParanteza::deschisa:
		out << "(";
		break;
	case ETipParanteza::inchisa:
		out << ")";
		break;
	case ETipParanteza::pDeschisa:
		out << "[";
		break;
	case ETipParanteza::pInchisa:
		out << "]";
		break;
	}
	return out;
}
std::ostream& operator<<(std::ostream& out, const Token& t)
{
	switch (t.tip)
	{
	case ETipToken::operatie:
		out << t.val.op;
		break;
	case ETipToken::parenteza:
		out << t.val.paranteza;
		break;
	case ETipToken::numar:
		out << t.val.numar;
		break;
	default:
		out << "Necunoscut";
		break;
	}
	return out;
}


istream& operator >>(istream& in, Token& t)
{
	for (char primul = in.get(); !in.eof(); primul = in.get())
	{
		switch (primul)
		{
		case ' ':
			continue;
		case '+':
			t = Token(ETipOperator::plus);
			break;
		case '-':
			t = Token(ETipOperator::minus);
			break;
		case '*':
			t = Token(ETipOperator::steluta);
			break;
		case '^':
			t = Token(ETipOperator::caret);
			break;
		case '/':
			t = Token(ETipOperator::slash);
			break;
		case '#':
			t = Token(ETipOperator::hash);
			break;
		case Token::kParantezaRDeschisa:
			t = Token(ETipParanteza::deschisa);
			break;
		case ')':
			t = Token(ETipParanteza::inchisa);
			break;
		case '[':
			t = Token(ETipParanteza::pDeschisa);
			break;
		case ']':
			t = Token(ETipParanteza::pInchisa);
			break;
		case '\n':
			t = Token();
			break;
		default:
			in.unget();
			double value = 0;
			in >> value;
			if (!in.fail())
			{
				t = Token(value);
				return in;
			}
			else
			{
				in.clear();
				throw runtime_error("Caracter necunoscut!");
			}
		}
		break;
	}
	return in;
}

class VectorToken
{
private:
	int dimensiune;
	Token* vector = nullptr;
public:
	VectorToken();
	VectorToken(const VectorToken& v);
	const VectorToken& operator=(const VectorToken& v);
	VectorToken& operator +=(const Token& t);
	const Token& operator[](int pozitie) const;

	int getDimensiuneVectorToken() const;
	Token* getVectorToken();

	~VectorToken();

	friend std::ostream& operator<<(std::ostream& out, const VectorToken& v);
	friend std::istream& operator>>(std::istream& in, VectorToken& v);
};

VectorToken::VectorToken() :
	dimensiune(0)
{
	vector = nullptr;
}

VectorToken::VectorToken(const VectorToken& v)
{
	this->vector = new Token[v.dimensiune];
	this->dimensiune = v.dimensiune;
	for (int i = 0; i < v.dimensiune; i++)
	{
		this->vector[i] = v.vector[i];
	}
}
const VectorToken& VectorToken:: operator=(const VectorToken& v)
{
	if (this->vector != nullptr)
	{
		delete[] vector;
		vector = nullptr;
	}
	this->dimensiune = v.dimensiune;
	this->vector = new Token[v.dimensiune];
	for (int i = 0; i < v.dimensiune; i++)
	{
		this->vector[i] = v.vector[i];
	}
	return *this;
}

VectorToken& VectorToken::operator +=(const Token& t)
{
	Token* vector_nou = new Token[this->dimensiune + 1];
	for (int i = 0; i < this->dimensiune; i++)
	{
		vector_nou[i] = vector[i];
	}
	vector_nou[dimensiune++] = t;
	delete[] vector;
	vector = vector_nou;
	return *this;
}
const Token& VectorToken:: operator[](int pozitie) const
{
	return vector[pozitie];
}
int VectorToken::getDimensiuneVectorToken() const
{
	return this->dimensiune;
}
Token* VectorToken::getVectorToken()
{
	return this->vector;
}
VectorToken:: ~VectorToken()
{
	if (this->vector != nullptr)
	{
		delete[] vector;
		vector = nullptr;
	}
}

std::ostream& operator<<(std::ostream& out, const VectorToken& v)
{
	for (int i = 0; i < v.dimensiune; i++)
	{
		out << v.vector[i] << endl;

	}
	return out;
}

std::istream& operator>>(std::istream& in, VectorToken& v)
{
	while (in.good())
	{
		Token t;
		in >> t;
		if (t.getTip() != ETipToken::sfarsit) {
			v += t;
		}
		else {
			break;
		}
	}
	if (v.getDimensiuneVectorToken() == 0) {
		throw runtime_error("expresie nula");
	}
	return in;
}



class EvalResult {
	double rezultat = 0;
	char* eroare = nullptr;
public:
	explicit EvalResult(const char* e);

	EvalResult(double r);
	EvalResult();
	~EvalResult();
	EvalResult(const EvalResult& r);

	EvalResult& operator = (const EvalResult& r);

	operator bool() const;

	double getRezultat() const;

	const char* getEroare() const;

	friend std::ostream& operator << (std::ostream& str, const EvalResult& r);
};

EvalResult::EvalResult(const char* e)
{
	int l = strlen(e) + 1;
	this->eroare = new char[l + 1];
	memcpy(this->eroare, e, l);
}

EvalResult::EvalResult(double r) :
	rezultat(r == 0.0 ? abs(r) : r)
{
}
EvalResult::EvalResult() :
	EvalResult("Necunoscut")
{
}

EvalResult::~EvalResult()
{
	delete[] eroare;
}

EvalResult::EvalResult(const EvalResult& r) :
	rezultat(r.rezultat)
{
	if (r.eroare)
	{
		int l = strlen(r.eroare) + 1;
		eroare = new char[l];
		memcpy(eroare, r.eroare, l);
	}
}

EvalResult& EvalResult::operator = (const EvalResult& r)
{
	rezultat = r.rezultat;
	delete[] eroare;
	eroare = nullptr;
	if (r.eroare) {
		int l = strlen(r.eroare) + 1;
		eroare = new char[l];
		memcpy(eroare, r.eroare, l);
	}
	return *this;
}

EvalResult:: operator bool() const
{
	return eroare == nullptr;
}

double  EvalResult::getRezultat() const
{
	return rezultat;
}

const char* EvalResult::getEroare() const
{
	return eroare;
}

std::ostream& operator << (std::ostream& str, const EvalResult& r)
{
	if (r) {
		str << r.getRezultat();
	}
	else {
		str << r.getEroare();
	}
	return str;
}

class Expresie
{
public:
	virtual ~Expresie() = default;
	virtual EvalResult val() = 0;
	virtual unsigned prioritate() = 0;
	virtual bool completa() = 0;

	Expresie* inlocuieste(Expresie* nou);
	Expresie* getParinte();


	void setParinte(Expresie* p);

protected:
	virtual Expresie* inlocuiesteContinut(Expresie* nou) = 0;

private:
	Expresie* parinte = nullptr;
};


Expresie* Expresie::inlocuieste(Expresie* nou)
{
	if (nou) {
		nou->setParinte(this);
	}
	return inlocuiesteContinut(nou);
}

Expresie* Expresie::getParinte()
{
	return parinte;
}

void Expresie::setParinte(Expresie* p)
{
	parinte = p;
}

class Expresie1 : public Expresie
{
protected:
	Expresie* c1;
public:
	Expresie1(Expresie* c1);
	Expresie1();

	Expresie1(const Expresie1&) = delete;
	Expresie1& operator=(const Expresie1&) = delete;
	~Expresie1();

	bool completa() override;

protected:
	Expresie* inlocuiesteContinut(Expresie* nou) override;
};

Expresie1::Expresie1(Expresie* c1)
{
	this->c1 = c1;
	if (this->c1) {
		this->c1->setParinte(this);
	}
}

Expresie1::Expresie1() :
	c1(nullptr)
{}

Expresie1::~Expresie1()
{
	delete c1;
}

bool Expresie1::completa()
{
	return c1 != nullptr;
}

Expresie* Expresie1::inlocuiesteContinut(Expresie* nou)
{
	Expresie* vechi = this->c1;
	this->c1 = nou;
	return vechi;
}

class Expresie2 : public Expresie
{
protected:
	Expresie* c1;
	Expresie* c2;
public:
	Expresie2(Expresie* c1, Expresie* c2);

	Expresie2();

	void setC1(Expresie* ex);

	Expresie2(const Expresie2&) = delete;
	Expresie2& operator=(const Expresie2&) = delete;
	~Expresie2();

	bool completa() override;
protected:
	Expresie* inlocuiesteContinut(Expresie* nou) override;
};

Expresie2::Expresie2(Expresie* c1, Expresie* c2)
{
	this->c1 = c1;
	if (c1) {
		c1->setParinte(this);
	}
	this->c2 = c2;
	if (c2) {
		c2->setParinte(this);
	}
}

Expresie2::Expresie2() :
	Expresie2(nullptr, nullptr)
{}

void Expresie2::setC1(Expresie* ex)
{
	delete this->c1;
	this->c1 = ex;
	if (this->c1) {
		this->c1->setParinte(this);
	}
}

Expresie2::~Expresie2()
{
	delete c1;
	delete c2;
}


bool Expresie2::completa()
{
	return c1 != nullptr && c2 != nullptr;
}

Expresie* Expresie2::inlocuiesteContinut(Expresie* nou)
{
	Expresie* vechi = this->c2;
	this->c2 = nou;
	return vechi;
}


class ExpresieAdunare : public Expresie2
{
	using Expresie2::Expresie2;
	EvalResult val() override;
	unsigned prioritate() override;
};


EvalResult ExpresieAdunare::val()
{
	EvalResult e1 = c1->val();
	if (!e1) {
		return e1;
	}
	EvalResult e2 = c2->val();
	if (!e2) {
		return e2;
	}
	return e1.getRezultat() + e2.getRezultat();
}

unsigned  ExpresieAdunare::prioritate()
{
	return 1;
}

class ExpresieConstanta : public Expresie {

	const double cst;

public:
	ExpresieConstanta(double val);

	bool completa() override;

	unsigned prioritate() override;

	EvalResult val() override;

protected:
	Expresie* inlocuiesteContinut(Expresie*) override;
};


ExpresieConstanta::ExpresieConstanta(double val) : cst(val)
{
}
bool ExpresieConstanta::completa()
{
	return true;
}

unsigned ExpresieConstanta::prioritate()
{
	return 5;
}

EvalResult ExpresieConstanta::val()
{
	return cst;
}

Expresie* ExpresieConstanta::inlocuiesteContinut(Expresie*)
{
	return nullptr;
}

class ExpresieIdentitate : public Expresie1
{
public:
	ExpresieIdentitate(unsigned p, Expresie* c);

	EvalResult val() override;

	unsigned prioritate() override;

private:
	unsigned p;
};

ExpresieIdentitate::ExpresieIdentitate(unsigned p, Expresie* c) :
	Expresie1(c),
	p(p)
{
}

EvalResult ExpresieIdentitate::val()
{
	return c1->val();
}

unsigned ExpresieIdentitate::prioritate()
{
	return p;
}


class ExpresieImpartire : public Expresie2
{
	EvalResult val() override;
	unsigned prioritate() override;
};

EvalResult ExpresieImpartire::val()
{
	EvalResult e1 = c1->val();
	if (!e1) {
		return e1;
	}
	EvalResult e2 = c2->val();
	if (!e2) {
		return e2;
	}

	if (e2.getRezultat() == 0) {
		return EvalResult("Impartirea la 0 nu are sens");
	}

	return e1.getRezultat() / e2.getRezultat();
}

unsigned ExpresieImpartire::prioritate()
{
	return 2;
}

class ExpresieInmultire : public Expresie2
{
	EvalResult val() override;
	unsigned prioritate() override;
};

EvalResult  ExpresieInmultire::val()
{
	EvalResult e1 = c1->val();
	if (!e1) {
		return e1;
	}
	EvalResult e2 = c2->val();
	if (!e2) {
		return e2;
	}
	return e1.getRezultat() * e2.getRezultat();
}

unsigned  ExpresieInmultire::prioritate()
{
	return 2;
}


class ExpresieInvers : public Expresie1
{
	using Expresie1::Expresie1;
	EvalResult val() override;
	unsigned prioritate() override;
};

EvalResult ExpresieInvers::val()
{
	auto cv = c1->val();
	if (cv) {
		return -cv.getRezultat();
	}
	else {
		return cv;
	}
}

unsigned ExpresieInvers::prioritate()
{
	return 4;
}


class ExpresiePutere : public Expresie2
{
	EvalResult val() override;
	unsigned prioritate() override;
};

EvalResult ExpresiePutere::val()
{
	EvalResult e1 = c1->val();
	if (!e1) {
		return e1;
	}
	EvalResult e2 = c2->val();
	if (!e2) {
		return e2;
	}

	if (abs(e1.getRezultat()) < 1e-7 && abs(e2.getRezultat()) < 1e-7) {
		return EvalResult("0 la puterea 0 nu are sens");
	}

	return pow(e1.getRezultat(), e2.getRezultat());
}

unsigned ExpresiePutere::prioritate()
{
	return 3;
}

class ExpresieRadical : public Expresie2
{
	EvalResult val() override;
	unsigned prioritate() override;

};

EvalResult ExpresieRadical::val()
{
	EvalResult e1 = c1->val();
	if (!e1) {
		return e1;
	}
	EvalResult e2 = c2->val();
	if (!e2) {
		return e2;
	}

	double sub = e1.getRezultat();
	double ordin = e2.getRezultat();
	return pow(sub, 1 / ordin);
}

unsigned  ExpresieRadical::prioritate()
{
	return 3;
}

class ExpresieScadere : public Expresie2
{
	EvalResult val() override;
	unsigned prioritate() override;
};

EvalResult ExpresieScadere::val()
{
	EvalResult e1 = c1->val();
	if (!e1) {
		return e1;
	}
	EvalResult e2 = c2->val();
	if (!e2) {
		return e2;
	}
	return e1.getRezultat() - e2.getRezultat();
}

unsigned ExpresieScadere::prioritate()
{
	return 1;
}

class ParseResult {
private:
	bool ok;
	EvalResult rezultat;
	Token t;
	int pozitie;
public:
	ParseResult(Token t, int pozitie);
	ParseResult(EvalResult rezultat);
	ParseResult();

	bool success() const;
	EvalResult getRezultat() const;

	Token getTokenErr() const;
	int getPozitieErr() const;

	friend std::ostream& operator << (std::ostream& str, const ParseResult& r);
	friend std::istream& operator >> (std::istream& in, ParseResult& pr);

	static ParseResult parseazaTokens(const VectorToken& v);

private:
	static Expresie* parseazaTokens(const VectorToken& v, int& i, bool rDeschisa, bool pDeschisa);
};

ParseResult::ParseResult(Token t, int pozitie) :
	ok(false),
	t(t),
	pozitie(pozitie)
{}

ParseResult::ParseResult(EvalResult rezultat) :
	ok(true),
	rezultat(rezultat),
	pozitie(0)
{}

ParseResult::ParseResult() :
	ok(false),
	pozitie(0)
{
}


bool ParseResult::success() const
{
	return ok;
}

EvalResult ParseResult::getRezultat() const
{
	return rezultat;
}

Token ParseResult::getTokenErr() const
{
	return t;
}

int ParseResult::getPozitieErr() const
{
	return pozitie;
}


Expresie* ParseResult::parseazaTokens(const VectorToken& v, int& i, bool rDeschisa, bool pDeschisa)
{
	ExpresieIdentitate radacina(0, nullptr);
	Expresie* fresh = &radacina;
	bool oRotunda = false;

	for (; i < v.getDimensiuneVectorToken(); ++i)
	{
		bool trebuieCompleta = false;
		Expresie* deAdaugat = nullptr;
		bool eBinara = false;

		switch (v[i].getTip()) {
		case ETipToken::numar:
			trebuieCompleta = false;
			deAdaugat = new ExpresieConstanta((double)v[i]);
			break;

		case ETipToken::operatie: {
			// distingem intre minus unar si scadere
			ETipOperator op = (ETipOperator)v[i];
			if (op == ETipOperator::minus && !fresh->completa()) {
				trebuieCompleta = false;
				deAdaugat = new ExpresieInvers(nullptr);
				break;
			}


			// sigur nu e minus unar
			trebuieCompleta = true;
			eBinara = true;

			switch ((ETipOperator)v[i])
			{
			case ETipOperator::caret:
				deAdaugat = new ExpresiePutere();
				break;
			case ETipOperator::hash:
				deAdaugat = new ExpresieRadical();
				break;
			case ETipOperator::minus:
				deAdaugat = new ExpresieScadere();
				break;
			case ETipOperator::plus:
				deAdaugat = new ExpresieAdunare();
				break;
			case ETipOperator::slash:
				deAdaugat = new ExpresieImpartire();
				break;
			case ETipOperator::steluta:
				deAdaugat = new ExpresieInmultire();
				break;
			}

			break;
		}
		case ETipToken::parenteza: {
			ETipParanteza paranteza = (ETipParanteza)(v[i]);
			switch (paranteza)
			{
			case ETipParanteza::deschisa:
			case ETipParanteza::pDeschisa:
			{
				trebuieCompleta = false;
				bool newrDeschisa = paranteza == ETipParanteza::deschisa;
				bool newpDeschisa = paranteza == ETipParanteza::pDeschisa;
				oRotunda = oRotunda || newrDeschisa;

				if (newrDeschisa && rDeschisa)
				{
					return nullptr;
				}

				if (newpDeschisa && (pDeschisa || rDeschisa)) {
					return nullptr;
				}
				Expresie* pResult = parseazaTokens(v, ++i, newrDeschisa, newpDeschisa);
				if (!pResult || !pResult->completa()) {
					delete pResult;
					return nullptr;
				}
				deAdaugat = new ExpresieIdentitate(5, pResult);
				break;
			}
			case ETipParanteza::inchisa:
				if (!rDeschisa) {
					return nullptr;
				}
				return radacina.inlocuieste(nullptr);
			case ETipParanteza::pInchisa:
				if (!pDeschisa || !oRotunda) {
					return nullptr;
				}
				return radacina.inlocuieste(nullptr);
			}
			break;
		}
		default:
			return nullptr;
		}

		if (fresh->completa() != trebuieCompleta || !deAdaugat) {
			delete deAdaugat;
			return nullptr;
		}

		Expresie* slot = fresh;
		while (slot && slot->completa() && slot->prioritate() >= deAdaugat->prioritate()) {
			slot = slot->getParinte();
		}
		if (!slot) {
			delete deAdaugat;
			return nullptr;
		}

		fresh = deAdaugat;
		Expresie* vechi = slot->inlocuieste(deAdaugat);
		if (eBinara) {
			((Expresie2*)deAdaugat)->setC1(vechi);
		}
		else if (vechi) {
			delete vechi;
			delete deAdaugat;
			return nullptr;
		}
	}

	if (pDeschisa || rDeschisa || !fresh->completa()) {
		// o paranteza deschisa nu a fost inchisa
		// sau o expresie nu a fost terminata
		return nullptr;
	}
	return radacina.inlocuieste(nullptr);
}

ParseResult ParseResult::parseazaTokens(const VectorToken& v)
{
	int i = 0;
	Expresie* e = parseazaTokens(v, i, false, false);
	if (e) {
		EvalResult rezultat = e->val();
		delete e;
		return ParseResult(rezultat);
	}
	return i < v.getDimensiuneVectorToken() ? ParseResult(v[i], i) :
		ParseResult(Token(), i);
}

std::ostream& operator << (std::ostream& str, const ParseResult& r)
{
	if (!r.success()) {
		str << "Eroare la parsare. ";
		Token t = r.getTokenErr();
		if (t.getTip() != ETipToken::sfarsit) {
			str << "Token neasteptat : " << r.getTokenErr()
				<< ". Pe pozitia " << r.getPozitieErr();
		}
		else {
			str << "Expresie incompleta";
		}
	}
	else {
		str << r.getRezultat();
	}
	return str;
}

std::istream& operator >> (std::istream& in, ParseResult& r)
{
	VectorToken v;
	in >> v;
	r = ParseResult::parseazaTokens(v);
	return in;
}


int mainEx(istream& in)
{
	try {
		ParseResult pr;
		in >> pr;
		cout << pr << endl;
	}
	catch (const exception& e) {
		cout << e.what() << std::endl;
		return -1;
	}
}

int main(int argc, char* argv[])
{
	
	if (argc == 1)
	{
		return mainEx(cin);
	}
	else if(argc == 2)
	{
		istringstream ss(argv[1]);
		return mainEx(ss);
	}
	cout << "Parametrii necunoscuti!" << endl;
	return -1;
}
