#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "backend.h"
#include "QString"

std::vector<QSharedPointer<Movie>> all_movies;
std::string MY_PATH = "/Users/senya/CLionProjects/aoaoaoaooa/src/qt";
QSharedPointer<AllUsers> main_user;

QString qFilePath(const std::string& path) {
    return QString::fromStdString(MY_PATH + path);
}


FilmType strToType(const std::string& type){
    if (type == "Movie") return FilmType::Movie;
    else if (type == "TvMovie") return FilmType::Movie;
    else if (type == "TvSeries") return FilmType::TvSeries;

    return FilmType::Default;
}

void loadMovies() {
    // Load all genres in map {tconst: [genre1, genre2, ...]}
    std::map<std::string, std::vector<std::string>> genres;
    std::string query = "SELECT top_movies.tconst, g.genre_name FROM (SELECT t.tconst FROM titles t JOIN ratings r ON "
                        "t.tconst = r.tconst WHERE t.description IS NOT NULL AND t.description != '' AND t.year_start > "
                        "1950 AND r.num_votes > 200 ORDER BY r.num_votes DESC LIMIT 10000) AS top_movies JOIN "
                        "titles_genres tg ON top_movies.tconst = tg.tconst JOIN genres g ON tg.genre_id = g.genre_id "
                        "ORDER BY (SELECT r.num_votes FROM ratings r WHERE r.tconst = top_movies.tconst) DESC;";
    genres = ExecuteSelectGenresQuery("library", query);

    // Load all movies in vector of maps [{title_name: name, ...}, {title_name: name, ...}]
    query = "SELECT t.title_name, t.tconst, t.description, t.title_type, t.year_start, t.year_end, t.is_adult, r.rating, "
            "r.num_votes, tl.image_url FROM titles t JOIN ratings r ON t.tconst = r.tconst JOIN titles_image tl on "
            "t.tconst = tl.tconst WHERE t.description IS NOT NULL AND t.description != '' AND t.year_start > 1950 AND "
            "r.num_votes > 200 ORDER BY r.num_votes DESC LIMIT 10000;";
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);

    int counter = 0;
    for (auto el: buf){
        auto movie = QSharedPointer<Movie>::create(el["title_name"], el["tconst"],  el["description"],
                                                   el["image_url"], strToType(el["title_type"]), std::stoi(el["year_start"]),
                                                   std::stoi(el["year_end"]), std::stoi(el["is_adult"]),
                                                   std::stod(el["rating"]), std::stoi(el["num_votes"]));
        all_movies.push_back(movie);
        movie->setGenre(genres[el["tconst"]]);
        counter++;
    }

    Logger::getInstance().logInfo(std::to_string(counter) + " movies was uploaded.");
}

bool isLiked(){
    std::string query = "SELECT COUNT(*) FROM user_ratings WHERE user_id = '" + main_user->getLogin() + "'";
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);
    return std::stoi(buf[0]["COUNT(*)"]);
}

std::vector<QSharedPointer<Movie>> getMoviesSorted(int n, const std::string& genre, const FilmType filmType,
                                                   const bool is_adult) {

    std::vector<QSharedPointer<Movie>> genreMovies;

    if (!genre.empty()) {
        for (const auto &movie: all_movies) {
            if (std::find(movie->getGenre().begin(), movie->getGenre().end(), genre) != movie->getGenre().end()) {
                genreMovies.push_back(movie);
            }
        }
    }
    else if (filmType != FilmType::Default){
        for (const auto &movie: all_movies) {
            if (movie->getFilmType() == filmType) {
                genreMovies.push_back(movie);
            }
        }
    }
    else if (!is_adult){
        for (const auto &movie: all_movies) {
            if (movie->IsAdult() == 0) {
                genreMovies.push_back(movie);
            }
        }
    }
    else {
        genreMovies = all_movies;
    }

    std::sort(genreMovies.begin(), genreMovies.end(),[](const QSharedPointer<Movie>& a, const QSharedPointer<Movie>& b) {
        return a->getVotes() > b->getVotes(); });

    std::cout << genreMovies.size() << '\n';
    if (genreMovies.size() <= n) return genreMovies;
    return {genreMovies.begin(), genreMovies.begin() + n};
}

