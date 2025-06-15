/**************************************************************************
This webpage is displayed when a module has been selected in lecturerModule.cgi. As usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated. The selected modules table is displayed, with the ability to change student marks. Marks can be added in the “alt” field.
***************************************************************************
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
string module1;
string emailPinCookie;
string phaseCookie;
string userIDCookie;
string emailPinSaved;
string userIDSaved;
string phaseSaved;
time_t timeSaved;
time_t currentTime;
char cmd[100];		
//		std::cout << "Set-Cookie:phase=2;\n";
		Cgicc cgi;
                const_cookie_iterator cci;
                cout << HTTPContentHeader("text/html; charset=utf-8");
		
     		form_iterator module2 = cgi.getElement("module1");
                
      		if(module2 != cgi.getElements().end()) {
      		

      		//	cout << "Your UserName: " << **userName << "<br>";

      		}

       	 string moduleStr((module2)->getValue());
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
      		cout << html() << head(title("Lecturer Module")) << endl;
      		cout << "<head>\n";
       	cout << "<title> Lecturer Module </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Lecturer Module</h1>\n";
                cout << moduleStr;

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
                const char * moduleStrp =moduleStr.c_str();
                sprintf(cmd,"SELECT * FROM password.%s",moduleStrp);
                mysql_query (connect,cmd);
      //          mysql_query (connect,"SELECT * FROM password.maths"); // sql query all users
                unsigned int i=1;
                res_set=mysql_store_result(connect);       // get data
                unsigned int numrows = mysql_num_rows(res_set); // get number of rows
                cout<< "<form method=\"post\" action=\"saveModule.cgi\">"<< endl;
                sprintf(cmd,"<input type=\"hidden\" id=\"module1\" name=\"module1\" value=\"%s\">",moduleStrp);
                cout << cmd;	                       
                cout << "<input type=submit value=\"Save Module\" style=\"margin-left:000px; margin-top:250px\">";

                cout << "<table style=\"margin-top:-230px; margin-left:0px;\">";
	
                cout<<"<table border=\"1\" cellpadding=\"10\" >";   // setup html table
      		cout<<"<tr>";                                      // start of table headers
                cout<<"<th>Student</th>";                               // first header entry
  		cout<<"<th>Mark</th>";
                cout << "<th>Alt</th>";

                cout << "</tr>";                                    // end of headers
                cout << "<tr>";                                     // start of table data
                while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      			{
      			  	
      				string student=row[0];                 // get first field of row
      				string mark=row[1];
                                

                                cout<< "<td>"<< student <<"</td>";   // load table data from row field
                                cout<< "<td>"<< mark<<"</td>";
                                cout<< "<td>"; 
                                const char * studentp =student.c_str();
                                sprintf(cmd,"<input type=\"text\" size=2 name=\"%s\">",studentp);
                                cout << cmd;   
                             //   cout << "<input type=\"text\" size=2 name=\"value1\">" << endl; 
                                cout <<"</td>";
                                cout << "</td>";                     
                                cout <<"</td>"<<"</tr>";          // end of table
                              //  i++;
                                
  			}
	                       cout << endl;  
                          //     const char * module1p =module1.c_str();
                       	cout << "</form>" << endl; 
			        mysql_close (connect); // close sql connection  
                    cout << "</form>" << endl;
		    cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	            cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-35px; margin-top:250px\">";
	            cout << endl;  
                    cout << "</form>" << endl;
		}	                           
      		
            

	}
	catch(exception& e) {
      	logout();
   	}
}
