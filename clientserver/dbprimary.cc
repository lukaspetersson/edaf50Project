#include "dbprimary.h"
#include <iostream>

using std::cout;
using std::endl;

bool ArticleDatabasePrimary::store_article(Article& article, unsigned int newsgroup_id) {
    Newsgroup& ng = check_and_return_newsgroup(newsgroup_id);

    if(ng == nullptr) return false;

    article.id = ng.articles.size(); //Change to come from id generator

    try {
        ng.articles.push_back(article);
    } catch(...) {
        cout << "ERROR: Unable to store article " << article.title << " with id " << article.id << endl;
        return false; 
    }

    return true;
}

bool ArticleDatabasePrimary::create_newsgroup(const std::string& name) {
    Newsgroup ng;
    ng.id = db.size();
    ng.name = name;

    db.push_back(ng);
}

Newsgroup& ArticleDatabasePrimary::check_and_return_newsgroup(unsigned int newsgroup_id) {
    auto it = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;})

    if(it == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
        return nullptr;
    }

    return static_cast<Newsgroup>(*it);
}

