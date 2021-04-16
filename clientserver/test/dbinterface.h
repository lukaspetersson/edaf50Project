#ifndef ADBINTERFACE_H
#define ADBINTERFACE_H

#include "article.h"
#include <vector>

struct Newsgroup {
    unsigned int id; //Should always be the index position in the db vector
    std::string name;
    std::vector<Article> articles;
};

class ArticleDatabase {
    public:

        //Takes an article reference and stores it in some form, return true if success else false
        virtual bool store_article(Article& article, unsigned int newsgroup_id) = 0;

        //Takes a newsgroup id and an article id and returns the associated article or null if none was found
        virtual const Article get_article(unsigned int newsgroup_id, unsigned int article_id, int& failstate) = 0;

        //Deletes an article from the database, returns true if success else false
        virtual bool delete_article(unsigned int newsgroup_id, unsigned int article_id) = 0;

        //Fetches a vector of all articles in the given newsgroup, returns null if invalid newsgroup
        virtual const std::vector<Article> list_articles(unsigned int newsgroup_id) = 0;

        //Fetches a vector of all newsgroups in the database
        virtual const std::vector<Newsgroup> list_newsgroups() = 0;

        //Create a newsgroup in the database, return true if success else false
        virtual bool create_newsgroup(const std::string& name) = 0;

        //Deletes the given newsgroup, returns true if success else false
        virtual bool delete_newsgroup(unsigned int newsgroup_id) = 0;
};

#endif