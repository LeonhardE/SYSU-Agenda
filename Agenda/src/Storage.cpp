//
//  Storage.cpp
//  C++
//
//  Created by 曾宪欣 on 2018/11/28.
//  Copyright © 2017年 曾宪欣. All rights reserved.
//

#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>
using namespace std;
shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage()
{
	m_dirty = false;
	readFromFile();
}

bool Storage::readFromFile(void)
{
	ifstream fMeeting(Path::meetingPath);
	ifstream fUser(Path::userPath);
	if (!fMeeting.is_open() || !fUser.is_open()) return false;
	// Read Meeting
	while (1)
	{
		vector<int> sign;
		vector<int> sep;
		string str;
		getline(fMeeting, str);
		vector<string> participators;
		string sponsor, start, end, title;
		int len = str.size();
		if (len <= 2) break;
		for (int i = 0; i < len; i++) 
		{
			if (str[i] == '"') sign.push_back(i);
			if (str[i] == '&') sep.push_back(i);
		}
		if (sign.size() < 2) break;
		for(int i = sign[0] + 1 ; i < sign[1] ; i++) sponsor.push_back(str[i]);
        for(int i = sign[4] + 1 ; i < sign[5] ; i++) start.push_back(str[i]);
        for(int i = sign[6] + 1 ; i < sign[7] ; i++) end.push_back(str[i]);
        for(int i = sign[8] + 1 ; i < sign[9] ; i++) title.push_back(str[i]);
		len = sep.size() + 1;
		if (len != 1)
		{
			for (int i = 0; i < len; i++)
			{
				if (i == 0)
				{
					string ptr;
					for (int j = sign[2] + 1; j < sep[0]; j++) ptr.push_back(str[j]);
					participators.push_back(ptr);
					continue;
				}
				else if (i == len - 1)
				{
					string ptr;
					for (int j = sep[i - 1] + 1; j < sign[3]; j++) ptr.push_back(str[j]);
					participators.push_back(ptr);
					continue;
				}
				string ptr;
				for (int j = sep[i - 1] + 1; j < sep[i]; j++) ptr.push_back(str[j]);
				participators.push_back(ptr);
			}
		}
		else
		{
			string htr;
			for (int j = sign[2] + 1; j < sign[3]; j++) htr.push_back(str[j]);
			participators.push_back(htr);
		}
		Date startdate(start);
		Date enddate(end);
		Meeting newmeeting(sponsor, participators, startdate, enddate, title);
		m_meetingList.push_back(newmeeting);
	}
	fMeeting.close();
	// Read User
	while (1)
	{
		string str;
		getline(fUser, str);
		vector<int> sign;
		int len = str.size();
		for (int i = 0; i < len; i++) if (str[i] == '"') sign.push_back(i);
		if (sign.size() != 8) break;
		string name, pass, email, phone;
		for (int i = sign[0] + 1; i < sign[1]; i++) name.push_back(str[i]);
		for (int i = sign[2] + 1; i < sign[3]; i++) pass.push_back(str[i]);
		for (int i = sign[4] + 1; i < sign[5]; i++) email.push_back(str[i]);
		for (int i = sign[6] + 1; i < sign[7]; i++) phone.push_back(str[i]);
		User newuser(name, pass, email, phone);
		m_userList.push_back(newuser);
	}
	fUser.close();
	return true;
}

