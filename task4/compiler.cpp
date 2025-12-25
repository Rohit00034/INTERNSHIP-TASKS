#include <iostream>
#include <string>
#include <cctype>

using namespace std;

string s;
int pos = 0;

int parseExpression();

int parseNumber() {
    int value = 0;
    while (pos < s.size() && isdigit(s[pos])) {
        value = value * 10 + (s[pos] - '0');
        pos++;
    }
    return value;
}

int parseFactor() {
    if (s[pos] == '(') {
        pos++;
        int value = parseExpression();
        pos++;
        return value;
    }
    return parseNumber();
}

int parseTerm() {
    int value = parseFactor();
    while (pos < s.size() && (s[pos] == '*' || s[pos] == '/')) {
        char op = s[pos++];
        int next = parseFactor();
        if (op == '*') value *= next;
        else value /= next;
    }
    return value;
}

int parseExpression() {
    int value = parseTerm();
    while (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
        char op = s[pos++];
        int next = parseTerm();
        if (op == '+') value += next;
        else value -= next;
    }
    return value;
}

int main() {
    getline(cin, s);
    cout << parseExpression();
    return 0;
}
