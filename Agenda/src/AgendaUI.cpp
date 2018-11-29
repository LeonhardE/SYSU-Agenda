//
//  AgendaUI.cpp
//  C++
//
//  Created by 曾宪欣 on 2018/11/28.
//  Copyright © 2017年 曾宪欣. All rights reserved.
//

#include "AgendaUI.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

AgendaUI::AgendaUI()
{
    startAgenda();
}

void AgendaUI::OperationLoop(void)
{   
    string op;
    do
    {
        cout << "--------------------------------------Agenda--------------------------------------" << endl;
        cout << "Action :" << endl;
        cout << "l    - log in Agenda by user name and password" << endl;
        cout << "r    - register an Agenda account" << endl;
        cout << "q    - quit Agenda" << endl;
        cout << "----------------------------------------------------------------------------------" << endl << endl;
        cout << "Agenda :~$ ";

        cin >> op;
        switch( op[0] )
        {
            case 'l': userLogIn(); break;
            case 'r': userRegister(); break;
            case 'q': quitAgenda(); break;
            default: cout << "Illegal input, please try again." << endl; break;
        }
    } while( op[0] != 'q' );
}   

void AgendaUI::startAgenda(void)
{
    m_agendaService.startAgenda();
}

string AgendaUI::getOperation()
{
    cout << "--------------------------------------Agenda--------------------------------------" << endl;
    cout << "Action :" << endl;
    cout << "o    - log out Agenda" << endl;
    cout << "dc   - delete Agenda user" << endl;
    cout << "lu   - list all Agenda user" << endl;
    cout << "cm   - create a meeting" << endl;
    cout << "amp  - add meeting participator" << endl;
    cout << "rmp  - remove meeting participator" << endl;
    cout << "rqm  - request to quit meeting" << endl;
    cout << "la   - list all meetings" << endl;
    cout << "las  - list all sponsor meetings" << endl;
    cout << "lap  - list all participator meetings" << endl;
    cout << "qm   - query meeting by title" << endl;
    cout << "qt   - query meeting by time interval" << endl;
    cout << "dm   - delete meeting by title" << endl;
    cout << "da   - delete all meetings" << endl;
    cout << "----------------------------------------------------------------------------------" << endl << endl;
    cout << "Agenda@" << m_userName << " :~# ";
    string op;
    cin >> op;
    return op;
}

bool AgendaUI::executeOperation(const string t_operation)
{
    if( t_operation == "o" )
    {
        userLogOut();
        cout << "User logged out." << endl;
        return false;
    }
    else if( t_operation == "dc" )
    {
        deleteUser();
        return false;
    }
    else if( t_operation == "lu" )
    {
       listAllUsers();
       return true;
    }
    else if( t_operation == "cm" )
    {
        createMeeting();
        return true;
    }
    else if (t_operation == "amp")
    {
        addMeetingParticipator();
        return true;
    }
    else if (t_operation == "rmp")
    {
        removeMeetingParticipator();
        return true;
    }
    else if (t_operation == "rqm")
    {
        quitMeeting();
        return true;
    }
    else if( t_operation == "la" )
    {
        listAllMeetings();
        return true;
    }
    else if( t_operation == "las" )
    {
        listAllSponsorMeetings();
        return true;
    }
    else if( t_operation == "lap" )
    {
        listAllParticipateMeetings();
        return true;
    }
    else if( t_operation == "qm" )
    {
        queryMeetingByTitle();
        return true;
    }
    else if( t_operation == "qt" )
    {
        queryMeetingByTimeInterval();
        return true;
    }
    else if( t_operation == "dm" )
    {
        deleteMeetingByTitle();
        return true;
    }
    else if( t_operation == "da" )
    {
        deleteAllMeetings();
        return true;
    }
    else
    {
        cout << "Illegal input, please try again." << endl;
        return true;
    }
}

void AgendaUI::userLogIn(void)
{
    cout << endl << "[log in] [username] [password]" << endl;
    cout << "[log in] ";
    string username, password;
    cin >> username >> password;
    if (m_agendaService.userLogIn(username,password))
    {
        cout << "[log in] Succeed!\n" << endl;
        m_userName = username;
        m_userPassword = password;
        while (executeOperation(getOperation()));
    }
    else cout << "[log in] Password incorrect or user doesn't exist" << endl;
}

void AgendaUI::userRegister(void)
{
    cout << endl << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string name, password, email, phone;
    cin >> name >> password >> email >> phone;
    if (m_agendaService.userRegister(name,password,email,phone)) cout << "[register] Succeed!" << endl;
    else cout << "[register] This username has been registered!" << endl;
}

void AgendaUI::userLogOut(void)
{
    m_userName = "";
    m_userPassword = "";
}

void AgendaUI::quitAgenda(void)
{
    m_agendaService.quitAgenda();
    cout << endl << "[quit Agenda] Thank you for using the Agenda! Bye-bye!" << endl;
}

