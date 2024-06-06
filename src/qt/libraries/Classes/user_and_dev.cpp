#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include "../movie/movies.h"
#include "user_and_dev.h"
#include "../logger/logger.h"
#include "../../mysql-queries/mysql-queries.h"


// Definition of AllUsers class methods
AllUsers::AllUsers(std::string name, std::string login, std::string password, int age, std::string photo):
        _name(std::move(name)), _login(std::move(login)), _password(std::move(password)), _age(age), _photo_url(std::move(photo)){
    Logger::getInstance().logInfo("User " + _login + " has logged in.");
}

std::string AllUsers::getName() const {
    return _name;
}

void AllUsers::setName(const std::string& name){
    _name = name;
    ExecuteUpdateQuery("library", "UPDATE user_profile SET name = '" + _name + "' WHERE user_id = '" + _login + "';");

    Logger::getInstance().logInfo("User " + _login + " changed name.");
}


std::string AllUsers::getLogin() const {
    return _login;
}

std::string AllUsers::getPassword() const {
    return _password;
}

void AllUsers::setPassword(const std::string& pass){
    _password = pass;
    ExecuteUpdateQuery("library", "UPDATE auth SET pass = '" + _password + "' WHERE user_id = '" + _login + "';");

    Logger::getInstance().logInfo("User " + _login + " changed password.");
}


int AllUsers::getAge() const{
    return _age;
}

void AllUsers::setAge(int age){
    _age = age;
    ExecuteUpdateQuery("library", "UPDATE user_profile SET age = '" + std::to_string(_age) + "' WHERE user_id = '" + _login + "';");

    Logger::getInstance().logInfo("User " + _login + " changed age.");
}


void AllUsers::setEmail(const std::string& email) {
    _email_address = email;
    ExecuteUpdateQuery("library", "UPDATE user_profile SET email = '" + _email_address + "' WHERE user_id = '" + _login + "';");

    Logger::getInstance().logInfo("User " + _login + " changed email.");
}

std::string AllUsers::getEmail() const {
    return _email_address;
}

void AllUsers::setPhoto(const std::string& photo) {
    _photo_url = photo;
    ExecuteUpdateQuery("library", "UPDATE user_profile SET photo_url = '" + _photo_url + "' WHERE user_id = '" + _login + "';");

    Logger::getInstance().logInfo("User " + _login + " changed photo.");
}

std::string AllUsers::getPhoto() const {
    return _photo_url;
}

Gender AllUsers::getGender() const {
    return _gender;
}

bool compareCol(const std::shared_ptr<Collection>& col1, const std::shared_ptr<Collection>& col2) {
    return col1.get() == col2.get();
}

void AllUsers::loadCol() {
    std::string query = "SELECT collection_id, collection_name FROM user_collections WHERE user_id = '" + this->getLogin() + "'";
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);

    int counter = 0;
    for (auto el: buf){
        auto col = std::make_shared<Collection>(std::stoi(el.at("collection_id")),
                                                el.at("collection_name"));

        if (std::find_if(_all_collection.begin(), _all_collection.end(), [&col](const std::shared_ptr<Collection>& c) {
            return compareCol(c, col); }) == _all_collection.end()) {
            _all_collection.push_back(col);
            counter++;
        }
    }

    Logger::getInstance().logInfo(std::to_string(counter) + " collections was loaded.");
}

const std::vector<std::shared_ptr<Collection>>& AllUsers::getAllCol() const{
    return _all_collection;
}

void AllUsers::clearCol() {
    for (auto& col : _all_collection) {
        col.reset();
    }
    _all_collection.clear();

    Logger::getInstance().logInfo("All movies were removed from " + _name + ".");
}

bool AllUsers::removeCol(const std::shared_ptr<Collection>& collection) {
    auto it = std::find(_all_collection.begin(), _all_collection.end(), collection);
    if (it != _all_collection.end()) {
        _all_collection.erase(it);
        Logger::getInstance().logInfo("Collection " + collection->getName() + " was removed from all collections, " + _name);
        return true;
    }

    Logger::getInstance().logInfo("Collection " + collection->getName() + " doesn't exist in the collections, " + _name);
    return false;
}

void AllUsers::createCol(const std::string& name) {
    for (const auto& el: _all_collection){
        if (el->getName() == name){
            std::cout << "Collection with that name already exists" << '\n';
            Logger::getInstance().logWarning("Can`t create collection with same name");
            return;
        }
    }
    int id = 0;
    auto newCollection = std::make_shared<Collection>(id, name);
    _all_collection.push_back(newCollection);
    Logger::getInstance().logInfo("Collection " + name + " was created by " + _name);
}

bool AllUsers::leaveComment(const std::shared_ptr<Movie>& movie, const std::string& com) {
    std::vector<std::map<std::string, std::string>> data = {
            {{"user_id", _login}, {"tconst", movie->getTconst()}, {"comment", com}}
    };
    if (ExecuteInsertQuery("library", "insert", "comments", data)) {
        movie->leaveComment(com);
        Logger::getInstance().logInfo(_login + " left a comment to " + movie->getName() + ".");
        return true;
    }

    Logger::getInstance().logInfo(_login + " couldn`t left a comment to " + movie->getName() + " for some error.");
    return false;
}

void AllUsers::makeVote(const std::shared_ptr<Movie>& movie, int vote){
    if (0 <= vote and vote <= 10){
        movie->updateRating(vote);
        Logger::getInstance().logInfo("Movie (" + movie->getName() + ") rating update.");
    }
    else Logger::getInstance().logError("New vote for " + movie->getName() + "isn`t from diapason of 0-10.");
}

User::User(std::string name, std::string login, std::string password, int age, std::string photo):
        AllUsers(std::move(name), std::move(login), std::move(password), age, std::move(photo)) {}

Developer::Developer(std::string name, std::string login, std::string password, int age, std::string photo):
        AllUsers(std::move(name), std::move(login), std::move(password), age, std::move(photo)) {}