/****************************************************************************************************
This web page is next inline from lecture.cgi. It as usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated and the page prompts for:
User name
Password
Email address
There is a save option and a return to lecturer.cgi.

 Pre:

 Post:

*****************************************************************************************************
*/

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

bool error=false;
 void logout(){ 
               if (!error){              
               cout << html() << head(title("Login Time Out")) << endl;
      		cout << "<head>\n";
       	cout << "<title> Login Time Out </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Login Time Out</h1>\n";
                cout<< "<a href=\"./logOut.cgi\" rel=\"external\" title=\"Login Failure\">"
		    <<" Login Failure Retry </a>";
		    error =true;
		 }   
 } 
int main()
{
	
	try{
char sql[512];
char cgiOut[512];
string emailPinCookie;
string userIDCookie;
string phaseCookie;
string emailPinSaved;
string userIDSaved;
string phaseSaved;
time_t timeSaved;
time_t currentTime;
char cmd[100];
	
		Cgicc cgi;
                const_cookie_iterator cci;


    		cout << HTTPContentHeader("text/html; charset=utf-8");	    
               
 
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
                 mysql_query (connect,"SELECT * FROM password.emailPin");
                unsigned int i=1;
                res_set=mysql_store_result(connect);       // get data
                unsigned int numrows = mysql_num_rows(res_set); // get number of rows
                 while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      			{
      			  	
      				 emailPinSaved=row[1];                 // get first field of row
      				 userIDSaved=row[2];
      				 phaseSaved= row[3]; 
      				 timeSaved = stoi(row[4]);  
   			}
      		const CgiEnvironment& env = cgi.getEnvironment();
               for( cci = env.getCookieList().begin();
                  cci != env.getCookieList().end(); 
                    ++cci ) {
                     if (cci->getName() == "emailPin") {
                      emailPinCookie = cci->getValue();
                      }
                        if (cci->getName() == "phase") {
                          phaseCookie = cci->getValue();                                                      
                        }
                         if (cci->getName() == "userID") {
                          userIDCookie = cci->getValue();
                          }                                                      
                        }
                      currentTime =time(NULL);
                     int timeOut= currentTime - timeSaved;
                   //  cout << " timeout = "<< timeOut;
              if ((emailPinCookie != emailPinSaved )|| (emailPinSaved != emailPinCookie)||(timeOut > 30)){
                    logout();
                   // cout<< "timeout = "<< timeOut;
                    //  cout<< "<a href=\"./logOut.cgi\" rel=\"external\" title=\"Login Failure\">"
		  //	<<" Login Failure Retry </a>";
              }else{                             		
                sprintf(cmd,"update emailPin set time=%ld where id=1",currentTime);
                mysql_query (connect,cmd); // sql query all users
                mysql_close (connect); // close sql connection  

      		// Set up the HTML document
      		cout << html() << head(title("Add Lecturer")) << endl;
      		cout << "<head>\n";
       	cout << "<title> User Options </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Add Lecturer</h1>\n";
               
 
                cout<< "<form method=\"post\" action=\"saveLecturer.cgi\">"<< endl;
                cout << "<p style=\"black:white; margin-left:-120px; margin-top:80px\">User Name</p>" <<endl; 
	        cout << "<input type=text size=20 name=\"name\" style=\"margin-left:179px; margin-top:-44px\">";
                cout << "<p style=\"black:white; margin-left:-120px; margin-top:0px\">Password</p>" <<endl; 
	        cout << "<input type=password size=20 name=\"password\" style=\"margin-left:179px; margin-top:-44px\">";
                cout << "<p style=\"black:white; margin-left:-150px; margin-top:0px\">Email Address</p>" <<endl; 
	        cout << "<input type=text size=20 name=\"emailAddress\" style=\"margin-left:180px; margin-top:-40px\">";
                
                cout << "<input type=submit value=\"Save Lecturer\" style=\"position:absolute;margin-left:-200px; margin-top:00px\">";
                cout << endl;
                  
                cout << "</form>" << endl;
                cout<< "<form method=\"post\" action=\"lecturer.cgi\">"<< endl;
                cout << "<input type=submit value=\"Lecturer Options \" style=\"position:absolute;margin-left:-9px; margin-top:10px\">";
                cout << endl;  
			         
               cout << "</form>" << endl;
		cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
//	        cout << "<input type=submit value=\"Log Out\" style=\"margin-left:-00px; margin-top:00px\">";
	        cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-9px; margin-top:35px\">";
	        cout << endl;  
                cout << "</form>" << endl;
                 
              

               }
           
	}
	catch(exception& e) {
      	logout();
   	}
}

