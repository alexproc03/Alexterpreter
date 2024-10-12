#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <stack>
#include <cmath>
using namespace std;

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

unordered_map<string, Var> s;

static Var varmap(string var_name, unordered_map<string,  Var> state) {
    Var var = state[var_name];
    string val = var.getVal();
    if (val.length() == 0) {
        cout << var_name << " not found in state, ";
    }
    return var;
}

static bool contains(list<string> list, string target) {
    return (find(list.begin(), list.end(), target) != list.end());
}

static bool is_in_state(string var_name, unordered_map<string, Var> state) {
    if (state.find(var_name) == state.end()) {
        return false;
    }
    else {
        return true;
    }
}

static string remove_whitespace(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    return str;
}

bool is_number(const string &s) {
  for (char c: s) {
    string digits = "0123456789";
    if (digits.find(c) == string::npos) {
        return false;
    }
  }
  return true;
}

static int MDEC(string number) {
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

static list<string> parse_expr(string expr) {
    list<string> tokens;
    string operators = "+-*/%()<>!";
    string multi_operators[] = {"&&", "||", "==", "<=", ">="};
    expr = remove_whitespace(expr);
    //parse expression into tokens
    int index = 0;
    while (expr.length() != 0) {
        if (expr.substr(index, 2) == multi_operators[0] || expr.substr(index, 2) == multi_operators[1] || expr.substr(index, 2) == multi_operators[2]){
            if (index > 0) {
                tokens.push_back(expr.substr(0, index));
            }
            tokens.push_back(expr.substr(index, 2));
            expr = expr.substr(index+2,expr.length() - 1 - index);
            index = 0;
        }
        else if (operators.find(expr.substr(index, 1)) != string::npos) {
            if (index > 0) {
                tokens.push_back(expr.substr(0, index));
            }
            tokens.push_back(expr.substr(index, 1));
            expr = expr.substr(index+1,expr.length() - 1 - index);
            index = 0;
        }
        else if (expr == "true" || expr == "false" || is_in_state(expr, s) || is_number(expr) || (expr.find('\"') == 0 && expr.rfind('\"') == expr.length()-1)) {
            tokens.push_back(expr);
            expr = "";
        }
        else {
            //TODO: Something here because this is an error I think
            index++;
        }
        for (string i : tokens) {
            cout << i << ", ";
        }
        cout << endl;
    }
    return tokens;
}

static string determine_type(string value) {
    if (is_in_state(value, s)) {
        return varmap(value, s).getType();
    }
    if (value == "true" || value == "false") {
        return "bool";
    }
    else if (is_number(value)) {
        return "int";
    }
    else if (value[0] == '"' && value[value.length()-1] == '"') {
        return "string";
    }
    else {
        throw runtime_error("\"" + value + "\" is not of valid type");
    }
}

//Helper for evaluate_expr
static list<string> evaluate_parentheses(list<string> tokens) {
    //TODO: implement stack for dealing with parentheses, calling evaluate_expr on the insides of the parentheses
        int startIndex = distance(tokens.begin(), find(tokens.begin(), tokens.end(), "("));
        int endIndex = tokens.size() - distance(tokens.rbegin(), find(tokens.rbegin(), tokens.rend(), ")")) - 1;
        cout << startIndex << ", " << endIndex << endl;
        
        auto it = tokens.begin();
        advance(it, startIndex);
        for (int i = startIndex; i <= endIndex; i++) {
                string elemnt = *it;
                it = tokens.erase(it);
        }
        for (string i : tokens) {
            cout << i << ", ";
        }
        cout << endl;
    return tokens;
}

static string evaluate_expr(list<string> tokens) {
    // return type is based on the type of the first identifier
    string return_type = determine_type(tokens.front());
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
        evaluate_parentheses(tokens);
    }

    // // iterative case: evaluate operations in correct precedence
    // if (return_type == "int") {
        auto it = tokens.begin();
        while (contains(tokens, "*") || contains(tokens, "/") || contains(tokens, "%")) {
            //Identify if current index is an operator
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
            advance(it, 1);
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
        return tokens.front();
    // }
    // else if (return_type = "bool") {

    // }
}

static string evaluate_expr(string expr) {
        // parse expression
        list<string> tokens = parse_expr(expr);
        // determine if all tokens are valid
        for (string& token : tokens) {
            if (is_in_state(token, s)) {
               token = varmap(token, s).getVal();
            }
        // call sister-function
        }
        return evaluate_expr(tokens);
}

static void execute(string line) {
    if (line.length() >= 4 && line.rfind("let ") == 0) {
        line = remove_whitespace(line.substr(4));
        string var = line.substr(0, line.find("="));
        string expr = line.substr(line.find("=") + 1);
        string val = evaluate_expr(expr);
        s[var] = Var(val, determine_type(val));
    }
    else if (line.length() >= 5 && line.rfind("print" == 0)) {
        line = line.substr(line.rfind("(") + 1, line.find(")") - line.rfind("(")-1);
        cout << evaluate_expr(line) << endl;
    }
    else {
        throw runtime_error("\"" + line + "\" is not a valid or known statement\n");
    }
}

static void interpret(string filepath) {

}

int main(int argc, char *argv[]) {

    if (argc >= 1) {
        string filepath = "test.math"; //argv[1];
        // read file
        ifstream lines(filepath);

        // read lines into line
        string line;
        while (getline (lines, line)) {
            execute(line);
        }
    }
    else {
        cout << argc - 1 << " args recieved, expected 1\n";
    }
    return 0;
}