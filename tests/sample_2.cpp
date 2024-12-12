/*
 * Example unit test for the embtest library.
 */
#include "embtest.hpp"

/**
 * Define a test fixture. Fixtures provide a common context
 * across many related tests. Fixture classes can also
 * override the ::SetUp() and ::TearDown() methods for more
 * test preparation.
 */
class ByteOrder: public embtest::Test
{
  protected:
    ByteOrder()
    {
        m_intBytes.val = 0x10203040;
    }

    union IntBytes {
        int val;
        unsigned char bytes[sizeof(int)];
    };
    IntBytes m_intBytes;
};

TEST_F(ByteOrder, byte1)
{
    EXPECT_EQ(m_intBytes.bytes[0], 0x40);
}

TEST_F(ByteOrder, byte2)
{
    EXPECT_EQ(m_intBytes.bytes[1], 0x30);
}

TEST_F(ByteOrder, byte3)
{
    EXPECT_EQ(m_intBytes.bytes[2], 0x20);
}

TEST_F(ByteOrder, byte4)
{
    EXPECT_EQ(m_intBytes.bytes[3], 0x10);
}

TEST_F(ByteOrder, structSize)
{
    EXPECT_EQ(sizeof(m_intBytes), sizeof(int));
}
