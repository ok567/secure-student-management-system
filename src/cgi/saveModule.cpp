/***********************************************************************************
This webpage is displayed when the save option is pressed in module.cgi. As usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated. Validation for the marks is that it must be between 0-100. If the field is left blank, no change made.  If this is fulfilled then the module table on the database is updated. 
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
char cmd[100];
int res;
string emailPinCookie;
string phaseCookie;
string userIDCookie;
string emailPinSaved;
string userIDSaved;
string phaseSaved;
string name;
string mark;
string moduleStr;
time_t timeSaved;
time_t currentTime;
		
		
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
                   // cout<< currentTime <<" time "<<timeSaved << " "<< timeOut;
             if ((emailPinCookie != emailPinSaved )|| (emailPinSaved != emailPinCookie)||(timeOut > 60)){
                   logout();
               }else{
                sprintf(cmd,"update emailPin set time=%ld where id=1",currentTime);
                mysql_query (connect,cmd); // sql query all users
	         mysql_close (connect); // close sql connection  
                 
      		// Send HTTP header
      	//	cout << HTTPContentHeader("text/html; charset=utf-8");
      		//cout << HTTPHTMLHeader() << endl;

      		// Set up the HTML document
      		cout << html() << head(title("Save Module")) << endl;
      		cout << "<head>\n";
       	cout << "<title> Save Module </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Save Module</h1>\n";
                
                
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
/*                mysql_query (connect,"SELECT * FROM password.maths"); // sql query all users
                unsigned int i=1;
                res_set=mysql_store_result(connect);       // get data
                unsigned int numrows = mysql_num_rows(res_set); // get number of rows
*/      		

      		
         		
      		
                const_form_iterator iter;
                 for(iter = cgi.getElements().begin(); 
                  iter != cgi.getElements().end(); 
                   ++iter){
                     name = iter->getName() ;
                     // cout << name;
                      if (name == "module1"){
                      moduleStr = iter->getValue();
                      const char * moduleStrp =moduleStr.c_str();
                     sprintf(cmd,"SELECT * FROM password.%s",moduleStrp);
                     mysql_query (connect,cmd);
                     res_set=mysql_store_result(connect);       // get data
                     } 
      		  }
                 bool valid = true;
                 for(iter = cgi.getElements().begin(); 
                  iter != cgi.getElements().end(); 
                   ++iter){
      		      name = iter->getName() ;
      		      mark = iter->getValue();
      		      if (mark !="" ){
      		        if (name != "module1"){
                    char markChar[mark.length() + 1];
                    strcpy(markChar, mark.c_str());      		      
      		      if (!isInRange(markChar)){
                     valid = false;
                     cout << name << " ";
                      }
                     }
                    } 
      		  }
                if (valid){
      		cout << "</body>" << html();
      		cout << "<table style=\"position:absolute;margin-top:-00px; margin-left:0px;\">";
                cout<<"<table border=\"1\" cellpadding=\"10\" >";   // setup html table
     		cout<<"<tr>";                                      // start of table headers
                cout<<"<th>Student</th>";                               // first header entry
  		cout<<"<th>Mark</th>";
             //   cout << "<th>Alt</th>";

                cout << "</tr>";                                    // end of headers
                cout << "<tr>";                                     // start of table data
                       		  
                 for(iter = cgi.getElements().begin(); 
                  iter != cgi.getElements().end(); 
                   ++iter){
                   name = iter->getName() ;
              //       cout << name;
                    if (name != "module1"){
                    const char * namep =name.c_str();
                    cout << "<tr>""<td>" ;
                    cout << name  << "</td>";
                    mark = iter->getValue();
                     if (mark !="" ){
                    const char * markp =mark.c_str();
                    const char * moduleStrp =moduleStr.c_str();
                    sprintf(cmd,"update %s set mark='%s' where name='%s'",moduleStrp,markp,namep);
                //  cout << cmd << "<br>";
                   mysql_query (connect,cmd); // sql query all users
                   res=mysql_query(connect,cmd);
      			
      		        if(res){
      		           	cout<<"[-] Query Input Error!"<< "<br>";
      		        }
                    cout << "<td>" << mark << "</td>"; 
                    cout <<  "</tr>" << endl;
                    }
                    }
                  }
               }   
                    mysql_close (connect); // close sql connection
                   if (!valid){
                   cout << "</body>" << html();
                   cout<< " have  invalid entry in the ";
                   }else{
		    cout<<"The following changes have been made to the  ";
		    }
		    cout<< moduleStr << " Module";
		    cout<< "."<<"<br>";
		    if (!valid){
		    cout<<"No changes have been made to the module ";
		    }else{
		    cout<< "Blank means no change to students mark.";
		    }
		    cout<< "<form method=\"post\" action=\"lecturer.cgi\">"<< endl;
	       cout << "<input type=submit value=\"Back To Lecturer Modules\" style=\"position:absolute;margin-left:-330px; margin-top:210px\">";
	            cout << endl;  
                    cout << "</form>" << endl;
		    cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	            cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-330px; margin-top:250px\">";
	            cout << endl;  
                    cout << "</form>" << endl;
                }    
           
           }
	catch(exception& e) {
      logout();
   	}
}
                             		

