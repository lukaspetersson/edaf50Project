#include "dbprimary.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

bool ArticleDatabasePrimary::store_article(Article& article, unsigned int newsgroup_id) {
    auto it = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
        return false;
    }

    Newsgroup& ng = static_cast<Newsgroup&>(*it);

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
    try {
        db.push_back(ng);
    } catch(...) {
        cout << "Unable to create new newsgroup with name " << name << endl;
        return false;
    }

    return true;
}

const Article& ArticleDatabasePrimary::get_article(unsigned int newsgroup_id, unsigned int article_id) {
    auto it1 = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it1 == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
    }

    const Newsgroup& ng = static_cast<Newsgroup>(*it1);

    auto it2 = find_if(ng.articles.begin(), ng.articles.end(), [article_id](const Article& a){return a.id == article_id;});

    if(it2 == ng.articles.end()) {
        cout << "ERROR: Article with id " << article_id << " not found in newsgroup with id " << newsgroup_id << '!' << endl;
    }

    return *it2;
}

bool ArticleDatabasePrimary::delete_article(unsigned int newsgroup_id, unsigned int article_id) {
    auto it1 = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it1 == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
        return false;
    }

    Newsgroup& ng = static_cast<Newsgroup&>(*it1);

    auto it2 = find_if(ng.articles.begin(), ng.articles.end(), [article_id](const Article& a){return a.id == article_id;});

    if(it2 == ng.articles.end()) {
        cout << "ERROR: Article with id " << article_id << " not found in newsgroup with id " << newsgroup_id << '!' << endl;
        return false;
    }

    ng.articles.erase(it2);
    return true;
}

const std::vector<Article> ArticleDatabasePrimary::list_articles(unsigned int newsgroup_id) const {
    auto it1 = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it1 == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
    }

    return static_cast<const Newsgroup&>(*it1).articles;
}

const std::vector<Newsgroup>& ArticleDatabasePrimary::list_newsgroups() const {return db;}

bool ArticleDatabasePrimary::delete_newsgroup(unsigned int newsgroup_id) {
    auto it1 = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it1 == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
        return false;
    }

    db.erase(it1);

    return true;
}

