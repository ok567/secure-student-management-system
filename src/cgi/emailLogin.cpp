/****************************************************************************************************************
This web gets the username and password entered in the index.html page. It then accesses the users table on the database and looks for a match of username/password and an email address, if so the cookies are set.
Cookie:Email  with a random number range 1000-99999 (only set here for debugging
                         Will be set in lecture.cgi/checkToken.cgi ).                      
 Cookie:Phase  to 1 (valid username/password).
Cookie:userID  1 for administer 2-9 lecturers (students 10-100, for later purposes)

To track and secure, these values are written to the emailpin table with a time stamp. Now with the login users email address the email pin is sent. Forms with action tokenLogin.cgi for administer or lecturer.cgi for lecturers is created to input the email pin and pass it on.
If there is no match then the “time out” is displayed with a prompt to re-login.
 
 Pre:
 username
 password
 Post:
 emailpin
*****************************************************************************************************************
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

bool error=false;

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

		Cgicc cgi;
		//cout << HTTPContentHeader("text/html; charset=utf-8");
      		// Print out the submitted element
      		form_iterator userName = cgi.getElement("userName");
      		if(userName != cgi.getElements().end()) {
      		//	cout << "Your UserName: " << **userName << "<br>";
      		}

      		form_iterator passwd = cgi.getElement("password");
      		if(passwd != cgi.getElements().end()) {

         		//cout << "Your password: " << **passwd << "<br>";
      		}
      		string name((userName)->getValue());
      		//cout<< name;
      		string password((passwd)->getValue());
      		//cout<< " entred password" <<password;
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
                bool match = false;
	
                     while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      			{	
      				user =row[0];                 // get ID
      			 //	cout << "ID: " << row[0] << "<br>";
      				string row1=row[1];                 // get username
      			//	cout << "UserName: " << row1 << "<br>";
      				 row2=row[2];                 // get password
      			 //	cout<< password ;
      			//	cout<< " passwd "<< row2;
      			//	cout << " decode "<< decrypt (row2)<< "<br>";
                         //      if ((row1 == name) && (password == decrypt (row2))){
                               if (row1 == name){
                             //   match = true;
                               break;
                               }
                          }
                          if (password == decrypt (row2)){
                          match = true;
                          }
                          if (match) {
                char cmd[100];         
                string pin=randomNumber();
                const char * pinp =pin.c_str();
//   move to lecture.cgi/checkToken.cgi               
                sprintf(tempStr,"Set-Cookie:emailPin=%s",pinp);
                
                cout << tempStr << endl;
		cout << "Set-Cookie:phase=1;\n";
		const char * userIDp =user.c_str();
                sprintf(tempStr,"Set-Cookie:userID=%s",userIDp);
                cout << tempStr << endl;
                tStamp =time(NULL);
                 sprintf(cmd,"update emailPin set pin='%s',userID='%s',phase='1',time=%ld where id=1",pinp,userIDp,tStamp);
                mysql_query (connect,cmd); // sql query all users
                
    		// Send HTTP header
             
      		cout << HTTPContentHeader("text/html; charset=utf-8");
      		// Set up the HTML document

      		cout << html() << head(title("Email Login")) << endl;
                cout << "<head>\n";
       	cout << "<title> Email Login </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Email Login</h1>\n";
                if (user == "1"){
                     cout<< "<form method=\"post\" action=\"tokenLogin.cgi\">"<< endl;
                 }else{
                   cout<< "<form method=\"post\" action=\"lecturer.cgi\">"<< endl;
                }
                cout << "<p style=\"black:white; margin-left:-120px; margin-top:80px\">Email Pin</p>" <<endl; 
	        cout << "<input type=text size=20 name=\"emailPin\" style=\"margin-left:-30px; margin-top:-60px\">";
	        cout << endl; 	 
	        cout << "<input type=submit value=\"Email Login\" style=\"margin-left:-300px; margin-top:-18px\">";
	        cout << endl;  
                cout << "</form>" << endl;
                cout << "</form>" << endl;
		cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	        cout << "<input type=submit value=\"Log Out\" style=\"margin-left:-150px; margin-top:00px\">";
//	        cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-300px; margin-top:250px\">";
	        cout << endl;  
                cout << "</form>" << endl;
                       	                              
                          string email=row[8];
                          //cout << "email: " << email << "<br>";
      				
                          const char * emailp =email.c_str();
                         
                           sprintf(cmd,"echo \"Is email sending OK..?\" | mail -s %s %s",pinp,emailp);
                          // cout<< cmd;
                           system(cmd);
  			     } else {
  			             cout << HTTPContentHeader("text/html; charset=utf-8");
  			             logout();
  			    }
 			        mysql_close (connect); // close sql connection
		// Close the HTML document

      		cout << "</body>" << html();
	}

	catch(exception& e) {

      	logout();

   	}

}
