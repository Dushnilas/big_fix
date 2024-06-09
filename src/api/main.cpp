#include "../../libraries/mysql-queries/mysql-queries.h"
#include <iostream>

int main() {
    const std::string project_path = PROJECT_PATH;

    if (!initializePythonInterpreter(project_path)) {
        return 1;
    }

    std::string query = "SELECT COUNT(*) FROM similarity_table";
    std::vector<std::map<std::string, std::string>> results = ExecuteSelectQuery("library", query);

    for (const auto& row : results) {
        for (const auto& [key, value] : row) {
            std::cout << yellow_color_code << key << ": " << value << "\t" << reset_color_code;
        }
        std::cout << '\n';
    }

    std::vector<std::map<std::string, std::string>> data = {
        {{"user_id", "alEko777"}, {"name", "Aleko Maisuradze"}, {"age", "17"}, {"photo_uri", ""}},
        {{"user_id", "26kstrochekcoda"}, {"name", "Makar verbzhinskiy"}, {"age", "19"}, {"photo_uri", ""}}
    };
    bool insert_success = ExecuteInsertQuery("library", "insert", "users", data);

    if (insert_success) {
        std::cout << yellow_color_code << "Data inserted successfully." << reset_color_code << std::endl;
    } else {
        std::cout << yellow_color_code << "Data insertion failed." << reset_color_code << std::endl;
    }

    std::string update_query = "UPDATE users SET name = 'Updated Name' WHERE user_id = 'alEko777'";
    bool update_success = ExecuteUpdateQuery("library", update_query);

    if (update_success) {
        std::cout << yellow_color_code << "Data updated successfully." << reset_color_code << std::endl;
    } else {
        std::cout << yellow_color_code << "Data update failed." << reset_color_code << std::endl;
    }

    std::string delete_query = "DELETE FROM users WHERE user_id = 'alEko777'";
    bool delete_success = ExecuteDeleteQuery("library", delete_query);

    if (delete_success) {
        std::cout << yellow_color_code << "Data deleted successfully." << reset_color_code << std::endl;
    } else {
        std::cout << yellow_color_code << "Data delete failed." << reset_color_code << std::endl;
    }

    finalizePythonInterpreter();

    return 0;
}
