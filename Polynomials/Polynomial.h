#pragma once
#include<string>
#include<vector>
class Term {
private:
	int coeff;
	int exponent;
	char sign;
public:
	Term();
	Term(int coeff, int exponent,char sign);
	int calculate_value(int x_val);
	int get_coeff();
	int get_exponent();
	char get_sign();
	void set_coeff(int val);
	void set_exponent(int val);
};

class Polynomial {
private:
	int no_of_terms;
	Term** terms;
public:
	Polynomial(int no_of_terms, Term** terms);
	Polynomial(int no_of_terms, std::vector<Term*> termarr);
	void print();
	std::string find_output_set(const int& start,const int& finish);
	std::string get_expression();
	~Polynomial();
};

Polynomial* find_polynomial(std::vector<int> values);