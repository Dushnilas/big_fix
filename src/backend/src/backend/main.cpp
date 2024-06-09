#include <iostream>
#include <vector>
#include <iomanip>
#include "movie/movies.h"
#include "users/user_and_dev.h"
#include "logger/logger.h"
#include "../../libraries/mysql-queries/mysql-queries.h"


static std::shared_ptr<AllUsers> main_user;

FilmType strToType(const std::string& type){
    if (type == "Movie") return FilmType::Movie;
    else if (type == "TvMovie") return FilmType::Movie;
    else if (type == "TvSeries") return FilmType::TvSeries;

    return FilmType::Default;
}


void loadMovies(std::vector<std::shared_ptr<Movie>> &allMovies) {
    std::string query = "SELECT t.title_name, t.tconst, t.description, t.title_type, t.year_start, t.year_end, "
                        "t.is_adult, r.rating, r.num_votes FROM titles t JOIN ratings r ON t.tconst = r.tconst WHERE "
                        "t.description IS NOT NULL AND t.description != '' AND t.year_start > 1950 AND r.num_votes > 200 ORDER BY r.num_votes DESC LIMIT 100;";
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);

    int counter = 0;
    for (auto el: buf){
        auto movie = std::make_shared<Movie>(el["title_name"], el["tconst"],  el["description"],
                                             strToType(el["title_type"]), std::stoi(el["year_start"]), std::stoi(el["year_end"]),
                                             std::stoi(el["is_adult"]), std::stod(el["rating"]), std::stoi(el["num_votes"]));
        allMovies.push_back(movie);
        counter++;
    }

    Logger::getInstance().logInfo(std::to_string(counter) + "movies was uploaded.");
}

std::vector<std::shared_ptr<Movie>> getMoviesSorted(const std::vector<std::shared_ptr<Movie>>& allMovies,
                                                    int n, const std::string& genre="", const FilmType filmType=FilmType::Default,
                                                    const bool is_adult=true) {

    std::vector<std::shared_ptr<Movie>> genreMovies;

    if (!genre.empty()) {
        for (const auto &movie: allMovies) {
            if (std::find(movie->getGenre().begin(), movie->getGenre().end(), genre) != movie->getGenre().end()) {
                genreMovies.push_back(movie);
            }
        }
    }
    else if (filmType != FilmType::Default){
        for (const auto &movie: allMovies) {
            if (movie->getFilmType() == filmType) {
                genreMovies.push_back(movie);
            }
        }
    }
    else if (!is_adult){
        for (const auto &movie: allMovies) {
            if (movie->IsAdult() == 0) {
                genreMovies.push_back(movie);
            }
        }
    }

    std::sort(genreMovies.begin(), genreMovies.end(),[](const std::shared_ptr<Movie>& a, const std::shared_ptr<Movie>& b) {
                  return a->getRating() > b->getRating(); });

    if (genreMovies.size() <= n) return genreMovies;
    return {genreMovies.begin(), genreMovies.begin() + n};
}

void getRecommendation();

bool compareMovies(const std::shared_ptr<Movie>& m1, const std::shared_ptr<Movie>& m2, const std::string& query) {
    size_t pos1 = m1->getName().find(query);
    size_t pos2 = m2->getName().find(query);

    if (pos1 != pos2) return pos1 < pos2;
    return m1->getName().length() < m2->getName().length();
}

void searchMovies(const std::vector<std::shared_ptr<Movie>>& all_movies, std::vector<std::shared_ptr<Movie>>& result,
                  const std::string& query, int n) {

    for (const auto& movie : all_movies) {
        if (movie->getName().find(query) != std::string::npos) {
            result.push_back(movie);
        }
    }

    std::sort(result.begin(), result.end(), [&](const std::shared_ptr<Movie>& m1, const std::shared_ptr<Movie>& m2) {
        return compareMovies(m1, m2, query); });

    if (n < result.size()) {
        result.erase(result.begin() + n, result.end());
    }
}

