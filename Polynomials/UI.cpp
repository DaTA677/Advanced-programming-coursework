#include<fstream>
#include<stdexcept>
#include<exception>
#include<iostream>
#include<string>
#include<functional>
#include<vector>

#include"Polynomial.h"
#include"UI.h"

template<typename T>
bool check_input(T input, std::function<bool(T)> comparator) { 
	if (comparator(input)) {
		std::cout << "\nWrong input!!\nRe-enter the correct value when prompted\n";
		return true;
	}
	return false;
}


Term* receive_Term() {
	int coeff=-1;
	int exponent=-1;
	char sign='*';
	do {
		std::cout << "Enter the sign (+ or -):";
		std::cin.clear();
		std::cin >> sign;
	} while (check_input<char>(sign, [](char sign) {return sign != '-' && sign != '+'; }));
	do {
		std::cout << "Enter the exponent(0-4):";
		std::cin.clear();
		std::cin >> exponent;
	} while ((check_input<int>(exponent, [](int exponent) {return exponent < 0 || exponent>4; })));
	do {
		std::cout << ((exponent == 0) ? "Enter the constant(1 to 1000):" : "Enter the coeff(1 to 9):");
		std::cin.clear();
		std::cin >> coeff;
	} while (check_input<int>(coeff, ((exponent == 0) ? ([](int coeff) {return coeff < 1 || coeff>1000; }) : ([](int coeff) {return coeff < 1 || coeff>9; }))));
	return new Term(coeff, exponent, sign);
}

Polynomial* receive_polynomial() {
	int no_of_terms;
	do {
		std::cout << "Enter the number of terms(1 to 4):";
		std::cin.clear();
		std::cin >> no_of_terms;
	} while ((check_input<int>(no_of_terms, [](int no_of_terms) {return no_of_terms < 1 || no_of_terms>4; })));
	
	Term** terms = new Term * [no_of_terms];
	for (int i = 0; i < no_of_terms; i++) {
		std::cout << "Enter the details of the term " << (i + 1) << ":\n";
		terms[i] = receive_Term();
	}
	return new Polynomial(no_of_terms, terms);

}
std::string generate_output_set(Polynomial* poly) {
	int start, end;
	std::cout << "Enter the start value:";
	std::cin >> start;
	std::cout << "Enter the end value:";
	std::cin >> end;
	std::string output = poly->find_output_set(start, end);
	return output;
}


void store_output(std::string output, std::string file_name) throw (std::invalid_argument) {
	std::ofstream out_file;
	out_file.open(file_name + ".txt", std::ios::app);
	if (out_file.fail()) {
		throw std::invalid_argument("Wrong file Name!!");
	}
	out_file << output;
	out_file.close();
}

void store_in_file(std::string output) {
	std::cout << "Enter file name to store the output:";
	std::string file_name;
	std::cin.clear();
	std::cin >> file_name;
	
	try {
		store_output(output, file_name);
	}
	catch (const std::invalid_argument& iae) {
		std::cout << "Error:" << iae.what();
	}

}

std::vector<std::string> read_from_file(std::string input)throw (std::invalid_argument) {
	std::ifstream i_file;
	i_file.open(input + ".txt");
	std::vector<std::string> lines;
	if (i_file.fail()) {
		i_file.close();
		throw std::invalid_argument("Wrong file Name!!");
	}
	else {
		std::string line="nostring";
		while (std::getline(i_file, line)) {
			lines.push_back(line+'\n');
		}
		i_file.close();
	}
	return lines;
}

void display_output_set() {
	std::cout << "Enter name of txt file to display:";
	std::string file_name;
	std::cin.clear();
	std::cin >> file_name;
	try {
		std::vector<std::string>lines=read_from_file(file_name);
		for (auto val : lines) {
			std::cout << val;
		}
		std::cin.ignore();
		system("pause");		
	}
	catch (const std::invalid_argument& iae) {
		std::cout << "Error:" << iae.what();
	}
}

std::vector<int> get_values_from_string(std::string line) {
	std::vector<int> values;
	int last = 0;
	while (last != line.size() && line.find(',', last) != -1) {
		values.push_back(stoi(line.substr(last, line.find(',', last))));
		last = line.find(',', last) + 1;
	}
	if (last != line.size())values.push_back(stoi(line.substr(last, line.size())));
	return values;
}

void generate_expressions() {
	std::cout << "Enter name of txt file to read the output sets:";
	std::string file_name;
	std::cin.clear();
	std::cin >> file_name;
	std::string expressions="";
	try {
		std::vector<std::string>lines=read_from_file(file_name);
		for (int i = 0; i < lines.size(); i++) {
			Polynomial* poly = find_polynomial(get_values_from_string(lines[i]));
			expressions += poly->get_expression();
			delete poly;
		}
		std::cout << expressions;
		store_in_file(expressions);
	}
	catch (const std::invalid_argument& iae) {
		std::cout << "Error:" << iae.what();
	}
}

void create_polynomial() {
	Polynomial* poly = receive_polynomial();
	std::cout << "The polynomial is: ";
	poly->print();
	std::string output = generate_output_set(poly);
	std::cout << output;
	store_in_file(output);
	delete poly;
}


void menu() {
	while (true) {
		system("cls");
		std::cout << "Welcome.\n1.Generate output set\n2.Display from file\n3.Generate Polynomial expression from output sets\n4.Exit\n";
		int choice;
	get_choice:
		std::cout << "Enter your choice:";
		std::cin.clear();
		std::cin >> choice;

		switch (choice) {
		case 1: {
			create_polynomial();
			break;
		}
		case 2:
			display_output_set();
			
			break;
		case 3:
			generate_expressions();
			break;
		case 4:
			exit(0);
			break;
		default:std::cout << "Choice not available! Please enter the correct choice\n";
			goto get_choice;
		}
	}
}