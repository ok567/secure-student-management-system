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
                 mysql_query (connect,"drop table emailPin");
                 mysql_query (connect,"CREATE TABLE emailPin ( id int,pin varchar(20),userID varchar(2),phase varchar(2),time bigint)");
                  mysql_query (connect,"insert into emailPin value (1,'xxx','0','0',0)");
            mysql_close (connect); // close sql connection
            cout << "Emailpin Database Reset "<< endl;
	}
	catch(exception& e) {
      	// handle any errors - omitted for brevity
   	}
}
                             		

