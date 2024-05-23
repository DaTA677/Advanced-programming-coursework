#include"Polynomial.h"
#include<iostream>
#include<string>
#include<cmath>



Term::Term() {
		coeff = -1;
		exponent = -1;
		sign = '*';
	}
Term::Term(int coeff, int exponent, char sign='*') {
    if (sign != '*') {
        this->coeff = coeff;
        this->exponent = exponent;
        this->sign = sign;
    }
    else {
        this->set_coeff(coeff);
        this->exponent = exponent;
    }
		
	}

int Term::calculate_value(int x_val) {
		return sign=='+'?coeff * (pow(x_val, exponent)): -(coeff * (pow(x_val, exponent)));
	}
int Term::get_coeff() {
		return coeff;
}
int Term::get_exponent() {
		return exponent;
}
char Term::get_sign() {
	return sign;
}
void Term::set_coeff(int coeff) {
	sign = coeff >= 0 ? '+' : '-';
	this->coeff = abs(coeff);
}
void Term::set_exponent(int exponent) {
	if (exponent >= 0 && exponent<=4) {
		this->exponent = exponent;
	}
}

Polynomial::Polynomial(int no_of_terms, Term** terms) {
		this->no_of_terms = no_of_terms;
		this->terms = terms;
}

Polynomial::Polynomial(int no_of_terms, std::vector<Term*> termarr) {
    this->no_of_terms = no_of_terms;
    this->terms = new Term * [no_of_terms];
    for (int i = 0; i < termarr.size(); i++) {
        this->terms[i] = termarr[i];
    }
}


void Polynomial::print() {
	if (terms[0]->get_coeff() != 0) {
		if (terms[0]->get_sign() == '-') {
			std::cout << '-';
		}
		std::cout << this->terms[0]->get_coeff();
		if(this->terms[0]->get_exponent() != 0) std::cout<< "x^" << this->terms[0]->get_exponent();
	}
	for (int i = 1; i < this->no_of_terms; i++) {
		std::cout << this->terms[i]->get_sign() << this->terms[i]->get_coeff();
		if (this->terms[i]->get_exponent() != 0) std::cout << "x^" << this->terms[i]->get_exponent(); 
	}
	std::cout << "\n";
}
std::string Polynomial::find_output_set(const int& start, const int& finish) {
	std::string return_val = "";
	for (int i = start; i < finish; i++) {
		int sum = 0;
		for (int j = 0; j < this->no_of_terms; j++) {
			sum+= this->terms[j]->calculate_value(i);
		}
		return_val+= std::to_string(sum) +',';}
	int sum = 0;
	for (int j = 0; j < this->no_of_terms; j++) {
		sum += this->terms[j]->calculate_value(finish);}
	return_val+= std::to_string(sum)+'\n';
	return return_val;
}

std::string Polynomial::get_expression() {
    std::string out = "";
    if (terms[0]->get_coeff() != 0) {
        if (terms[0]->get_sign() == '-') {
            out+= '-';
        }
        out+= std::to_string(this->terms[0]->get_coeff());
        if (this->terms[0]->get_exponent() != 0) out+= "x^" + std::to_string(this->terms[0]->get_exponent());
    }
    for (int i = 1; i < this->no_of_terms; i++) {
        out+= this->terms[i]->get_sign() + std::to_string( this->terms[i]->get_coeff());
        if (this->terms[i]->get_exponent() != 0) out += "x^" + std::to_string(this->terms[i]->get_exponent());
    }
    out+="\n";
    return out;
}

Polynomial::~Polynomial() {
	delete[] terms;
}

int find_nearest_x(std::vector<int> values, Term* term1) {
    int x = 0;
    char sign = term1->get_sign();
    int value2 = values[0];
    while (true) {
        int value1 = term1->get_coeff() * (pow(x, term1->get_exponent()));
        value1 = sign == '+' ? value1 : -value1;
        if (value1 >= value2 || (sign == '-' && value1 <= value2)) break;
        x++;
    }

    return x;
}

void update_values(std::vector<int>& values, Term* term1, int x) {
    for (int i = 0; i < values.size(); i++) {
        int value1 = term1->get_coeff() * (pow(x + i, term1->get_exponent()));
        value1 = term1->get_sign() == '+' ? value1 : -value1;
        values[i] -= value1;
    }
}

int count_common_elements(std::vector<int> values) {
    int count = 0;
    for (int i = 0; i < values.size() - 1 && values[i] == values[i + 1]; i++, count++);
    return count;
}

bool is_ascending(std::vector<int>values) {
    int count_a = 0, count_d = 0;
    for (int i = 0; i < values.size() - 1; i++) {
        if (values[i] < values[i + 1]) {
            count_a += 1;
            count_d -= 1;
        }
        else {
            count_a -= 1;
            count_d += 1;
        }
    }
    return count_a > count_d;
}

bool check_coeff(int& coeff, int& x, int& count, bool& update_flag, const std::vector<int> &arr) {
    if (!update_flag && (coeff > 9 || coeff < -9) && x != -1) {

        if (is_ascending(arr) && x > 0 && !update_flag) {
            x--;
            count++;
        }
        else if (count > 0) {
            x = x + count;
            update_flag = true;
            count = 0;
        }
        else {
            x++;
        }
        return true;

    }
    return false;
}

Polynomial* find_polynomial(std::vector<int> values) {
    std::vector<Term*> termarr;
    int count3 = 0;
    bool flag = false;
    int x = -1;
    std::vector<int> copy_vector;
    int x_count = 0;
    copy(values.begin(), values.end(), back_inserter(copy_vector));
    while (count3 < 4) {
        int diff_count = 0;
        std::vector<int> new_vector;
        copy(values.begin(), values.end(), back_inserter(new_vector));
        const int fact[4] = { 1,2,6,24 };
        while (diff_count <= 4) {
            diff_count++;
            for (int i = 0; i < new_vector.size() - 1; i++) {
                new_vector[i] = new_vector[i + 1] - new_vector[i];
            }
            if (count_common_elements(new_vector) == new_vector.size() - diff_count - 1) {
                int coeff = new_vector[0] / fact[(diff_count > 0) ? diff_count - 1 : diff_count];
                Term* term1;
                if (check_coeff(coeff, x, x_count, flag, copy_vector)) {
                    values.clear();
                    copy(copy_vector.begin(), copy_vector.end(), back_inserter(values));
                    while (termarr.size() > 1) termarr.pop_back();
                    count3 = 0;
                    update_values(values, termarr[0], x);
                    break;
                }
                term1 =new Term(coeff,diff_count);
                termarr.push_back(term1);
                if (x == -1) {
                    x = find_nearest_x(values, term1);
                }
                update_values(values, term1, x);
                break;
            }
        }
        if (diff_count == 0) break;
        if (count_common_elements(values) == values.size() - 1) {
            if (values[0] != 0) {
                int coeff = values[0];
                Term* term1 = new Term(coeff, 0);
                termarr.push_back(term1);
            }
            break;
        }
        count3++;
    }
    return  new Polynomial(termarr.size(), termarr);
}



