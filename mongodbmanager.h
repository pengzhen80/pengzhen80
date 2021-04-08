
#ifndef MONGODBMANAGER
#define MONGODBMANAGER

#include <cstdint>
#include <iostream>
#include <vector>
#include <stdio.h>
#include<string>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

class MongodbManager
{
public:
    enum ORDERTYPE
    {
        ORDERTYPE_RESTAURANT_DELEVER = 0,
        ORDERTYPE_RESTAURANT_RECYCLE,
        ORDERTYPE_STOREHOUSE,
    };
    enum ORDERRESULT
    {
        ORDERRESULT_SUCCESS = 0,
        ORDERRESULT_FAILURE,
        ORDERRESULT_DEFAULT,
    };
    struct Goods_Info
    {
        std::string name;
    };
    struct Order_Data
    {
        Goods_Info goods_info;
        int goods_pos;
        int target_pos;
    };
    struct Order
    {
        int number;
        int time;
        Order_Data data;
        ORDERTYPE type;
        ORDERRESULT result;
    }; 

public :
     explicit MongodbManager()
    {
        mongocxx::instance instance{};
    }
    bool Init();
    bool OpenDB(std::string db_name);
    bool OpenCollection(std::string coll_name);
    bool Insert(Order order);
    bool Multi_Insert();
    bool Delete_by_filter();
    bool Multi_Delete();
    Order Find_by_filter(int order_num);
    bool Multi_Find();
    bool Update();
    bool Multi_Update();

private:
    Order Trans_Json_To_Order(bsoncxx::document::view view);
    bool rool_back();

private:
    mongocxx::client* ptr_client_;
    mongocxx::database db_;
    mongocxx::collection coll_;
};

#endif
