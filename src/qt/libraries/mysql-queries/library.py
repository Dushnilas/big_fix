import mysql.connector
from mysql.connector import Error
# -------------- connection data --------------
host_name = "92.53.120.173"
user_name = "adminchik"
user_password = "aaa_woman_IgNoReALEKO228"
db_name = "recsys_project_database"
port = 3306

# -------------- create connection function --------------
def create_connection(host_name, user_name, user_password, db_name, port):
    connection = None
    try:
        connection = mysql.connector.connect(
            host=host_name,
            user=user_name,
            password=user_password,
            database=db_name,
            port=port
        )
        # print("Connection to MySQL DB successful")
    except Error as e:
        print(f"The error '{e}' occurred")
        return
    return connection

# -------------- select --------------
def select(query):
    connection = create_connection(host_name, user_name, user_password, db_name, port)
    cursor = connection.cursor()
    try:
        cursor.execute(query)
        columns = [desc[0] for desc in cursor.description]
        results = cursor.fetchall()
        result_dicts = []
        for row in results:
            row_dict = {}
            for i in range(len(columns)):
                value = row[i]
                if value is None:
                    if cursor.description[i][1] in (mysql.connector.FieldType.TINY, mysql.connector.FieldType.SHORT, mysql.connector.FieldType.LONG, mysql.connector.FieldType.FLOAT, mysql.connector.FieldType.DOUBLE, mysql.connector.FieldType.INT24):
                        value = 0
                    else:
                        value = ""
                row_dict[columns[i]] = str(value)
            result_dicts.append(list(row_dict.items()))
        return result_dicts
    except Error as e:
        print(f"The error '{e}' occurred")
        return []
    finally:
        cursor.close()
        connection.close()

# -------------- select specially for genres --------------
# SRECIALLY FOR SENYA
def select_genres(query):
    connection = create_connection(host_name, user_name, user_password, db_name, port)
    cursor = connection.cursor()
    try:
        cursor.execute(query)
        results = cursor.fetchall()
        grouped_results = {}
        for row in results:
            tconst = row[0]
            genre_name = row[1]
            if tconst not in grouped_results:
                grouped_results[tconst] = []
            grouped_results[tconst].append(genre_name)

        formatted_results = [(tconst, tuple(set(genres))) for tconst, genres in grouped_results.items()]

        return formatted_results
    except Error as e:
        print(f"The error '{e}' occurred")
        return []
    finally:
        cursor.close()
        connection.close()
# -------------- delete --------------
def delete(delete_query):
    connection = create_connection(host_name, user_name, user_password, db_name, port)
    cursor = connection.cursor()
    try:
        cursor.execute(delete_query)
        connection.commit()
        print(f"Query executed successfully: {delete_query}")
    except Error as e:
        print(f"The error '{e}' occurred")
        connection.rollback()
    finally:
        cursor.close()
        connection.close()

# -------------- insert --------------
def insert(table_name, data):
    connection = create_connection(host_name, user_name, user_password, db_name, port)
    cursor = connection.cursor()
    try:
        # if not isinstance(data[0], tuple):
        # #     data = [data]
        # print(data)
        columns = ', '.join([item[0] for item in data[0]])
        placeholders = ', '.join(['%s'] * len(data[0]))
        sql = f"INSERT IGNORE INTO {table_name} ({columns}) VALUES ({placeholders})"
        values = [tuple(item[1] for item in record) for record in data]
        cursor.executemany(sql, values)
        connection.commit()
        print(f"{cursor.rowcount} rows inserted successfully into {table_name}")
    except Error as e:
        print(f"The error '{e}' occurred")
        connection.rollback()
    finally:
        cursor.close()
        connection.close()


# -------------- update --------------
def update(update_query):
    connection = create_connection(host_name, user_name, user_password, db_name, port)
    cursor = connection.cursor()
    try:
        cursor.execute(update_query)
        connection.commit()
        print(f"Query executed successfully: {update_query}")
    except Error as e:
        print(f"The error '{e}' occurred")
        connection.rollback()
    finally:
        cursor.close()
        connection.close()






# -------------- some queries (temp) --------------




# ---------------- TESTING PART ------------------
#
# data = [
#     [
#         ("user_id", "alEko777"),
#         ("name", "Aleko Maisuradze"),
#         ("age", 17),
#         ("photo_uri", ""),
#     ],
#     [
#         ("user_id", "26kstrochekcoda"),
#         ("name", "Makar verbzhinskiy"),
#         ("age", 19),
#         ("photo_uri", ""),
#     ]
# ]
# insert_data('user_profile', data)
# print('--------------------------------------')
# update("UPDATE user_profile SET name = 'PAVEL POPOV' WHERE user_id = '26kstrochekcoda';")
# print('--------------------------------------')
# print(*select("SELECT * FROM user_profile;"), sep='\n')
# print('--------------------------------------')
# print(*execute_select_query("SELECT * FROM user_collections;"), sep="\n")
# print('--------------------------------------')
# wanted_user = 'alEko777'
#
# rating = [
#     [
#         ('user_id', 'alEko777'),
#         ('tconst', 'tt11097240'),
#         ('rating', 10)
#     ],
#     [
#         ('user_id', 'alEko777'),
#         ('tconst', 'tt0795421'),
#         ('rating', 5)
#     ],
#     [
#         ('user_id', '26kstrochekcoda'),
#         ('tconst', 'tt21819994'),
#         ('rating', 7)
#     ],
#     [
#         ('user_id', '26kstrochekcoda'),
#         ('tconst', 'tt21819994'),
#         ('rating', 7)
#     ]
# ]
#
# #
# insert_data('user_ratings', rating)
# print(*execute_select_query("SELECT * FROM user_ratings;"), sep="\n")
# print('--------------------------------------')
# print(*execute_select_query(f"""SELECT t.* FROM titles t
#                             JOIN user_ratings u ON t.tconst = u.tconst
#                             WHERE u.user_id = '{wanted_user}';"""), sep='\n')
#
# print('--------------------------------------')
#
# execute_delete_query(f"DELETE FROM user_profile WHERE user_id = '{wanted_user}';")
# print('--------------------------------------')
# print(*execute_select_query("SELECT * FROM user_ratings;"), sep="\n")
# print('--------------------------------------')
# print(*execute_select_query("SELECT * FROM user_collections;"), sep="\n")







# print(execute_select_query(f'''
#     SELECT t.* FROM titles t
#     JOIN titles_collections tc ON t.tconst = tc.tconst;
#
# '''))

# print(execute_select_query("SELECT * FROM titles WHERE tconst = 'tt0795421';"))
#
# execute_delete_query(f"-- DELETE FROM user_profile WHERE user_id = '{wanted_user}';")
#
# print(execute_select_query(f'select * from user_ratings WHERE user_id = "{wanted_user}";'))