#ifndef BACKEND_H
#define BACKEND_H

#include <QSharedPointer>
#include <iostream>
#include <vector>
#include <iomanip>
#include "../backend/libraries/Classes/movie/movies.h"
#include "../backend/libraries/Classes/users/user_and_dev.h"
#include "../backend/libraries/Classes/logger/logger.h"
#include "../backend/libraries/mysql-queries/mysql-queries.h"

static QSharedPointer<AllUsers> main_user;
static std::vector<QSharedPointer<Movie>> all_movies;

FilmType strToType(const std::string& type);

void loadMovies(std::vector<QSharedPointer<Movie>> &all_movies);

std::vector<QSharedPointer<Movie>> getMoviesSorted(std::vector<QSharedPointer<Movie>> &mmovies_all, int n, const std::string& genre="", FilmType filmType=FilmType::Default,
                                                   bool is_adult=true);

bool isLiked();

void getRecommendation(const std::vector<QSharedPointer<Movie>>& AllMovies, std::vector<QSharedPointer<Movie>>& buf,
                       const std::vector<std::string>& tconsts);

bool compareMovies(const QSharedPointer<Movie>& m1, const QSharedPointer<Movie>& m2, const std::string& query);

void searchMovies(std::vector<QSharedPointer<Movie>>& result, const std::string& query, int n);

bool SignInFun(const std::string& login, const std::string& password);

bool SignUpFun(const std::string& login, const std::string& password, int age);

void print_select_genres(std::vector<std::pair<std::string, std::vector<std::string>>> results);

void print_select(std::vector<std::map<std::string, std::string>> results);

bool SignIn(const std::string& login, const std::string& password);

std::vector<std::string> GetGenres(const std::string& library);

#endif
