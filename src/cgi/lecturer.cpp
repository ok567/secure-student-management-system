/**************************************************************************************************
This web page is next inline from the emailpin page for lecturers. It collects the entered email pin and compares it with the emailpin database table. If the stored values match and that the current time - timestamp is not greater than 30 (30 second time out value) is set cookie:phase = 2 to indicate a completed login. The emailpin table is also update with phase 2 and time stamped with the current time (gives an other 30 seconds time out period). A menu option for either lecturers/administer is displayed.
Lecturer
Add Lecturer
Lecturer Modules

Administer 
Add Lecturer
Select Module
Select Lecturer
Lecturer Module       

 Pre:
 emailPin
 Post:
 

***************************************************************************************************
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
char cmd[100];
int main()
{
	
	try{
char sql[512];
char cgiOut[512];
string emailPinCookie;
string userIDCookie;
string phaseCookie;
//string emailPinStr;
string phase;
string userID;
string emailPinSaved;
string userIDSaved;
string phaseSaved;
time_t timeSaved;
time_t currentTime;
bool error=false;
		
		Cgicc cgi;
                const_cookie_iterator cci;
       		form_iterator emailPin = cgi.getElement("emailPin");
      		if(emailPin != cgi.getElements().end()) {
         		//cout << "emailPin: " << **emailPin << "<br>";
         		
      		}
               string emailPinStr((emailPin)->getValue());
      		
     			    
               
 
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
          //      unsigned int i=1;
                res_set=mysql_store_result(connect);       // get data
          //      unsigned int numrows = mysql_num_rows(res_set); // get number of rows
                 while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      			{
      			  	
      				 emailPinSaved=row[1];                 // get first field of row
      				 userIDSaved=row[2];
      				 phaseSaved= row[3];
      				 timeSaved = stoi(row[4]); 
      				// cout<< row[4] << " "; 
   			}
   		//	cout<< "time = "<< test;
   		//	timeSaved = stoi(timeStr);
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
                     } if (phaseSaved == "2"){      // used for reentry
                           emailPinStr = emailPinSaved;
                     }
                     currentTime =time(NULL);
                     int timeOut= currentTime - timeSaved;
                    // cout<< "timeOut "<< timeOut;
               if ((emailPinCookie != emailPinSaved )|| (emailPinSaved != emailPinStr)||(timeOut > 130)){
                    cout << HTTPContentHeader("text/html; charset=utf-8");
                     logout();
               }else{
                     std::cout << "Set-Cookie:phase=2;\n"; 
                 sprintf(cmd,"update emailPin set phase='2', time=%ld where id=1",currentTime);
                mysql_query (connect,cmd); // sql query all users
	         mysql_close (connect); // close sql connection  
                                           		
             cout << HTTPContentHeader("text/html; charset=utf-8");
              if ( phaseSaved !=  phaseCookie ){
                    logout();
                 //   cout<< "<a href=\"../index.html\" rel=\"external\" title=\"Login Failure\">"
		//	<<" Login Failure Retry </a>";
              }else{
                    if (userIDSaved == "1"){
      		// Set up the HTML document
      		cout << html() << head(title("Administrators Options")) << endl;
      		      	        cout << "<head>\n";
       	cout << "<title> User Options </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Administrators Options</h1>\n";
               
                cout<< "<form method=\"post\" action=\"addLecturer.cgi\">"<< endl;
                cout << "<input type=submit value=\"Add Lecturer\" style=\"margin-left:000px; margin-top:0px\">";
                cout << endl;  
                cout << "</form>" << endl;
                
                cout<< "<form method=\"post\" action=\"selectModule.cgi\">"<< endl;
                cout << "<input type=submit value=\"Add Student To module\" style=\"margin-left:000px; margin-top:0px\">";
                cout << endl;  
                cout << "</form>" << endl;

                cout<< "<form method=\"post\" action=\" selectLecturer.cgi\">"<< endl;
                cout << "<input type=submit value=\"Select Lecturer \" style=\"margin-left:000px; margin-top:0px\">";
                cout << endl;  
                cout << "</form>" << endl;
                
                cout<< "<form method=\"post\" action=\"lecturerModule.cgi\">"<< endl;
                cout << "<input type=submit value=\"Lecturer Modules \" style=\"margin-left:000px; margin-top:0px\">";
                cout << endl;
                cout << "</form>" << endl;  
                
		cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	        cout << "<input type=submit value=\"Log Out\" style=\"margin-left:-00px; margin-top:00px\">";
//	        cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-300px; margin-top:250px\">";
	        cout << endl;  
                cout << "</form>" << endl;     
                
                    }else{   

    		// Send HTTP header
      		//cout << HTTPContentHeader("text/html; charset=utf-8");
      		//cout << HTTPHTMLHeader() << endl;

      		// Set up the HTML document
      		cout << html() << head(title("Lecturer Options")) << endl;
      		      	        cout << "<head>\n";
       	cout << "<title> User Options </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Lecturer Options</h1>\n";
               
                cout<< "<form method=\"post\" action=\"addLecturer.cgi\">"<< endl;
                cout << "<input type=submit value=\"Add Lecturer\" style=\"margin-left:000px; margin-top:0px\">";
                cout << endl;  
                cout << "</form>" << endl;

                cout<< "<form method=\"post\" action=\"lecturerModule.cgi\">"<< endl;
                cout << "<input type=submit value=\"Lecturer Modules \" style=\"margin-left:000px; margin-top:0px\">";
                cout << endl;  
                cout << "</form>" << endl;
		cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	        cout << "<input type=submit value=\"Log Out\" style=\"margin-left:-00px; margin-top:00px\">";
//	        cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-300px; margin-top:250px\">";
	        cout << endl;  
                cout << "</form>" << endl;
                
                  }
               }
           }
	}
	catch(exception& e) {
	     cout << HTTPContentHeader("text/html; charset=utf-8");
        	logout();
   	}
}

