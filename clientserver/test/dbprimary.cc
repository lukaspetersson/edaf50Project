#include "dbprimary.h"
#include "idgen.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

bool ng_sorting_cond(Newsgroup& a, Newsgroup& b) {return a.id < b.id;}
bool ar_sorting_cond(Article& a, Article& b) {return a.id < b.id;}

bool ArticleDatabasePrimary::store_article(Article& article, unsigned int newsgroup_id) {
    auto it = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
        return false;
    }

    Newsgroup& ng = static_cast<Newsgroup&>(*it);

    try {
        article.id = get_unique_id();
        ng.articles.push_back(article);
    } catch(...) {
        cout << "ERROR: Unable to store article " << article.title << " with id " << article.id << endl;
        return false; 
    }

    return true;
}

bool ArticleDatabasePrimary::create_newsgroup(const std::string& name) {
    Newsgroup ng;

    auto it = find_if(db.begin(), db.end(), [name](const Newsgroup& n){return n.name == name;});

    if(it != db.end()) {
        cout << "ERROR: Newsgroup name '" << name << "' already exists in the database" << endl;
        return false;
    }

    try {
        ng.id = get_unique_id();
        ng.name = name;
        db.push_back(ng);
    } catch(...) {
        cout << "Unable to create new newsgroup with name " << name << endl;
        return false;
    }

    return true;
}

const Article ArticleDatabasePrimary::get_article(unsigned int newsgroup_id, unsigned int article_id, int& failstate) {
    Article a;

    auto it1 = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it1 == db.end()) {
        failstate = 1;
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
        return a;
    }
    auto it2 = find_if(it1->articles.begin(), it1->articles.end(), [article_id](const Article& a){return a.id == article_id;});

    if(it2 == it1->articles.end()) {
        failstate = 2;
        cout << "ERROR: Article with id " << article_id << " not found in newsgroup with id " << newsgroup_id << '!' << endl;
        return a;
    }

    return *it2;
}

bool ArticleDatabasePrimary::delete_article(unsigned int newsgroup_id, unsigned int article_id, int& failstate) {
    auto it1 = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it1 == db.end()) {
        failstate = 1;
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
        return false;
    }

    Newsgroup& ng = static_cast<Newsgroup&>(*it1);

    auto it2 = find_if(ng.articles.begin(), ng.articles.end(), [article_id](const Article& a){return a.id == article_id;});

    if(it2 == ng.articles.end()) {
        failstate = 2;
        cout << "ERROR: Article with id " << article_id << " not found in newsgroup with id " << newsgroup_id << '!' << endl;
        return false;
    }

    ng.articles.erase(it2);
    return true;
}

const std::vector<Article> ArticleDatabasePrimary::list_articles(unsigned int newsgroup_id, bool& fail) {
    auto it1 = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it1 == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
    } else {
        fail = false;
    }

    Newsgroup res = static_cast<Newsgroup>(*it1);

    std::sort(res.articles.begin(), res.articles.end(), ar_sorting_cond);

    return res.articles;
}

const std::vector<Newsgroup> ArticleDatabasePrimary::list_newsgroups() {
    std::sort(db.begin(), db.end(), ng_sorting_cond);
    return db;
}

bool ArticleDatabasePrimary::delete_newsgroup(unsigned int newsgroup_id) {
    auto it1 = find_if(db.begin(), db.end(), [newsgroup_id](const Newsgroup& n){return n.id == newsgroup_id;});

    if(it1 == db.end()) {
        cout << "ERROR: Newsgroup with id " << newsgroup_id << " not found!" << endl;
        return false;
    }

    db.erase(it1);

    return true;
}

