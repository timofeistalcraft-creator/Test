#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>
#include <string>
#include <cstdlib>
#include <cctype>

class Calculator {
private:
    Fl_Input *display;
    std::string current_input;
    double first_operand;
    char operation;
    bool new_operation;

    void handle_button(const char *label) {
        std::string btn_text = label;

        if (btn_text == "0" || btn_text == "1" || btn_text == "2" || 
            btn_text == "3" || btn_text == "4" || btn_text == "5" || 
            btn_text == "6" || btn_text == "7" || btn_text == "8" || btn_text == "9") {
            
            if (new_operation) {
                current_input = "";
                new_operation = false;
            }
            current_input += btn_text;
            display->value(current_input.c_str());
        } 
        else if (btn_text == ".") {
            if (new_operation) {
                current_input = "0.";
                new_operation = false;
            } else if (current_input.find('.') == std::string::npos) {
                current_input += ".";
            }
            display->value(current_input.c_str());
        }
        else if (btn_text == "+" || btn_text == "-" || btn_text == "*" || btn_text == "/") {
            if (!new_operation && !current_input.empty()) {
                calculate();
            }
            first_operand = std::atof(current_input.c_str());
            operation = btn_text[0];
            new_operation = true;
        }
        else if (btn_text == "=") {
            calculate();
            new_operation = true;
        }
        else if (btn_text == "C") {
            current_input = "0";
            display->value(current_input.c_str());
            new_operation = true;
        }
        else if (btn_text == "±") {
            if (!current_input.empty() && current_input != "0") {
                if (current_input[0] == '-') {
                    current_input = current_input.substr(1);
                } else {
                    current_input = "-" + current_input;
                }
                display->value(current_input.c_str());
            }
        }
    }

    void calculate() {
        if (!current_input.empty()) {
            double second_operand = std::atof(current_input.c_str());
            double result = 0;

            switch (operation) {
                case '+': result = first_operand + second_operand; break;
                case '-': result = first_operand - second_operand; break;
                case '*': result = first_operand * second_operand; break;
                case '/': 
                    if (second_operand != 0) {
                        result = first_operand / second_operand;
                    } else {
                        fl_alert("Error: Division by zero!");
                        return;
                    }
                    break;
                default: result = second_operand; break;
            }

            // Убираем дробную часть, если число целое
            if (result == static_cast<int>(result)) {
                current_input = std::to_string(static_cast<int>(result));
            } else {
                current_input = std::to_string(result);
                // Убираем лишние нули в конце
                current_input = current_input.substr(0, current_input.find_last_not_of('0') + 1);
                if (current_input.back() == '.') {
                    current_input = current_input.substr(0, current_input.length() - 1);
                }
            }
            
            display->value(current_input.c_str());
            first_operand = result;
        }
    }

public:
    // Callback функции для кнопок
    static void button_callback(Fl_Widget *widget, void *data) {
        ((Calculator*)data)->handle_button(((Fl_Button*)widget)->label());
    }

public:
    Calculator(Fl_Input *disp) : display(disp), first_operand(0), operation(0), new_operation(true) {
        current_input = "0";
        display->value(current_input.c_str());
    }
};

int main() {
    Fl_Window *window = new Fl_Window(300, 400, "Calculator");

    // Создаем поле ввода/отображения
    Fl_Input *display = new Fl_Input(10, 10, 280, 50, "");
    display->textsize(24);
    display->readonly();

    // Создаем калькулятор
    Calculator calc(display);

    // Создаем кнопки
    Fl_Button *btn[18];
    int btn_width = 65;
    int btn_height = 40;
    int start_x = 10;
    int start_y = 70;

    const char* labels[] = {
        "C", "±", "/", "*",
        "7", "8", "9", "-",
        "4", "5", "6", "+",
        "1", "2", "3", "=",
        "0", ".", "", ""
    };

    for (int i = 0; i < 18; i++) {
        if (labels[i][0] != '\0') {  // Проверяем, что кнопка не пустая
            int x = start_x + (i % 4) * (btn_width + 5);
            int y = start_y + (i / 4) * (btn_height + 5);
            
            btn[i] = new Fl_Button(x, y, btn_width, btn_height, labels[i]);
            btn[i]->callback(Calculator::button_callback, &calc);
        }
    }

    // Устанавливаем размер шрифта для кнопок
    for (int i = 0; i < 18; i++) {
        if (labels[i][0] != '\0') {
            btn[i]->labelsize(16);
        }
    }

    window->end();
    window->show();

    return Fl::run();
}