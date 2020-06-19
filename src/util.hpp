#pragma once

#ifdef DEBUG_BUILD
#define debug_print(x) std::cerr << x << std::endl;
#else
#define debug_print(x)                                                         \
    do {                                                                       \
    } while (0)
#endif
