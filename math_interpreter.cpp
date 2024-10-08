#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <list>
using namespace std;

unordered_map<string, string> s;
static string varmap(string var, unordered_map<string,  string> state) {
    string val = state[var];
    if (val.length() == 0) {
        cout << var << " not found in state, ";
    }
    return val;
}

static string remove_whitespace(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    return str;
}

bool is_number(const std::string &s) {
  return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

static list<string> parse_expr(string expr) {
    list<string> tokens;
    string operators = "+-*/%";
    expr = remove_whitespace(expr) + " ";
    //parse expression into tokens
    int index = 0;
    while (expr.length() != 0) {
        if (operators.find(expr.substr(index, 1)) != string::npos) {
            tokens.push_back(expr.substr(0, index));
            tokens.push_back(expr.substr(index, 1));
            expr = expr.substr();
            continue;
        }
        index++;
    }
}

static string evaluate_expr(string expr) {
    list<string> tokens = parse_expr(expr);
    for (string elem : tokens) {
        cout << elem << ", ";
    }
}

static void execute(string line) {
    if (line.rfind("let ") == 0) {
        line = remove_whitespace(line.substr(4, line.length()-1));
        string var = line.substr(0, line.rfind("="));
        string expr = line.substr(line.rfind("=") + 1);
        string val = evaluate_expr(expr);
        s[var] = val;
    }
    else {
        cout << evaluate_expr(line) << endl;
    }
}

static void interpret(string filepath) {
    // read file
    ifstream lines(filepath);

    // read lines into line
    string line;
    while (getline (lines, line)) {
        execute(line);
    }
}

int main(int argc, char *argv[]) {

    if (argc >= 1) {
        string fun_path = "test.math"; //argv[1];
        interpret(fun_path);
    }
    else {
        cout << argc - 1 << " args recieved, expected 1\n";
    }
    return 0;
}