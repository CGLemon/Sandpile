#include <iostream>
#include <string>

#include "Utils.h"
#include "SandModel.h"

int main(int argc, char **argv){

    int s = 0;
    int c = 0;
    bool success = false;
    auto input = std::string{};

    std::cout << "How much sand do you want? (Please enter integer.)" << std::endl;
    std::getline(std::cin, input);
    auto sandstr = CommandParser(input);
    if (auto res = sandstr.get_command(0)) {
        if (is_integer(res->str) && sandstr.get_count() == 1) {
            c = res->get<int>();
            success = true;
        }
    }
    if (!success) {
        std::cout << "Syntax not understood." << std::endl;
        return 0;
    }

    success = false;
    std::cout << "How many size you want? (Please enter integer.)" << std::endl;
    std::getline(std::cin, input);
    auto sizestr = CommandParser(input);
    if (auto res = sizestr.get_command(0)) {
        if (is_integer(res->str) && sandstr.get_count() == 1) {
            s = res->get<int>();
        }
    } 
    if (!success) {
        std::cout << "Syntax not understood." << std::endl;
        return 0;
    }

    std::cout << "Set the file name." << std::endl;
    std::getline(std::cin, input);

    auto m = SandModel(s, s);
    m.set_xy(s/2, s/2);

    Timer timer;
    timer.clock();
    m.refresh(c);
    auto seconds = timer.get_duration_seconds();
    auto milliseconds = timer.get_duration_milliseconds();

    if (seconds > 100) {
        printf("%d second(s)\n", timer.get_duration_seconds());
    } else {
        printf("%.4f second(s)\n", (double)milliseconds / 1000.f );
    }


    timer.clock();
    input += ".pgm";
    m.draw(input.c_str());
    printf("%d millisecond(s)\n", timer.get_duration_milliseconds());

    return 0;
}
