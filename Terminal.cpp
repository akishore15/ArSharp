#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

// Simplified ArSharp interpreter (for demonstration purposes)

class ArSharpInterpreter {
private:
    string code;
    map<string, double> variables;
    map<string, double> constants;
    map<string, double> grandValues;

public:
    ArSharpInterpreter(const string& code) : code(code) {}

    int execute() {
        vector<string> lines = split(code, '\n');

        for (const string& line : lines) {
            // Remove comments
            string trimmedLine = trim(line);
            if (trimmedLine.empty() || trimmedLine[0] == '/' && trimmedLine[1] == '/') {
                continue;
            }
            if (trimmedLine.find("s;") != string::npos) {
                continue; // Skip comments with "s;"
            }

            // Handle variable declarations
            if (trimmedLine.find("var{") != string::npos) {
                size_t start = trimmedLine.find("{") + 1;
                size_t end = trimmedLine.find("}");
                string varName = trimmedLine.substr(start, end - start);
                size_t equalsPos = trimmedLine.find("=");
                if (equalsPos != string::npos) {
                    string valueStr = trimmedLine.substr(equalsPos + 1);
                    variables[varName] = evaluateExpression(valueStr);
                } else {
                    variables[varName] = 0.0; // Initialize to 0.0 by default
                }
                continue;
            }

            // Handle constant declarations
            if (trimmedLine.find("const{") != string::npos) {
                size_t start = trimmedLine.find("{") + 1;
                size_t end = trimmedLine.find("}");
                string constName = trimmedLine.substr(start, end - start);
                size_t equalsPos = trimmedLine.find("=");
                if (equalsPos != string::npos) {
                    string valueStr = trimmedLine.substr(equalsPos + 1);
                    constants[constName] = evaluateExpression(valueStr);
                } else {
                    constants[constName] = 0.0; // Initialize to 0.0 by default
                }
                continue;
            }

            // Handle grand value declarations
            if (trimmedLine.find("g[") != string::npos) {
                size_t start = trimmedLine.find("[") + 1;
                size_t end = trimmedLine.find("]");
                string gName = trimmedLine.substr(start, end - start);
                size_t equalsPos = trimmedLine.find("=");
                if (equalsPos != string::npos) {
                    string valueStr = trimmedLine.substr(equalsPos + 1);
                    grandValues[gName] = evaluateExpression(valueStr) * pow(10, 6); // Simplified grand value handling
                } else {
                    grandValues[gName] = 0.0; // Initialize to 0.0 by default
                }
                continue;
            }

            // Handle print statements
            if (trimmedLine.find("cout::") != string::npos) {
                size_t start = trimmedLine.find("::") + 2;
                size_t end = trimmedLine.find("endl;");
                string output = trimmedLine.substr(start, end - start);
                cout << evaluateExpression(output) << endl;
                continue;
            }

            // Handle other expressions (e.g., assignments, calculations) 
            // (This part needs to be more robust)
            // ...

        }

        return 0; // Return 0 for success
    }

    double evaluateExpression(const string& expression) {
        // Basic expression evaluation (can be expanded significantly)
        if (is_number(expression)) {
            return stod(expression);
        }

        // Handle variables and constants
        if (expression.find("var{") != string::npos) {
            size_t start = expression.find("{") + 1;
            size_t end = expression.find("}");
            string varName = expression.substr(start, end - start);
            if (variables.find(varName) != variables.end()) {
                return variables[varName];
            }
        } else if (expression.find("const{") != string::npos) {
            size_t start = expression.find("{") + 1;
            size_t end = expression.find("}");
            string constName = expression.substr(start, end - start);
            if (constants.find(constName) != constants.end()) {
                return constants[constName];
            }
        }

        // Handle grand values
        if (expression.find("g[") != string::npos) {
            size_t start = expression.find("[") + 1;
            size_t end = expression.find("]");
            string gName = expression.substr(start, end - start);
            if (grandValues.find(gName) != grandValues.end()) {
                return grandValues[gName];
            }
        }

        // Handle basic operators
        size_t pos = expression.find("+");
        if (pos != string::npos) {
            return evaluateExpression(expression.substr(0, pos)) + evaluateExpression(expression.substr(pos + 1));
        }
        pos = expression.find("-");
        if (pos != string::npos) {
            return evaluateExpression(expression.substr(0, pos)) - evaluateExpression(expression.substr(pos + 1));
        }
        pos = expression.find("*");
        if (pos != string::npos) {
            return evaluateExpression(expression.substr(0, pos)) * evaluateExpression(expression.substr(pos + 1));
        }
        pos = expression.find("/");
        if (pos != string::npos) {
            return evaluateExpression(expression.substr(0, pos)) / evaluateExpression(expression.substr(pos + 1));
        }
        pos = expression.find("**");
        if (pos != string::npos) {
            return pow(evaluateExpression(expression.substr(0, pos)), evaluateExpression(expression.substr(pos + 2)));
        }
        pos = expression.find("%");
        if (pos != string::npos) {
            return fmod(evaluateExpression(expression.substr(0, pos)), evaluateExpression(expression.substr(pos + 1)));
        }

        // Handle other cases (e.g., function calls, more complex expressions)
        // ...

        return 0.0; // Return 0.0 by default
    }

    // Helper function to split a string by a delimiter
    vector<string> split(const string& str, char delim) {
        vector<string> tokens;
        size_t start = 0;
        size_t end = str.find(delim);
        while (end != string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delim, start);
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }

    // Helper function to check if a string represents a number
    bool is_number(const string& s) {
        string::const_iterator it = s.begin();
        while (it != s.end() &&
