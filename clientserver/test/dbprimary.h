#ifndef DBPRIMARY_H
#define DBPRIMARY_H

#include "dbinterface.h"
#include "article.h"


/*
This implementation of the database interface stores articles using a vector of newsgroups 
which in turn contain a vector of articles.
*/
class ArticleDatabasePrimary : public ArticleDatabase {
    public:
        
        //Takes an article reference and stores it in a vector of newsgroups, return true if success else false
        bool store_article(Article& article, unsigned int newsgroup_id);

        //Takes a newsgroup id and an article id and returns the associated article or null if none was found
        const Article get_article(unsigned int newsgroup_id, unsigned int article_id, int& failstate);

        //Deletes an article from the database, returns true if success else false
        bool delete_article(unsigned int newsgroup_id, unsigned int article_id, int& failstate);

        //Fetches a vector of all articles in the given newsgroup, returns null if invalid newsgroup
        const std::vector<Article> list_articles(unsigned int newsgroup_id);

        //Fetches a vector of all newsgroups in the database
        const std::vector<Newsgroup> list_newsgroups();

        //Create a newsgroup in the database, return true if success else false
        bool create_newsgroup(const std::string& name);

        //Deletes the given newsgroup, returns true if success else false
        bool delete_newsgroup(unsigned int newsgroup_id);

    private:
        std::vector<Newsgroup> db;
};


#endif