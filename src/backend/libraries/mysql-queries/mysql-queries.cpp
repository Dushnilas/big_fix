#include "mysql-queries.h"
#include <Python.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>

const std::string red_color_code = "\033[1;31m";
const std::string reset_color_code = "\033[0m";
const std::string yellow_color_code = "\033[1;33m";

bool initializePythonInterpreter(const std::string& fixed_path) {
    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("print('Current working directory:', os.getcwd())");

    std::string library_path_cmd = "sys.path.append('" + std::string(fixed_path) + "')";

    std::string venv_path = std::string(fixed_path) + "/mysqlenv/lib/python3.12/site-packages";

    std::string venv_path_cmd = "sys.path.append('" + std::string(venv_path) + "')";

    PyRun_SimpleString(library_path_cmd.c_str());
    PyRun_SimpleString(venv_path_cmd.c_str());

    PyRun_SimpleString("print('sys.path:', sys.path[::-1])");

    return true;
}


void finalizePythonInterpreter() {
    Py_Finalize();
}

std::vector<std::map<std::string, std::string>> ExecuteSelectQuery(const std::string& library, const std::string& query) {
    std::string function_name = "select";
    std::vector<std::map<std::string, std::string>> results;

    PyObject *pName = PyUnicode_DecodeFSDefault(library.c_str());
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, function_name.c_str());

        if (PyCallable_Check(pFunc)) {
            PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString(query.c_str()));

            PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != nullptr) {
                PyObject *iterator = PyObject_GetIter(pValue);
                if (iterator == nullptr) {
                    PyErr_Print();
                    std::cout << red_color_code << "Failed to get iterator" << reset_color_code << std::endl;
                    Py_DECREF(pValue);
                    Py_DECREF(pModule);
                    return results;
                }

                PyObject *item;
                while ((item = PyIter_Next(iterator)) != NULL) {
                    PyObject* row_iterator = PyObject_GetIter(item);
                    if (row_iterator == nullptr) {
                        PyErr_Print();
                        std::cout << red_color_code << "Failed to get row iterator" << reset_color_code << std::endl;
                        Py_DECREF(item);
                        Py_DECREF(iterator);
                        Py_DECREF(pValue);
                        Py_DECREF(pModule);
                        return results;
                    }

                    PyObject* column_item;
                    std::map<std::string, std::string> row;
                    while ((column_item = PyIter_Next(row_iterator)) != NULL) {
                        PyObject *key = PyTuple_GetItem(column_item, 0);
                        PyObject *value = PyTuple_GetItem(column_item, 1);
                        if (key == nullptr || value == nullptr) {
                            PyErr_Print();
                            std::cout << red_color_code << "Failed to get key or value from column_item" << reset_color_code << std::endl;
                            Py_DECREF(column_item);
                            Py_DECREF(row_iterator);
                            Py_DECREF(item);
                            Py_DECREF(iterator);
                            Py_DECREF(pValue);
                            Py_DECREF(pModule);
                            return results;
                        }
                        row[PyUnicode_AsUTF8(key)] = PyUnicode_AsUTF8(value);
                        Py_DECREF(column_item);
                    }
                    Py_DECREF(row_iterator);
                    results.push_back(row);
                    Py_DECREF(item);
                }
                Py_DECREF(iterator);
                Py_DECREF(pValue);
            } else {
                PyErr_Print();
                std::cout << red_color_code << "Call to " << function_name << " failed" << reset_color_code << std::endl;
            }
        } else {
            PyErr_Print();
            std::cout << red_color_code << "Cannot find function '" << function_name << "'" << reset_color_code << std::endl;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cout << red_color_code << "Failed to load '" << library << "'" << reset_color_code << std::endl;
    }
    return results;
}

