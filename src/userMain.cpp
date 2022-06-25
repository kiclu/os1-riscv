// #include "../test/Threads_C_API_test.hpp"
// #include "../test/Threads_CPP_API_test.hpp"

// #include "../test/ConsumerProducer_C_API_test.h"
#include "../test/ConsumerProducer_CPP_Sync_API_test.hpp"

// #include "../test/ThreadSleep_C_API_test.hpp"
// #include "../test/ConsumerProducer_CPP_API_test.hpp"

void userMain() {
    // Threads_C_API_test();
    // Threads_CPP_API_test();

    // producerConsumer_C_API();
    producerConsumer_CPP_Sync_API();

    // testSleeping();
    // ConsumerProducerCPP::testConsumerProducer();
}