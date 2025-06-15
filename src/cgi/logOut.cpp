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
string emailPinEntered;
string phase;
string userID;
char cmd[100];
int main()
{
	
	try{



                cout << "Set-Cookie:phase=0;\n";
                cout << "Set-Cookie:emailPin=0;\n";
                cout<< "Set-Cookie:userID=0;\n";
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
                
            //    sprintf(cmd,"update emailPin set pin='%s',userID='%s',phase='1' where id=1",pinp,userIDp);
                mysql_query (connect,"update emailPin set pin=\"xxx\",userID=\"0\",phase=\"0\" where id=1"); // sql query all users
                
                // Send HTTP header

      		cout << HTTPContentHeader("text/html; charset=utf-8");
      		// Set up the HTML document

      		cout << html() << head(title("Login Out")) << endl;
                cout << "<head>\n";
       	cout << "<title> Email Login </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Login Out</h1>\n";
            
  	     cout<< "<a href=\"../index.html\" rel=\"external\" title=\"Login Out\">"
                 <<" Return to Login  </a>";
        		
        		
	}
	catch(exception& e) {
      	// handle any errors - omitted for brevity
   	}
}
        		

