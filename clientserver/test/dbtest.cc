#include <iostream>
#include "dbprimary.h"
#include "dbinterface.h"

using std::cout;
using std::endl;

void test_primary_db() {
    ArticleDatabasePrimary db;

    bool success = true;

    success &= db.create_newsgroup("News1");
    success &= db.create_newsgroup("News2");

    Article a1("a1", "Alice", "Hello world!");

    success &= db.store_article(a1, 0);
    success &= db.store_article(a1, 1);

    success &= db.delete_article(1, 0);

    success &= db.list_articles(1).size() == 0;

    success &= db.list_newsgroups().size() == 2;
    success &= db.delete_newsgroup(1);
    success &= db.list_newsgroups().size() == 1;

    success &= db.get_article(0, 0).id == 0;

    if(success) {
        cout << "Primary database tests passed" << endl;
    } else {
        cout << "Primary database tests failed" << endl;
    }
}

int main(void) {
    test_primary_db();
}