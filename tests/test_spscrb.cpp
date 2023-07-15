#include "gtest/gtest.h"
#include "../include/spscrb.h"

TEST(TestPushback, TestPushbackSimple) {
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

TEST(TestPushback, TestCountersOverflowing) {
    char buf[4];
    spscrb_t rb = { buf, sizeof(buf) / sizeof(buf[0]), SIZE_MAX - 2, SIZE_MAX - 2 }; // , SIZE_MAX - 2 };

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

