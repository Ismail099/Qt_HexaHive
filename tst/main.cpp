//
// Created by Haidar Ibo on 19.02.2022.
//

#include <iostream>
#include <gtest/gtest.h>


TEST(HelloTest, BasicAssertion) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}


GTEST_API_ int main(int argc, char **argv) {

    std::cout << "Running main() from " << __FILE__ << "\n";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}