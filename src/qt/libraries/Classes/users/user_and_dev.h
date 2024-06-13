#ifndef USER_AND_DEV_H
#define USER_AND_DEV_H
#pragma ide diagnostic ignored "modernize-use-nodiscard"

#include <string>
#include <vector>
#include <memory>
#include <QSharedPointer>
#include "genders.h"
#include "../movie/movies.h"

enum class userAccess
{
    User,
    Developer
};


class AllUsers {
private:
    std::string _name;
    std::string _login;
    std::string _password;
    int _age;
    std::string _email_address = "";
    std::string _photo_url = "";
    Gender _gender = Gender::Undefined;

public:
    AllUsers(std::string name, std::string login, std::string password, int age, std::string photo, const std::string& gender);

    std::string getName() const;
    void setName(const std::string& name);
    std::string getLogin() const;
    std::string getPassword() const;
    void setPassword(const std::string& pass);
    int getAge() const;
    void setAge(int age);
    void setEmail(const std::string& email);
    std::string getEmail() const;
    void setPhoto(const std::string& photo);
    std::string getPhoto() const;
    Gender getGender() const;
    void setGender(const std::string& gender);

private:
    std::vector<QSharedPointer<Collection>> _all_collection;

public:
    void loadCol();
    void clearCol();
    bool addToCol(const std::string& col, const QSharedPointer<Movie>& movie);
    const std::vector<QSharedPointer<Collection>>& getAllCol() const;
    bool createCol(const std::string& name, const std::string& photo_url);
    bool removeCol(const QSharedPointer<Collection>& collection);

    bool leaveComment(const QSharedPointer<Movie>& movie, const std::string& com);
    void makeVote(const QSharedPointer<Movie>& movie, int vote, bool status=true, int user_rating=0);
};

#endif