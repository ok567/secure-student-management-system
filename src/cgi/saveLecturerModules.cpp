/***********************************************************************************
This stems from the webpage assignModule.cgi, when the administrator has selected “Save Modules”. As usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated. If valid the module record for that lecturer is updated and is displayed as a table with updates on it.
************************************************************************************
*/
#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 
#include <mysql/mysql.h>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h> 
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

int main () {
try{
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
const char* modules[5]= { "maths", "physics", "chem", "english","history" };

   Cgicc cgi;
   const_cookie_iterator cci;
   bool maths_flag, physics_flag, chem_flag, english_flag, history_flag;
    cout << HTTPContentHeader("text/html; charset=utf-8");

 //  cout << "Content-type:text/html\r\n\r\n";
      		// Set up the HTML document
      		cout << html() << head(title("Save Lecturer Modules")) << endl;
      		cout << "<head>\n";
       	cout << "<title> Save Lecturer Modules </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Save Lecturer Modules</h1>\n";
 

   maths_flag = cgi.queryCheckbox("maths");
   if( maths_flag ) {  
   //   cout << "Maths Flag: ON " << endl; 
       moduleStr = " module1=1, ";
    //   cout << " "<< moduleStr;
   } else {
    //  cout << "Maths Flag: OFF " << endl; 
      moduleStr = moduleStr + " module1= 0, "; 
   }
  // cout << "<br/>\n";

   physics_flag = cgi.queryCheckbox("physics");
   if( physics_flag ) {  
   //   cout << "Physics Flag: ON " << endl; 
      moduleStr = moduleStr + " module2=1,"; 
   } else {
   //   cout << "Physics Flag: OFF " << endl;
       moduleStr = moduleStr + " module2=0, ";  
   }
   //cout << "<br/>\n";
   chem_flag = cgi.queryCheckbox("chem");
   if( chem_flag ) {  
   //   cout << "chem Flag: ON " << endl;
      moduleStr = moduleStr + " module3=1,";
    //  cout << " "<< moduleStr; 
       
   } else {
   //   cout << "chem Flag: OFF " << endl; 
       moduleStr = moduleStr + " module3=0, "; 
   }
  // cout << "<br/>\n";
 english_flag = cgi.queryCheckbox("english");
   if( english_flag ) {  
   //   cout << "English Flag: ON " << endl; 
      moduleStr = moduleStr + " module4=1,"; 
   } else {
   //   cout << "English Flag: OFF " << endl;
      moduleStr = moduleStr + " module4=0, ";  
   }
 //  cout << "<br/>\n";
   
     history_flag = cgi.queryCheckbox("history");
   if( history_flag ) {  
   //   cout << "history Flag: ON " << endl;
      moduleStr = moduleStr + " module5=1"; 
   } else {
  //    cout << "history Flag: OFF " << endl; 
       moduleStr = moduleStr + " module5=0 "; 
   }
   
  // cout << "<br/>\n";
 
  // cout << moduleStr;
   form_iterator lecturer1 = cgi.getElement("lecturer");
    if(lecturer1 != cgi.getElements().end()) {
   // 	cout << "lecturer: " << **lecturer1 << "<br>";
    
    }
     string lecturer((lecturer1)->getValue());

         MYSQL *connect;                               // setup mysql
        connect = mysql_init(NULL);                // initiate sql connection
        if (!connect){                              // test if connected
            cout << "Mysql Initialization Failed"; 
            return 1;
        }		
        connect = mysql_real_connect(connect,
                  "127.0.0.1","harrison","global20","password",
                   0,NULL,0);                    // connect to sql server database password
        if (connect){                         // test if database password                
      	// for debug cout << "[+] Connection Success!\n" << "<br>";      			
      	}else{
              cout << "Connection Failed\n";    // if problem exit
              return 1;
             }
        MYSQL_RES *res_set;              // get  result set which contains data
        MYSQL_ROW row;                   // row of data
        MYSQL_FIELD column;
        mysql_query (connect,"SELECT * FROM password.emailPin");
        res_set=mysql_store_result(connect);       // get data
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
         //       mysql_query (connect,cmd); // sql query all users          
               cout << "<table style=\"margin-top:-0px; margin-left:0px;\">";	
               cout<<"<table border=\"1\" cellpadding=\"10\" >";   // setup html table
      	       cout<<"<tr>";                                      // start of table headers
               cout<<"<th>ID</th>";                               // first header entry
  	       cout<<"<th>User</th>";
                //cout << "<th>Password</th>";
               cout << "<th>"<< modules[0]<<"</th>";
               cout << "<th>"<< modules[1]<<"</th>";
               cout << "<th>"<< modules[2]<<"</th>";
               cout << "<th>"<< modules[3]<<"</th>";
               cout << "<th>"<< modules[4]<<"</th>";;  
               cout << "<th>Email</th>";                           // final header entry
               
               cout << "</tr>";                                    // end of headers
               cout << "<tr>";                                     // start of table data
               unsigned int i;
               const char * namep =lecturer.c_str();
               const char * moduleStrp =moduleStr.c_str();
               sprintf(cmd,"update users set %s where name='%s'",moduleStrp,namep);
            //   cout<< cmd;
                mysql_query (connect,cmd); // sql query all users
               mysql_query (connect,"SELECT * FROM password.users"); // sql query all users
               res_set=mysql_store_result(connect);       // get data
 
               while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      		      {      			  	
      		        string userID1=row[0];                 // get first field of row
      			    string user1=row[1];
      			    if (user1 == lecturer){
                          cout<< "<td>"<< userID1 <<"</td>";   // load table data from row field
                           cout<< "<td>"<< user1 <<"</td>";
      			    for (i=3; i<8; i++){
      			         string module1=row[i];
      			         if (module1 =="1"){
      			         cout<< "<td>"<< modules[i-3] <<"</td>";
      			         }else{
      			         cout<< "<td>"<< "" <<"</td>";
      			         }
      			    }
                           string email1=row[8];
                           cout<< "<td>"<< email1 <<"</td>";  //load table data from final row 
                           cout <<"</td>"<<"</tr>";          // end of table
                        }
  		}
         cout<< "<form method=\"post\" action=\"selectLecturer.cgi\">"<< endl;
	  cout << "<input type=submit value=\"Back To Select Lecturer\" style=\"position:absolute;margin-left:-335px; margin-top:120px\">";
          cout << endl;  
          cout << "</form>" << endl;
         cout<< "<form method=\"post\" action=\"lecturer.cgi\">"<< endl;
	  cout << "<input type=submit value=\"Back To Lecturer Options\" style=\"position:absolute;margin-left:-335px; margin-top:150px\">";
          cout << endl;  
          cout << "</form>" << endl;
          
          cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	  cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-335px; margin-top:180px\">";
          cout << endl;  
          cout << "</form>" << endl;
          cout << "<br/>\n";
          cout << "</body>\n";
          cout << "</html>\n";
         }
  	}
	catch(exception& e) {
      	     cout << HTTPContentHeader("text/html; charset=utf-8");
      	     logout();
   	}
  }  
  

