#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<sw/redis++/redis++.h>
#include<thread>
#include<chrono>
#include<set>

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

void test11()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.lpush("mylist1", "hello");
    redis.lpush("mylist1", { "world", "nihao", "redis"});
    std::vector<std::string> arr{"111", "222", "333"};
    redis.lpush("mylist1", arr.begin(), arr.end());
    std::vector<std::string> back;
    auto it = std::back_inserter(back);
    redis.lrange("mylist1", 0, -1, it);
    for(auto& e : back)
        std::cout << e << std::endl;
}

void test12()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.rpush("mylist1", "hello");
    redis.rpush("mylist1", { "world", "nihao", "redis"});
    std::vector<std::string> arr{"111", "222", "333"};
    redis.rpush("mylist1", arr.begin(), arr.end());
    std::vector<std::string> back;
    auto it = std::back_inserter(back);
    redis.lrange("mylist1", 0, -1, it);
    for(auto& e : back)
        std::cout << e << std::endl;
}

void test13()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.lpush("mylist1", { "1", "2", "3", "4"});
    sw::redis::OptionalString ret1 = redis.lpop("mylist1");
    if(ret1) std::cout << ret1.value() << std::endl;
    sw::redis::OptionalString ret2 = redis.rpop("mylist1");
    if(ret2) std::cout << ret2.value() << std::endl;
}

void test14()
{
    using namespace std::chrono_literals;
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    auto ret = redis.brpop({ "key1", "key2", "key3" }, 0s);
    if(ret)
    {
        std::cout << "key: " << ret.value().first << std::endl;
        std::cout << "elem: " << ret.value().second << std::endl;
    }
    else
    {
        std::cout << "result error" << std::endl;
    }
}

void test15()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.lpush("key1", {"1", "2", "3"});
    auto ret = redis.llen("key1");
    std::cout << ret << std::endl;
}

void test16()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.sadd("myset1", "1");
    redis.sadd("myset1", {"2", "3", "4"});
    std::vector<std::string> arr{"5", "6", "7"};
    redis.sadd("myset1", arr.begin(), arr.end());

    // std::vector<std::string> ret;
    // auto it = std::back_inserter(ret);
    std::set<std::string> ret;
    auto it = std::inserter(ret, ret.end());
    redis.smembers("myset1", it);
    for(auto& e : ret)
        std::cout << e << std::endl;
}

void test17()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.sadd("myset1", {"1", "2", "3"});
    std::cout << redis.sismember("myset1", "2") << std::endl;
    std::cout << redis.sismember("myset1", "4") << std::endl;
    std::cout << redis.scard("myset1") << std::endl;
    auto ret = redis.spop("myset1");
    if(ret) std::cout << ret.value() << std::endl;
}

void test18()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.sadd("myset1", {"1", "2", "3"});
    redis.sadd("myset2", {"2", "3", "4"});
    std::set<std::string> ret1;
    auto it = std::inserter(ret1, ret1.end());
    redis.sinter({"myset1", "myset2"}, it);
    for(auto& e : ret1)
        std::cout << e << std::endl;
    std::cout << std::endl;
    std::cout << redis.sinterstore("myset3", {"myset1", "myset2"}) << std::endl << std::endl;
    std::set<std::string> ret2;
    it = std::inserter(ret2, ret2.end());
    redis.smembers("myset3", it);
    for(auto& e : ret2)
        std::cout << e << std::endl;
}

void test19()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.hset("myhash1", "f1", "1");
    redis.hset("myhash1", std::make_pair("f2", "2"));
    redis.hset("myhash1", {std::make_pair("f3", "3"), std::make_pair("f4", "4")});
    std::vector<std::pair<std::string, std::string>> arr{std::make_pair("f5", "5"), std::make_pair("f6", "6")};
    redis.hset("myset1", arr.begin(), arr.end());
    auto ret = redis.hget("myhash1", "f1");
    if(ret) std::cout << ret.value() << std::endl;
}

void test20()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.hset("myhash1", {std::make_pair("f1", "1"), std::make_pair("f2", "2"), std::make_pair("f3", "3")});
    std::cout << redis.hexists("myhash1", "f1") << std::endl;
    std::cout << redis.hdel("myhash1", "f1") << std::endl;
    std::cout << redis.hlen("myhash1") << std::endl;
}

void test21()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.hset("myhash1", {std::make_pair("f1", "1"), std::make_pair("f2", "2"), std::make_pair("f3", "3")});
    std::vector<std::string> ret1;
    auto it = std::back_inserter(ret1);
    redis.hkeys("myhash1", it);
    redis.hvals("myhash1", it);
    for(auto& e : ret1)
        std::cout << e << std::endl;
    std::cout << std::endl;
    std::vector<std::string> ret2;
    it = std::back_inserter(ret2);
    redis.hmget("myhash1", {"f1", "f2"}, it);
    for(auto& e : ret2)
        std::cout << e << std::endl;
}

void test22()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.zadd("myzset1", "a", 1);
    redis.zadd("myzset1", {
        std::make_pair("b", 2),
        std::make_pair("c", 3)
    });
    std::vector<std::pair<std::string, double>> arr{
        std::make_pair("d", 4),
        std::make_pair("e", 5)
    };
    redis.zadd("myzset1", arr.begin(), arr.end());
    std::vector<std::string> ret1;
    auto it1 = std::back_inserter(ret1);
    redis.zrange("myzset1", 0, -1, it1);
    for(auto& e : ret1)
        std::cout << e << std::endl;
    std::cout << std::endl;

    std::vector<std::pair<std::string, double>> ret2;
    auto it2 = std::back_inserter(ret2);
    redis.zrange("myzset1", 0, -1, it2);
    for(auto& e : ret2)
        std::cout << e.first << " " << e.second << std::endl;
}

void test23()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    redis.flushall();
    redis.zadd("myzset1", {
        std::make_pair("a", 1),
        std::make_pair("b", 2),
        std::make_pair("c", 3)
    });
    redis.zrem("myzset1", "a");
    std::cout << redis.zcard("myzset1") << std::endl;
    auto score = redis.zscore("myzset1", "b");
    if(score) std::cout << score.value() << std::endl;
    auto rank = redis.zrank("myzset1", "b");
    if(rank) std::cout << rank.value() << std::endl;
}

int main()
{
    test23();
    return 0;
}