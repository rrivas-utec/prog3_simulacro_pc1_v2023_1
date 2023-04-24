#ifndef REDIRECT_IO_H
#define REDIRECT_IO_H

// Created by Rub�n Rivas

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

template<typename T>
T& ltrim(T& text) {
    auto first = text.find_first_not_of(" \t");
    if(T::npos != first)
        text = text.substr(first);
    return text;
}

template<typename T>
T& rtrim(T& text) {
    auto last = text.find_last_not_of(" \t\n");
    if(T::npos != last)
        text = text.substr(0, last+1);
    return text;
}

template<typename T>
T& trim(T& text) {
    ltrim(text);
    rtrim(text);
    return text;
}

class redirect_io_t {
    stringstream out_;
    streambuf *buffer_out{};
    streambuf *buffer_in{};
    bool redirect_flag = false;
public:
    redirect_io_t() :out_{ std::ios_base::in | std::ios_base::out } {
        redirect();
    }

    template<typename T>
    stringstream &operator>>(T &&target) {
        out_ >> target;
        return out_;
    }

    template<typename T>
    stringstream &operator<<(T &target) {
        out_ << target;
        return out_;
    }

    stringstream& getline(string& target) {
        std::getline(out_, target);
        return out_;
    }

    void redirect() {
        // Check if it was redirected before
        if (redirect_flag) return;
        // Backup buffer
        buffer_in = cin.rdbuf();
        buffer_out = cout.rdbuf();

        // Redirect to file buffer
        cin.rdbuf(out_.rdbuf());
        cout.rdbuf(out_.rdbuf());

        // Flagged as redirected
        redirect_flag = true;
    }
    void reestablish() {
        // Check if it was not redirected before
        if (!redirect_flag) return;
        // Clear 
        out_.str("");
        out_.clear();
        // Restore Buffers
        cin.rdbuf(buffer_in);
        cout.rdbuf(buffer_out);
        redirect_flag = false;
    }

    ~redirect_io_t() {
        if (redirect_flag) reestablish();
    }
};

template<typename VoidFunction>
void execute_test(const string& file_name, VoidFunction func) {
    redirect_io_t rd;
    ifstream file(file_name);

    if(!file.is_open())
        REQUIRE (!"ERROR TEST FILE");
    if(file.eof())
        REQUIRE (!"EMPTY TEST FILE");

    vector<string> output_test;
    vector<string> result;

    string line{};
    string input_text{};
    string output_text{};
    string result_text{};

    while(!file.eof()) {
        line = {};
        while(getline(file, line) && line != "#input");
        if (line.empty())
            break;

        input_text = {};
        while (getline(file, line) && line != "#output")
            input_text += trim(line) + (!line.empty()?"\n":"");

        output_text = {};
        while (getline(file, line) && line != "#endtest")
            output_text += trim(line) + (!line.empty()?"\n":"");
        output_test.push_back(output_text);

        rd.redirect();
        rd << input_text;

        func();

        result_text = {};
        while (rd.getline(line))
            result_text += trim(line) + (!line.empty()?"\n":"");
        result.push_back(result_text);
        rd.reestablish();
    }

    if (input_text.empty() && output_test.empty()) {
        REQUIRE (!"NO TEST:");
    }
    else if (output_test.empty()) {
        REQUIRE (!"ERROR READING OUTPUT TEST");
    }
    else if(result.empty()) {
        REQUIRE (!"ERROR READING TEST:");
    }

    // Resultado
    cout << string(40, '=') << endl;
    cout << file_name << endl;
    cout << string(40, '=') << endl;
    cout << "Valores Esperados:\n";
    cout << string(40, '.') << endl;
    auto i = 0;
    for (const auto& item: output_test)
        cout << "Caso #" << ++i << " :" << endl
            << item;
    cout << string(40, '-') << endl;
    cout << "Valores Obtenidos:\n";
    cout << string(40, '.') << endl;
    i = 0;
    for (const auto& item: result)
        cout << "Caso #" << ++i << " :" << endl
            << item;
    auto it_1 = begin(output_test);
    auto it_2 = begin(result);
    while(it_1 != end(output_test))
        REQUIRE(*it_1++ == *it_2++);
}

#endif