void AgendaUI::deleteUser(void)
{
    if (m_agendaService.deleteUser(m_userName,m_userPassword)) cout << endl << "[delete agenda account] Succeed!" << endl;
    userLogOut();
}

void AgendaUI::listAllUsers(void)
{
    list<User> user = m_agendaService.listAllUsers();
    cout << endl << "[list all users]" << endl << endl;
    cout << left << setw(15) << "name" << left << setw(25) << "email" << "phone" << endl;
    for (auto i = user.begin(); i != user.end(); i++) cout << left << setw(15) << (*i).getName() << left << setw(25) << (*i).getEmail() << (*i).getPhone() << endl; 
}

void AgendaUI::createMeeting(void)
{
    cout << endl << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int count = 0;
    cin >> count;
    vector<string> par;
    for (int i = 0; i < count; i++ )
    {
        cout << "[create meeting] [please enter the participator " << i + 1 << " ]" << endl;
        string name;
        cin >> name;
        par.push_back(name);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    string title, start, end;
    cin >> title >> start >> end;
    
    if(m_agendaService.createMeeting(m_userName,title,start,end,par)) cout << "[create meeting] Succeed!" << endl;
    else cout << "[create meeting] Fail!" << endl;
}

void AgendaUI::addMeetingParticipator(void)
{
    cout << endl << "[add participator] [meeting title] [participator username]" << endl;
    cout << "[add participator] ";
    string title, participator;
    cin >> title >> participator;
    if (m_agendaService.addMeetingParticipator(m_userName, title, participator)) cout << "[add participator] Succeed!" << endl;
    else cout << "[add participator] Fail!"<< endl;
}

void AgendaUI::removeMeetingParticipator(void)
{
    cout << endl << "[remove participator] [meeting title] [participator username]" << endl;
    cout << "[remove participator] ";
    string title, participator;
    cin >> title >> participator;
    if (m_agendaService.removeMeetingParticipator(m_userName, title, participator)) cout << "[remove participator] Succeed!" << endl;
    else cout << "[remove participator] Fail!"<< endl;
}

void AgendaUI::quitMeeting(void)
{
    cout << endl << "[quit meeting] [meeting title]" << endl;
    cout << "[quit meeting] ";
    string title;
    cin >> title;
    if (m_agendaService.quitMeeting(m_userName, title)) cout << "[quitMeeting] Succeed!" << endl;
    else cout << "[quitMeeting] Fail!"<< endl;
}

void AgendaUI::listAllMeetings(void)
{
    cout << endl << "[list all meetings]" << endl << endl;
    list<Meeting> m = m_agendaService.listAllMeetings(m_userName);
    printMeetings(m);
}

void AgendaUI::listAllSponsorMeetings(void)
{
    cout << endl << "[list all sponsor meetings]" << endl << endl;
    list<Meeting> m = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(m);
}

void AgendaUI::listAllParticipateMeetings(void)
{
    cout << endl << "[list all participator meetings]" << endl << endl;
    list<Meeting> m = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(m);
}

void AgendaUI::queryMeetingByTitle(void)
{
    cout << endl << "[query meeting] [title]:" << endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    list<Meeting> m = m_agendaService.meetingQuery(m_userName,title);
    printMeetings(m);
}

void AgendaUI::queryMeetingByTimeInterval(void)
{
    cout << endl << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meetings] ";
    string start, end;
    cin >> start >> end;
    cout << "[query meetings]" << endl;
    list<Meeting> m = m_agendaService.meetingQuery(m_userName,start,end);
    printMeetings(m);
}

void AgendaUI::deleteMeetingByTitle(void)
{
    cout << endl << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName,title)) cout << "[delete meeting] Succeed!" << endl;
    else cout << "[error] Delete meeting fail!" << endl;
}

void AgendaUI::deleteAllMeetings(void)
{
    if (m_agendaService.deleteAllMeetings(m_userName)) cout << "[delete all meeting] Succeed!" << endl;
    else cout << "[delete all meeting] Fail!" << endl;
}

void AgendaUI::printMeetings(const list<Meeting> t_meetings)
{
    cout << left << setw(15) << endl << "Title" << left << setw(12) << "Sponsor" << left << setw(20) << "Start time" << left << setw(20) << "End time" << left << setw(15) << "Participators" << endl;
    for (auto i = t_meetings.begin(); i != t_meetings.end(); i++)
    {
      vector<string> p = (*i).getParticipator();
      string participatorstr;
      if (p.size() == 0) participatorstr = "";
      else
      {
          for (int i = 0; i < p.size() - 1; i++)
          {
              participatorstr += p[i];
              participatorstr += ", ";
          }
          participatorstr += p[p.size() - 1];
      }
      cout << left << setw(15) << (*i).getTitle() << left << setw(12) << (*i).getSponsor() << left << setw(20) << Date::dateToString( (*i).getStartDate() ) << left << setw(20)  << Date::dateToString( (*i).getEndDate() ) << left << setw(15) << participatorstr << endl;
    }
}