bool ExecuteInsertQuery(const std::string& library, const std::string& function_name, const std::string& table_name, const std::vector<std::map<std::string, std::string>>& data) {
    bool success = false;

    PyObject *pName = PyUnicode_DecodeFSDefault(library.c_str());
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, function_name.c_str());

        if (PyCallable_Check(pFunc)) {
            PyObject *pDataList = PyList_New(data.size());
            for (size_t i = 0; i < data.size(); ++i) {
                PyObject *pRowList = PyList_New(data[i].size());
                size_t j = 0;
                for (const auto& pair : data[i]) {
                    PyObject *pTuple = PyTuple_Pack(2, PyUnicode_FromString(pair.first.c_str()), PyUnicode_FromString(pair.second.c_str()));
                    PyList_SetItem(pRowList, j, pTuple);
                    ++j;
                }
                PyList_SetItem(pDataList, i, pRowList);
            }

            PyObject *pArgs = PyTuple_Pack(2, PyUnicode_FromString(table_name.c_str()), pDataList);

            PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            Py_DECREF(pDataList);

            if (pValue != nullptr) {
                success = true;
                Py_DECREF(pValue);
            } else {
                PyErr_Print();
                std::cout << red_color_code << "Call to " << function_name << " failed" << reset_color_code << std::endl;
            }
        } else {
            PyErr_Print();
            std::cout << red_color_code << "Cannot find function '" << function_name << "'" << reset_color_code << std::endl;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cout << red_color_code << "Failed to load '" << library << "'" << reset_color_code << std::endl;
    }

    return success;
}

bool ExecuteUpdateQuery(const std::string& library, const std::string& update_query) {
    std::string function_name = "update";
    bool success = false;

    PyObject *pName = PyUnicode_DecodeFSDefault(library.c_str());
    if (!pName) {
        std::cout << red_color_code << "Failed to decode library name" << reset_color_code << std::endl;
        return success;
    }

    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, function_name.c_str());
        if (!pFunc || !PyCallable_Check(pFunc)) {
            if (PyErr_Occurred()) PyErr_Print();
            std::cout << red_color_code << "Cannot find function '" << function_name << "'" << reset_color_code << std::endl;
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            return success;
        }

        PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString(update_query.c_str()));
        if (!pArgs) {
            std::cout << red_color_code << "Failed to pack arguments" << reset_color_code << std::endl;
            Py_DECREF(pFunc);
            Py_DECREF(pModule);
            return success;
        }

        PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);

        if (pValue != nullptr) {
            success = true;
            Py_DECREF(pValue);
        } else {
            PyErr_Print();
            std::cout << red_color_code << "Call to " << function_name << " failed" << reset_color_code << std::endl;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cout << red_color_code << "Failed to load '" << library << "'" << reset_color_code << std::endl;
    }

    return success;
}

bool ExecuteDeleteQuery(const std::string& library, const std::string& delete_query) {
    std::string function_name = "delete";
    bool success = false;

    PyObject *pName = PyUnicode_DecodeFSDefault(library.c_str());
    if (!pName) {
        std::cout << red_color_code << "Failed to decode library name" << reset_color_code << std::endl;
        return success;
    }

    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, function_name.c_str());
        if (!pFunc || !PyCallable_Check(pFunc)) {
            if (PyErr_Occurred()) PyErr_Print();
            std::cout << red_color_code << "Cannot find function '" << function_name << "'" << reset_color_code << std::endl;
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            return success;
        }

        PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString(delete_query.c_str()));
        if (!pArgs) {
            std::cout << red_color_code << "Failed to pack arguments" << reset_color_code << std::endl;
            Py_DECREF(pFunc);
            Py_DECREF(pModule);
            return success;
        }

        PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);

        if (pValue != nullptr) {
            success = true;
            Py_DECREF(pValue);
        } else {
            PyErr_Print();
            std::cout << red_color_code << "Call to " << function_name << " failed" << reset_color_code << std::endl;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cout << red_color_code << "Failed to load '" << library << "'" << reset_color_code << std::endl;
    }

    return success;
}

