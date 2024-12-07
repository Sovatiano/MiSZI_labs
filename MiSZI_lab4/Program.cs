using System;
using System.Data.SQLite;

class Program
{
    const string connectionString = "Data Source=database.db;Version=3;";

    static void Main(string[] args)
    {
        InitializeDatabase();

        while (true)
        {
            Console.WriteLine("\nМеню:");
            Console.WriteLine("1. Добавить запись");
            Console.WriteLine("2. Удалить запись");
            Console.WriteLine("3. Посмотреть все записи");
            Console.WriteLine("4. Поиск (уязвимый для SQL Injection)");
            Console.WriteLine("5. Поиск (защищённый)");
            Console.WriteLine("0. Выход");

            Console.Write("\nВыберите опцию: ");
            string choice = Console.ReadLine();


            switch (choice)
            {
                case "1":
                    AddRecord();
                    break;
                case "2":
                    DeleteRecord();
                    break;
                case "3":
                    ViewAllRecords();
                    break;
                case "4":
                    VulnerableSearch();
                    break;
                case "5":
                    SecureSearch();
                    break;
                case "0":
                    return;
                default:
                    Console.WriteLine("Некорректный выбор.");
                    break;
            }
        }
    }

    static void InitializeDatabase()
    {
        using (SQLiteConnection connection = new SQLiteConnection(connectionString))
        {
            connection.Open();
            string createTableQuery = @"
                CREATE TABLE IF NOT EXISTS People (
                    Id INTEGER PRIMARY KEY AUTOINCREMENT,
                    Name TEXT NOT NULL,
                    Age INTEGER NOT NULL
                );";
            using (SQLiteCommand command = new SQLiteCommand(createTableQuery, connection))
            {
                command.ExecuteNonQuery();
            }
        }
    }

    static void AddRecord()
    {
        Console.Write("Введите имя: ");
        string name = Console.ReadLine();

        Console.Write("Введите возраст: ");
        int age = int.Parse(Console.ReadLine());

        using (SQLiteConnection connection = new SQLiteConnection(connectionString))
        {
            connection.Open();
            string query = "INSERT INTO People (Name, Age) VALUES (@Name, @Age)";
            using (SQLiteCommand command = new SQLiteCommand(query, connection))
            {
                command.Parameters.AddWithValue("@Name", name);
                command.Parameters.AddWithValue("@Age", age);
                command.ExecuteNonQuery();
                Console.WriteLine("Запись добавлена.");
            }
        }
    }

    static void DeleteRecord()
    {
        Console.Write("Введите ID записи для удаления: ");
        int id = int.Parse(Console.ReadLine());

        using (SQLiteConnection connection = new SQLiteConnection(connectionString))
        {
            connection.Open();
            string query = "DELETE FROM People WHERE Id = @Id";
            using (SQLiteCommand command = new SQLiteCommand(query, connection))
            {
                command.Parameters.AddWithValue("@Id", id);
                command.ExecuteNonQuery();
                Console.WriteLine("Запись удалена.");
            }
        }
    }

    static void ViewAllRecords()
    {
        using (SQLiteConnection connection = new SQLiteConnection(connectionString))
        {
            connection.Open();
            string query = "SELECT * FROM People";
            using (SQLiteCommand command = new SQLiteCommand(query, connection))
            using (SQLiteDataReader reader = command.ExecuteReader())
            {
                Console.WriteLine("\nСписок записей:");
                while (reader.Read())
                {
                    Console.WriteLine($"ID: {reader["Id"]}, Имя: {reader["Name"]}, Возраст: {reader["Age"]}");
                }
            }
        }
    }

    static void VulnerableSearch()
    {
        Console.Write("Введите имя для поиска: ");
        string name = Console.ReadLine();

        using (SQLiteConnection connection = new SQLiteConnection(connectionString))
        {
            connection.Open();
            string query = $"SELECT * FROM People WHERE Name = '{name}'"; // Уязвимый запрос
            using (SQLiteCommand command = new SQLiteCommand(query, connection))
            using (SQLiteDataReader reader = command.ExecuteReader())
            {
                Console.WriteLine("\nРезультаты поиска:");
                while (reader.Read())
                {
                    Console.WriteLine($"ID: {reader["Id"]}, Имя: {reader["Name"]}, Возраст: {reader["Age"]}");
                }
            }
        }
    }

    static void SecureSearch()
    {
        Console.Write("Введите имя для поиска: ");
        string name = Console.ReadLine();

        using (SQLiteConnection connection = new SQLiteConnection(connectionString))
        {
            connection.Open();
            string query = "SELECT * FROM People WHERE Name = @Name";
            using (SQLiteCommand command = new SQLiteCommand(query, connection))
            {
                command.Parameters.AddWithValue("@Name", name); // Безопасный параметр
                using (SQLiteDataReader reader = command.ExecuteReader())
                {
                    Console.WriteLine("\nРезультаты поиска:");
                    while (reader.Read())
                    {
                        Console.WriteLine($"ID: {reader["Id"]}, Имя: {reader["Name"]}, Возраст: {reader["Age"]}");
                    }
                }
            }
        }
    }
}
