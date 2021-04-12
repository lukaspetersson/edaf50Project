#ifndef ADBINTERFACE_H
#define ADBINTERFACE_H

#include "article.h"

class ArticleDatabase {
    public: 

        //Takes an article reference and stores it in some form, return true if success else false
        virtual bool store_article(Article& article, unsigned int newsgroup_id) = 0;

        //Takes a newsgroup id and an article id and returns the associated article or null if none was found
        virtual Article get_article(unsigned int newsgroup_id, unsigned int article_id) = 0;

        //Deletes an article from the database, returns true if success else false
        virtual bool delete_article(unsigned int newsgroup_id, unsigned int article_id) = 0;

        //Fetches a vector of all articles in the given newsgroup, returns null if invalid newsgroup
        virtual std::vector<Article>& list_articles(unsigned int newsgroup_id) = 0;

        //Fetches a vector of all newsgroups in the database
        virtual std::vector<Newsgroup>& list_newsgroups() = 0;

        //Create a newsgroup in the database, return true if success else false
        virtual bool create_newsgroup(unsigned int newsgroup_id, const std::string& name) = 0;

        //Deletes the given newsgroup, returns true if success else false
        virtual bool delete_newsgroup(unsigned int newsgroup_id) = 0;
}

#endif