#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>
#include "./tokenization.hpp"



std::string tokens_to_asm(const std::vector <Token> &tokens) {
    std::stringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0; i<tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::exit) {
            if (i + 1 < tokens.size() && tokens.at(i+1).type == TokenType::int_lit) {
                if (i+2 < tokens.size() && tokens.at(i+2).type == TokenType::semi ) {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " <<tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }

    }
    return output.str();
}


int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Incorrect Usage" << std::endl;
        std::cerr << "ove <input.ov>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;{
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }


    //error in tokenizer
    Tokenizer tokenizer(std::move(contents)); //improved performance here with no copies made
    std::vector<Token> tokens = tokenizer.tokenize();

    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    //error in executable here -> fix
    system("nasm -f elf64 out.asm -o out.o");
    system("ld out.o -o out");
    return EXIT_SUCCESS;
}