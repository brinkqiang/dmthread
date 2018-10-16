
#include "dmthread.h"
#include "dmformat.h"

int main( int argc, char* argv[] ) {
    std::cout << fmt::format("{0} {1}", "hello world! ", DMTHREAD_MAIN(argc, argv));
    return 0;
}