bool SignIn(const std::string& login, const std::string& password){
    std::string query = "SELECT a.user_id, a.pass, u.name, u.age, u.photo_url FROM auth a JOIN user_profile u ON u.user_id = a.user_id;";
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);
    for (const auto& el: buf){
        if (login == el.at("user_id") and password == el.at("pass")){
            main_user = std::make_shared<User>(el.at("name"), login, password, std::stoi(el.at("age")), el.at("photo_url"));
            main_user->loadCol();
            Logger::getInstance().logError("User " + login + " signed in.");
            return true;
        }
    }
    Logger::getInstance().logError("Wrong login or password");
    buf.clear();
    return false;
}

bool SignUp(const std::string& login, const std::string& password){
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", "SELECT * FROM auth;");;

    if (std::find_if(buf.begin(), buf.end(), [&](const auto& c) {
        return login == c.at("user_id"); }) == buf.end()) {

        std::vector<std::map<std::string, std::string>> data = {
                {{"user_id", login}, {"name", login,}, {"age", "0"}, {"photo_url", ""}}};

        std::vector<std::map<std::string, std::string>> data2 = {
                {{"user_id", login}, {"pass", password,}}};

        if (ExecuteInsertQuery("library", "insert", "user_profile", data) and
            ExecuteInsertQuery("library", "insert", "auth", data2)){

            Logger::getInstance().logInfo("User " + login + " signed up.");
            main_user = std::make_shared<User>(login, login, password, 0, "");
            main_user->loadCol();
            return true;
        }
    }

    Logger::getInstance().logError("User with this login already exists.");
    buf.clear();
    return false;
}

