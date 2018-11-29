//
//  AgendaService.cpp
//  C++
//
//  Created by 曾宪欣 on 2018/11/28.
//  Copyright © 2017年 曾宪欣. All rights reserved.
//

#include "AgendaService.hpp"
#include <list>
#include <string>
#include <memory>
#include <functional>
#include <iostream>
using namespace std;

AgendaService::AgendaService()
{
    startAgenda();
}

AgendaService::~AgendaService()
{
    quitAgenda();
}

bool AgendaService::userLogIn(const string &userName, const string &password)
{
    list<User> userlist = m_storage->queryUser([&](const User &userA){return (userA.getName() == userName && userA.getPassword() == password);});
    return (userlist.size() != 0);
}

bool AgendaService::userRegister(const string &userName, const string &password,
                    const string &email, const string &phone)
{
    User newuser(userName, password, email, phone);
    if (m_storage->queryUser([&](const User &userA){return userA.getName() == userName;}).size() == 0) m_storage->createUser(newuser);
    return m_storage->sync();
}

bool AgendaService::deleteUser(const string &userName, const string &password)
{
    int count = m_storage->deleteUser([&](const User &userA){return (userA.getName() == userName && userA.getPassword() == password);});
    if (count == 0) return false;
    else // side effect
    {
        m_storage->deleteMeeting([&](const Meeting &meetingA){return (meetingA.getSponsor() == userName);});
        m_storage->updateMeeting([&](const Meeting &meetingA){return meetingA.isParticipator(userName);},
                                 [&](Meeting &meetingB){meetingB.removeParticipator(userName);});
        m_storage->deleteMeeting([&](const Meeting &meetingA){return (meetingA.getParticipator().size() == 0);});
    }
    return m_storage->sync();
}

list<User> AgendaService::listAllUsers(void) const
{
    return m_storage->queryUser([&](const User &userA){return true;});
}

bool AgendaService::createMeeting(const string &userName, const string &title,
                   const string &startDate, const string &endDate,
                   const vector<string> &participator)
{
    Date start(startDate);
    Date end(endDate);
    cout << Date::dateToString(start) << " " << Date::dateToString(end) << endl;
    if (!(start.isValid(start) && start.isValid(end))) return false;
    if (m_storage->queryMeeting([&](const Meeting &meetingA){return meetingA.getTitle() == title;}).size() == 0) 
    {
        if (start < end)
        {
            list<User> involve = m_storage->queryUser([&](const User &userA){
                bool flag = false;
                for (auto iter = participator.begin(); iter != participator.end(); iter++)
                {
                    if ((*iter) == userA.getName()) flag = true;
                }
                if (userName == userA.getName()) flag = true;
                return flag;
            });
            if (involve.size() != participator.size() + 1) return false;
            for (auto iter = participator.begin(); iter != participator.end(); iter++)
            {
                if ((*iter) == userName) return false;
                list<Meeting> m = m_storage->queryMeeting([&](const Meeting &meetingA){
                    if (meetingA.getSponsor() == (*iter) || meetingA.isParticipator((*iter)))
                    {
                        if (end <= meetingA.getStartDate() || start >= meetingA.getEndDate()) return false;
                        else return true;
                    }
                    else return false;
                });
                if (m.size() != 0) return false;
            }
            list<Meeting> m = m_storage->queryMeeting([&](const Meeting &meetingA){
                if (meetingA.getSponsor() == userName || meetingA.isParticipator(userName))
                {
                    if (end <= meetingA.getStartDate() || start >= meetingA.getEndDate()) return false;
                    else return true;
                }
                else return false;
            });
            if (m.size() != 0) return false;

            Meeting newmeeting(userName, participator, start, end, title);
            m_storage->createMeeting(newmeeting);
            return m_storage->sync();
        }
    }
    return false;
}

