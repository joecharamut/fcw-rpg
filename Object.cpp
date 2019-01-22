#include "Object.h"
#include <fstream>

void Container::type() {
    std::cout << "Container" << std::endl;
}

void Sign::type() {
    std::cout << "Sign" << std::endl;
}

void Test::test() {
    {
        std::ofstream ofstream("asdf.json");
        cereal::JSONOutputArchive out(ofstream);
        std::shared_ptr<Object> one = std::make_shared<Container>();
        std::shared_ptr<Object> two = std::make_shared<Sign>();
        std::shared_ptr<Container> con = std::static_pointer_cast<Container>(one);
        std::shared_ptr<Sign> sin = std::static_pointer_cast<Sign>(two);
        con->contents.emplace_back("Test");
        sin->text = "asdf";
        out(one, two);
    }
    {
        std::ifstream ifstream("asdf.json");
        cereal::JSONInputArchive in(ifstream);
        std::shared_ptr<Object> one;
        std::shared_ptr<Object> two;
        in(one, two);
        std::shared_ptr<Container> con = std::static_pointer_cast<Container>(one);
        std::shared_ptr<Sign> sin = std::static_pointer_cast<Sign>(two);
        one->type();
        std::cout << con->contents[0] << std::endl;
        two->type();
        std::cout << sin->text << std::endl;
    }
};