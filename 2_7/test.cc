#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<sw/redis++/redis++.h>
#include<thread>
#include<chrono>

void test1()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    std::string ret = redis.ping();
    std::cout << ret << std::endl;
}

void test2()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.set("key1", "value1");
    redis.set("key2", "value2");

    sw::redis::OptionalString ret1 = redis.get("key1");
    sw::redis::OptionalString ret2 = redis.get("key2");
    sw::redis::OptionalString ret3 = redis.get("key3");
    if(ret1) std::cout << ret1.value() << std::endl;
    if(ret2) std::cout << ret2.value() << std::endl;
    if(ret3) std::cout << ret3.value() << std::endl;
}

void test3()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.set("key1", "value1");
    redis.set("key2", "value2");
    redis.set("key3", "value3");

    std::cout << redis.exists({"key1", "key2"}) << std::endl;
    std::cout << redis.exists("key1") << std::endl << std::endl;
    std::cout << redis.exists("key2") << std::endl << std::endl;

    redis.del("key1");
    std::cout << redis.exists("key1") << std::endl;
    std::cout << redis.exists("key2") << std::endl << std::endl;

    redis.del({"key1", "key2", "key3"});
    std::cout << redis.exists("key1") << std::endl;
    std::cout << redis.exists("key2") << std::endl;
}

void test4()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.set("key1", "value1");
    redis.set("key2", "value2");
    redis.set("key3", "value3");

    std::vector<std::string> arr;
    auto it = std::back_inserter(arr);
    redis.keys("*", it);
    for(auto& e : arr)
        std::cout << e << std::endl;
}

void test5()
{
    using namespace std::chrono_literals;
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.set("key1", "value1");
    redis.expire("key1", std::chrono::seconds(10)); // 10也行
    std::this_thread::sleep_for(3s); // 3也行，3s需要展开std::chrono_literals命名空间
    auto ret = redis.ttl("key1");
    std::cout << ret << std::endl;
    std::this_thread::sleep_for(7s);
    std::cout << redis.exists("key1") << std::endl;
}

void test6()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.set("key1", "value1");
    redis.lpush("key2", "hello");
    redis.hset("key3", "key1", "value1");
    redis.sadd("key4", "hello");
    std::cout << redis.type("key1") << std::endl;
    std::cout << redis.type("key2") << std::endl;
    std::cout << redis.type("key3") << std::endl;
    std::cout << redis.type("key4") << std::endl;
}

void test7()
{
    using namespace std::chrono_literals;
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.set("key1", "value1", 5s);
    std::this_thread::sleep_for(6s);
    auto ret = redis.get("key1");
    if(ret) std::cout << ret.value() << std::endl;
    redis.set("key2", "value2", 0s, sw::redis::UpdateType::EXIST);
    ret = redis.get("key2");
    if(ret) std::cout << ret.value() << std::endl;
    redis.set("key3", "value3", 0s, sw::redis::UpdateType::NOT_EXIST);
    ret = redis.get("key3");
    if(ret) std::cout << ret.value() << std::endl;
}

void test8()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    // redis.mset({std::make_pair("key1", "value1"), std::make_pair("key2", "value2"), std::make_pair("key3", "value3")});
    // redis.mset({std::pair<std::string, std::string>{"key1", "value1"}, 
    //     {"key2", "value2"}, 
    //     {"key3", "value3"}});
    std::vector<std::pair<std::string, std::string>> arr{
        {"key1", "value1"},
        {"key2", "value2"},
        {"key3", "value3"}
    };
    redis.mset(arr.begin(), arr.end());
    std::vector<sw::redis::OptionalString> buffer;
    auto it = std::back_inserter(buffer);
    redis.mget({ "key1", "key2", "key3", "key4" }, it);
    for(auto& e : buffer)
        if(e) std::cout << e.value() << std::endl;
        else std::cout << "not exists." << std::endl;
}

void test9()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.set("key1", "abcdefghij");
    auto ret1 = redis.getrange("key1", 1, 3);
    std::cout << ret1 << std::endl;
    redis.setrange("key1", 3, "aaaaaaaaaaaaaaaaa");
    auto ret2 = redis.get("key1");
    if(ret2) std::cout << ret2.value() << std::endl;
}

void test10()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.set("key1", "1");
    redis.incr("key1");
    auto ret1 = redis.get("key1");
    if(ret1) std::cout << ret1.value() << std::endl;
    redis.decr("key1");
    auto ret2 = redis.get("key1");
    if(ret2) std::cout << ret2.value() << std::endl;
}

int main()
{
    test10();
    return 0;
}