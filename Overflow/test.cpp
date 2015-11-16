#include <iostream>

int main()
{
    char gwas[2] = {'a', 'b'};
    char test = 'c';
    
    std::cout << "\n" << static_cast<void*>(&gwas[0]);
    std::cout << "\n" << static_cast<void*>(&gwas[1]);
    std::cout << "\n" << static_cast<void*>(&test);
}
    
