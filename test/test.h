#define TEST 1

#if     TEST == 1

#include "../test/Threads_C_API_test.hpp"
void test(){ Threads_C_API_test(); }

#elif   TEST == 2

#include "../test/Threads_CPP_API_test.hpp"
void test(){ Threads_C_API_test(); }

elif    TEST == 3

#include "../test/ConsumerProducer_C_API_test.h"
void test(){ ConsumerProducer_C_API_test(); }

elif    TEST == 4

#include "../test/ConsumerProducer_CPP_Sync_API_test.hpp"
void test(){ ConsumerProducer_CPP_API_test(); }

elif    TEST == 5

#include "../test/ThreadSleep_C_API_test.hpp"
void test(){ ThreadSleep_C_API_test(); }

elif    TEST == 6

#include "../test/ConsumerProducer_CPP_API_test.hpp"
void test(){ ConsumerProducerCPP::testConsumerProducer(); }

#endif
