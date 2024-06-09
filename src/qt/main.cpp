// #include "firstloginsignup.h"
// #include "pagemain.h"
// #include "userinfo.h"
#include "backend.h"
#include "signinwindow.h"
#include <QApplication>
#include "mysql-queries.h"
#include "mainwindow.h"



int main(int argc, char *argv[])
{
    initializePythonInterpreter("/Users/senya/CLionProjects/big_fix/src/qt/libraries/mysql-queries");

////     ---------------- TEST OF DATABASE------------------
////     ---------------- INITIALIZING VARIABLES -------------
    std::vector<std::map<std::string, std::string>> select;
    bool update;
    bool insert;
    bool del;
    std::string user_id;
    std::string tconst;
    std::vector<std::pair<std::string, std::vector<std::string>>> select_genres;
    std::string query;
    std::string collection_name;
    std::string collection_id;
    std::string comment;
    std::vector<std::map<std::string, std::string>> data;
//
////    ---------------- FUNCTIONS AND QUERIES -----------------
////    -------------- SELECT ALL GENRES IN SENYA'S FORMAT
//    query = "SELECT t.tconst, g.genre_name FROM titles t JOIN ratings r ON t.tconst = r.tconst JOIN titles_genres tg ON t.tconst = tg.tconst JOIN genres g ON tg.genre_id = g.genre_id WHERE t.description IS NOT NULL AND t.description != '' AND t.year_start > 1950 AND r.num_votes > 200 ORDER BY r.num_votes DESC;";
//    select_genres = ExecuteSelectGenresQuery("library", query);
//    print_select_genres(select_genres);
//
////    -------------- CHECK REC TABLE --------------
//    select = ExecuteSelectQuery("library", "SELECT COUNT(*) FROM cb_similarity;");
//    print_select(select);
//
////    --------------- GET ALL USERS ------------
//    query = "SELECT * FROM user_profile";
//    select = ExecuteSelectQuery("library", query);
//    print_select(select);
//
////    ----------- USER ОЦЕНИВАЕТ FILM --------------
//    tconst = "tt0096928";
//    user_id = "PavelPopov";
//    data = {
//     {{"user_id", user_id}, {"tconst", tconst}, {"rating", "10"}},
//     };
//    insert = ExecuteInsertQuery("library", "insert", "user_ratings", data);
//
////    ------------ GET ONE USER'S RATINGS ------------
//    user_id = "PavelPopov";
//    query = "SELECT * FROM user_ratings WHERE user_id = '" + user_id + "';";
//    select = ExecuteSelectQuery("library", query);
//    print_select(select);
//
////    ------------- CREATE NEW COLLECTION TO USER ----------------
//    user_id = "PavelPopov";
//    collection_name = "DUROV IS THE BEST";
//    data = {
//     {{"collection_name", collection_name}, {"user_id", user_id}},
//     };
//    insert = ExecuteInsertQuery("library", "insert", "user_collections", data);
//
////    ---------- USER ADDS FILM TO COLLECTION -----------
////    ----------ALREADY DONE, THIS PART IS JUST FIOR TESTING----------
//    user_id = "PavelPopov";
//    collection_name = "DUROV IS THE BEST";
//    query = "SELECT collection_id FROM user_collections WHERE user_id = '" + user_id + "' AND collection_name = '" + collection_name + "';";
//    select = ExecuteSelectQuery("library", query);
//
//    tconst = "tt0096928";
//    collection_id = select[0]["collection_id"];
//
//    data = {
//     {{"collection_id", collection_id}, {"tconst", "tt0103617"}}
//    };
//    insert = ExecuteInsertQuery("library", "insert", "titles_collections", data);
//    query = "SELECT * FROM titles_collections WHERE collection_id = " + collection_id + ";";
//    select = ExecuteSelectQuery("library", query);
//    print_select(select);
//
//// --------------------- USER WRITES A COMMENT ON FILM -----------------
//    tconst = "tt0096928";
//    user_id = "PavelPopov";
//    comment = "Really like this film!";
//
//    data = {
//        {{"user_id", user_id}, {"tconst", tconst}, {"comment", comment}}
//    };
//    insert = ExecuteInsertQuery("library", "insert", "comments", data);
//
////     ------------ GET COMMENTS ON FILM --------------
//    query = "SELECT * FROM comments WHERE tconst = '" + tconst + "'";
//    select = ExecuteSelectQuery("library", query);
//    print_select(select);
//
////     ------------------ INSERT LOG INTO LOG TABLE --------------
    select = ExecuteSelectQuery("library", "SELECT genre_name FROM genres ORDER BY genre_name ASC");
    print_select(select);
//    data = {
//        {{"timestamp", "2024-06-02 18:23:13"}, {"type", "[INFO]"}, {"message", "User abobus233 has logged in."}}
//    };
//    insert = ExecuteInsertQuery("library", "insert", "logging", data);
//    query = "SELECT * FROM logging";
//    select = ExecuteSelectQuery("library", query);
//    print_select(select);




//    ------------- QT PART -----------------

    loadMovies(all_movies);

    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();

}
