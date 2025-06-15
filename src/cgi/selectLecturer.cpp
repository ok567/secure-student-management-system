/***********************************************************************************
This stems from the webpage lecturer.cgi and is activated when the administrator selects “selects lecturer”. As usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated.
This now displays a drop down box with all the lecturers available on the system. From here the administrator can select the relevant lecturer to assign additional modules. 

************************************************************************************
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
string phaseCookie;
string userIDCookie;
string emailPinSaved;
string userIDSaved;
string phaseSaved;
time_t timeSaved;
time_t currentTime;
char cmd[100];
		
	//	std::cout << "Set-Cookie:phase=2;\n";
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
              if ((emailPinCookie != emailPinSaved )|| (emailPinSaved != emailPinCookie)||(timeOut > 60)){
                   logout();
              }else{                             		
                sprintf(cmd,"update emailPin set time=%ld where id=1",currentTime);
                mysql_query (connect,cmd); // sql query all users
	         mysql_close (connect); // close sql connection  
               
               // Send HTTP header

      	//	cout << HTTPContentHeader("text/html; charset=utf-8");
      		// Set up the HTML document

      		cout << html() << head(title("Select Lecturer")) << endl;
                cout << "<head>\n";
       	cout << "<title> Select Lecturer  </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Select Lecturer</h1>\n";
               
                    
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
       //     cout << "</form>" << endl;
		cout<< "<form method=\"post\" action=\"assignModule.cgi\">"<< endl;
		cout<< "<p style=\"margin-left:-180px; margin-top:0px\">Lecturer:</p>"<< endl;
		cout<< "<select name=\"lecturer\" style=\"margin-left:80px; margin-top:-42px\">"<< endl;
                
                MYSQL_RES *res_set;              // get  result set which contains data
                MYSQL_ROW row;                   // row of data
                mysql_query (connect,"SELECT * FROM password.users"); // sql query all users
                unsigned int i=0;
                res_set=mysql_store_result(connect);       // get data
                 while (((row=mysql_fetch_row(res_set)) !=NULL)){     // get each row data
       		string user1=row[1];
       		 if (user1 != "admin"){
       		     const char * user1p = user1.c_str();
                             sprintf(cmd,"<option value=%s selected> %s </option>",user1p,user1p);
                            cout<< cmd; 
                          }        
                 }
                 cout<< "</select> <br>" << endl;
                cout << "<input type=submit value=\"Select Lecturer\" style=\"position:absolute;margin-left:-130px; margin-top:00px\">";
                cout << "</form>" << endl;
 	     cout<< "<form method=\"post\" action=\"lecturer.cgi\">"<< endl;
	     cout << "<input type=submit value=\"Back To Lecturer Modules\" style=\"position:absolute;margin-left:-190px; margin-top:50px\">";
	     cout << endl;  
               
                cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	        cout << "<input type=submit value=\"Log Out\" style=\"margin-left:-190px; margin-top:20px\">"<< endl;
	        cout << endl;  
                cout << "</form>" << endl;
            }    
  	mysql_close (connect); // close sql connection 
  	
                  
    }
	catch(exception& e) {
      	     cout << HTTPContentHeader("text/html; charset=utf-8");
      	     logout();
   	}
}

