#include "movies.h"
#include <iostream>
#include <utility>
#include "../logger/logger.h"
#include "../../mysql-queries/mysql-queries.h"
#include <QSharedPointer>
#include "../../../backend.h"


// Definition of Actor class methods
Actor::Actor(std::string name, std::string character_played, std::string nconst, std::string photo_url, int birth_year, int death_year, int actor_importance)
        : _name(std::move(name)), _character_played(std::move(character_played)), _nconst(std::move(nconst)),
          _photo_url(std::move(photo_url)), _birth_year(birth_year), _death_year(death_year),
          _actor_importance(actor_importance) {

//    Logger::getInstance().logInfo("Actor class object was created (" + _name + ").");
}

std::string Actor::getName() const {
    return _name;
}

std::string Actor::getId() const {
    return _nconst;
}

const std::vector<QSharedPointer<Movie>>& Actor::getMovies() const {
    return _movies;
}

std::string Actor::getPhoto() const {
    return _photo_url;
}

std::vector<int> Actor::getLifeYears() const {
    return std::vector<int>({_birth_year, _death_year});
}

int Actor::getImportance() const {
    return _actor_importance;
}

const std::map<std::string, QSharedPointer<Movie>>& Actor::getAllCharacters() const {
    return _all_characters;
}

bool compareMovies(const QSharedPointer<Movie>& movie1, const QSharedPointer<Movie>& movie2) {
    return movie1->getTconst() == movie2->getTconst();
}

// void Actor::addToMovie(const QSharedPointer<Movie>& movie) {
//     if (std::find_if(_movies.begin(), _movies.end(), [&movie](const QSharedPointer<Movie>& m) {
//         return compareMovies(m, movie); }) == _movies.end()) {
//         _movies.push_back(movie);
//         Logger::getInstance().logInfo("Actor`s movies list was updated. Actor " + _name + " was added to "
//                                       + movie->getName() + ".");
//
//         movie->addActor(QSharedPointer<Actor>(this));
//     } else {
//         Logger::getInstance().logWarning("Actor" + _name + " already in " + movie->getName() + ".");
//     }
// }
//
// void Actor::removeMovie(const QSharedPointer<Movie>& movie) {
//     auto it = std::find_if(_movies.begin(), _movies.end(), [&movie](const QSharedPointer<Movie>& m) {
//         return compareMovies(m, movie); });
//     if (it != _movies.end()) {
//         _movies.erase(it);
//         Logger::getInstance().logInfo("Movie " + movie->getName() + " was removed from actor's (" + _name + ") movies list.");
//
//         movie->removeActor(QSharedPointer<Actor>(this));
//     } else {
//         Logger::getInstance().logWarning("Actor " + _name + " didn't play in " + movie->getName() + ".");
//     }
// }

// Definition of Movie class methods
Movie::Movie(std::string name, std::string tconst, std::string description, std::string url, FilmType film_type,
             int year_start, int year_end, bool is_adult, double rating, int num_votes)
        : _name(std::move(name)), _tconst(std::move(tconst)), _description(std::move(description)), _film_type(film_type),
          _year_start(year_start), _year_end(year_end), _is_adult(is_adult), _rating(rating), _num_votes(num_votes), _photo_url(url) {

//    Logger::getInstance().logInfo("Movie class object was created (" + _name + ").");
}

std::string Movie::getName() const {
    return _name;
}

std::string Movie::getTconst() const {
    return _tconst;
}

void Movie::setGenre(const std::vector<std::string>& genres) {
    _genre.assign(genres.begin(), genres.end());
}

const std::vector<std::string>& Movie::getGenre() const {
    return _genre;
}

std::string Movie::getDescription() const {
    return _description;
}

FilmType Movie::getFilmType() const {
    return _film_type;
}

void Movie::setPhoto(std::string url) {
    _photo_url = url;
}

std::string Movie::getPhoto() {
    return _photo_url;
}

std::vector<int> Movie::getYears() const {
    return std::vector<int>({_year_start, _year_end});
}

bool Movie::IsAdult() const {
    return _is_adult;
}

double Movie::getRating() const {
    return _rating;
}

int Movie::getVotes() const {
    return _num_votes;
}

void Movie::updateRating(int new_vote, bool status, int user_rating) {
    if (status) {
        _rating = (_rating * _num_votes + new_vote) / (_num_votes + 1);
        _num_votes++;
    }
    else _rating = ((_rating * _num_votes) - user_rating + new_vote) / _num_votes;
}

bool compareActors(const QSharedPointer<Actor>& actor1, const QSharedPointer<Actor>& actor2) {
    return actor1->getId() == actor2->getId();
}

void Movie::loadActors() {
    std::string query = "SELECT nt.n_role, nt.character_played, nt.nconst, nt.actor_importance, '' as photo_url, n.name, "
                        "n.birth_year, n.death_year FROM names_titles nt JOIN names n on n.nconst = nt.nconst WHERE "
                        "nt.tconst = '" + _tconst + "'";

    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);

    if (_actors.empty()) {
        for (auto el: {"director", "actor", "producer"}) _actors[el] = {};
    }

    int counter = 0;
    for (auto el: buf) {
        auto actor = QSharedPointer<Actor>::create(el.at("name"), el.at("character_played"),
                                                   el.at("nconst"), el.at("photo_url"),
                                                   std::stoi(el.at("birth_year")), std::stoi(el.at("death_year")),
                                                   std::stoi(el.at("actor_importance")));

        if (std::find_if(_aboba.begin(), _aboba.end(), [&actor](const QSharedPointer<Actor>& a) {
            return compareActors(a, actor); }) == _aboba.end()) {
            _aboba.push_back(actor);
            std::cout << el.at("n_role");
            _actors[el.at("n_role")].push_back(actor);
            std::cout << _actors[el.at("n_role")].size()<< '\n';
            counter++;
        }
        else {
            actor.reset();
        }
    }

    Logger::getInstance().logInfo(std::to_string(counter) + " actors was added to " + _name + ".");
}

