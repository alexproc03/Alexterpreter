#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <stack>
#include <cmath>
using namespace std;

// Holds 2 values, value and type
class Var {
private:
    string value;
    string type;
public:
    Var() : value(""), type("") {}
    Var(string value, string type) {
        this->value = value;
        this->type = type;
    }
    string getVal() {
        return value;
    }
    void setVal(string value) {
        //TODO: Implement type checking logic
        this->value = value;
    }
    string getType() {
        return type;
    }
};

// Holds contents and indentLevel, which acts as a "pseudoscope"
class Line {
    private:
    string contents;
    int indentLevel;
    public:
    Line(string contents, int indentLevel) {
        this->contents = contents;
        this->indentLevel = indentLevel;
    }
    string getContents() {
        return contents;
    }
    int getIndentLevel(){
        return indentLevel;
    }
};

// state of program, which is global
unordered_map<string, Var> s;

// helper function, which returns true if string is in list
bool contains(list<string> list, string target) {
    return (find(list.begin(), list.end(), target) != list.end());
}

// returns true if variable has been defined
bool is_in_state(string var_name, unordered_map<string, Var> state) {
    if (state.find(var_name) == state.end()) {
        return false;
    }
    else {
        return true;
    }
}

// returns variable if it exists in state
Var varmap(string var_name, unordered_map<string,  Var> state) {
    if (is_in_state(var_name, state)) {
        
        Var var = state[var_name];
        return var;
    }
    else {
        throw runtime_error("\"" + var_name + "\" not defined");
    }    
}

// strips input of all spaces and newlines
string remove_whitespace(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    return str;
}

// returns true if string can be converted into a postive integer or 0
bool is_number(const string &s) {
  for (char c: s) {
    string digits = "0123456789";
    if (digits.find(c) == string::npos) {
        return false;
    }
  }
  return true;
}

// converts string representations of numbers into an int. Only works for zero and positive integers
int MDEC(string number) {
    if (is_number(number)) {
        int sum = 0;
        int multiplier = pow(10, number.length()-1);
        for (char c : number) {
            switch (c) {
                case '0':
                    sum += multiplier * 0;
                    break;
                case '1':
                    sum += multiplier * 1;
                    break;
                case '2':
                    sum += multiplier * 2;
                    break;
                case '3':
                    sum += multiplier * 3;
                    break;
                case '4':
                    sum += multiplier * 4;
                    break;
                case '5':
                    sum += multiplier * 5;
                    break;
                case '6':
                    sum += multiplier * 6;
                    break;
                case '7':
                    sum += multiplier * 7;
                    break;
                case '8':
                    sum += multiplier * 8;
                    break;
                case '9':
                    sum += multiplier *9;
                    break;
            }
            multiplier /= 10;
        }
        return sum;
    }
    else {
        throw runtime_error("\"" + number + "\" is not a number\n");
    }
}

// parses an expression into operators and identifier tokens
list<string> parse_expr(string expr) {
    list<string> tokens;
    string operators = "+-*/%()<>";
    list<string> multi_operators = {"&&", "||", "==", "<=", ">=", "!="};
    expr = remove_whitespace(expr);
    //parse expression into tokens
    int index = 0;
    while (expr.length() != 0) {
        if (index == expr.length()) {
            throw runtime_error("invalid expression");
        }
        // check for two character long operators
        if (contains(multi_operators, expr.substr(index, 2))){
            if (index > 0) {
                tokens.push_back(expr.substr(0, index));
            }
            tokens.push_back(expr.substr(index, 2));
            expr = expr.substr(index+2,expr.length() - 1 - index);
            index = 0;
        }
        // check for single character operators
        else if (operators.find(expr.substr(index, 1)) != string::npos) {
            if (index > 0) {
                tokens.push_back(expr.substr(0, index));
            }
            tokens.push_back(expr.substr(index, 1));
            expr = expr.substr(index+1,expr.length() - 1 - index);
            index = 0;
        }
        // check if expression is just a boolean, is a variable in program state, or is a string TODO: this logic is lazy and wrong
        else if (expr == "true" || expr == "false" || is_in_state(expr, s) || is_number(expr) || (expr.find('\"') == 0 && expr.rfind('\"') == expr.length()-1)) {
            tokens.push_back(expr);
            expr = "";
        }
        else {
            index++;
        }
    }
    return tokens;
}

