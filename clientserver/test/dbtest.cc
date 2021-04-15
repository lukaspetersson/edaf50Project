#include <iostream>
#include "dbprimary.h"

using std::cout;
using std::endl;

void test_primary_db() {
    ArticleDatabasePrimary db;

    bool success = true;

    success &= db.create_newsgroup("News1");
    success &= db.create_newsgroup("News2");
    success &= !db.create_newsgroup("News2");

    Article a1("a1", "Alice", "Hello world!");

    success &= db.store_article(a1, 1);
    success &= db.store_article(a1, 2);

    success &= db.delete_article(2, 4);

    success &= db.list_articles(2).size() == 0;

    success &= db.list_newsgroups().size() == 2;
    success &= db.list_newsgroups()[0].id == 1;
    success &= db.delete_newsgroup(2);
    success &= db.list_newsgroups().size() == 1;

    success &= db.get_article(1, 3).id == 3;

    cout << db.get_article(1, 3).id << endl;

    if(success) {
        cout << "Primary database tests passed" << endl;
    } else {
        cout << "Primary database tests failed" << endl;
    }
}

void test_disk_db() {
    
}

int main(void) {
    test_primary_db();
}