#include "crow.h"

class User
{
private:
    std::string id;
    std::string name;

public:
    User()
    {
        this->id = "";
        this->name = "";
    }

    User(const std::string id, const std::string name)
    {
        this->id = id;
        this->name = name;
    }

    std::string getId() const
    {
        return this->id;
    }

    std::string getName() const
    {
        return this->name;
    }

    void setId(const std::string id)
    {
        this->id = id;
    }

    void setName(const std::string name)
    {
        this->name = name;
    }

    // Function to convert User to crow::json::wvalue
    crow::json::wvalue to_json()
    {
        crow::json::wvalue x;
        x["id"] = crow::json::wvalue(this->id);
        x["name"] = crow::json::wvalue(this->name);
        return x;
    }
};

// Function to find a User by ID
std::optional<User> findUserById(const std::vector<User> &users, const std::string &id)
{
    for (const auto &user : users)
    {
        if (user.getId() == id)
        {
            return user;
        }
    }
    return std::nullopt; // Return an empty optional if the user is not found
}

// Function to find a User by ID and return an iterator to the user
std::vector<User>::iterator findUserIteratorById(std::vector<User> &users, const std::string &id) {
    return std::find_if(users.begin(), users.end(), [&id](const User &user) {
        return user.getId() == id;
    });
}

int main()
{
    crow::SimpleApp app;

    std::vector<User> users = {User("1", "Bob"), User("2", "Alice")};

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    // GET /users
    CROW_ROUTE(app, "/users")
    ([&users]()
     {
        crow::json::wvalue output;
        for (size_t i = 0; i < users.size(); ++i)
        {
            output[i] = users[i].to_json();
        }
        return output; 
    });

    // GET /users/{id}
    CROW_ROUTE(app,"/users/<string>")
    ([&users](std::string id){
        std::optional<User> user = findUserById(users, id);
        if (!user) {
            return crow::response(crow::status::NOT_FOUND);
        }
        crow::json::wvalue user_json = user->to_json();  // Call the to_json member function
        return crow::response(200, user_json);
    });

    // POST /users
    CROW_ROUTE(app, "/users")
    .methods("POST"_method)
    ([&users](const crow::request &req){
        auto x = crow::json::load(req.body);
        if (!x) {
            return crow::response(crow::status::BAD_REQUEST); // same as crow::response(400)
        }
        User new_user = User(std::string(x["id"]), std::string(x["name"]));
        users.push_back(new_user);
        crow::json::wvalue output;
        for (size_t i = 0; i < users.size(); ++i)
        {
            output[i] = users[i].to_json();
        }
        return crow::response(201, output); 
    });

    // DELETE /users/{id}
    CROW_ROUTE(app, "/users/<string>").methods("DELETE"_method)
    ([&users](const std::string &id) {
        auto it = findUserIteratorById(users, id);
        if (it == users.end()) {
            return crow::response(crow::status::NOT_FOUND);
        }

        users.erase(it);
        return crow::response(204);
    });

    // PUT /users/{id}
    CROW_ROUTE(app, "/users/<string>").methods("PUT"_method)
    ([&users](const crow::request &req, const std::string &id) -> crow::response {
        auto it = findUserIteratorById(users, id);

        if (it == users.end()) {
            return crow::response(404);
        }

        auto x = crow::json::load(req.body);
        if (!x) {
            return crow::response(400);
        }

        it->setId(x["id"].s());
        it->setName(x["name"].s());

        crow::json::wvalue user_json = it->to_json();  // Call the to_json member function
        return crow::response(200, user_json);
    });


    app.port(18080).multithreaded().run();
}