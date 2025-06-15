/***********************************************************************************
This stems from the webpage selectLecturer, when the administrator has selected the lecturer they want to add modules to and has pressed “select lecturer”. As usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated. If valid this displays all possible modules that lecturers can teach, with a tick box checked next to the modules that they teach. From here the administrator can assign other modules.
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
string module1;
 const char* modules[5]= { "maths", "physics", "chem", "english","history" };
 char cmd[100];		
	//	std::cout << "Set-Cookie:phase=2;\n";
		Cgicc cgi;
                const_cookie_iterator cci;
                cout << HTTPContentHeader("text/html; charset=utf-8");
                form_iterator lecturer = cgi.getElement("lecturer");
                if(lecturer != cgi.getElements().end()) {
          //   	cout << "lecturer: " << **lecturer << "<br>";
                }
              string lecturerStr((lecturer)->getValue());
                
                
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
      		// Set up the HTML document
      		cout << html() << head(title("Assign Modules")) << endl;
                cout << "<head>\n";
       	cout << "<title> Assign Modules  </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                const char * lecturerStrp =lecturerStr.c_str();
                sprintf(cmd,"<h1 align=\"center\">Assign Modules To Lecturer %s</h1>\n",lecturerStrp);
                cout<< cmd;                
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
                mysql_query (connect,"SELECT * FROM password.users"); // sql query all users
                res_set=mysql_store_result(connect);       // get data
                unsigned int i=3;
		cout<< "<form method=\"post\" action=\"saveLecturerModules.cgi\">"<< endl;
                
                while (((row=mysql_fetch_row(res_set)) !=NULL)){     // get each row data
      		string user1=row[1];
      		   if (user1 == lecturerStr){
        	      for ( i=3;i<8; i++){
      		           module1 = row[i];
      		           const char * modulesp =modules[i-3];
      		              if (module1 == "1"){
      		              sprintf(cmd,"<input type=\"checkbox\" name=%s value=\"on\" checked=\"checked\" onclick=\"return false;\" />%s",modulesp,modulesp);
      		              }else{
      		                sprintf(cmd,"<input type=\"checkbox\" name=%s value=\"on\" / >%s",modulesp,modulesp);

      		              }
      		              cout<< cmd;
      		              
                           }
                     }    
                }
          sprintf(cmd,"<input type=\"hidden\" id=\"lecturer\" name=\"lecturer\" value=\"%s\">",lecturerStrp);
 	  cout<< cmd;
          cout <<  "<input type=\"submit\"   value=\"Save Modules\" style=\"position:absolute;margin-left:-370px; margin-top:30px\">";

          cout << "</form>" << endl;

         cout<< "<form method=\"post\" action=\"selectLecturer.cgi\">"<< endl;
	  cout << "<input type=submit value=\"Back To Select Lecturer\" style=\"position:absolute;margin-left:-185px; margin-top:30px\">";
          cout << endl;  
          cout << "</form>" << endl;
          
          cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	  cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-185px; margin-top:70px\">";
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