std::map<std::string, std::vector<std::string>> ExecuteSelectGenresQuery(const std::string& library, const std::string& query) {
    std::string function_name = "select_genres";
    std::map<std::string, std::vector<std::string>> results;

    PyObject *pName = PyUnicode_DecodeFSDefault(library.c_str());
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, function_name.c_str());

        if (PyCallable_Check(pFunc)) {
            PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString(query.c_str()));

            PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != nullptr) {
                PyObject *iterator = PyObject_GetIter(pValue);
                if (iterator == nullptr) {
                    PyErr_Print();
                    std::cout << red_color_code << "Failed to get iterator" << reset_color_code << std::endl;
                    Py_DECREF(pValue);
                    Py_DECREF(pModule);
                    return results;
                }

                PyObject *item;
                while ((item = PyIter_Next(iterator)) != NULL) {
                    if (PyTuple_Check(item) && PyTuple_Size(item) == 2) {
                        PyObject *first = PyTuple_GetItem(item, 0);
                        PyObject *second = PyTuple_GetItem(item, 1);

                        if (PyUnicode_Check(first) && PyTuple_Check(second)) {
                            std::string tconst = PyUnicode_AsUTF8(first);
                            std::vector<std::string> genres;

                            PyObject *genre_item;
                            Py_ssize_t genre_size = PyTuple_Size(second);
                            for (Py_ssize_t i = 0; i < genre_size; ++i) {
                                genre_item = PyTuple_GetItem(second, i);
                                if (PyUnicode_Check(genre_item)) {
                                    genres.push_back(PyUnicode_AsUTF8(genre_item));
                                } else {
                                    PyErr_Print();
                                    std::cout << red_color_code << "Genre item is not a string" << reset_color_code << std::endl;
                                }
                            }
                            results[tconst] = genres;
                        } else {
                            PyErr_Print();
                            std::cout << red_color_code << "Tuple items are not of expected types" << reset_color_code << std::endl;
                        }
                    } else {
                        PyErr_Print();
                        std::cout << red_color_code << "Item is not a tuple or does not have 2 elements" << reset_color_code << std::endl;
                    }
                    Py_DECREF(item);
                }
                Py_DECREF(iterator);
                Py_DECREF(pValue);
            } else {
                PyErr_Print();
                std::cout << red_color_code << "Call to " << function_name << " failed" << reset_color_code << std::endl;
            }
        } else {
            PyErr_Print();
            std::cout << red_color_code << "Cannot find function '" << function_name << "'" << reset_color_code << std::endl;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cout << red_color_code << "Failed to load '" << library << "'" << reset_color_code << std::endl;
    }

    return results;
}

std::vector<std::string> GetContentRecommendations(const std::string& target_user_id) {
    std::string library = "library";
    std::string likes_query = "SELECT user_id, tconst, rating FROM user_ratings WHERE user_id = '" + target_user_id + "' and rating >= 6 ORDER BY rating DESC;";
    std::string dislikes_query = "SELECT user_id, tconst, rating FROM user_ratings WHERE user_id = '" + target_user_id + "' and rating < 6 ORDER BY rating DESC;";

    std::vector<std::map<std::string, std::string>> likes = ExecuteSelectQuery(library, likes_query);
    std::vector<std::map<std::string, std::string>> dislikes = ExecuteSelectQuery(library, dislikes_query);

    std::vector<std::string> disliked;
    for (const auto& d : dislikes) {
        disliked.push_back(d.at("tconst"));
    }

    std::vector<std::string> recommended;
    for (const auto& l : likes) {
        std::string tc = l.at("tconst");
        std::string sel_query = "SELECT * FROM cb_similarity WHERE index1 = '" + tc + "' OR index2 = '" + tc + "' ORDER BY similarity DESC LIMIT 5;";
        std::vector<std::map<std::string, std::string>> sel = ExecuteSelectQuery(library, sel_query);

        for (const auto& j : sel) {
            if (j.at("index1") == tc && std::find(disliked.begin(), disliked.end(), j.at("index2")) == disliked.end()) {
                recommended.push_back(j.at("index2"));
            } else if (j.at("index2") == tc && std::find(disliked.begin(), disliked.end(), j.at("index1")) == disliked.end()) {
                recommended.push_back(j.at("index1"));
            }
        }
    }

    return recommended;
}

