/*****************************************************************************
This webpage is next inline from tokenLogin.cgi. We run base64 on the received authorisation code and then decrypt it. It is then checked against the stored emailPin and if they match we set cookie:phase = 2 to indicate a completed login. The emailpin table is also update with phase 2 and time stamped with the current time (gives another 30 seconds time out period). A menu option for the administrator is displayed.

Administer 
Add Lecturer
Select Module
Select Lecturer
Lecturer Module       

*******************************************************************************
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
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "base64.h"
#include <iostream>

using namespace std;
using namespace cgicc;

bool error=false;
int gcm_encrypt(unsigned char *plaintext, int plaintext_len,
                unsigned char *aad, int aad_len,
                unsigned char *key,
                unsigned char *iv, int iv_len,
                unsigned char *ciphertext,
                unsigned char *tag);
int gcm_decrypt(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *aad, int aad_len,
                unsigned char *tag,
                unsigned char *key,
                unsigned char *iv, int iv_len,
                unsigned char *plaintext);
 
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
    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";
    size_t iv_len = 16;
    
    /* Buffer for the decrypted text */
    unsigned char decryptedtext[128];

    /* Buffer for the tag */
    unsigned char tag[16];
    
   /* Additional data */
    unsigned char *additional =
        (unsigned char *)"The five boxing wizards jump quickly.";

    int decryptedtext_len, ciphertext_len;
    
	
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
string emailPinStr;
string decrypt;
string ciphertext1;
int emailPinNum;
int tokenNum;
char cmd[100];
void handleErrors(void);

		
		Cgicc cgi;
                const_cookie_iterator cci;
      			                    
       	form_iterator code = cgi.getElement("code");
      		if(code != cgi.getElements().end()) {
                  // cout << "Authorisation: " << **code << "<br>";         		
      		}
               string token((code)->getValue());
               if(token !=""){
               try {
                ciphertext1 = base64_decode(token);
                }
	        catch(exception& e) {
	         if (!error){
      	            logout();
   	          }
               }  	      
               try{
               ciphertext_len = ciphertext1.length();
               unsigned char *ciphertext=new unsigned char[ciphertext1.length()+1];
               strcpy((char *)ciphertext,ciphertext1.c_str());
               // Decrypt the ciphertext 
               decryptedtext_len = gcm_decrypt(ciphertext,ciphertext_len,
                                    additional, strlen ((char *)additional),
                                    tag,
                                    key, iv, iv_len,
                                    decryptedtext);
                }
	       catch(exception& e) {
	         if (!error){
      	            logout();
   	          }
   	      }
               // Add a NULL terminator. We are expecting printable text 
               decryptedtext[decryptedtext_len] = '\0'; 
               // Show the decrypted text 
               //cout<<"Decrypted text is: ";
               //cout<< decryptedtext;
               string decrypt(reinterpret_cast<char*>(decryptedtext));    
      	       // cout<< decryptedtext << "<br>";
      	      
            
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
      			//	 cout<< emailPinSaved << "<br>";
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
                 //     cout<<"emailCookie :"<< emailPinCookie << "<br>";
                      }
                        if (cci->getName() == "phase") {
                          phaseCookie = cci->getValue();                                                      
                        }
                         if (cci->getName() == "userID") {
                          userIDCookie = cci->getValue();                                                      
                        }
 //                    } if (phaseSaved == "2"){      // used for reentry
 //                          emailPinStr = emailPinSaved;
                     }
                     currentTime =time(NULL);
                     int timeOut= currentTime - timeSaved;
                 //    cout<< "timeOut "<< timeOut;
                     try{                
                         emailPinNum = stoi(emailPinCookie);
                         tokenNum = stoi(decrypt);
                     }
	             catch(exception& e) {
	               if (!error){
      	                   logout();
   	                 }
   	             }
       //            cout << tokenNum << "<br>";
           //        cout << emailPinNum << "<br>";
              //       cout<< "<br> decrypt is :"<< decrypt << "<br> emailPinsaved :"<< emailPinSaved ;
               //      if (emailPinNum== tokenNum){
              //           cout<< "<br>" << " token match"<< "<br>";
              //       }else {
              //       cout<< "<br>" << "no token match"<< "<br>";
              //       }
                     
                     
            if ((emailPinCookie != emailPinSaved )||userIDCookie !=userIDSaved || emailPinNum != tokenNum ){
                   logout(); // no time out as undeterminated time to get Authorisation Code
              }else{
                    std::cout << "Set-Cookie:phase=2;\n"; 
                 sprintf(cmd,"update emailPin set  time=%ld,phase='2' where id=1",currentTime);
                mysql_query (connect,cmd); // sql query all users
	         mysql_close (connect); // close sql connection  
                 //    cout<< "phase : ";                      		
                //    cout << phase << " "<<  phaseSaved;
                      if (userIDSaved == "1"){
      		// Set up the HTML document
      		cout << HTTPContentHeader("text/html; charset=utf-8");
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
                cout << "<input type=submit value=\"Lecturer Modules\" style=\"margin-left:000px; margin-top:0px\">";
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
	        cout << "<input type=submit value=\"Log Out\" style=\"margin-left:-00px; margin-top:30px\">";
	  //      cout << "<input type=submit value=\"Log Out\" style=\"position:absolute;margin-left:-300px; margin-top:250px\">";
	        cout << endl;  
                cout << "</form>" << endl;
                    }             
                  } 
   //           }
           }else{
                 cout << HTTPContentHeader("text/html; charset=utf-8");
                 logout();
//								<<" Login Failure Retry </a>";
           }
	}
	catch(exception& e) {
      	logout();
   	}
}
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}


int gcm_encrypt(unsigned char *plaintext, int plaintext_len,
                unsigned char *aad, int aad_len,
                unsigned char *key,
                unsigned char *iv, int iv_len,
                unsigned char *ciphertext,
                unsigned char *tag)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;


    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /* Initialise the encryption operation. */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        handleErrors();

    /*
     * Set IV length if default 12 bytes (96 bits) is not appropriate
     */
    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
        handleErrors();

    /* Initialise key and IV */
    if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv))
        handleErrors();

    /*
     * Provide any AAD data. This can be called zero or more times as
     * required
     */
    if(1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Normally ciphertext bytes may be written at
     * this stage, but this does not occur in GCM mode
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Get the tag */
    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
        handleErrors();

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


int gcm_decrypt(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *aad, int aad_len,
                unsigned char *tag,
                unsigned char *key,
                unsigned char *iv, int iv_len,
                unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    int ret;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /* Initialise the decryption operation. */
    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        handleErrors();

    /* Set IV length. Not necessary if this is 12 bytes (96 bits) */
    if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
        handleErrors();

    /* Initialise key and IV */
    if(!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv))
        handleErrors();

    /*
     * Provide any AAD data. This can be called zero or more times as
     * required
     */
    if(!EVP_DecryptUpdate(ctx, NULL, &len, aad, aad_len))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /* Set expected tag value. Works in OpenSSL 1.0.1d and later */
    if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
        handleErrors();

    /*
     * Finalise the decryption. A positive return value indicates success,
     * anything else is a failure - the plaintext is not trustworthy.
     */
    ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    if(ret > 0) {
        /* Success */
        plaintext_len += len;
        return plaintext_len;
    } else {
        /* Verify failed */
        return -1;
    }
}
