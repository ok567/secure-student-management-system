/********************************************************************************************
This webpage is displayed when Lecturer Modules is selected from the lecturer.cgi. As usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated. It uses the userID of the lecture that has logged in, to display a table of the modules they teach. From here he can select any of the modules which he wishes to update marks for. The table shows the following modules “Maths”, “Physics”, “Chem”, “English” and “History”, as this is a system set amount of modules.
*********************************************************************************************
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
const char* modules[5]= { "maths", "physics", "chem", "english","history" };
		
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

      		cout << html() << head(title("Lecturer Modules")) << endl;
                cout << "<head>\n";
       	cout << "<title> Lecturer Modules </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Lecturer Modules</h1>\n";
               
                    
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
                unsigned int i=0;
                res_set=mysql_store_result(connect);       // get data
                unsigned int numrows = mysql_num_rows(res_set); // get number of rows
    
                cout << "<table style=\"margin-top:-0px; margin-left:0px;\">";
	
                cout<<"<table border=\"1\" cellpadding=\"10\" >";   // setup html table
      		cout<<"<tr>";                                      // start of table headers
                cout<<"<th>ID</th>";                               // first header entry
  		cout<<"<th>User</th>";
          //      cout << "<th>Password</th>";
                cout << "<th>Module1</th>";
                cout << "<th>Module2</th>";
                cout << "<th>Module3</th>";
                cout << "<th>Module4</th>";
                cout << "<th>Module5</th>";  
                cout << "<th>Email</th>";                           // final header entry

                cout << "</tr>";                                    // end of headers
                cout << "<tr>";                                     // start of table data
                while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      			{
      			  	
      				string userID1=row[0];                 // get first field of row
      				if (userID1 == userIDSaved ||userIDSaved == "1"){
      				string user1=row[1];
                //                string passwd1=row[2];
       			string email1=row[8];                  // get final field of row
                                

                                cout<< "<td>"<< userID1 <<"</td>";   // load table data from row field
                                cout<< "<td>"<< user1 <<"</td>";
               //                 cout<< "<td>"<< passwd1 <<"</td>";
                                 for (i=3; i<8; i++){
  		                   string moduleStr=row[i];
  		                    if (moduleStr != "0"){
  	                       //       const char * modulep =moduleStr.c_str();
  	                              const char * modulep =modules[i-3];
  	                              cout << "<td>";
                                     sprintf(cmd,"<a href=\"./module.cgi?module1=%s\" rel=\"external\" title=\"%s Module\">",modulep,modulep);
                                     cout << cmd;
                                     
                                     sprintf(cmd,"%s </a>",modulep);
                                     cout << cmd;
                                     cout << "</td>";
                       }else {
                               cout<< "<td>"<< " " <<"</td>";
                       }
                     }
                                cout<< "<td>"<< email1 <<"</td>";  //load table data from final row 
                                cout <<"</td>"<<"</tr>";          // end of table
                                }
  			}
			        mysql_close (connect); // close sql connection
                cout<< "<form method=\"post\" action=\"lecturer.cgi\">"<< endl;
                cout << "<input type=submit value=\"Lecturer Modules \" style=\"position:absolute;margin-left:-550px; margin-top:100px\">";
                cout << endl;  
			         
               cout << "</form>" << endl;
		cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
//	        cout << "<input type=submit value=\"Log Out\" style=\"margin-left:-00px; margin-top:00px\">";
	        cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-550px; margin-top:130px\">";
	        cout << endl;  
                cout << "</form>" << endl;
                
            
           } // phase test             
	}
	catch(exception& e) {
      	logout();
   	}
}

