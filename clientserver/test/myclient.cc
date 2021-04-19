/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "messageHandler.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>

using namespace std;

void write(const Connection& conn, int com_num){

	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	//write commandbyte
	conn.write(com_num & 0xFF);
	//write parameter
	if(com_num ==static_cast<int>(Protocol::COM_LIST_NG)){
	     //list newsgroups
	     //no parameters
	}
	else if(com_num ==static_cast<int>(Protocol::COM_CREATE_NG)){
	    //create new newsgroup
	    //parameter: string title
	    cout<<"Type title"<<endl;
	    string title;
	    getline(cin, title);
	    writeString(conn, title);
	}
	else if(com_num ==static_cast<int>(Protocol::COM_DELETE_NG)){
	    //delete newsgroup
	    //parameter: int ID
	    cout<<"Type newsgroup ID"<<endl;
	    int id;
	    cin >> id;
	    writeNumber(conn, id);
	}
	else if(com_num ==static_cast<int>(Protocol::COM_LIST_ART)){
	    //list articles
	    //parameter: int ID
	    cout<<"Type newsgroup ID"<<endl;
	    int id;
	    cin >> id;
	    writeNumber(conn, id);
	}
	else if(com_num ==static_cast<int>(Protocol::COM_CREATE_ART)){
	    //create article
	    //parameters: int ID, string title, string author, string text
	    cout<<"Type newsgroup ID"<<endl;
	    int id;
	    cin >> id;
	    writeNumber(conn, id);
	    cout<<"Type title"<<endl;
	    string line;
	    cin.ignore();
	    getline(cin, line);
	    writeString(conn, line);
	    cout<<"Type author"<<endl;
	    getline(cin, line);
	    writeString(conn, line);
	    cout<<"Type text"<<endl;
	    getline(cin, line);
	    writeString(conn,line);
	}
	else if(com_num ==static_cast<int>(Protocol::COM_DELETE_ART)){
	    //delete article
	    //parameters: int ID, int ID
	    cout<<"Type newsgroup ID"<<endl;
	    int id;
	    cin >> id;
	    writeNumber(conn, id);
	    cout<<"Type article ID"<<endl;
	    cin >> id;
	    writeNumber(conn, id);
	}
	else if(com_num ==static_cast<int>(Protocol::COM_GET_ART)){
	    //get article
	    //parameters: int ID, int ID
	    cout<<"Type newsgroup ID"<<endl;
	    int id;
	    cin >> id;
	    writeNumber(conn, id);
	    cout<<"Type article ID"<<endl;
	    cin >> id;
	    writeNumber(conn, id);
	}
	//write endbyte
	conn.write(static_cast<int>(Protocol::COM_END) & 0xFF);

}
/*
// write string_p
void writeString(const Connection& conn, string s){
	    conn.write(static_cast<int>(Protocol::PAR_STRING));
	    int len = s.size();
            conn.write((len >> 24) & 0xFF);
            conn.write((len >> 16) & 0xFF);
	    conn.write((len >> 8) & 0xFF);
	    conn.write(len & 0xFF);
	    for(char c : s)conn.write(c);
}

// write num_p
void writeNumber(conn, const Connection& conn, int value){
	conn.write(static_cast<int>(Protocol::PAR_NUM));
        conn.write((value >> 24) & 0xFF);
        conn.write((value >> 16) & 0xFF);
        conn.write((value >> 8) & 0xFF);
        conn.write(value & 0xFF);
}
*/
void read(const Connection& conn){
	int com_num = conn.read();
	if(com_num ==static_cast<int>(Protocol::ANS_LIST_NG)){
		int num_ng = readNumber(conn);
		for(int i = 0; i != num_ng; i++){
			int id = readNumber(conn);
			string title = readString(conn);
			cout<<"ID: "<<id<<", TITLE: "<<(title)<<endl;
		}	
	}
	else if(com_num ==static_cast<int>(Protocol::ANS_CREATE_NG)){

		int ans = conn.read();
		if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
			cout<<"News group added"<<endl;
		}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
			printError(conn);
		}
	}
	else if(com_num ==static_cast<int>(Protocol::ANS_DELETE_NG)){
		int ans = conn.read();
		if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
			cout<<"News group deleted"<<endl;
		}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
			printError(conn);
		}
	}
	else if(com_num ==static_cast<int>(Protocol::ANS_LIST_ART)){
		int ans = conn.read();
		if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
			int num_art = readNumber(conn);
		cout<<"UUUUUUUUU"<<num_art<<endl;
			for(int i = 0; i != num_art; i++){
				cout<<"YYYY"<<endl;
				int id = readNumber(conn);
			cout<<"OOOO"<<id<<endl;
				string title = readString(conn);
				cout<<"RRRRR"<<endl;
				cout<<"ID: "<<id<<", TITLE: "<<title<<endl;
			}	
		}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
			printError(conn);
		}
	}
	else if(com_num ==static_cast<int>(Protocol::ANS_CREATE_ART)){
		int ans = conn.read();
		if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
			cout<<"Article added"<<endl;
		}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
			printError(conn);
		}
	}
	else if(com_num ==static_cast<int>(Protocol::ANS_DELETE_ART)){
		int ans = conn.read();
		if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
			cout<<"Article deleted"<<endl;
		}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
			printError(conn);
		}
	}
	else if(com_num ==static_cast<int>(Protocol::ANS_GET_ART)){
		int ans = conn.read();
		if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
			string title = readString(conn);
			string author = readString(conn);
			string text = readString(conn);
			cout<<"TITLE: "<<title<<endl<<"AUTHOR: "<<author<<endl<<text<<endl;
		}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
			printError(conn);
		}
	}
	//remove ANS_END
	conn.read();
}
/*
void printError(const Connection& conn){
	cout<<"ERROR: ";
        unsigned char err = conn.read();
	switch(err)){
		else if(com_num ==static_cast<int>(Protocol::ERR_NG_ALREADY_EXISTS)){
			cout<<"News group already exists"<<endl;
		}
		else if(com_num ==static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST)){
			cout<<"News group does not exist"<<endl;
		}
		else if(com_num ==static_cast<int>(Protocol::ERR_ART_DOES_NOT_EXIST)){
			cout<<"Article does not exist"<<endl;
}

//read num_p
int readNumber(const Connection& conn){
	//remove PAR_NUM
	conn.read();

	//read number
        unsigned char byte1 = conn.read();
        unsigned char byte2 = conn.read();
        unsigned char byte3 = conn.read();
        unsigned char byte4 = conn.read();
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

//read string_p
string readString(const Connection& conn){
	//remove PAR_STRING
	conn.read();

        string s;
        char   c;
        while ((c = conn.read()) != '$') {
                s += c;
        }
        return s;
}
*/
/* Creates a client for the given args, if possible.
 * Otherwise exits with error code.
 */
