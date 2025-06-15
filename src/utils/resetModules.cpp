#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <cstring>
#include <cstdio>
#include <mysql/mysql.h>

#include "cgicc/Cgicc.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPContentHeader.h"

using namespace std;
using namespace cgicc;

char sql[512];
char cgiOut[512];
string emailPinCookie;
string userIDCookie;
string phaseCookie;
string emailPinSaved;
string userIDSaved;
string phaseSaved;
string lastUserID;
time_t timeSaved;
time_t currentTime;
string moduleStr;
char cmd[100];
int res;
int isNumber(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

int isInRange(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    int num = stoi(s);
    if (num >= 0 && num <=100){
    return 1;
    }else{
    return 0;
    }
    return 1;
}
string decrypt (string s){ //presumes attacker doesnt have access sourcecode and write permissions to pwds.txt, as they would find out the encryption

    for (int i=0 ; i < s.length(); i++)
        s[i]++;
    return s;
}
// Pre:
// - s is not null
// - s is a string
// Post:
// - returns the encrypted password

string encrypt (string s){ //presumes attacker doesnt have access sourcecode and write permissions to pwds.txt, as they would find out the encryption

    for (int i=0 ; i < s.length(); i++)
        s[i]--;
    return s;
}
int main()
{
	
	try{
		
		
		Cgicc cgi;
                const_cookie_iterator cci;
   //  		cout << HTTPContentHeader("text/html; charset=utf-8");
               MYSQL *connect;                               // setup mysql
              connect = mysql_init(NULL);                // initiate sql connection

              if (!connect)                              // test if connected
              {
                cout << "Mysql Initialization Failed"; 
                  return 1;
               }
     			
      		connect = mysql_real_connect(connect,
                   "127.0.0.1","harrison","global20","password",
                       0,NULL,0);                    // connect to sql server database password
                if (connect)                         // test if database password
                {
      			// for debug cout << "[+] Connection Success!\n" << "<br>";
      			
      		}
                 else
                {
                     cout << "Connection Failed\n";    // if problem exit
                     return 1;
                }
                MYSQL_RES *res_set;              // get  result set which contains data
                MYSQL_ROW row;                   // row of data
                 mysql_query (connect,"drop table maths");
                 mysql_query (connect,"CREATE TABLE maths( name varchar(20),mark varchar(20))");
                  mysql_query (connect,"insert into maths  value ('smith','10')");
                 mysql_query (connect,"insert into maths  value ('jones','20')");
                 mysql_query (connect,"insert into maths  value ('wiliams','30')");
                 mysql_query (connect,"drop table physics");
                 mysql_query (connect,"CREATE TABLE physics( name varchar(20),mark varchar(20))");
                  mysql_query (connect,"insert into physics  value ('smith','40')");
                 mysql_query (connect,"insert into physics  value ('jones','50')");
                 mysql_query (connect,"insert into physics  value ('wiliams','60')");
                 mysql_query (connect,"drop table chem");
                 mysql_query (connect,"CREATE TABLE chem( name varchar(20),mark varchar(20))");
                  mysql_query (connect,"insert into chem  value ('smith','70')");
                 mysql_query (connect,"insert into chem  value ('jones','80')");
                 mysql_query (connect,"insert into chem  value ('wiliams','90')");
                 mysql_query (connect,"drop table english");
                mysql_query (connect,"CREATE TABLE english( name varchar(20),mark varchar(20))");
                  mysql_query (connect,"insert into english value ('smith','15')");
                 mysql_query (connect,"insert into english  value ('jones','25')");
                 mysql_query (connect,"insert into english  value ('wiliams','35')");
                 mysql_query (connect,"drop table history");
                mysql_query (connect,"CREATE TABLE history( name varchar(20),mark varchar(20))");
                  mysql_query (connect,"insert into history  value ('smith','45')");
                 mysql_query (connect,"insert into history  value ('jones','55')");
                 mysql_query (connect,"insert into history  value ('wiliams','65')");

            mysql_close (connect); // close sql connection
            cout << "Modules Database Reset "<< endl;
	}
	catch(exception& e) {
      	// handle any errors - omitted for brevity
   	}
}
                             		

