#ifndef ADBINTERFACE_H
#define ADBINTERFACE_H

#include "article.h"

class ArticleDatabase {
    public: 

        //Takes an article reference and stores it in some form
        virtual void store_article(Article& article) = 0;

        //Takes a unique title and newsgroup combination and returns the associated article object
        virtual Article get_article(const std::string& newsgroup, const std::string& title) = 0;
}

#endif