// determines type of single value
string determine_type(string value) {
    // variable
    if (is_in_state(value, s)) {
        return varmap(value, s).getType();
    }
    // boolean literal
    if (value == "true" || value == "false") {
        return "bool";
    }
    // integer
    else if (is_number(value)) {
        return "int";
    }
    // string
    else if (value[0] == '"' && value[value.length()-1] == '"') {
        return "string";
    }
    // invalid
    else {
        throw runtime_error("\"" + value + "\" is not of valid type");
    }
}
// determines type for a parsed expression
string determine_type(list<string> tokens) {
    // initial guess based on first token
    string initial_guess = determine_type(tokens.front());
    // automatically determines return type to be bool in the case that a bool operator exists
    for (string token : tokens) {
        if (token == "&&" || token == "||" || token == ">=" || token == "<=" || token == "=="|| token == "!=" || token == ">" || token == "<") {
            return "bool";
        }
    }
    return initial_guess;
}

// works with evaluate_expression to recursively work through parentheses
list<string> evaluate_parentheses(list<string> tokens) {
    //TODO: implement stack for dealing with parentheses, calling evaluate_expr on the insides of the parentheses
        // int startIndex = distance(tokens.begin(), find(tokens.begin(), tokens.end(), "("));
        // int endIndex = tokens.size() - distance(tokens.rbegin(), find(tokens.rbegin(), tokens.rend(), ")")) - 1;
        // cout << startIndex << ", " << endIndex << endl;
        
        // auto it = tokens.begin();
        // advance(it, startIndex);
        // for (int i = startIndex; i <= endIndex; i++) {
        //         string elemnt = *it;
        //         it = tokens.erase(it);
        // }
    return tokens;
}