bool compareMovies(const QSharedPointer<Movie>& m1, const QSharedPointer<Movie>& m2, const std::string& query) {
    size_t pos1 = m1->getName().find(query);
    size_t pos2 = m2->getName().find(query);

    if (pos1 != pos2) return pos1 < pos2;
    return m1->getName().length() < m2->getName().length();
}

void searchMovies(std::vector<QSharedPointer<Movie>>& result, const std::string& query, int n, const std::string& genre) {
    for (const auto& movie : all_movies) {
        if (genre.empty() or std::find(movie->getGenre().begin(), movie->getGenre().end(), genre) != movie->getGenre().end()) {
            if (movie->getName().find(query) != std::string::npos) {
                result.push_back(movie);
            }
        }
    }

    std::sort(result.begin(), result.end(), [&](const QSharedPointer<Movie>& m1, const QSharedPointer<Movie>& m2) {
        return compareMovies(m1, m2, query); });

    if (n < result.size()) {
        result.erase(result.begin() + n, result.end());
    }
}

bool SignIn(const std::string &login, const std::string &password) {
    std::string query = "SELECT a.user_id, a.pass, u.name, u.age, u.photo_url, u.gender FROM auth a JOIN user_profile u ON u.user_id = a.user_id;";
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);
    for (const auto& el : buf) {
        if (login == el.at("user_id") && password == el.at("pass")) {
            std::cout << el.at("name") << '\n';
            main_user = QSharedPointer<AllUsers>::create(el.at("name"), login, password, std::stoi(el.at("age")),
                el.at("photo_url"), el.at("gender"));
            main_user->loadCol();
            Logger::getInstance().logError("User " + login + " signed in.");
            return true;
        }
    }
    Logger::getInstance().logError("Wrong login or password");
    buf.clear();
    return false;
}

bool SignUp(const std::string& login, const std::string& password, const std::string& name, int age, const std::string& email, const std::string& gender){
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", "SELECT * FROM auth;");

    if (std::find_if(buf.begin(), buf.end(), [&](const auto& c) {
        return login == c.at("user_id"); }) == buf.end()) {

        std::vector<std::map<std::string, std::string>> data = {
                {{"user_id", login}, {"name", name,}, {"age", std::to_string(age)}, {"photo_url", ""},
                    {"email", email}, {"gender", gender}}};

        std::vector<std::map<std::string, std::string>> data2 = {
                {{"user_id", login}, {"pass", password,}}};

        if (ExecuteInsertQuery("library", "insert", "user_profile", data) and
            ExecuteInsertQuery("library", "insert", "auth", data2)){

            Logger::getInstance().logInfo("User " + login + " signed up.");
            main_user = QSharedPointer<AllUsers>::create(name, login, password, age, "", gender);
            main_user->setEmail(email);
            return true;
        }
    }

    Logger::getInstance().logError("User with this login already exists.");
    buf.clear();
    return false;
}

void print_select_genres(std::vector<std::pair<std::string, std::vector<std::string>>> results) {
    std::cout << "IM OK";
    for (const auto& result : results) {
        std::cout << yellow_color_code << "TConst: " << result.first << "\nGenres: ";
        for (const auto& genre : result.second) {
            std::cout << genre << " ";
        }
        std::cout << "\n";
    }
    std::cout << reset_color_code;
}

void print_select(std::vector<std::map<std::string, std::string>> results) {
    for (const auto& row : results) {
        for (const auto& [key, value] : row) {
            std::cout << yellow_color_code << key << ": " << value << "\t" << reset_color_code;
        }
        std::cout << '\n';
    }
}

bool aboba(const QSharedPointer<Movie>& m, const std::string& query) {
    return m->getTconst() == query;
}

void getRecommendation(std::vector<QSharedPointer<Movie>>& buf,
    const std::vector<std::string>& tconsts) {

    for (auto el: tconsts) {
        auto it = std::find_if(all_movies.begin(), all_movies.end(), [&](const auto& movie) {
        return aboba(movie, el); });
        if (it != all_movies.end()) {
            buf.push_back(all_movies[it - all_movies.begin()]);
        }
    }
}

std::vector<std::string> GetGenres(const std::string& library) {
    std::vector<std::string> genres;
    std::string query = "SELECT DISTINCT genre_name FROM genres WHERE genre_name != 'Adult' ORDER BY genre_name;";
    std::vector<std::map<std::string, std::string>> results = ExecuteSelectQuery(library, query);

    for (const auto& row : results) {
        genres.push_back(row.at("genre_name"));
    }

    return genres;
}