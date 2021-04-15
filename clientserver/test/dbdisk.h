#ifndef DBDISK_H
#define DBDISK_H

#include "dbinterface.h"
#include "article.h"

const std::string delimiter = "|";

/*
id_file keeps track of the id count.
Stores article entries in ar_file with the format: NEWSGROUP_ID | ARTICLE_ID | ARTICLE_TITLE | ARTICLE_AUTHOR | ARTICLE_CONTENTS
Stores newsgroups in ng_file with the format: NEWSGROUP_ID | NEWSGROUP_NAME
*/
class ArticleDatabaseDisk : public ArticleDatabase {
    public:
        ArticleDatabaseDisk(const std::string& ng_file, const std::string& ar_file, const std::string& id_file)
        : ng_file(ng_file)
        , ar_file(ar_file)
        , id_file(id_file) {}

        //Takes an article reference and stores it in a file on disk, return true if success else false
        bool store_article(Article& article, unsigned int newsgroup_id);

        //Takes a newsgroup id and an article id and returns the associated article or null if none was found
        const Article get_article(unsigned int newsgroup_id, unsigned int article_id);

        //Deletes an article from the database, returns true if success else false
        bool delete_article(unsigned int newsgroup_id, unsigned int article_id);

        //Fetches a vector of all articles in the given newsgroup, returns null if invalid newsgroup
        const std::vector<Article> list_articles(unsigned int newsgroup_id);

        //Fetches a vector of all newsgroups in the database
        const std::vector<Newsgroup> list_newsgroups();

        //Create a newsgroup in the database, return true if success else false
        bool create_newsgroup(const std::string& name);

        //Deletes the given newsgroup, returns true if success else false
        bool delete_newsgroup(unsigned int newsgroup_id);

        unsigned int get_next_id();

        bool clear_database();

    private:
        const std::string ng_file; //Path to file that stores newsgroups
        const std::string ar_file; //Path to file that stores articles
        const std::string id_file; //Path to file that stores the id counts
};


#endif