bool Storage::writeToFile(void)
{
	m_dirty = false;
	fstream fMeeting(Path::meetingPath, ios::out);
	fstream fUser(Path::userPath, ios::out);
	if (!fUser.is_open() || !fMeeting.is_open()) return false;
	int Ulen = m_userList.size();
	int Mlen = m_meetingList.size();
	if (Ulen == 0 && Mlen == 0) return true;
	// Write Meeting
	auto iter = m_meetingList.begin();
	for (iter; iter != m_meetingList.end(); iter++)
	{
		fMeeting << '"' << (*iter).getSponsor() << '"' << "," << '"';
		vector<string> participatorlist = (*iter).getParticipator();
		int len = participatorlist.size();
		for (int i = 0; i < len; i++)
		{
			fMeeting << participatorlist[i];
			if (i != len - 1) fMeeting << '&';
		}
		fMeeting << '"' << ',';
		fMeeting << '"' << (*iter).getStartDate().dateToString((*iter).getStartDate()) 
		         << '"' << ',' << '"' << (*iter).getEndDate().dateToString((*iter).getEndDate())
				 << '"' << "," << '"' << (*iter).getTitle() << '"';
		auto iter1 = iter;
		iter1 ++;
		if (iter1 != m_meetingList.end()) fMeeting << "\n";
	}
	fMeeting.close();
	// Write User
	auto iter2 = m_userList.begin();
	for (iter2; iter2 != m_userList.end(); iter2++)
	{
		fUser << "\"" << (*iter2).getName() << "\"" << ',' << "\"" << (*iter2).getPassword() 
		      << "\"" << ',' <<"\"" << (*iter2).getEmail() << "\"" << ',' <<"\"" << (*iter2).getPhone() << "\""; 
	    auto iter3 = iter2;
		iter3 ++;
		if (iter3 != m_userList.end()) fUser << "\n";
	}
	fUser.close();
	return true;
}

shared_ptr<Storage> Storage::getInstance(void)
{
	if(m_instance == nullptr) m_instance =  shared_ptr<Storage> (new Storage());
    return m_instance;
}

Storage::~Storage()
{
	sync();
}

void Storage::createUser(const User &t_user)
{
    m_userList.push_back(t_user);
	m_dirty = true;
}

list<User> Storage::queryUser(function<bool(const User &)> filter) const
{
	list<User> target;
	auto iter = m_userList.begin();
	for (iter; iter != m_userList.end(); iter++) if (filter(*iter)) target.push_back(*iter);
	return target;
}

int Storage::updateUser(function<bool(const User &)> filter, 
	                    function<void(User &)> switcher)
{
	int count = 0;
	auto iter = m_userList.begin();
	for (iter; iter != m_userList.end(); iter++)
	{
		if (filter(*iter))
		{
			switcher(*iter);
			count ++;
		}
	}
	if (count != 0) m_dirty = true;
	return count;
}

int Storage::deleteUser(function<bool(const User &)> filter)
{
	int count = 0;
	auto iter = m_userList.begin();
	for (iter; iter != m_userList.end();)
	{
		if (filter(*iter))
		{
			auto target = iter;
			iter = m_userList.erase(target);
			count ++;
			continue;
		}
		iter ++;
	}
	if (count != 0) m_dirty = true;
	return count;
}

void Storage::createMeeting(const Meeting &t_meeting)
{
	m_meetingList.push_back(t_meeting);
	m_dirty = true;
}

list<Meeting> Storage::queryMeeting(function<bool(const Meeting &)> filter) const
{
	list<Meeting> target;
	auto iter = m_meetingList.begin();
	for (iter; iter != m_meetingList.end(); iter++) if (filter(*iter)) target.push_back(*iter);
	return target;
}

int Storage::updateMeeting(function<bool(const Meeting &)> filter,
	                       function<void(Meeting &)> switcher)
{
	int count = 0;
	auto iter = m_meetingList.begin();
	for (iter; iter != m_meetingList.end(); iter++)
	{
		if (filter(*iter))
		{
			switcher(*iter);
			count ++;
		}
	}
	if (count != 0) m_dirty = true;
	return count;
}

int Storage::deleteMeeting(function<bool(const Meeting &)> filter)
{
	int count = 0;
	auto iter = m_meetingList.begin();
	for (iter; iter != m_meetingList.end();)
	{
		if (filter(*iter))
		{
			auto target = iter;
			iter = m_meetingList.erase(target);
			count ++;
			continue;
		}
		iter ++;
	}
	if (count != 0) m_dirty = true;
	return count;
}

bool Storage::sync(void)
{
	if (m_dirty == false) return false;
	else return writeToFile();
}
