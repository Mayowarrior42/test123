#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <ctime>
#include <cctype>
#include <windows.h>

struct Object {
    std::string name;
    double x, y;
    std::string type;
    double creation_time;
};

std::vector<Object> readObjectsFromFile(const std::string& filename) {
    std::vector<Object> objects;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return objects;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Object obj;
        if (!(iss >> obj.name >> obj.x >> obj.y >> obj.type >> obj.creation_time)) {
            std::cerr << "Ошибка чтения строки: " << line << std::endl;
            continue;
        }
        objects.push_back(obj);
    }
    return objects;
}

void addObject(std::vector<Object>& objects, const Object& obj, const std::string& filename) {
    objects.push_back(obj);
    std::ofstream file(filename, std::ios::app); 
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла для записи: " << filename << std::endl;
        return;
    }
    file << obj.name << " " << obj.x << " " << obj.y << " " << obj.type << " " << obj.creation_time << "\n";
}

void saveObjectsToFile(const std::vector<Object>& objects, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла для записи: " << filename << std::endl;
        return;
    }
    for (const auto& obj : objects) {
        file << obj.name << " " << obj.x << " " << obj.y << " " << obj.type << " " << obj.creation_time << "\n";
    }
}

std::map<std::string, std::vector<Object>> groupObjectsByName(const std::vector<Object>& objects) {
    std::map<std::string, std::vector<Object>> groups;
    for (const auto& obj : objects) {
        if (obj.name.empty()) {
            continue; 
        }
        std::string group_key = obj.name.substr(0, 1);
        if (!std::isalpha(static_cast<unsigned char>(group_key[0]))) {
            group_key = "#";
        }
        groups[group_key].push_back(obj);
    }
    return groups;
}

std::map<std::string, std::vector<Object>> groupObjectsByType(const std::vector<Object>& objects) {
    std::map<std::string, std::vector<Object>> groups;
    for (const auto& obj : objects) {
        groups[obj.type].push_back(obj);
    }
    return groups;
}

std::map<std::string, std::vector<Object>> groupObjectsByCreationTime(const std::vector<Object>& objects) {
    std::map<std::string, std::vector<Object>> groups;
    std::time_t now = std::time(nullptr);
    for (const auto& obj : objects) {
        std::time_t creation_time = static_cast<std::time_t>(obj.creation_time);
        double diff = std::difftime(now, creation_time);
        std::string group_key;
        if (diff < 86400) {
            group_key = "Сегодня";
        }
        else if (diff < 172800) {
            group_key = "Вчера";
        }
        else if (diff < 604800) {
            group_key = "На этой неделе";
        }
        else if (diff < 2592000) {
            group_key = "В этом месяце";
        }
        else if (diff < 31536000) {
            group_key = "В этом году";
        }
        else {
            group_key = "Ранее";
        }
        groups[group_key].push_back(obj);
    }
    return groups;
}

void printObjects(const std::vector<Object>& objects) {
    for (const auto& obj : objects) {
        std::cout << "Имя: " << obj.name << ", x: " << obj.x << ", y: " << obj.y << ", тип: " << obj.type << ", время создания: " << obj.creation_time << std::endl;
    }
}

void executeCommand(const std::string& command, std::vector<Object>& objects, const std::string& filename) {
    if (command == "read") {
        objects = readObjectsFromFile(filename);
        std::cout << "Объекты прочитаны из файла." << std::endl;
        printObjects(objects);
    }
    else if (command == "add") {
        Object newObj;
        std::cout << "Введите имя, x, y, тип и время создания объекта: ";
        std::cin >> newObj.name >> newObj.x >> newObj.y >> newObj.type >> newObj.creation_time;
        addObject(objects, newObj, filename);
        std::cout << "Объект добавлен." << std::endl;
    }
    else if (command == "save") {
        saveObjectsToFile(objects, filename);
        std::cout << "Объекты сохранены в файл." << std::endl;
    }
    else if (command == "group_name") {
        auto groups = groupObjectsByName(objects);
        std::cout << "Объекты сгруппированы по имени." << std::endl;
    }
    else if (command == "group_type") {
        auto groups = groupObjectsByType(objects);
        std::cout << "Объекты сгруппированы по типу." << std::endl;
    }
    else if (command == "group_time") {
        auto groups = groupObjectsByCreationTime(objects);
        std::cout << "Объекты сгруппированы по времени создания." << std::endl;
    }
    else {
        std::cout << "Неизвестная команда." << std::endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::vector<Object> objects;
    std::string command;
    std::string filename = "objects.txt";
    while (true) {
        std::cout << "Введите команду (read, add, save, group_name, group_type, group_time, exit): ";
        std::cin >> command;
        if (command == "exit") {
            break;
        }
        executeCommand(command, objects, filename);
    }
    std::cout << "Программа завершена." << std::endl;
    return 0;
}
