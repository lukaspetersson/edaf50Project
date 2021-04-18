#include <iostream>
#include "dbinterface.h"
#include "dbprimary.h"
#include "dbdisk.h"

using std::cout;
using std::endl;

void test_primary_db() {
    ArticleDatabasePrimary db;

    bool success = true;

    success &= db.create_newsgroup("News1");
    success &= db.create_newsgroup("News2");
    success &= !db.create_newsgroup("News2");

    Article a1("a1", "Alice", "Hello world!");

    int failstate = 0;

    success &= db.store_article(a1, 1);
    success &= db.store_article(a1, 2);

    success &= db.delete_article(2, 4, failstate);
    success &= !failstate;

    success &= !db.delete_article(7, 6, failstate);
    success &= failstate == 1;

    success &= !db.delete_article(2, 4, failstate);
    success &= failstate == 2; 

    success &= db.list_articles(2).size() == 0;

    success &= db.list_newsgroups().size() == 2;
    success &= db.list_newsgroups()[0].id == 1;
    success &= db.delete_newsgroup(2);
    success &= db.list_newsgroups().size() == 1;
    
    
    failstate = 0;
    success &= db.get_article(1, 3, failstate).id == 3;
    success &= !failstate;
    
    db.get_article(7, 6, failstate);
    success &= failstate == 1;

    db.get_article(1, 100, failstate);
    success &= failstate == 2;
    

    if(success) {
        cout << "Primary database tests passed" << endl;
    } else {
        cout << "Primary database tests failed" << endl;
    }
}

void test_disk_db() {
    ArticleDatabaseDisk db("ng_file", "ar_file", "id_file");

    Article a1("a1", "Alice", "Hello world!");
    Article a2("a2", "Bob", "BOOO");

    db.clear_database();

    bool success = true;

    success &= db.get_next_id() == 1;
    success &= db.get_next_id() == 2;
    success &= db.get_next_id() == 3;

    success &= db.create_newsgroup("News1");
    success &= db.create_newsgroup("News2");

    int failstate = 0;

    const std::vector<Newsgroup>& ngs = db.list_newsgroups();

    success &= ngs[0].name == "News1";
    success &= ngs[1].name == "News2";

    success &= db.store_article(a1, 4);
    success &= db.store_article(a2, 4);
    success &= db.store_article(a1, 5);

    const std::vector<Article>& ars = db.list_articles(4);

    success &= ars[0].title =="a1";
    success &= ars[1].id == 7;

    success &= !db.delete_article(7, 6, failstate);
    success &= failstate == 1;

    success &= !db.delete_article(4, 100, failstate);
    success &= failstate == 2;

    failstate = 0;
    success &= db.delete_article(4, 6, failstate);
    success &= !failstate;
    success &= db.list_articles(4).size() == 1;

    success &= db.store_article(a2, 5);
    success &= db.store_article(a1, 5);
    success &= db.store_article(a2, 5);

    const std::vector<Newsgroup>& ngs2 = db.list_newsgroups();
    const std::vector<Article>& ars2 = db.list_articles(ngs2[1].id);
    failstate = 0;
    Article atest = db.get_article(ngs2[1].id, ars2[1].id, failstate);

    success &= atest.title == "a2" && atest.id == 9;
    success &= !failstate;

    db.get_article(7, 6, failstate);
    success &= failstate == 1;

    db.get_article(4, 100, failstate);
    success &= failstate == 2;

    success &= db.delete_newsgroup(ngs2[0].id);
    success &= db.list_newsgroups().size() == 1;

    if(success) {
        cout << "Disk database tests passed" << endl;
    } else {
        cout << "Disk database tests failed" << endl;
    }
}

int main(void) {
    test_primary_db();
    test_disk_db();
}