#include "connection.h"
#include "server.h"
#include "article.h"
#include "dbinterface.h"
#include "connectionclosedexception"
#include "protocol.h"

#include "messageHandler.h"


#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

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


//________________________________________________________


void listNewsgroups(dbinterface& db, const shared_ptr<Connection>& conn){
    writeString(conn, static_cast<unsigned char>(Protocol::ANS_LIST_NG));
    auto newsGroup = db.list_newsgroups();

    sendCode(Protocol::PAR_NUM); 
    writeNumber(newgroup.size());

    for(auto& news; newsgroup){
        sendCode(Protocol::PAR_NUM); 
        writeNumber(news.id);

        sendCode(Protocol.PAR_STRING);
        writeNumber(news.name.size())
        writeString(conn, param )
    }
}




void deleteNewsgroup(dbinterface& db,  const shared_ptr<Connection>& conn){
        auto ngid = readNumber();
        unsigned char ans = sendCode(Protocol::ANS_DELETE_NG);
        if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
				db.delete_newsgroup(ngid)
			}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);
			}       

}
void listArticlesInNewgroup(dbinterface& db, delete_newsgroup){
         auto newsgroupID = readNumber();
         auto articleID = readNumber();
        unsigned char ans = sendCode(Protocol::ANS_GET_ART);
        if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
                auto articles = db.list_articles(newsgroupID, articleID
                for (auto& article : articles) {
            writeNumber(article.id);
            writeNumber(article.title);
        }
                )
	}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);

}

void readArticle(dbinterface& db,  const shared_ptr<Connection>& conn){
        auto newsgroupID = readNumber();
        auto articleID = readNumber();
        unsigned char ans = sendCode(Protocol::ANS_GET_ART);

        sendCode(Protocol::ANS_GET_ART);

        if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
                auto article = db.get_article(newsgroupID, articleID
                sendCode(protocol::ANS_ACK);
                writeString(article.title);
                writeString(article.author);
                writeString(article.content);
                )
	}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);

}


}
void createNewsGroup(dbinterface& db,  const shared_ptr<Connection>& conn){
      auto title = readString();
        unsigned char ans = sendCode(Protocol::ANS_CREATE_NG);
        if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
				db.create_newsgroup(title)
			}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);
			}   

}
void writeArticle(dbinterface& db,  const shared_ptr<Connection>& conn){
        //auto newsgroupID = readNUmber();
        auto id = readNumber()
        auto title = readString();
        auto author = readString();
        auto content= readString()
       
       Article newArticle = new Article(title, author, content)
       newArticle.id = id;
        unsigned char ans = sendCode(Protocol::ANS_CREATE_NG);
        if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
				db.store_article(newArticle, newsgroupID )
			}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);
			}   
}
void deleteArticle(dbinterface& db,  const shared_ptr<Connection>& conn){
         auto newsgroupID = readNumber();
         auto articleID = readNumber();

        unsigned char ans = sendCode(Protocol::ANS_DELETE_ART);
        if(ans == static_cast<int>(Protocol::ANS_ACK)){ 
				db.delete_article(newsgroupID, articleID)
			}else if(ans == static_cast<int>(Protocol::ANS_NAK)){
				printError(conn);
			}  
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