int main() {
    initializePythonInterpreter(PROJECT_PATH);
    Logger::getInstance().setLogFile("/Users/senya/CLionProjects/recsys_project/src/Data/LogFile.txt");
    std::vector<std::shared_ptr<Movie>> all_movies;
    loadMovies(all_movies);
    std::cout << all_movies.size() << '\n';

    std::string sign = "";
    while (sign != "in" and sign != "up"){
        std::cout << "Do you want to sign in or sing up (write in or up): ";
        std::cin >> sign;
    }

    std::string login, password;
    std::cout << std::left << std::setw(10) << "Login: ";
    std::cin >> login;
    std::cout << std::left << std::setw(10) << "Password: ";
    std::cin >> password;

    if (sign == "in"){
        while (!SignIn(login, password)){
            std::cout << "Wrong login or password. Try again." << '\n';
            std::cout << std::left << std::setw(10) << "Login: ";
            std::cin >> login;
            std::cout << std::left << std::setw(10) << "Password: ";
            std::cin >> password;
        }

        std::cout << main_user->getName() << " welcome to Maze online cinema"  << '\n';
    }
    else if (sign == "up"){
        while (!SignUp(login, password)){
            std::cout << "User with this login already exists. Try again." << '\n';
            std::cout << std::left << std::setw(10) << "Login: ";
            std::cin >> login;
            std::cout << std::left << std::setw(10) << "Password: ";
            std::cin >> password;
        }

        std::cout << main_user->getName() << " welcome to Maze online cinema" << '\n';
    }

    std::string name;
    std::cout << "Current name: " << main_user->getName() << '\n'
                << "New name: ";
    std::cin >> name;
    main_user->setName(name);

    std::string pass;
    std::cout << "Current password: " << main_user->getPassword() << '\n'
              << "New password: ";
    std::cin >> pass;
    main_user->setPassword(pass);

    int age;
    std::cout << "Current age: " << main_user->getAge() << '\n'
              << "New age: ";
    std::cin >> age;
    main_user->setAge(age);

    std::string email;
    std::cout << "Current email: " << main_user->getEmail() << '\n'
              << "New email: ";
    std::cin >> email;
    main_user->setEmail(email);

    std::string photo;
    std::cout << "Current photo url: " << main_user->getPhoto() << '\n'
              << "New photo: ";
    std::cin >> photo;
    main_user->setPhoto(photo);

//    int action = 1;
//    while (action != 0){
//        std::cout << "Choose one of options:" << '\n'
//                    << "1. Search" << '\n'
//                    << "2. Add collection" << '\n'
//                    << "3. Open collections" << '\n'
//                    << "4. Open movies in collection" << '\n'
//                    << "5. Add movie to collection" << '\n'
//                    << "6. Remove movie from collection" << '\n'
//                    << "7. Add movie to collection" << '\n'
//                    << "0. Exit" << '\n'
//                    << "Your action: ";
//        std::cin >> action;
//
//        switch (action) {
//            case 1: {
//                std::string movie_name;
//                std::vector<std::shared_ptr<Movie>> search_mode;
//
//                std::cout << "Search: ";
//                std::cin >> movie_name;
//                searchMovies(all_movies, search_mode, movie_name, 10);
//
//                std::cout << "Search result for " << movie_name << '\n';
//                for (int i = 1; i < search_mode.size() + 1; i++) {
//                    std::cout << i << ". " << search_mode[i]->getName() << '\n';
//                }
//                std::cout
//                        << "If you want to see more information about the film type movie_number. If you want to leave type 0."
//                        << '\n';
//
//                int search_action;
//                std::cout << "Action:";
//                std::cin >> search_action;
//
//                if (search_action != 0) {
//                    auto movie = search_mode[search_action - 1];
//                    std::cout << "Name: " << movie->getName() << '\n'
//                              << "Genres: ";
//                    std::copy(movie->getGenre().begin(), movie->getGenre().end(),
//                              std::ostream_iterator<std::string>(std::cout, ", "));
//                    std::cout << '\n' << "Description: " << movie->getDescription() << '\n'
//                              << "Year start: " << movie->getYears()[0] << '\n'
//                              << "Rating: " << movie->getRating() << '\n';
//
//                    std::cout << "Choose one of options:" << '\n'
//                              << "1. Make a vote" << '\n'
//                              << "2. Check comments" << '\n'
//                              << "3. Leave comment" << '\n'
//                              << "0. Exit" << '\n';
//
//                    int film_action;
//                    std::cout << "Action: ";
//                    std::cin >> film_action;
//
//                    switch (film_action) {
//                        case 1: {
//                            double rating;
//                            std::cout << "Type your rating 0-10: ";
//                            std::cin >> rating;
//                            movie->updateRating(rating);
//                            break;
//                        }
//                        case 2: {
//                            std::copy(movie->getComments().begin(), movie->getComments().end(),
//                                      std::ostream_iterator<std::string>(std::cout, "\n"));
//                            break;
//                        }
//                        case 3: {
//                            std::string new_comment;
//                            std::cout << "Type your comment" << '\n' << "Comment: ";
//                            std::cin >> new_comment;
//                            main_user->leaveComment(movie, new_comment);
//                            break;
//                        }
//                        default:
//                            break;
//                    }
//                }
//
//                break;
//            }
//            case 2: {
//                std::string collection_name;
//                std::cout << "New collection name: ";
//                std::cin >> collection_name;
//
//                main_user->createCol(collection_name);
//
//                break;
//            }
//            case 3:{
//                std::cout << 1;
//                for (const auto& el: main_user->getAllCol()){
//                    std::cout << el->getName() << '\n';
//                }
//            }
//        }
//    }

//    auto movie = std::make_shared<Movie>("Aboba", "23456", "Western", "Lupa zabolel i za Lupu na rabotu vishel Pupa",
//                FilmType::TvSeries, 1909, 2024, true, 8.1, 7845);
//
//    auto actor = std::make_shared<Actor>("Pupa", "2344567", "porthub", 2000, 0, 1);
//
//    movie->addActor(actor);
//
//    User Aleko("Aleko", "Messi", "abobus233", "123456");
//
//    Aleko.createCol("Plusi Dla loxov");
//
//    auto all_collections = Aleko.getAllCol();
//
//    for (const auto& col : all_collections){
//        if (col->getName() == "Plusi Dla loxov"){
//            col->addMovie(movie);
//            auto all_movies = col->getMovies();
//
//            for (const auto& mov : all_movies){
//                std::cout << mov->getName() << '\n';
//            }
//        }
//    }

    finalizePythonInterpreter();
    return 0;
}