Connection init(int argc, char* argv[])
{
        if (argc != 3) {
                cerr << "Usage: myclient host-name port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = stoi(argv[2]);
        } catch (exception& e) {
                cerr << "Wrong port number. " << e.what() << endl;
                exit(2);
        }

        Connection conn(argv[1], port);
        if (!conn.isConnected()) {
                cerr << "Connection attempt failed" << endl;
                exit(3);
        }

        return conn;
}

void printHelp(){
	cout<<"1 - list newsgroups"<<endl;
	cout<<"2 - create newsgroup"<<endl;
	cout<<"3 - delete newsgroup"<<endl;
	cout<<"4 - list articles"<<endl;
	cout<<"5 - create article"<<endl;
	cout<<"6 - delete article"<<endl;
	cout<<"7 - get article"<<endl;
}

int app(const Connection& conn){
	
        cout << "Type the number for the desired action, 0 for help: ";
        int nbr;
        while (cin >> nbr) {
                try {
			if(nbr == 0){
				printHelp();

			}else{
				write(conn, nbr);
				read(conn);
			}
                        cout << "Type another number: ";
                } catch (ConnectionClosedException&) {
                        cout << " no reply from server. Exiting." << endl;
                        return 1;
                }
        }
        cout << "\nexiting.\n";
        return 0;
}

int main(int argc, char* argv[])
{
        Connection conn = init(argc, argv);
        return app(conn);
}
