/******************************************************************************
This web page is next inline from the emailpin page for the administrator. It collects the entered email pin and compares it with the emailpin database table. If the stored values match and that the current time - timestamp is not greater than 30 (30 second time out value ), an input box for the authorisation code is displayed. 
*******************************************************************************
*/
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <mysql/mysql.h>

#include "cgicc/Cgicc.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPContentHeader.h"

using namespace std;
using namespace cgicc;


// Pre:
// - s is not null
// - s is a string
// Post:
// - returns the decrypted password

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

string randomNumber() {
	
		
		// Retrieve a random number between 100 and 200
		// Offset = 10000
		// Range = 99999
		int random = 10000 + (rand() % 99999);

		// Print the random number
		//cout<<random<<endl;
		auto s = std::to_string(random);
                return s;
 }

int main()

{

srand((unsigned) time(NULL));
time_t tStamp;	

	try{  
char tempStr[200];
string user;
char tempString[512];
char cgiOut[512];
char webStr[100];
string buffer;
string names[10];	//assumes there is a max of 10 teachers
string pwds[10];	//assumes there is a max of 10 passwords
string row2;
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


		Cgicc cgi;
		std::cout << "Set-Cookie:phase=2;\n";
		cout << HTTPContentHeader("text/html; charset=utf-8");

                const_cookie_iterator cci;
       		form_iterator emailPin = cgi.getElement("emailPin");
      		if(emailPin != cgi.getElements().end()) {
         	//	cout << "emailPin: " << **emailPin << "<br>";
         		
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
               if ((emailPinCookie != emailPinSaved )|| (emailPinSaved != emailPinStr)||(timeOut > 60)){
                    logout();
               }else{ 
                           
            tStamp =time(NULL);

      		cout << html() << head(title("Authorisation Login")) << endl;
                cout << "<head>\n";
       	cout << "<title> Authorisation Login </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Token Login</h1>\n";
                cout<< "<form method=\"post\" action=\"checkToken.cgi\">"<< endl;
                cout << "<p style=\"black:white; margin-left:-120px; margin-top:-0px\">Authorisation Code</p>" <<endl; 
	        cout << "<input type=text size=10 name=\"code\" style=\"margin-left:60px; margin-top:-60px\">";
	        cout << endl; 	 
	        cout << "<input type=submit value=\"Authorisation Login\" style=\"margin-left:-260px; margin-top:-18px\">";
	        cout << endl;  
                cout << "</form>" << endl;
                cout << "</form>" << endl;
		cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	        cout << "<input type=submit value=\"Log Out\" style=\"margin-left:-150px; margin-top:00px\">";
//	        cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-300px; margin-top:250px\">";
	        cout << endl;  
                cout << "</form>" << endl;
                       	                              
 	       mysql_close (connect); // close sql connection
		// Close the HTML document

      		cout << "</body>" << html();
      		}
	}

	catch(exception& e) {

      	logout();

   	}

}
