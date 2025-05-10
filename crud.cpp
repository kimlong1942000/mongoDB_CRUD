#include <iostream>
#include <string>
#include <vector>
#include <mongo-cxx-driver-r4.1.0/client.hpp>
#include <mongo-cxx-driver-r4.1.0/instance.hpp>
#include <mongo-cxx-driver-r4.1.0/uri.hpp>
#include <mongo-cxx-driver-r4.1.0/json.hpp>
#include <mongo-cxx-driver-r4.1.0/builder/stream/document.hpp>

using namespace mongocxx;
using namespace bsoncxx::builder::stream;

void createDocument(collection& coll) {
    auto doc = document{} << "name" << "John Doe" << "age" << 30 << finalize;
    coll.insert_one(doc.view());
    std::cout << "Document inserted." << std::endl;
}

void readDocuments(collection& coll) {
    auto cursor = coll.find({});
    std::cout << "Documents in collection:" << std::endl;
    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

void updateDocument(collection& coll, const std::string& name) {
    auto result = coll.update_one(
        document{} << "name" << name << finalize,
        document{} << "$set" << open_document << "age" << 35 << close_document << finalize
    );
    if (result && result->modified_count() > 0) {
        std::cout << "Document updated." << std::endl;
    } else {
        std::cout << "No document found to update." << std::endl;
    }
}

void deleteDocument(collection& coll, const std::string& name) {
    auto result = coll.delete_one(document{} << "name" << name << finalize);
    if (result && result->deleted_count() > 0) {
        std::cout << "Document deleted." << std::endl;
    } else {
        std::cout << "No document found to delete." << std::endl;
    }
}

int main() {
    // Khởi tạo thư viện MongoDB
    mongocxx::instance instance{};

    // Kết nối tới MongoDB server
    mongocxx::client client{uri{}};

    // Chọn database và collection
    auto db = client["mydatabase"];
    auto coll = db["mycollection"];

    int choice;
    do {
        std::cout << "\nMongoDB CRUD Operations:\n";
        std::cout << "1. Create\n2. Read\n3. Update\n4. Delete\n0. Exit\n";
        std::cout << "Chọn chức năng: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                createDocument(coll);
                break;
            case 2:
                readDocuments(coll);
                break;
            case 3: {
                std::string name;
                std::cout << "Nhập tên của document muốn cập nhật: ";
                std::cin >> name;
                updateDocument(coll, name);
                break;
            }
            case 4: {
                std::string name;
                std::cout << "Nhập tên của document muốn xóa: ";
                std::cin >> name;
                deleteDocument(coll, name);
                break;
            }
            case 0:
                std::cout << "Thoát chương trình.\n";
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ.\n";
        }
    } while (choice != 0);

    return 0;
}