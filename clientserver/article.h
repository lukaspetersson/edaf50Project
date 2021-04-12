#ifndef ARTICLE_H
#define ARTICLE_H

/*
Article ojects keep track of the title, author and contents of an article.
They also have an overloaded operator for comparing their title to strings
which means all titles in a newsgroup should be unique.
*/
class Article {
    public:
        Article (const std::string& title, const std::string& author, const std::string& contents)
                : title(title)
                , author(author)
                , contents(contents) {}
        
        //case sensitive Article to title string equals operator
        bool operator==(const std::string& title) { 
            return title == this->title;
        }

        bool operator!=(const std::string& title) {
            return title != this->title;
        }

        unsigned int id;
        const std::string title;
        const std::string author;
        const std::string contents; 
};

#endif