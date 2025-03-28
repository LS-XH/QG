#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <cmath>


using namespace std;
template <typename T>
class LinkedStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* top; 

public:
    LinkedStack() : top(nullptr) {}
    ~LinkedStack() { clear(); }

    bool isEmpty() const { return top == nullptr; }

    void push(const T& item) {
        top = new Node(item, top);
    }

    T pop() {
        if (isEmpty()) throw "Pop from empty stack";
        Node* temp = top;
        T data = temp->data;
        top = top->next;
        delete temp;
        return data;
    }

    T peek() const {
        if (isEmpty()) throw "Peek from empty stack";
        return top->data;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};





class Calculator {
private:
    //判断运算符优先级
    static int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    // 执行运算
    static double applyOp(double a, double b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw runtime_error("Division by zero");
            return a / b;
        default: throw runtime_error("Invalid operator");
        }
    }

public:
    //计算表达式值
    static double evaluate(const string& expression) {
        LinkedStack<double> values;
        LinkedStack<char> ops;

        for (size_t i = 0; i < expression.length(); i++) {
            //跳过空格
            if (expression[i] == ' ') continue;

            //处理数字
            if (isdigit(expression[i]) || expression[i] == '.') {
                string numStr;
                while (i < expression.length() &&
                    (isdigit(expression[i]) || expression[i] == '.')) {
                    numStr += expression[i++];
                }
                i--; //回退一个字符
                values.push(stod(numStr));
            }
            //处理左括号
            else if (expression[i] == '(') {
                ops.push(expression[i]);
            }
            //处理右括号
            else if (expression[i] == ')') {
                while (!ops.isEmpty() && ops.peek() != '(') {
                    double val2 = values.pop();
                    double val1 = values.pop();
                    char op = ops.pop();
                    values.push(applyOp(val1, val2, op));
                }
                if (!ops.isEmpty()) ops.pop(); //弹出左括号
            }
            //处理运算符
            else if (expression[i] == '+' || expression[i] == '-' ||
                expression[i] == '*' || expression[i] == '/') {
                while (!ops.isEmpty() && precedence(ops.peek()) >= precedence(expression[i])) {
                    double val2 = values.pop();
                    double val1 = values.pop();
                    char op = ops.pop();
                    values.push(applyOp(val1, val2, op));
                }
                ops.push(expression[i]);
            }
        }

        //处理剩余运算符
        while (!ops.isEmpty()) {
            double val2 = values.pop();
            double val1 = values.pop();
            char op = ops.pop();
            values.push(applyOp(val1, val2, op));
        }

        return values.pop();
    }
};

int main() {
    string expr;
    cout << "请输入表达式 (支持+-*/和括号): ";
    getline(cin, expr);

    try {
        double result = Calculator::evaluate(expr);
        cout << "计算结果: " << result << endl;
    }
    catch (const exception& e) {
        cerr << "错误: " << e.what() << endl;
    }

    return 0;
}