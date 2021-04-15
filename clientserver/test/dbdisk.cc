#include <iostream>
#include "dbdisk.h"
#include <fstream>
#include <sstream>
#include <string>

using std::cout;
using std::endl;
using std::fstream;

unsigned int ArticleDatabaseDisk::get_next_id() {
    fstream id_db;
    unsigned int id = 0;
    id_db.open(id_file, fstream::in | fstream::out);

    if(!id_db) {
        cout << "Couldn't open " << id_file << endl;
        return 0;
    }

    if(id_db.peek() == std::fstream::traits_type::eof()) {
        id_db.clear();
        id_db << 0;
    }
    id_db.seekg(0, std::ios::beg);
    id_db.clear();
    id_db >> id;

    id_db.seekg(0, std::ios::beg);
    id_db.clear();
    id_db << ++id;

    id_db.close();

    return id;
}

bool ArticleDatabaseDisk::store_article(Article& article, unsigned int newsgroup_id) {
    fstream ar_db;
    ar_db.open(ar_file, fstream::out | fstream::app);

    //Check for valid newsgroup first

    article.id = get_next_id();

    ar_db << newsgroup_id << '|'
    << article.id << '|'
    << article.title << '|'
    << article.author << '|'
    << article.contents
    << '\n';

    ar_db.close();

    return true;
}

bool ArticleDatabaseDisk::create_newsgroup(const std::string& name) {
    fstream ng_db;
    ng_db.open(ng_file, fstream::out | fstream::app);

    if(!ng_db) {
        cout << "Couldn't open " << ng_file << endl;
        return false;
    }
    //Check for unique newsgroup name first
    ng_db << get_next_id() << '|'
    << name
    << '\n';

    ng_db.close();

    return true;
}

const Article ArticleDatabaseDisk::get_article(unsigned int newsgroup_id, unsigned int article_id) {
    fstream ar_db;
    std::string line;
    unsigned int ng_id, ar_id;
    Article a;

    ar_db.open(ar_file, fstream::in);

    while(getline(ar_db, line)) {
        std::stringstream ss1, ss2;
        ss1 << line;
        ss2 << read_field(ss1);
        ss2 >> ng_id;      

        if(ng_id == newsgroup_id) {
            ss2.clear();
            ss2 << read_field(ss1);
            ss2 >> ar_id;
            if(ar_id == article_id) {
                ss2.str("");
                ss2.clear();
                ss2 << line;
                read_field(ss2);
                ss2 >> a;
                return a;
            }
        }
    }
    ar_db.close();

    return a;
}

bool ArticleDatabaseDisk::delete_article(unsigned int newsgroup_id, unsigned int article_id) {
    bool success = false;
    fstream ar_db, tmp;
    std::string line;
    std::stringstream ss;
    unsigned int ng_id, ar_id;

    ar_db.open(ar_file, fstream::in);
    tmp.open("tmp", fstream::out | fstream::app);

    while(getline(ar_db, line)) {
        ss << line;
        ss >> ng_id;
        if(ng_id == newsgroup_id) {
            read_field(ss);
            ss >> ar_id;
            if(ar_id == article_id) {
                success = true;
                continue;
            }
        }

        tmp << line << '\n';
    }

    tmp.close();
    ar_db.close();

    remove(ar_file.c_str());
    rename("tmp", ar_file.c_str());

    return success;
}

const std::vector<Article> ArticleDatabaseDisk::list_articles(unsigned int newsgroup_id) { //Make return reference
    std::vector<Article> articles;
    fstream ar_db;
    std::stringstream ss;
    ar_db.open(ar_file, fstream::in);
    Article a;
    std::string s;

    while(!ar_db.eof()) {
        s = read_field(ar_db);
        ss << s;
        unsigned int ng_id = 0;
        ss >> ng_id;
        ss.clear();

        if(ng_id == newsgroup_id) {
            ar_db >> a;
            articles.push_back(a);
        } else {
            getline(ar_db, s); // discard rest of line;
        }  
    }

    ar_db.close();

    return articles;
}

const std::vector<Newsgroup> ArticleDatabaseDisk::list_newsgroups() {
    std::vector<Newsgroup> newsgroups;
    fstream ng_in;
    std::stringstream ss;
    std::string line;

    ng_in.open(ng_file, fstream::in);

    while(getline(ng_in, line)) {
        Newsgroup ng;
        ss << line.substr(0, line.find(delimiter));
        ss >> ng.id;
        ss.clear();
        ng.name = line.substr(line.find(delimiter)+1, line.length());
        newsgroups.push_back(ng);
        ss.clear();
    }

    ng_in.close();

    return newsgroups;
}

bool ArticleDatabaseDisk::delete_newsgroup(unsigned int newsgroup_id) {
    bool success = false;
    fstream ng_db, tmp;
    std::string line;
    std::stringstream ss;
    unsigned int ng_id;

    ng_db.open(ng_file, fstream::in);
    tmp.open("tmp", fstream::out | fstream::app);

    while(getline(ng_db, line)) {
        ss << line;
        ss >> ng_id;
        if(ng_id == newsgroup_id) {
            success = true;
            continue;
        }

        tmp << line << '\n';
    }

    tmp.close();
    ng_db.close();

    remove(ng_file.c_str());
    rename("tmp", ng_file.c_str());

    return success;
}

bool ArticleDatabaseDisk::clear_database() {
    fstream fs;

    fs.open(id_file, fstream::out | fstream::trunc);
    fs.close();

    fs.open(ng_file, fstream::out | fstream::trunc);
    fs.close();

    fs.open(ar_file, fstream::out | fstream::trunc);
    fs.close();

    return true;
}