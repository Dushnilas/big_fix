#ifndef MOVIES_H
#define MOVIES_H
#pragma ide diagnostic ignored "modernize-use-nodiscard"

#include <string>
#include <vector>
#include <map>
#include <QSharedPointer>


enum class FilmType {
    Movie,
    TvMovie,
    TvSeries,
    Default
};

class Movie;

class Actor {
private:
    const std::string _name;
    const std::string _character_played;
    const std::string _nconst;
    std::string _photo_url;
    int _birth_year;
    int _death_year;
    int _actor_importance;

public:
    Actor(std::string name, std::string character_played, std::string nconst, std::string photo_url, int birth_year,
          int death_year, int actor_importance);

    std::string getName() const;
    std::string getId() const;
    std::string getPhoto() const;
    std::vector<int> getLifeYears() const;
    int getImportance() const;

private:
    std::vector<QSharedPointer<Movie>> _movies;
    std::map<std::string, QSharedPointer<Movie>> _all_characters;

public:
    const std::map<std::string, QSharedPointer<Movie>>& getAllCharacters() const;
    // void addToMovie(const QSharedPointer<Movie>& movie);
    // void removeMovie(const QSharedPointer<Movie>& movie);
    const std::vector<QSharedPointer<Movie>>& getMovies() const;
};

class Movie {
private:
    const std::string _name;
    const std::string _tconst;
    std::vector<std::string> _genre;
    std::string _description;
    std::string _photo_url;
    const FilmType _film_type;
    const int _year_start;
    int _year_end;
    const bool _is_adult;
    double _rating;
    int _num_votes;

public:
    Movie(std::string name, std::string tconst, std::string description, std::string url, FilmType film_type,
          int year_start, int year_end, bool is_adult, double rating, int num_votes);

    std::string getName() const;
    std::string getTconst() const;
    void setGenre(const std::vector<std::string>& genres);
    const std::vector<std::string>& getGenre() const;
    std::string getDescription() const;
    void setPhoto(std::string url);
    std::string getPhoto();
    FilmType getFilmType() const;
    std::vector<int> getYears() const;
    bool IsAdult() const;
    double getRating() const;
    int getVotes() const;

    void updateRating(int new_vote, bool status, int user_rating);

private:
    std::vector<QSharedPointer<Actor>> _aboba;
    std::map<std::string, std::vector<QSharedPointer<Actor>>> _actors;

public:
    void loadActors();
    // void clearActors();
    std::map<std::string, std::vector<QSharedPointer<Actor>>>& getActors() ;
    // void addActor(const QSharedPointer<Actor>& actor);
    // void removeActor(const QSharedPointer<Actor>& actor);

private:
    std::vector<std::pair<std::string, std::string>> _comments;

public:
    void loadComments();
    const std::vector<std::pair<std::string, std::string>>& getComments() const;
    void leaveComment(const std::string& user_id, const std::string& com);
};

class Collection {
private:
    std::vector<QSharedPointer<Movie>> _collection;
    std::string _name;
    std::string _photo_url;
    int _collection_id;

public:
    Collection(int collection_id, const std::string& name="Collection", const std::string& photo_url="");

    std::string getName() const;
    void setName(const std::string& name);
    std::string getId() const;
    std::string getPhoto() const;
    void setPhoto(const std::string& url);
    void loadMovies(const std::vector<std::string>& tconsts);
    const std::vector<QSharedPointer<Movie>>& getMovies() const;
    bool addMovie(const QSharedPointer<Movie>& movie);
    void removeMovie(const QSharedPointer<Movie>& movie);
};

#endif