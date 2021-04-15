#include <iostream>
#include "dbdisk.h"
#include <fstream>

using std::cout;
using std::endl;
using std::fstream;

bool ArticleDatabaseDisk::store_article(Article& article, unsigned int newsgroup_id) {
    fstream ar_db;
    ar_db.open(db_file);

    ar_db << newsgroup_id << '|'
    << article.id << '|'
    << article.title << '|'
    << article.author << '|'
    << article.contents

    ar_db.close();
}

bool ArticleDatabaseDisk::create_newsgroup(const std::string& name) {

}

const Article& ArticleDatabaseDisk::get_article(unsigned int newsgroup_id, unsigned int article_id) {

}

bool ArticleDatabaseDisk::delete_article(unsigned int newsgroup_id, unsigned int article_id) {
    
}

const std::vector<Article> ArticleDatabaseDisk::list_articles(unsigned int newsgroup_id) const {

}

const std::vector<Newsgroup>& ArticleDatabaseDisk::list_newsgroups() const {
    
}

bool ArticleDatabaseDisk::delete_newsgroup(unsigned int newsgroup_id) {

}

