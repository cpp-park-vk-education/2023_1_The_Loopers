#include "../include/AuthModel.hpp"
#include "../include/Encrypter.hpp"

#include <string>
#include <gtest/gtest.h>

TEST(AuthModelTest, CreateNewUserTest) {
    AuthModel authModel;
    std::string username = "user";
    std::string password = "password";
    ASSERT_TRUE(authModel.createUser(username, password));
}

TEST(AuthModelTest, CreateExistingUserTest) {
    AuthModel authModel;
    std::string username = "user";
    std::string password = "password";
    ASSERT_FALSE(authModel.createUser(username, password));
}

TEST(AuthModelTest, CreateUserWithInvalidLoginTest) {
    AuthModel authModel;
    std::string username{};
    std::string password = "password";
    ASSERT_FALSE(authModel.createUser(username, password));
}

TEST(AuthModelTest, CreateUserWithInvalidPasswordTest) {
    AuthModel authModel;
    std::string username = "user";
    std::string password{};
    ASSERT_FALSE(authModel.createUser(username, password));
}

TEST(AuthModelTest, LoginForExistingUserTest) {
    AuthModel authModel;
    std::string username = "user";
    std::string password = "password";
    ASSERT_TRUE(authModel.login(username, password));
}

TEST(AuthModelTest, LoginWithInvalidUsernameTest) {
    AuthModel authModel;
    std::string username{};
    std::string password = "password";
    ASSERT_FALSE(authModel.login(username, password));
}

TEST(AuthModelTest, LoginWithInvalidPasswordTest) {
    AuthModel authModel;
    std::string username = "user";
    std::string password{};
    ASSERT_FALSE(authModel.login(username, password));
}

TEST(AuthModelTest, GetLoginTest) {
    AuthModel authModel;
    std::string username = "user";
    std::string password = "password";
    authModel.login(username, password);
    ASSERT_EQ(authModel.getLogin(), "user");
}

TEST(AuthModelTest, GetTokenTest) {
    AuthModel authModel;
    std::string username = "user";
    std::string password = "password";
    authModel.login(username, password);
    ASSERT_FALSE(authModel.getToken().empty());
}

TEST(AuthModelTest, ExitTest) {
    AuthModel authModel;
    std::string username = "user";
    std::string password = "password";
    authModel.login(username, password);
    authModel.exit();
    ASSERT_TRUE(authModel.getToken().empty());
}


TEST(EncrypterTest, EncryptingTest) {
    Encrypter encrypter;
    std::string password = "password";
    std::string encyptedString = "AdsdjjJDFHfdsjwpooO23kjns@"; //there should be really encrypted string
    ASSERT_EQ(encrypter.encryptString(password), encyptedString);
}
