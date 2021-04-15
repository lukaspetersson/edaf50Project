#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include <istream>
#include <fstream>
#include <sstream>


inline std::string read_field(std::iostream& fs) {
    std::string s = "";
    char next = fs.get();

    while(next != '|' && next != '\n' && next != EOF) {
        s.push_back(next);
        next = fs.get();
    }
    return s;
}


/*
Article ojects keep track of the title, author and contents of an article.
They also have an overloaded operator for comparing their title to strings
which means all titles in a newsgroup should be unique.
*/
class Article {
    public:
        Article() = default;

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

        friend std::iostream& operator>> (std::iostream& fs, Article& a) {
            std::stringstream ss;

            ss << read_field(fs); // Reads article id;
            ss >> a.id;
            a.title = read_field(fs); //reads title;
            a.author = read_field(fs); //reads author;
            a.contents = read_field(fs); //reads contents;
            return fs;
        }

        unsigned int id;
        std::string title;
        std::string author;
        std::string contents; 
};



#endif