bool AgendaService::addMeetingParticipator(const string &userName,
                            const string &title,
                            const string &participator)
{
    list<User> person = m_storage->queryUser([&](const User &userA){return userA.getName() == participator;});
    if (person.size() != 1) return false;
    list<Meeting> target = m_storage->queryMeeting([&](const Meeting &meetingA){return !meetingA.isParticipator(participator) && meetingA.getTitle() == title;});
    if (target.size() == 0 || target.size() > 1) return false;
    else
    {
        Meeting m = target.front();
        Date start = m.getStartDate();
        Date end = m.getEndDate();
        list<Meeting> meetings = m_storage->queryMeeting([&](const Meeting &meetingA){
            if (meetingA.getSponsor() == participator || meetingA.isParticipator(participator))
            {
                if (end <= meetingA.getStartDate() || start >= meetingA.getEndDate()) return false;
                else return true;
            }
            else return false;
        });
        if (meetings.size() != 0) return false;
        m_storage->updateMeeting([&](const Meeting &meetingA){return meetingA.getTitle() == title;}, 
                                 [&](Meeting &meetingB){meetingB.addParticipator(participator);});
    }
    return m_storage->sync();
}

bool AgendaService::removeMeetingParticipator(const string &userName,
                               const string &title,
                               const string &participator)
{
    int count = m_storage->updateMeeting([&](const Meeting &meetingA){return meetingA.getTitle() == title && meetingA.isParticipator(participator);},
                                         [&](Meeting &meetingB){meetingB.removeParticipator(participator);});
    if (count == 0) return false;
    m_storage->deleteMeeting([&](const Meeting &meetingA){return meetingA.getParticipator().size() == 0;});
    return m_storage->sync();
}

bool AgendaService::quitMeeting(const string &userName, const string &title)
{
    int count = m_storage->updateMeeting([&](const Meeting &meetingA){return meetingA.isParticipator(userName) && meetingA.getTitle() == title;},
                                         [&](Meeting &meetingB){meetingB.removeParticipator(userName);});
    if (count == 0) return false;
    m_storage->deleteMeeting([&](const Meeting &meetingA){return meetingA.getParticipator().size() == 0;});
    return m_storage->sync();
}

list<Meeting> AgendaService::meetingQuery(const string &userName,
                                const string &title) const
{
    return m_storage->queryMeeting([&](const Meeting &meetingA){return (meetingA.isParticipator(userName) || meetingA.getSponsor() == userName) && meetingA.getTitle() == title;});
}

list<Meeting> AgendaService::meetingQuery(const string &userName,
                                const string &startDate,
                                const string &endDate) const
{
    Date start(startDate);
    Date end(endDate);
    if (!(start.isValid(start) && start.isValid(end)) || start > end) 
    {
        list<Meeting> m;
        return m;
    }
    else return m_storage->queryMeeting([&](const Meeting &meetingA){
        if (meetingA.getSponsor() == userName || meetingA.isParticipator(userName))
        {
            if (end < meetingA.getStartDate() || start > meetingA.getEndDate()) return false;
            else return true;
        }
        else return false;
    });
}

list<Meeting> AgendaService::listAllMeetings(const string &userName) const
{
    return m_storage->queryMeeting([&](const Meeting &meetingA){return meetingA.isParticipator(userName) || meetingA.getSponsor() == userName;});
}

list<Meeting> AgendaService::listAllSponsorMeetings(const string &userName) const
{
    return m_storage->queryMeeting([&](const Meeting &meetingA){return meetingA.getSponsor() == userName;});
}

list<Meeting> AgendaService::listAllParticipateMeetings(
    const string &userName) const
{
    return m_storage->queryMeeting([&](const Meeting &meetingA){return meetingA.isParticipator(userName);});
}

bool AgendaService::deleteMeeting(const string &userName, const string &title)
{
    int target = m_storage->deleteMeeting([&](const Meeting &meetingA){return meetingA.getSponsor() == userName && meetingA.getTitle() == title;});
    if (target == 0) return false;
    else return m_storage->sync();
}

bool AgendaService::deleteAllMeetings(const string &userName)
{
    int target = m_storage->deleteMeeting([&](const Meeting &meetingA){return meetingA.getSponsor() == userName;});
    if (target == 0) return false;
    else return m_storage->sync();
}

void AgendaService::startAgenda(void)
{
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void)
{
    m_storage->sync();
}
