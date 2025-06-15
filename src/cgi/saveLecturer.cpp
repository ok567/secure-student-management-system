/*************************************************************************************************
This web page is next inline from addLecture.cgi. It as usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated. Collecting user name, password and email address they are validated as follows:
User name must be greater than 4 characters long and must be unique. 
Password must be greater than 5 and must be unique.
Email address must contain “@” and be unique.
If these fulfilments are met then the lecturer record is added to the users table database. With no modules, as only the administrator can assign modules to a lecturer. 

**************************************************************************************************
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
string lastUserID;
time_t timeSaved;
time_t currentTime;
string moduleStr;
char cmd[100];
int res;
		
		
    Cgicc cgi;
    const_cookie_iterator cci;
    cout << HTTPContentHeader("text/html; charset=utf-8");
    bool valid = true;
    form_iterator nameL = cgi.getElement("name");
    if(nameL != cgi.getElements().end()) {
    //	cout << "Your Name: " << **name << "<br>";
    }
    form_iterator passwd = cgi.getElement("password");
    if(passwd != cgi.getElements().end()) {
    //cout << "Your password: " << **passwd << "<br>";
    }
    form_iterator emailAddress1 = cgi.getElement("emailAddress");
    if(emailAddress1!= cgi.getElements().end()) {
    //cout << "Your password: " << **passwd << "<br>";
    }
    string name((nameL)->getValue());
    if (name == ""){
    valid = false;
    }
    const char * nameChar =name.c_str();
    if (strlen(nameChar) < 4){
    // cout<< " name error ";
       valid = false;
    }      		
    string password((passwd)->getValue());
    if (password == ""){
    valid = false;
    }    
    const char * passwordChar =password.c_str();
    if (strlen(passwordChar) < 5){
    // cout << "password error ";
    valid = false;
    }
    string emailAddress((emailAddress1)->getValue());
    if (emailAddress == ""){
    valid = false;
    }    
    char emailAddressChar[emailAddress.length() + 1];
    strcpy(emailAddressChar, emailAddress.c_str());      		      
    char * ptr = strchr(emailAddressChar, '@');		
    if (ptr > 0){
    // valid = false;
    //   cout<< "@ found";	    
    }else{
      valid = false;
    //cout<< "@ not found";
    }	    
  //  if (valid){
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
                sprintf(cmd,"update emailPin set time=%ld where id=1",currentTime);
                mysql_query (connect,cmd); // sql query all users
       	// Set up the HTML document
      		cout << html() << head(title("Save Lecturer")) << endl;
      		cout << "<head>\n";
       	cout << "<title> Save Module </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                cout << "<h1 align=\"center\">Save Lecturer</h1>\n";                
                mysql_query (connect,"SELECT * FROM password.users");
                res_set=mysql_store_result(connect);       // get data
                bool notDup = true;
                while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      			{      			  	
      	        lastUserID=row[0];
      	        if (name == row[1]){
      	            notDup = false;
      	        }
      	        if (password == decrypt (row[2])){
      		    notDup = false;
      	        }
     	        if (emailAddress == row[8]){
      		    notDup = false;
      		}
   		}
        if (notDup && valid){
      		int num = stoi(lastUserID);
      		//cout<< " lastuserID "<< num<< "<br>";
      		num++;
      		char nextUserID[2];
      		// itoa(num,nextUserID,10);
      		sprintf(nextUserID,"%d",num);
      	        // cout<< nextUserID << " next userid <br>";
               string encryptedPassword = encrypt (password);
                // cout << encryptedPassword << "password <br>";
               const char * nextUserIDp =nextUserID;
               const char * namep =name.c_str();
               const char * passwordp =encryptedPassword.c_str();
               const char * emailAddressp =emailAddress.c_str();
// mysql_query (connect,"insert into users value (1,'admin','`clhm',0,0,0,0,0,'admin@localhost')");
               
               sprintf(cmd,"insert into users value (\"%s\",\"%s\",\"%s\",0,0,0,0,0,\"%s\")",nextUserIDp,namep,passwordp,emailAddressp);
               // cout << cmd;
               res=mysql_query(connect,cmd);      			
      	       if(res){
      		       cout<<"[-] Query Input Error!"<< "<br>";
      		        } else{
      		          //     cout<<"[+] Query OK!"<< "<br>";
      		      }      		        
               mysql_query (connect,"SELECT * FROM password.users"); // sql query all users
               res_set=mysql_store_result(connect);       // get data
               cout << "<table style=\"margin-top:-0px; margin-left:0px;\">";	
               cout<<"<table border=\"1\" cellpadding=\"10\" >";   // setup html table
      	       cout<<"<tr>";                                      // start of table headers
               cout<<"<th>ID</th>";                               // first header entry
  	       cout<<"<th>User</th>";
                //cout << "<th>Password</th>";
               cout << "<th>Modual</th>";
               cout << "<th>Modual</th>";
               cout << "<th>Modual</th>";
               cout << "<th>Modual</th>";
               cout << "<th>Modual</th>";  
               cout << "<th>Email</th>";                           // final header entry
               cout << "</tr>";                                    // end of headers
               cout << "<tr>";                                     // start of table data
               while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      		      {      			  	
      		        string userID1=row[0];                 // get first field of row
      			if (userID1 == nextUserID){
      			    string user1=row[1];
                        // string passwd1=row[2];
       		   string email1=row[8];                  // get final field of row
                           cout<< "<td>"<< userID1 <<"</td>";   // load table data from row field
                           cout<< "<td>"<< user1 <<"</td>";
                            // cout<< "<td>"<< passwd1 <<"</td>";
                           cout<< "<td>"<< " " <<"</td>";
                           cout<< "<td>"<< " " <<"</td>";
                           cout<< "<td>"<< " " <<"</td>";
                           cout<< "<td>"<< " " <<"</td>";
                           cout<< "<td>"<< " " <<"</td>";
                           cout<< "<td>"<< email1 <<"</td>";  //load table data from final row 
                           cout <<"</td>"<<"</tr>";          // end of table
                        }
  		}
	     cout<< "<form method=\"post\" action=\"lecturer.cgi\">"<< endl;
	     cout << "<input type=submit value=\"Back To Lecturer Modules\" style=\"position:absolute;margin-left:-330px; margin-top:110px\">";
	     cout << endl;  
             cout << "</form>" << endl;
	     cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	     cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-330px; margin-top:150px\">";
	     cout << endl;  
             cout << "</form>" << endl;
             mysql_close (connect); // close sql connection
            }else{
                  if (!valid){
               //  cout << "Invalid entry";
                    cout<< "<a href=\"./lecturer.cgi\" rel=\"external\" title=\"Invalid entry\">"
			<<" Invalid entry </a>";
			}
		 if (!notDup){
		  //cout << "Invalid entry";
                  cout<< "<a href=\"./lecturer.cgi\" rel=\"external\" title=\"Duplicate database entry\">"
		      <<" Duplicate database entry </a>";
		 }	
            }
           } 
          
           
	}
	catch(exception& e) {
      	logout();
   	}
}
                             		

