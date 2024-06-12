#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include "../movie/movies.h"
#include "user_and_dev.h"

#include "../../../backend.h"
#include "../logger/logger.h"
#include "../../mysql-queries/mysql-queries.h"


AllUsers::AllUsers(std::string name, std::string login, std::string password, int age, std::string photo, const std::string& gender)
    : _name(std::move(name)), _login(std::move(login)), _password(std::move(password)), _age(age), _photo_url(std::move(photo)) {

    _gender = strToGender(gender);

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

void AllUsers::setGender(const std::string& gender) {
    _gender = strToGender(gender);

    ExecuteUpdateQuery("library", "UPDATE user_profile SET gender = '" + genderToString(_gender) + "' WHERE user_id = '" + _login + "';");

    Logger::getInstance().logInfo("User " + _login + " changed gender to " + genderToString(_gender) + ".");
}


bool compareCol(const QSharedPointer<Collection>& col1, const QSharedPointer<Collection>& col2) {
    return col1->getId() == col2->getId();
}

void AllUsers::loadCol() {
    std::string query = "SELECT collection_id, collection_name, image_url FROM user_collections WHERE user_id = '" + this->getLogin() + "'";
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);

    int counter = 0;
    for (auto el: buf){
        auto col = QSharedPointer<Collection>::create(std::stoi(el.at("collection_id")),
                                                el.at("collection_name"), el.at("image_url"));

        if (std::find_if(_all_collection.begin(), _all_collection.end(), [&col](const QSharedPointer<Collection>& c) {
            return compareCol(c, col); }) == _all_collection.end()) {
            _all_collection.push_back(col);
            counter++;
        }
    }

    Logger::getInstance().logInfo(std::to_string(counter) + " collections was loaded.");
}

const std::vector<QSharedPointer<Collection>>& AllUsers::getAllCol() const{
    return _all_collection;
}

void AllUsers::clearCol() {
    for (auto& col : _all_collection) {
        col.reset();
    }
    _all_collection.clear();

    Logger::getInstance().logInfo("All movies were removed from " + _name + ".");
}

bool AllUsers::removeCol(const QSharedPointer<Collection>& collection) {
    auto it = std::find(_all_collection.begin(), _all_collection.end(), collection);
    if (it != _all_collection.end()) {
        _all_collection.erase(it);
        Logger::getInstance().logInfo("Collection " + collection->getName() + " was removed from all collections, " + _name);
        return true;
    }

    Logger::getInstance().logInfo("Collection " + collection->getName() + " doesn't exist in the collections, " + _name);
    return false;
}

bool AllUsers::createCol(const std::string& name, const std::string& photo_url) {
    // if (_all_collection.size() >= 6) return false;

    for (const auto& el: _all_collection){
        if (el->getName() == name){
            std::cout << "Collection with that name already exists" << '\n';
            Logger::getInstance().logWarning("Can`t create collection with same name");
            return false;
        }
    }

    std::string query = "SELECT MAX(collection_id) as max FROM user_collections";
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", query);

    auto newCollection = QSharedPointer<Collection>::create(std::stoi(buf[0].at("max")) + 1, name);
    newCollection->setPhoto(photo_url);
    _all_collection.push_back(newCollection);

    std::vector<std::map<std::string, std::string>> data = {
        {{"collection_name", newCollection->getName()}, {"user_id", _login},
            {"image_url", newCollection->getPhoto()}}};

    ExecuteInsertQuery("library", "insert", "user_collections", data);
    Logger::getInstance().logInfo("Collection " + newCollection->getName() + " was created by " + _name);
    return true;
}

bool AllUsers::leaveComment(const QSharedPointer<Movie>& movie, const std::string& com) {
    std::vector<std::map<std::string, std::string>> data = {
            {{"user_id", _login}, {"tconst", movie->getTconst()}, {"comment", com}}
    };
    if (ExecuteInsertQuery("library", "insert", "comments", data)) {
        movie->leaveComment(_login, com);
        Logger::getInstance().logInfo(_login + " left a comment to " + movie->getName() + ".");
        return true;
    }

    Logger::getInstance().logInfo(_login + " couldn`t left a comment to " + movie->getName() + " for some error.");
    return false;
}

void AllUsers::makeVote(const QSharedPointer<Movie>& movie, int vote){
    if (0 <= vote and vote <= 10){
        movie->updateRating(vote);
        Logger::getInstance().logInfo("Movie (" + movie->getName() + ") rating update.");
    }
    else Logger::getInstance().logError("New vote for " + movie->getName() + "isn`t from diapason of 0-10.");
}