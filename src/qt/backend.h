#ifndef BACKEND_H
#define BACKEND_H


#include <iostream>
#include <vector>
#include <iomanip>
#include "../backend/libraries/Classes/movie/movies.h"
#include "../backend/libraries/Classes/users/user_and_dev.h"
#include "../backend/libraries/Classes/logger/logger.h"
#include "../backend/libraries/mysql-queries/mysql-queries.h"


static std::shared_ptr<AllUsers> main_user;
static std::vector<std::shared_ptr<Movie>> all_movies;

FilmType strToType(const std::string& type);

void loadMovies(std::vector<std::shared_ptr<Movie>> &all_movies);

std::vector<std::shared_ptr<Movie>> getMoviesSorted(int n, const std::string& genre="", FilmType filmType=FilmType::Default,
                                                    bool is_adult=true);

void getRecommendation(const std::vector<std::shared_ptr<Movie>>& AllMovies, std::vector<std::shared_ptr<Movie>>& buf,
    const std::vector<std::string>& tconsts);

bool compareMovies(const std::shared_ptr<Movie>& m1, const std::shared_ptr<Movie>& m2, const std::string& query);

void searchMovies(std::vector<std::shared_ptr<Movie>>& result, const std::string& query, int n);

bool SignInFun(const std::string& login, const std::string& password);

bool SignUpFun(const std::string& login, const std::string& password, int age);

void print_select_genres(std::vector<std::pair<std::string, std::vector<std::string>>> results);

void print_select(std::vector<std::map<std::string, std::string>> results);
bool SignIn(const std::string& login, const std::string& password);
std::vector<std::string> GetGenres(const std::string& library);



#endif
