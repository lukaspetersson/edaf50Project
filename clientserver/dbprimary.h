#ifndef DBPRIMARY_H
#define DBPRIMARY_H

#include "dbinterface.h"
#include "article.h"

struct Newsgroup {
    unsigned int id;
    std::string name;
    std::vector<Article> articles;
}

class ArticleDatabasePrimary : public ArticleDatabase {
    public:
        //Takes an article reference and stores it in some form, return true if success else false
        bool store_article(Article& article, unsigned int newsgroup_id);

        //Takes a newsgroup id and an article id and returns the associated article or null if none was found
        Article get_article(unsigned int newsgroup_id, unsigned int article_id);

        //Deletes an article from the database, returns true if success else false
        bool delete_article(unsigned int newsgroup_id, unsigned int article_id);

        //Fetches a vector of all articles in the given newsgroup, returns null if invalid newsgroup
        std::vector<Article>& list_articles(unsigned int newsgroup_id);

        //Fetches a vector of all newsgroups in the database
        std::vector<Newsgroup>& list_newsgroups();

        //Store a newsgroup in the database, return true if success else false
        bool store_newsgroup(unsigned int newsgroup_id, const std::string& name);

        //Deletes the given newsgroup, returns true if success else false
        bool delete_newsgroup(unsigned int newsgroup_id);
    
    private:
        std::vector<Newsgroup> db;
}


#endif