//
//  User.cpp
//  C++
//
//  Created by 曾宪欣 on 2018/11/28.
//  Copyright © 2017年 曾宪欣. All rights reserved.
//

#include "User.hpp"
using namespace std;

User::User(const string &t_userName, const string &t_userPassword,
           const string &t_userEmail, const string &t_userPhone)
{
    m_name = t_userName;
    m_password = t_userPassword;
    m_email = t_userEmail;
    m_phone = t_userPhone;
}

User::User(const User &t_user)
{
        m_name = t_user.m_name;
        m_password = t_user.m_password;
        m_email = t_user.m_email;
        m_phone = t_user.m_phone;
}

string User::getName() const
{
    return m_name;
}

void User::setName(const string &t_name)
{
    m_name = t_name;
    return;
}

string User::getPassword() const
{
    return m_password;
}
    
void User::setPassword(const string &t_password)
{
    m_password = t_password;
    return;
}

string User::getEmail() const
{
    return m_email;
}

void User::setEmail(const string &t_email)
{
    m_email = t_email;
    return;
}

string User::getPhone() const
{
    return m_phone;
}

void User::setPhone(const string &t_phone)
{
    m_phone = t_phone;
    return;
}