std::vector<std::string> GetUserRecommendations(const std::string& target_user_id) {
    std::string library = "library";
    std::string user_ratings_query = "SELECT user_id, tconst, rating FROM user_ratings";
    std::string titles_query = R"(
        SELECT t.tconst
        FROM titles t
        JOIN ratings r ON t.tconst = r.tconst
        WHERE t.description IS NOT NULL
          AND t.description != ''
          AND t.year_start > 1950
          AND r.num_votes > 200
        ORDER BY r.num_votes DESC LIMIT 10000;
    )";

    auto user_ratings = ExecuteSelectQuery(library, user_ratings_query);
    if (user_ratings.empty()) {
        std::cerr << "Failed to retrieve user ratings" << std::endl;
        return {};
    }

    auto titles = ExecuteSelectQuery(library, titles_query);
    if (titles.empty()) {
        std::cerr << "Failed to retrieve titles" << std::endl;
        return {};
    }

    std::unordered_map<std::string, std::unordered_map<std::string, double>> user_movie_dict;
    for (const auto& row : user_ratings) {
        std::string user_id = row.at("user_id");
        std::string tconst = row.at("tconst");
        double rating = std::stod(row.at("rating"));
        user_movie_dict[user_id][tconst] = rating;
    }

    std::vector<std::string> movie_list;
    for (const auto& row : titles) {
        movie_list.push_back(row.at("tconst"));
    }

    std::unordered_map<std::string, double> cosine_similarities;
    const auto& target_ratings = user_movie_dict[target_user_id];
    for (const auto& [user_id, ratings] : user_movie_dict) {
        if (user_id == target_user_id) continue;
        double dot_product = 0.0, target_norm = 0.0, user_norm = 0.0;
        for (const auto& movie : movie_list) {
            double target_rating = target_ratings.count(movie) ? target_ratings.at(movie) : 0.0;
            double user_rating = ratings.count(movie) ? ratings.at(movie) : 0.0;
            dot_product += target_rating * user_rating;
            target_norm += target_rating * target_rating;
            user_norm += user_rating * user_rating;
        }
        double similarity = (target_norm == 0 || user_norm == 0) ? 0 : dot_product / (std::sqrt(target_norm) * std::sqrt(user_norm));
        cosine_similarities[user_id] = similarity;
    }

    std::unordered_map<std::string, std::unordered_map<std::string, double>> adjusted_user_movie_dict;
    for (const auto& [user_id, ratings] : user_movie_dict) {
        double similarity = cosine_similarities.count(user_id) ? cosine_similarities.at(user_id) : 1.0;
        for (const auto& [movie, rating] : ratings) {
            adjusted_user_movie_dict[user_id][movie] = rating * similarity;
        }
    }

    std::unordered_map<std::string, double> movie_sums;
    for (const auto& [user_id, ratings] : adjusted_user_movie_dict) {
        if (user_id == target_user_id) continue;
        for (const auto& [tconst, rating] : ratings) {
            movie_sums[tconst] += rating;
        }
    }

    std::vector<std::string> watched_list;
    for (const auto& row : user_ratings) {
        if (row.at("user_id") == target_user_id) {
            watched_list.push_back(row.at("tconst"));
        }
    }

    std::vector<std::pair<std::string, double>> recommendations;
    for (const auto& [tconst, sum] : movie_sums) {
        if (std::find(watched_list.begin(), watched_list.end(), tconst) == watched_list.end() && sum != 0) {
            recommendations.emplace_back(tconst, sum);
        }
    }
    std::sort(recommendations.begin(), recommendations.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::vector<std::string> recommended;
    for (const auto& rec : recommendations) {
        recommended.push_back(rec.first);
    }

    return recommended;
}