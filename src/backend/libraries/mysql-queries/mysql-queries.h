#ifndef MYSQL_QUERIES_H
#define MYSQL_QUERIES_H

#include <string>
#include <vector>
#include <map>

extern const std::string red_color_code;
extern const std::string yellow_color_code;
extern const std::string reset_color_code;

bool initializePythonInterpreter(const std::string& project_path);
void finalizePythonInterpreter();
std::vector<std::map<std::string, std::string> > ExecuteSelectQuery(const std::string& library, const std::string& query);
bool ExecuteInsertQuery(const std::string& library, const std::string& function_name, const std::string& table_name, const std::vector<std::map<std::string, std::string> >& data);
bool ExecuteUpdateQuery(const std::string& library, const std::string& update_query);
bool ExecuteDeleteQuery(const std::string& library, const std::string& delete_query);
std::map<std::string, std::vector<std::string>> ExecuteSelectGenresQuery(const std::string& library, const std::string& query);
std::vector<std::string> GetContentRecommendations(const std::string& target_user_id);
std::vector<std::string> GetUserRecommendations(const std::string& target_user_id);


#endif // MYSQL_QUERIES_H
