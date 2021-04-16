#include "connection.h"
#include "server.h"
#include "article.h"
#include "dbinterface.h"
#include "connectionclosedexception.h"
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

void listNewsgroups(ArticleDatabase&, const shared_ptr<Connection>&);
void createNewsGroup(ArticleDatabase&, const shared_ptr<Connection>&);
void deleteNewsgroup(ArticleDatabase&, const shared_ptr<Connection>&);
void listArticlesInNewgroup(ArticleDatabase&, const shared_ptr<Connection>&);

void readArticle(ArticleDatabase&, const shared_ptr<Connection>&);
void createNewsGroup(ArticleDatabase&, const shared_ptr<Connection>&);
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
//________________________________________________________


//________________________________________________________


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
	void listArticlesInNewgroup(ArticleDatabase& db, delete_newsgroup){
		 auto newsgroupID = readNumber();
		 sendCode(*conn,Protocol::ANS_LIST_ART);
		
		auto articles = db.list_articles(newsgroupID, articleID)
		if(articles.size() != 0){
			sendCode(*conn,Protocol::ANS_ACK);

			for (auto& article : articles) {
			    writeNumber(article.id);
			    writeNumber(article.title);
			}
		}else{
			sendCode(*conn,Protocol::ANS_NAK);
			sendCode(*conn,Protocol::ERR_NG_DOES_NOT_EXIST);

		}
		       
		sendCode(*conn,Protocol::ANS_END);
	}

	void readArticle(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		auto newsgroupID = readNumber();
		auto articleID = readNumber();
		sendCode(*conn,Protocol::ANS_GET_ART);
		int failstate = 0;
		auto article = db.get_article(newsgroupID, articleID, failstate)
		if(failstate == 0){
			sendCode(*conn,Protocol::ANS_ACK);
			writeString(article.title);
			writeString(article.author);
			writeString(article.content);
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


	void createNewsGroup(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		sendCode(*conn,Protocol::ANS_CREATE_NG);
	      auto title = readString();
		bool ans = db.create_newsgroup(title);
		if(ans){
			sendCode(*conn,Protocol::ANS_ACK);

		}else{
			sendCode(*conn,Protocol::ANS_NAK);
			sendCode(*conn,Protocol::ERR_NG_ALREADY_EXIST);
		}
		sendCode(*conn,Protocol::ANS_END);

	}
	void writeArticle(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		//auto newsgroupID = readNUmber();
		auto title = readString();
		auto author = readString();
		auto content= readString()
	       
	       Article newArticle = new Article(title, author, content);
		sendCode(*conn,Protocol::ANS_CREATE_ART);
		bool ans = db.store_article(newArticle, newsgroupID )
		if(ans){ 
			sendCode(*conn,Protocol::ANS_ACK);
			
		}else{
			sendCode(*conn,Protocol::ANS_NAK);
			sendCode(*conn,Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		sendCode(*conn,Protocol::ANS_END);
	}
	void deleteArticle(ArticleDatabase& db,  const shared_ptr<Connection>& conn){
		 auto newsgroupID = readNumber();
		 auto articleID = readNumber();
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



//TODO
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



