//
//  Date.cpp
//  C++
//
//  Created by 曾宪欣 on 2018/11/28.
//  Copyright © 2017年 曾宪欣. All rights reserved.
//

#include "Date.hpp"
#include <initializer_list>
using namespace std;

Date::Date()
{
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
{
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
}

Date::Date(const string &dateString)
{
    Date target = Date::stringToDate(dateString);
    if (Date::isValid(target)) *this = target;
    else
    {
        m_year = 0;
        m_month = 0;
        m_day = 0;
        m_hour = 0;
        m_minute = 0;
    }
}

int Date::getYear(void) const
{
    return m_year;
}

void Date::setYear(const int t_year)
{
    m_year = t_year;
    return;
}

int Date::getMonth(void) const
{
    return m_month;
}

void Date::setMonth(const int t_month)
{
    m_month = t_month;
    return;
}

int Date::getDay(void) const
{
    return m_day;
}

void Date::setDay(const int t_day)
{
    m_day = t_day;
    return;
}

int Date::getHour(void) const
{
    return m_hour;
}

void Date::setHour(const int t_hour)
{
    m_hour = t_hour;
    return;
}

int Date::getMinute(void) const
{
    return m_minute;
}

void Date::setMinute(const int t_minute)
{
    m_minute = t_minute;
    return;
}

bool Date::isValid(const Date &t_date)
{
    if (t_date.m_year < 1000 || t_date.m_year > 9999) return false;
    if (t_date.m_month < 1 || t_date.m_month > 12) return false;
    if (t_date.m_day < 1 || t_date.m_day > 31) return false;
    if (t_date.m_hour < 0 || t_date.m_hour > 23) return false;
    if (t_date.m_minute < 0 || t_date.m_minute > 59) return false;
    if (t_date.m_month == 4 || t_date.m_month == 6 || t_date.m_month == 9 || t_date.m_month == 11) if (t_date.m_day > 30) return false;
    if (t_date.m_month == 2)
    {
        if ((t_date.m_year % 4 == 0 && t_date.m_year % 100 != 0) || (t_date.m_year % 400 == 0)) 
        {
            if (t_date.m_day > 29) return false;
        }
        else if (t_date.m_day > 28)
        {
            return false;
        }
    }
    return true;
}

Date Date::stringToDate(const string &t_dateString)
{
    Date newdate = *new Date;
    string year, month, day, hour, minute;
    int judge = 0;
    for(int i = 0; i < 16; i++){
        if(i != 4 && i != 7 && i != 10 && i != 13 && ((int)t_dateString[i] < '0' || (int)t_dateString[i] > '9'))
            judge = 1;
        if((i == 4 || i == 7) && t_dateString[i] != '-')
            judge = 1;
        if(i == 10 && t_dateString[i] != '/')
            judge = 1;
        if(i == 13 && t_dateString[i] != ':')
            judge = 1;
    }
    if(judge){
        newdate.m_year = 0;
        newdate.m_month = 0;
        newdate.m_day = 0;
        newdate.m_hour = 0;
        newdate.m_minute = 0;
        return newdate;
    }
    for(int i = 0; i < 16; i++){
        if(i < 4) year[i] = t_dateString[i];
        if(i > 4 && i < 7) month[i - 5] = t_dateString[i];
        if(i > 7 && i < 10) day[i - 8] = t_dateString[i];
        if(i > 10 && i < 13) hour[i - 11] = t_dateString[i];
        if(i > 13) minute[i - 14] = t_dateString[i];
    }
    newdate.m_year = stoi(year);
    newdate.m_month = stoi(month);
    newdate.m_day = stoi(day);
    newdate.m_hour = stoi(hour);
    newdate.m_minute = stoi(minute);
    if (!newdate.isValid(newdate))
    {
        newdate.m_year = 0;
        newdate.m_month = 0;
        newdate.m_day = 0;
        newdate.m_hour = 0;
        newdate.m_minute = 0;
    }
    return newdate;
}

//0000-00-00/00:00
string Date::dateToString(const Date &t_date)
{
    char stringdate[17];
    if(isValid(t_date) == 0){
        string strdate = "0000-00-00/00:00";
        return strdate;
    }
    snprintf(stringdate, 17, "%04d-%02d-%02d/%02d:%02d", t_date.m_year, t_date.m_month, t_date.m_day, t_date.m_hour, t_date.m_minute);
    return string(stringdate);
}

Date &Date::operator=(const Date &t_date)
{
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
}

bool Date::operator==(const Date &t_date) const
{
    if(m_year == t_date.m_year && m_month == t_date.m_month &&
       m_day == t_date.m_day && m_hour == t_date.m_hour &&
       m_minute == t_date.m_minute)
        return true;
    return false;
}

bool Date::operator>(const Date &t_date) const
{
    if(m_year < t_date.m_year)
        return false;
    if(m_year == t_date.m_year){
        if(m_month < t_date.m_month)
            return false;
        if(m_month == t_date.m_month){
            if(m_day < t_date.m_day)
                return false;
            if(m_day == t_date.m_day){
                if(m_hour < t_date.m_hour)
                    return false;
                if(m_hour == t_date.m_hour){
                    if(m_minute <= t_date.m_minute)
                        return false;
                }
            }
        }
    }
    return true;
}

bool Date::operator<(const Date &t_date) const
{
    if(m_year > t_date.m_year)
        return false;
    if(m_year == t_date.m_year){
        if(m_month > t_date.m_month)
            return false;
        if(m_month == t_date.m_month){
            if(m_day > t_date.m_day)
                return false;
            if(m_day == t_date.m_day){
                if(m_hour > t_date.m_hour)
                    return false;
                if(m_hour == t_date.m_hour){
                    if(m_minute >= t_date.m_minute)
                        return false;
                }
            }
        }
    }
    return true;
}

bool Date::operator>=(const Date &t_date) const
{
    if(m_year < t_date.m_year)
        return false;
    if(m_year == t_date.m_year){
        if(m_month < t_date.m_month)
            return false;
        if(m_month == t_date.m_month){
            if(m_day < t_date.m_day)
                return false;
            if(m_day == t_date.m_day){
                if(m_hour < t_date.m_hour)
                    return false;
                if(m_hour == t_date.m_hour){
                    if(m_minute < t_date.m_minute)
                        return false;
                }
            }
        }
    }
    return true;
}

bool Date::operator<=(const Date &t_date) const
{
    if(m_year > t_date.m_year)
        return false;
    if(m_year == t_date.m_year){
        if(m_month > t_date.m_month)
            return false;
        if(m_month == t_date.m_month){
            if(m_day > t_date.m_day)
                return false;
            if(m_day == t_date.m_day){
                if(m_hour > t_date.m_hour)
                    return false;
                if(m_hour == t_date.m_hour){
                    if(m_minute > t_date.m_minute)
                        return false;
                }
            }
        }
    }
    return true;
}