std::map<std::string, std::vector<QSharedPointer<Actor>>>& Movie::getActors() {
    return _actors;
}

// void Movie::addActor(const QSharedPointer<Actor>& actor) {
//     if (std::find_if(_actors.begin(), _actors.end(), [&actor](const QSharedPointer<Actor>& a) {
//         return compareActors(a, actor); }) == _actors.end()) {
//         _actors.push_back(actor);
//         Logger::getInstance().logInfo("Movie`s actor list was updated. Actor " + actor->getName() +
//                                       " was added to " + _name + ".");
//
//         actor->addToMovie(QSharedPointer<Movie>(this));
//     } else {
//         Logger::getInstance().logWarning("Actor " + actor->getName() + " was already added.");
//     }
// }
//
// void Movie::clearActors() {
//     for (auto& actor : _actors) {
//         actor.reset();
//     }
//     _actors.clear();
//
//     Logger::getInstance().logInfo("All actors were removed from " + _name + ".");
// }
//
// void Movie::removeActor(const QSharedPointer<Actor>& actor) {
//     auto it = std::find_if(_actors.begin(), _actors.end(), [&actor](const QSharedPointer<Actor>& a) {
//         return compareActors(a, actor); });
//     if (it != _actors.end()) {
//         _actors.erase(it);
//         Logger::getInstance().logInfo("Actor " + actor->getName() + " was removed from " + _name + ".");
//
//         actor->removeMovie(QSharedPointer<Movie>(this));
//     } else {
//         Logger::getInstance().logWarning("Movie " + _name + " doesn't have " + actor->getName() + " in the list of actors.");
//     }
// }

void Movie::loadComments() {
    std::string query = "SELECT * FROM comments WHERE tconst = '" + _tconst + "'";
    std::vector<std::map<std::string, std::string>> select = ExecuteSelectQuery("library", query);

    for (auto el: select) {
        _comments.emplace_back(el["user_id"],el["comment"]);
    }
}

const std::vector<std::pair<std::string, std::string>>& Movie::getComments() const {
    return _comments;
}

void Movie::leaveComment(const std::string& user_id, const std::string& com) {
    _comments.emplace_back(user_id, com);
}

// Definition of Collection class methods
Collection::Collection(int collection_id, const std::string& name, const std::string& photo_url)
        : _collection_id(collection_id), _name(name), _photo_url(photo_url) {
//    Logger::getInstance().logInfo("Collection class object was created (" + name + ").");
}

std::string Collection::getName() const {
    return _name;
}

void Collection::setName(const std::string& name) {
    _name = name;

    ExecuteUpdateQuery("library", "UPDATE user_collections SET collection_name = '" + _name + "' WHERE collection_id = '" + std::to_string(_collection_id) + "';");

    Logger::getInstance().logInfo("Collection " + std::to_string(_collection_id) + " changed name to " + _name + ".");
}
std::string Collection::getId() const {
    return std::to_string(_collection_id);
}
std::string Collection::getPhoto() const {
    return _photo_url;
}
void Collection::setPhoto(const std::string& url) {
    _photo_url = url;

    ExecuteUpdateQuery("library", "UPDATE user_collections SET image_url = '" + _photo_url + "' WHERE collection_id = '" + std::to_string(_collection_id) + "';");

    Logger::getInstance().logInfo("Collection " + std::to_string(_collection_id) + " changed photo to " + _photo_url + ".");
}

void Collection::loadMovies(const std::vector<std::string>& tconsts){
    for (const auto& el: tconsts){
        auto it = std::find_if(all_movies.begin(), all_movies.end(), [&el](const QSharedPointer<Movie>& m) {
            return m->getTconst() == el; });

        _collection.push_back(all_movies[it - all_movies.begin()]);
    }
}

const std::vector<QSharedPointer<Movie>>& Collection::getMovies() const {
    return _collection;
}

bool Collection::addMovie(const QSharedPointer<Movie>& movie) {
    if (std::find(_collection.begin(), _collection.end(), movie) == _collection.end()) {

        _collection.push_back(movie);
        std::vector<std::map<std::string, std::string>> data = {
                {{"collection_id", std::to_string(_collection_id)}, {"tconst", movie->getTconst()}},
        };
        if (ExecuteInsertQuery("library", "insert", "titles_collections", data)) {
            Logger::getInstance().logInfo(
                    "Movie " + movie->getTconst() + " was added to collection " + std::to_string(_collection_id) + '.');
            return true;
        }

        Logger::getInstance().logError(
                "Movie " + movie->getTconst() + " can`t be added to collection " + std::to_string(_collection_id) +
                '.');
        return false;
    }
    return false;
}

void Collection::removeMovie(const QSharedPointer<Movie>& movie) {
    auto it = std::find(_collection.begin(), _collection.end(), movie);
    if (it != _collection.end()) {
        _collection.erase(it);
        std::string query = "DELETE FROM titles_collections WHERE collection_id = '" + std::to_string(_collection_id) +
                            "' AND tconst = '" + movie->getTconst() + "';";
        if (ExecuteDeleteQuery("library", query)) {
            Logger::getInstance().logInfo("Movie " + movie->getName() + " was removed from collection " + _name);
        }
    } else {
        Logger::getInstance().logInfo("Movie " + movie->getName() + " isn't in the collection " + _name);
    }
}
