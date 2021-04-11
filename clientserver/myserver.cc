#include "connection.h"
#include "server.h"
#include "article.h"
#include "dbinterface.h"
#include "connectionclosedexception"
#include "protocol.h"



#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
//#include "diskDatabase.h"
//#include "memoryDatabase"
using namespace std;
#include <iostream>

//listNewsgroups
//createNewsGroup
//deleteNewsgroup
//listArticlesInNewgroup

//readArticle
//writeArticle
//deleteArticle
//

void listNewsgroups(dbinterface&);
void createNewsGroup(dbinterface&);
void deleteNewsgroup(dbinterface&);
void listArticlesInNewgroup(dbinterface&);

void readArticle(dbinterface&);
void createNewsGroup(dbinterface&);
void writeArticle(dbinterface&);
void deleteArticle(dbinterface&);

/*
 * Read an integer from a client.
 */
int readNumber(const shared_ptr<Connection>& conn)
{
        unsigned char byte1 = conn->read();
        unsigned char byte2 = conn->read();
        unsigned char byte3 = conn->read();
        unsigned char byte4 = conn->read();
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Send a string to a client.
 */
void writeString(const shared_ptr<Connection>& conn, const string& s)
{
        for (char c : s) {
                conn->write(c);
        }
        conn->write('$');
}

Server init(int argc, char* argv[])
{
        if (argc != 2) {
                cerr << "Usage: myserver port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = stoi(argv[1]);
        } catch (exception& e) {
                cerr << "Wrong format for port number. " << e.what() << endl;
                exit(2);
        }

        Server server(port);
        if (!server.isReady()) {
                cerr << "Server initialization error." << endl;
                exit(3);
        }
        return server;
}
//________________________________________________________


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

void readInt(){
        conn.read();

	//read number
        unsigned char byte1 = conn.read();
        unsigned char byte2 = conn.read();
        unsigned char byte3 = conn.read();
        unsigned char byte4 = conn.read();
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void sendCode(Protocol code) {
    sendByte(static_cast<unsigned char>(code));
}

void sendByte(unsigned char byte) {
    conn.write(byte);
}

void sendInt(int value) {
    for (auto i = 3; i >= 0; --i) {
        sendByte((value >> (8 * i)) & 0xFF);
       
    }
}

void SendString(const Connection& conn, string s){
	    conn.write(static_cast<int>(Protocol::PAR_STRING));
	    int len = s.size();
            conn.write((len >> 24) & 0xFF);
            conn.write((len >> 16) & 0xFF);
	    conn.write((len >> 8) & 0xFF);
	    conn.write(len & 0xFF);
	    for(char c : s)conn.write(c);
}



void printError(const Connection& conn){
	cout<<"ERROR: ";
        unsigned char err = conn.read();
	switch(err):
		case static_cast<int>(Protocol::ERR_NG_ALREADY_EXISTS):
			cout<<"News group already exists"<<endl;
		break;
		case static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST):
			cout<<"News group does not exist"<<endl;
		break;
		case static_cast<int>(Protocol::ERR_ART_DOES_NOT_EXIST):
			cout<<"Article does not exist"<<endl;
}

int checkAck(){

}

//________________________________________________________


void listNewsgroups(dbinterface& db, const shared_ptr<Connection>& conn){
    writeString(conn, static_cast<unsigned char>(Protocol::ANS_LIST_NG));
    auto newsGroup = db.listAll();

    sendCode(Protocol::PAR_NUM); 
    sendInt(newgroup.size());

    for(auto& news; newsgroup){
        sendCode(Protocol::PAR_NUM); 
        sendInt(news.id);

        sendCode(Protocol.PAR_STRING);
        sendInt(news.name.size())
        writeString(conn, param )
    }
}

void createNewsGroup(dbinterface& db){
        auto title = readString();
        unsigned char ans = conn.sendCode(Protocol::ANS_CREATE_NG);
			if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
				cout<<"News group added"<<endl;
			}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);
}


void deleteNewsgroup(dbinterface& db){
        auto ngid = readInt();
        unsigned char ans = sendCode(Protocol::ANS_DELETE_NG);
        if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
				db.remove(ngid)
			}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);
			}       

}
void listArticlesInNewgroup(dbinterface& db){
         auto newsgroupID = readInt();
         auto articleID = readInt();
        unsigned char ans = sendCode(Protocol::ANS_GET_ART);
        if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
                auto article = db.read(newsgroupID, articleID
                sendCode(protocol::ANS_ACK);
                sendString(article.title);
                sendString(article.author);
                sendString(article.text);
                )
	}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);

}

void readArticle(dbinterface& db){

}
void createNewsGroup(dbinterface& db){

}
void writeArticle(dbinterface& db){

}
void deleteArticle(dbinterface& db){

}




int main(int argc, char* argv[]){

   auto server = init(argc, argv);

        while (true) {
                auto conn = server.waitForActivity();
                if (conn != nullptr) {
                        try {
                                int    nbr = readNumber(conn);
                                string result;
                                if (nbr > 0) {
                                        result = "positive";
                                } else if (nbr == 0) {
                                        result = "zero";
                                } else {
                                        result = "negative";
                                }
                                writeString(conn, result);
                        } catch (ConnectionClosedException&) {
                                server.deregisterConnection(conn);
                                cout << "Client closed connection" << endl;
                        }
                } else {
                        conn = make_shared<Connection>();
                        server.registerConnection(conn);
                        cout << "New client connects" << endl;
                }
        }
        return 0;


}



