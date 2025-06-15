/**********************************************************************************
This stems from the webpage addStudent.cgi. As usual it tests for a match of cookies/emailpin table and no time out. If there is a match the time out is updated
The previous page passes on the relevant module name, which is used to save the entered student name and mark. The validations are that the student name must be unique and must be more than 4 characters long. As well as the mark can either be blank or an integer between 0-100. If these are fulfilled the student record is added to the module table on the database.

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
    if (s==""){
     return 0;
    }
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
string emailPinCookie;
string userIDCookie;
string phaseCookie;
string emailPinSaved;
string userIDSaved;
string phaseSaved;
string lastUserID;
time_t timeSaved;
time_t currentTime;
//string moduleStr;
char cmd[100];
int res;
bool notDup;
		
		
    Cgicc cgi;
    const_cookie_iterator cci;
    cout << HTTPContentHeader("text/html; charset=utf-8");
         	// Set up the HTML document
      		cout << html() << head(title("Save Student to Module")) << endl;
      		cout << "<head>\n";
       	cout << "<title> Save Student to Module </title>\n";
                cout << "</head>\n";
                cout << "<body>\n";
                cout << cgicc::div().set("align","center") << endl;
                
 
    bool valid = true;
    form_iterator module1 = cgi.getElement("module1");
    if(module1 != cgi.getElements().end()) {
    //	cout << "module: " << **module1 << "<br>";
    }
     string moduleStr((module1)->getValue());
     const char * moduleStrp =moduleStr.c_str();
     sprintf(cmd,"<h1 align=\"center\">Add Student to Module %s</h1>\n",moduleStrp);
     cout<< cmd; 
                  
    
    form_iterator nameL = cgi.getElement("name");
    if(nameL != cgi.getElements().end()) {
    //	cout << "Student: " << **nameL << "<br>";
    }
    
    form_iterator mark = cgi.getElement("mark");
    if(mark != cgi.getElements().end()) {
   // cout << "mark: " << **mark << "<br>";
    }
    string name((nameL)->getValue());
    const char * nameChar =name.c_str();
    if (strlen(nameChar) < 4){
   //  cout<< " name error ";
       valid = false;
    } 
    string markStr =((mark))->getValue();
    if (markStr != ""){
       char markChar[markStr.length() + 1];
       strcpy(markChar, markStr.c_str());      		      
       if (!isInRange(markChar)){
        // cout<< " mark error ";
         valid = false;
       }
    }
 	    
    if (valid){
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
                const char * module1p =moduleStr.c_str();
                sprintf(cmd,"update emailPin set time=%ld where id=1",currentTime);
                mysql_query (connect,cmd); // sql query module
                sprintf(cmd,"SELECT * FROM password.%s",module1p);
               mysql_query (connect,cmd);
                res_set=mysql_store_result(connect);       // get data
                 notDup = true;
                while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      			{      			  	
      	                if (name == row[0]){
      	                notDup = false;      	                
      	                 }     	       
   		        }
    //        }
        if (notDup){
               const char * namep =name.c_str();
               const char * markStrp =markStr.c_str();
               const char * module1p =moduleStr.c_str();
               sprintf(cmd,"insert into  %s value (\"%s\",\"%s\")",module1p,namep,markStrp);
            //    cout << cmd;
               res=mysql_query(connect,cmd);      			
      	       if(res){
      		       cout<<"[-] Query Input Error!"<< "<br>";
      		        } else{
      		          //     cout<<"[+] Query OK!"<< "<br>";
      		      } 
     		           		        
             sprintf(cmd,"SELECT * FROM password.%s",module1p);  
               mysql_query (connect,cmd); // sql query all users
               res_set=mysql_store_result(connect);       // get data
               cout << "<table style=\"margin-top:-0px; margin-left:0px;\">";	
               cout<<"<table border=\"1\" cellpadding=\"10\" >";   // setup html table
      	       cout<<"<tr>";                                      // start of table headers
               cout<<"<th>Name</th>";                               // first header entry
  	       cout<<"<th>Mark</th>";
              cout << "</tr>";                                    // end of headers
               cout << "<tr>";                                     // start of table data
               while (((row=mysql_fetch_row(res_set)) !=NULL))     // get each row data
      		      {      			  	
      		        string nameStr=row[0];                 // get first field of row
      			    string markStr=row[1];
                           cout<< "<td>"<< nameStr <<"</td>";   // load table data from row field
                           cout<< "<td>"<< markStr <<"</td>";
                           cout <<"</td>"<<"</tr>";          // end of table
  		}
	     cout<< "<form method=\"post\" action=\"lecturer.cgi\">"<< endl;
	     cout << "<input type=submit value=\"Back To Lecturer Modules\" style=\"position:absolute;margin-left:-330px; margin-top:210px\">";
	     cout << endl;  
             cout << "</form>" << endl;
	     cout<< "<form method=\"post\" action=\"logOut.cgi\">"<< endl;
	     cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-330px; margin-top:250px\">";
	     cout << endl;  
             cout << "</form>" << endl;
             mysql_close (connect); // close sql connection
             
               }else{ // notDup
                  cout<< "<a href=\"./lecturer.cgi\" rel=\"external\" title=\"Duplicate database entry\">"
		      <<" Duplicate database entry </a>";
                   } 
        
            if (!valid){
                cout<< "<a href=\"./lecturer.cgi\" rel=\"external\" title=\"Invalid entry\">"
		    <<" Invalid entry </a>";
        	}	    
	    }
          } 
  }
	
	catch(exception& e) {
      	   cout << HTTPContentHeader("text/html; charset=utf-8");
           logout();
   	}
}
                             		

