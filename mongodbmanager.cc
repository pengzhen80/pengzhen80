#include"mongodbmanager.h"

bool MongodbManager::Init()
{
    ptr_client_ = new mongocxx::client{mongocxx::uri{"mongodb+srv://pengzhen:pengzhen@cluster0.958s1.mongodb.net/myFirstDatabase?retryWrites=true&w=majority"}};
    return true;
}
bool MongodbManager::OpenDB(std::string db_name)
{
    db_ = (*ptr_client_)[db_name];
    return true;
}
bool MongodbManager::OpenCollection(std::string coll_name)
{
    coll_ = db_[coll_name];
    return true;
}
bool MongodbManager::Insert(Order order)
{
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
                                         << "number" << order.number
                                         << "time" << order.time
                                         << "type" << order.type
                                         << "goods info" << bsoncxx::builder::stream::open_document
                                         << "goods name" << order.data.goods_info.name
                                         << "goods pos" << order.data.goods_pos
                                         << "target pos" << order.data.target_pos
                                         << bsoncxx::builder::stream::close_document
                                         << "result" << order.result
                                         << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view view = doc_value.view();
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = coll_.insert_one(view);
    if (result)
    {
        return true;
    }
    else
    {
        false;
    }
}
bool MongodbManager::Multi_Insert()
{
}
bool MongodbManager::Delete_by_filter()
{
}
bool MongodbManager::Multi_Delete()
{
}
MongodbManager::Order MongodbManager::Find_by_filter(int number)
{
    Order result;
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result_filter = coll_.find_one(document{} << "number" << number << finalize);
    if (maybe_result_filter)
    {
        std::cout << bsoncxx::to_json(*maybe_result_filter) << "\n";
    }
    else
    {
        std::cout << "not found\n";
    }

    result = Trans_Json_To_Order(*maybe_result_filter);
    return result;
}
bool MongodbManager::Multi_Find()
{
}
bool MongodbManager::Update()
{
}
bool MongodbManager::Multi_Update()
{
}

MongodbManager::Order MongodbManager::Trans_Json_To_Order(bsoncxx::document::view view)
{
    Order result;
    bsoncxx::document::element element = view["number"];
    if (element.type() != bsoncxx::type::k_int32)
    {
        printf("number not found\n");
    }
    else
    {
        result.number = element.get_int32().value;
    }

    element = view["time"];
    if (element.type() != bsoncxx::type::k_int32)
    {
        printf("time not found\n");
    }
    else
    {
        result.time = element.get_int32().value;
    }

    element = view["goods info"];
    if (element.type() != bsoncxx::type::k_document)
    {
        printf("goods info not found\n");
    }
    else
    {
        bsoncxx::document::view goods_info = element.get_document().value; // 获取到的元素依然为 document view
        bsoncxx::document::element ele1 = goods_info["goods name"];
        if (ele1.type() != bsoncxx::type::k_utf8)
        {
            printf("goods name not found\n");
        }
        else
        {
            result.data.goods_info.name = ele1.get_utf8().value.to_string();
        }
        ele1 = goods_info["goods pos"];
        if (ele1.type() != bsoncxx::type::k_int32)
        {
            printf("goods name not found\n");
        }
        else
        {
            result.data.goods_pos = ele1.get_int32().value;
        }

        ele1 = goods_info["target pos"];
        if (ele1.type() != bsoncxx::type::k_int32)
        {
            printf("goods name not found\n");
        }
        else
        {
            result.data.target_pos = ele1.get_int32().value;
        }
    }

    element = view["type"];
    if (element.type() != bsoncxx::type::k_int32)
    {
        printf("goods name not found\n");
    }
    else
    {
        result.type = (MongodbManager::ORDERTYPE)element.get_int32().value;
    }
    
    element = view["result"];
    if (element.type() != bsoncxx::type::k_int32)
    {
        printf("goods name not found\n");
    }
    else
    {
        result.result = (MongodbManager::ORDERRESULT)element.get_int32().value;
    }
    

    return result;
}
