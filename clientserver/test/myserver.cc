#include "connection.h"
#include "server.h"
#include "article.h"
#include "dbinterface.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "dbdisk.h"
#include "messageHandler.h"
#include "dbprimary.h"


#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

#include <iostream>

//listNewsgroups
//createNewsgroup
//deleteNewsgroup
//listArticles

//readArticle
//writeArticle
//deleteArticle
//

void listNewsgroups(ArticleDatabase&, const shared_ptr<Connection>&);
void createNewsgroup(ArticleDatabase&, const shared_ptr<Connection>&);
void deleteNewsgroup(ArticleDatabase&, const shared_ptr<Connection>&);
void listArticles(ArticleDatabase&, const shared_ptr<Connection>&);

void readArticle(ArticleDatabase&, const shared_ptr<Connection>&);
void createNewsgroup(ArticleDatabase&, const shared_ptr<Connection>&);
void writeArticle(ArticleDatabase&, const shared_ptr<Connection>&);
void deleteArticle(ArticleDatabase&, const shared_ptr<Connection>&);

/*
 * Read an integer from a client.
 
int readNumber(const shared_ptr<Connection>& conn)
{
        unsigned char byte1 = conn->read();
        unsigned char byte2 = conn->read();
        unsigned char byte3 = conn->read();
        unsigned char byte4 = conn->read();
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void writeString(const shared_ptr<Connection>& conn, const string& s)
{
        for (char c : s) {
                conn->write(c);
        }
        conn->write('$');
}
*/
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


	void listNewsgroups(ArticleDatabase& db, const shared_ptr<Connection>& conn){
	    auto newsGroup = db.list_newsgroups();

		sendCode(*conn,Protocol::ANS_LIST_NG);
	    writeNumber(*conn, newsGroup.size());

	    for(auto& news: newsGroup){
		writeNumber(*conn, news.id);
		writeString(conn, news.name );
	    }
		sendCode(*conn,Protocol::ANS_END);
	}




	void deleteNewsgroup(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		auto ngid = readNumber(*conn);
		sendCode(*conn,Protocol::ANS_DELETE_NG);
		unsigned char ans = db.delete_newsgroup(ngid);
		
		if(ans){ 
			sendCode(*conn,Protocol::ANS_ACK);
			
		}else{
			sendCode(*conn,Protocol::ANS_NAK);
			sendCode(*conn,Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		sendCode(*conn,Protocol::ANS_END);

	}
	void listArticles(ArticleDatabase& db, const shared_ptr<Connection>& conn){
		 auto newsgroupID = readNumber(*conn);
		 sendCode(*conn,Protocol::ANS_LIST_ART);
		
		auto articles = db.list_articles(newsgroupID);
		if(articles.size() != 0){
			sendCode(*conn,Protocol::ANS_ACK);

			for (auto& article : articles) {
			    writeNumber(*conn,article.id);
			    writeString(conn, article.title);
			}
		}else{
			sendCode(*conn,Protocol::ANS_NAK);
			sendCode(*conn,Protocol::ERR_NG_DOES_NOT_EXIST);

		}
		       
		sendCode(*conn,Protocol::ANS_END);
	}

	void readArticle(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		auto newsgroupID = readNumber(*conn);
		auto articleID = readNumber(*conn);
		sendCode(*conn,Protocol::ANS_GET_ART);
		int failstate = 0;
		auto article = db.get_article(newsgroupID, articleID, failstate);
		if(failstate == 0){
			sendCode(*conn,Protocol::ANS_ACK);
			writeString(conn, article.title);
			writeString(conn, article.author);
			writeString(conn, article.contents);
		}else {
			sendCode(*conn,Protocol::ANS_NAK);
			if(failstate == 1){
				sendCode(*conn,Protocol::ERR_NG_DOES_NOT_EXIST);
			}else if(failstate == 2){
				sendCode(*conn,Protocol::ERR_ART_DOES_NOT_EXIST);
			}

		}
		sendCode(*conn,Protocol::ANS_END);

	}


	void createNewsgroup(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		sendCode(*conn,Protocol::ANS_CREATE_NG);
	      auto title = readString(*conn);
		bool ans = db.create_newsgroup(title);
		if(ans){
			sendCode(*conn,Protocol::ANS_ACK);

		}else{
			sendCode(*conn,Protocol::ANS_NAK);
			sendCode(*conn,Protocol::ERR_NG_ALREADY_EXISTS);
		}
		sendCode(*conn,Protocol::ANS_END);

	}

	
	void writeArticle(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		int newsgroupID = readNumber(*conn);
		string title = readString(*conn);
		string author = readString(*conn);
		string content= readString(*conn);
	       
	    Article newart(title, author, content);
		sendCode(*conn,Protocol::ANS_CREATE_ART);
		bool ans = db.store_article(newart, newsgroupID );
		if(ans){ 
			sendCode(*conn,Protocol::ANS_ACK);
			
		}else{
			sendCode(*conn,Protocol::ANS_NAK);
			sendCode(*conn,Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		sendCode(*conn,Protocol::ANS_END);
	}


	void deleteArticle(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		 auto newsgroupID = readNumber(*conn);
		 auto articleID = readNumber(*conn);
		sendCode(*conn,Protocol::ANS_DELETE_ART);
		int failstate = 0;

		db.delete_article(newsgroupID, articleID, failstate);
		if(failstate == 0){
			sendCode(*conn,Protocol::ANS_ACK);
		}else {
			sendCode(*conn,Protocol::ANS_NAK);
			if(failstate == 1){
				sendCode(*conn,Protocol::ERR_NG_DOES_NOT_EXIST);
			}else if(failstate == 2){
				sendCode(*conn,Protocol::ERR_ART_DOES_NOT_EXIST);
			}

		}
		sendCode(*conn,Protocol::ANS_END);

}


int main(int argc, char* argv[]){


        auto server = init(argc, argv);
        

        ArticleDatabasePrimary dbp;
        ArticleDatabase& db = dbp;

        while (true) {
                auto conn = server.waitForActivity();
                if (conn != nullptr) {
                        try {
                                        int  code = readNumber(*conn);
                                
                                switch (code) {
                                case static_cast<int>(Protocol::COM_LIST_NG)   : 
                                        listNewsgroups(db, conn); break;
                                case static_cast<int>(Protocol::COM_CREATE_NG) :
                                        createNewsgroup(db, conn); break;
                                case static_cast<int>(Protocol::COM_DELETE_NG) : 
                                        deleteNewsgroup(db, conn); break;
                                case static_cast<int>(Protocol::COM_LIST_ART)  : 
                                        listArticles(db, conn); break;
                                case static_cast<int>(Protocol::COM_CREATE_ART):
                                                writeArticle(db, conn); break;
                                case static_cast<int>(Protocol::COM_DELETE_ART):
                                                deleteArticle(db, conn); break;
                                case static_cast<int>(Protocol::COM_GET_ART)   :
                                                readArticle(db, conn); break;
                                        default: 
                                                                throw ConnectionClosedException();
                                
                        
                                        }

                                                }catch(ConnectionClosedException&) {
                                server.deregisterConnection(conn);
                                cout << "Client closed connection" << endl;
                        }
                                }else{
                        conn = make_shared<Connection>();
                        server.registerConnection(conn);
                        cout << "New client connects" << endl;
                }
        }
        return 0;


}



