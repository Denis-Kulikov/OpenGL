#include <iostream>

#define TRY(command, str_error) try {                                   \
    if (command) {                                                      \
        throw std::runtime_error(str_error);                            \
    }                                                                   \
} catch (const std::exception& e) {                                     \
    std::cerr << e.what() << std::endl;                                 \
    abort();                                                            \
}
