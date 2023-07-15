#include "gtest/gtest.h"

#define SPSCRB_SIZE_TYPE uint8_t
#include "../include/spscrb.h"


#if 1
TEST(TestPushback, TestPushbackSimpleSizeType) {
    char buf[4];
    spscrb_t rb = { buf, sizeof(buf) / sizeof(buf[0]), 0, 0 }; // , 0 };

    EXPECT_TRUE(spscrb_empty(&rb));
    spscrb_push_back(&rb, 1);
    EXPECT_EQ(spscrb_size(&rb), 1);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_FALSE(spscrb_full(&rb));

    spscrb_push_back(&rb, 2);
    EXPECT_EQ(spscrb_size(&rb), 2);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_FALSE(spscrb_full(&rb));
    
    spscrb_push_back(&rb, 3);
    EXPECT_EQ(spscrb_size(&rb), 3);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_FALSE(spscrb_full(&rb));
    
    spscrb_push_back(&rb, 4);
    EXPECT_EQ(spscrb_size(&rb), 4);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_TRUE(spscrb_full(&rb));
    
    if (spscrb_size(&rb) > 0) {
        EXPECT_EQ(spscrb_top(&rb), 1);
        spscrb_pop_front(&rb);
        EXPECT_EQ(spscrb_size(&rb), 3);
    }

    spscrb_push_back(&rb, 5);
    EXPECT_EQ(spscrb_size(&rb), 4);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_TRUE(spscrb_full(&rb));

    EXPECT_EQ(spscrb_at(&rb, 0), 2);
    EXPECT_EQ(spscrb_at(&rb, 1), 3);
    EXPECT_EQ(spscrb_at(&rb, 2), 4);
    EXPECT_EQ(spscrb_at(&rb, 3), 5);

    for (int i = 0; i < rb.capacity; ++i)
        std::cout << static_cast<int>(rb.buf[i]) << " ";
    std::cout << std::endl;

    EXPECT_EQ(spscrb_size(&rb), 4);
}

TEST(TestIntegerType, TestOverflow)
{
    SPSCRB_SIZE_TYPE w = 2; 
    SPSCRB_SIZE_TYPE r = UINT8_MAX - 2; // 253
    EXPECT_EQ((SPSCRB_SIZE_TYPE)(w - r), (SPSCRB_SIZE_TYPE)5);
}

TEST(TestPushback, TestCountersOverflowingSizeType_preTest) {
    // As we are testing pre-overflowing initial values, let's be sure they are ok
    EXPECT_EQ((SPSCRB_SIZE_TYPE)-1, UINT8_MAX);
}

TEST(TestPushback, TestCountersOverflowingSizeType) {
    char buf[4];
    spscrb_t rb = { buf, sizeof(buf) / sizeof(buf[0]), (SPSCRB_SIZE_TYPE)(-1) - 2, (SPSCRB_SIZE_TYPE)(-1) - 2 }; //, (SPSCRB_SIZE_TYPE)(-1) - 2 };

    unsigned char w = 2; unsigned char r = 253;
    EXPECT_EQ(unsigned char(w - r), (unsigned char)5);
    EXPECT_TRUE(spscrb_empty(&rb));
    spscrb_push_back(&rb, 1);
    EXPECT_EQ(spscrb_size(&rb), 1);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_FALSE(spscrb_full(&rb));

    spscrb_push_back(&rb, 2);
    EXPECT_EQ(spscrb_size(&rb), 2);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_FALSE(spscrb_full(&rb));
    
    spscrb_push_back(&rb, 3);
    EXPECT_EQ(spscrb_size(&rb), 3);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_FALSE(spscrb_full(&rb));
    
    spscrb_push_back(&rb, 4);
    EXPECT_EQ(spscrb_size(&rb), 4);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_TRUE(spscrb_full(&rb));
    
    if (spscrb_size(&rb) > 0) {
        EXPECT_EQ(spscrb_top(&rb), 1);
        spscrb_pop_front(&rb);
        EXPECT_EQ(spscrb_size(&rb), 3);
    }

    spscrb_push_back(&rb, 5);
    EXPECT_EQ(spscrb_size(&rb), 4);
    EXPECT_FALSE(spscrb_empty(&rb));
    EXPECT_TRUE(spscrb_full(&rb));
    EXPECT_EQ(spscrb_top(&rb), 2);

    EXPECT_EQ(spscrb_at(&rb, 0), 2);
    EXPECT_EQ(spscrb_at(&rb, 1), 3);
    EXPECT_EQ(spscrb_at(&rb, 2), 4);
    EXPECT_EQ(spscrb_at(&rb, 3), 5);

    for (int i = 0; i < rb.capacity; ++i)
        std::cout << static_cast<int>(rb.buf[i]) << " ";
    std::cout << std::endl;

    EXPECT_EQ(spscrb_size(&rb), 4);
}
#endif