// determines the value of a set of parsed tokens
string evaluate_expr(list<string> tokens) {
    //check if token is in varmap
    for (string& token : tokens) {
        if (is_in_state(token, s)) {
            token = varmap(token, s).getVal();
         }
    }
    // determine return type
    string return_type = determine_type(tokens);
    // base case, there is a single token
    if (tokens.size() == 1) {
        string token = tokens.front();
        if (is_number(token) || token == "true" || token == "false") {
            return token;
        }
        else {
            runtime_error("\"" + token + "\" is not a valid identifier\n");
        }
    }
    // evaluate parentheses using recursive helper function evaluate_parentheses
    if (contains(tokens, "(") || contains(tokens, ")")) {
        tokens = evaluate_parentheses(tokens);
    }
    // iterative case: evaluate operations in correct precedence
    if (return_type != "string") {
        auto it = tokens.begin();
        while (contains(tokens, "*") || contains(tokens, "/") || contains(tokens, "%")) {
            string token = *it;
            if (token == "*") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
            *it = to_string(operand1 * operand2);
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            else if (token == "/") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
                *it = to_string(operand1 / operand2);
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            else if (token == "%") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
                *it = to_string(operand1 % operand2);
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            std::advance(it, 1);
        }
        it = tokens.begin();
        while(contains(tokens, "+") || contains(tokens, "-")) {
            //Identify if current index is an operator
            string token = *it;
            if (token == "+") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
                *it = to_string(operand1 + operand2);
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            else if (token == "-") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
                *it = to_string(operand1 - operand2);
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            advance(it, 1);
        }
    }
    if (return_type == "bool") {
        auto it = tokens.begin();
        while(contains(tokens, "<") || contains(tokens, "<=") || contains(tokens, ">") || contains(tokens, ">=")) {
            //Identify if current index is an operator
            string token = *it;
            if (token == "<") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
                *it = (operand1 < operand2) ? "true" : "false";
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            else if (token == "<=") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
                *it = (operand1 <= operand2) ? "true" : "false";
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            else if (token == ">") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
                *it = (operand1 > operand2) ? "true" : "false";
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            else if (token == ">=") {
                int operand1 = MDEC(*prev(it));
                int operand2 = MDEC(*next(it));
                *it = (operand1 >= operand2) ? "true" : "false";
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            advance(it, 1);
        }
        while(contains(tokens, "==") || contains(tokens, "!=")) {
            string token = *it;
            if (token == "==") {
                string operand1 = *prev(it);
                string operand2 = *next(it);
                string type1 = determine_type(operand1);
                string type2 = determine_type(operand2);
                *it = (type1 == type2 && operand1 == operand2) ? "true" : "false";
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            else if (token == "!=") {
                string operand1 = *prev(it);
                string operand2 = *next(it);
                string type1 = determine_type(operand1);
                string type2 = determine_type(operand2);
                *it = (type1 != type2 || operand1 != operand2) ? "true" : "false";
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            advance(it, 1);
        }
        it = tokens.begin();
        while(contains(tokens, "&&") || contains(tokens, "||")) {
            string token = *it;
            if (token == "&&") {
                string operand1 = *prev(it);
                string operand2 = *next(it);
                bool bool_operand1, bool_operand2;
                if ((operand1 != "true" && operand1 != "false") || (operand2 != "true" && operand2 != "false")) {
                    throw runtime_error("invalid operands for operator");
                }
                else {
                    bool_operand1 = (operand1 == "true") ? true : false;
                    bool_operand2 = (operand2 == "true") ? true : false;
                }
                *it = (bool_operand1 && bool_operand2) ? "true" : "false";
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            else if (token == "||") {
                string operand1 = *prev(it);
                string operand2 = *next(it);
                bool bool_operand1, bool_operand2;
                if ((operand1 != "true" && operand1 != "false") || (operand2 != "true" && operand2 != "false")) {
                    throw runtime_error("invalid operands for operator");
                }
                else {
                    bool_operand1 = (operand1 == "true") ? true : false;
                    bool_operand2 = (operand2 == "true") ? true : false;
                }
                *it = (bool_operand1 || bool_operand2) ? "true" : "false";
                tokens.erase(prev(it));
                tokens.erase(next(it));
            }
            advance(it, 1);
        }
    }
    if (return_type == "string") {
        auto it = tokens.begin();
        while(contains(tokens, "+")) {
            string token = *it;
            string operand1 = *prev(it);
            string operand2 = *next(it);
            *it = operand1 + operand2;
            tokens.erase(prev(it));
            tokens.erase(next(it));
        }
    }
    if (tokens.size() == 1) {
        return tokens.front();
    }
    else {
        throw runtime_error("invalid expression");
    }
}

// overloaded version of evaluate_expr which parses the expression first
string evaluate_expr(string expr) {
        // parse expression
        list<string> tokens = parse_expr(expr);
        // call sister-function
        return evaluate_expr(tokens);
}


// 1 off statements that don't affect the "program counter"
void execute(string contents) {
        //remove leading whitespace
        contents = contents.substr(contents.find_first_not_of(" "));
        // assignment
        if (contents.length() >= 4 && contents.rfind("let ") == 0) {
            contents = remove_whitespace(contents.substr(4));
            string var = contents.substr(0, contents.find("="));
            string expr = contents.substr(contents.find("=") + 1);
            string val = evaluate_expr(expr);
            s[var] = Var(val, determine_type(val));
        }
        //print statements
        else if (contents.length() >= 7 && contents.rfind("println") == 0) {
            contents = contents.substr(contents.rfind("(") + 1, contents.find(")") - contents.rfind("(")-1);
            if (contents.empty()) {
                cout << endl;
            }
            else{
                cout << evaluate_expr(contents) << endl;
            }    
        }
        else if (contents.length() >= 5 && contents.rfind("print") == 0) {
            contents = contents.substr(contents.rfind("(") + 1, contents.find(")") - contents.rfind("(")-1);
            cout << evaluate_expr(contents);
        }
        else {
            throw runtime_error("\"" + contents + "\" is not a valid or known statement\n");
        }
}

// executes all lines in a list, is recursive
int execute(list<Line> lines, int currentLevel) {
    int lines_executed = 0;
    auto it = lines.begin();
    // boolean just for for loops
    bool first_it = true;
    while (it != lines.end()) {
        // unpack line contents and remove leading whitespace
        Line line = *it;
        string contents = line.getContents();
        contents = contents.substr(contents.find_first_not_of(" "));
        int indentLevel = line.getIndentLevel();
        // scope checks
        if (indentLevel > currentLevel) {
            it++;
            continue;
        }
        else if (indentLevel < currentLevel) {
            break;
        }
        else {
            lines_executed++;
        }
    
        // if statement
        if (contents.length() >= 2 && contents.rfind("if") == 0) {
            contents = contents.substr(contents.rfind("(") + 1, contents.find(")") - contents.rfind("(")-1);
            if (evaluate_expr(contents) == "true") {
                list<Line> ifLines(next(it), lines.end());
                int lines_to_skip = execute(ifLines, currentLevel + 1);
                advance(it, lines_to_skip);
                continue;
            }
        }
        // while loop
        else if (contents.length() >= 5 && contents.rfind("while") == 0) {
            contents = contents.substr(contents.rfind("(") + 1, contents.find(")") - contents.rfind("(")-1);
            if (evaluate_expr(contents) == "true") {
                list<Line> whileLines(next(it), lines.end());
                execute(whileLines, currentLevel + 1);
                continue;
            }
        }
        // for loop
        else if (contents.length() >= 5 && contents.rfind("for") == 0) {
            contents = contents.substr(contents.rfind("(") + 1, contents.find(")") - contents.rfind("(")-1);
            string assignment = contents.substr(0, contents.find(';'));
            string condition = contents.substr(contents.find(';')+1, contents.find_last_of(';')-contents.find(';')-1);
            string iter_statement = contents.substr(contents.rfind(';') + 1, contents.length()-contents.find_last_of(';')-1);
            if(first_it) {
                 execute(assignment);
                 first_it = false;
            }
            if (evaluate_expr(condition) == "true") {
                list<Line> forLines(next(it), lines.end());
                execute(forLines, currentLevel + 1);
                execute(iter_statement);
                continue;
            }
            else {
                first_it = true;
            }
        }
        else {
            execute(contents);
        }
        it++;
    }
    return lines_executed;
}

int main(int argc, char *argv[]) {
    string filepath = "FizzBuzz.alex";
    if (argc >= 2) {
        filepath = argv[1];
    }
    else if (filepath.empty()) {
        throw runtime_error("no source file specified. Specify in .cpp source file or as a terminal argument");
    }

    // read file
    ifstream inputFile(filepath);
    // put lines into list object
    list<Line> lines;
    string line;
    // determine scope and add lines to list
    while (getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }
        int indentLevel = -1;
        if (line[0] != ' ') {
            indentLevel = 0;
        }
        else if (line.find_first_not_of(" ") > lines.back().getContents().find_first_not_of(" ")) {
            indentLevel = lines.back().getIndentLevel() + 1;
        }
        else if (line.find_first_not_of(" ") == lines.back().getContents().find_first_not_of(" ")) {
            indentLevel = lines.back().getIndentLevel();
        }
        else {
            for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
                Line currentComparison = *it;
                if (line.find_first_not_of(" ") == currentComparison.getContents().find_first_not_of(" ")) {
                    indentLevel = currentComparison.getIndentLevel();
                }   
            }
            if (indentLevel < 0) {
                throw runtime_error("invalid indentation syntax");
            }
        }
        // add to lines list
        lines.push_back(Line(line, indentLevel));  
    }

    execute(lines, 0);
    return 0;
} 
