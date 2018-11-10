#include "btest.h"

/*
 * Create a test fixture, to provide a common context
 * across related tests
 */
class ByteOrder: public btest::Test
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

BTEST_F(ByteOrder, byte1)
{
    EXPECT_EQ(m_intBytes.bytes[0], 0x40);
}

BTEST_F(ByteOrder, byte2)
{
    EXPECT_EQ(m_intBytes.bytes[1], 0x30);
}

BTEST_F(ByteOrder, byte3)
{
    EXPECT_EQ(m_intBytes.bytes[2], 0x20);
}

BTEST_F(ByteOrder, byte4)
{
    EXPECT_EQ(m_intBytes.bytes[3], 0x10);
}

BTEST_F(ByteOrder, structSize)
{
    EXPECT_EQ(sizeof(m_intBytes), sizeof(int));
}
