#include <iostream>
#include <crow.h>
#include "task_manager.h"

int main() {
    crow::SimpleApp app;

    TaskManager taskManager;

    CROW_ROUTE(app, "/api/tasks") 
    .methods("GET"_method)([](const crow::request& req) {
        return crow::json::wvalue(taskManager.getTasks());
    })
    .methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json) {
            return crow::response(400);
        }
        std::string taskText = json["task"].s();
        Task newTask = taskManager.addTask(taskText);
        return crow::json::wvalue({{"id", newTask.id}, {"text", newTask.text}, {"completed", newTask.completed}});
    });

    CROW_ROUTE(app, "/api/tasks/<int>") 
    .methods("PUT"_method)([](const crow::request& req, int id) {
        auto json = crow::json::load(req.body);
        if (!json) {
            return crow::response(400);
        }
        std::string taskText = json["task"].s();
        if (taskManager.updateTask(id, taskText)) {
            return crow::response(200);
        }
        return crow::response(404);
    })
    .methods("DELETE"_method)([](int id) {
        if (taskManager.deleteTask(id)) {
            return crow::response(204);
        }
        return crow::response(404);
    });

    app.port(8080).multithreaded().run();
    return 0;
}
