/**
 *
 *  @file CouchBaseClientImpl.cc
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#include "CouchBaseClientImpl.h"
#include "CouchBaseConnection.h"
using namespace drogon::nosql;

CouchBaseClientImpl::CouchBaseClientImpl(const std::string &connectString,
                                         const std::string &userName,
                                         const std::string &password,
                                         const std::string &bucket,
                                         size_t connNum)
    : connectString_(connectString),
      userName_(userName),
      password_(password),
      bucket_(bucket),
      connectionsNumber_(connNum),
      loops_(connNum < std::thread::hardware_concurrency()
                 ? connNum
                 : std::thread::hardware_concurrency(),
             "CouchBaseLoop")
{
    assert(connNum > 0);
    loops_.start();

    std::thread([this]() {
        for (size_t i = 0; i < connectionsNumber_; ++i)
        {
            auto loop = loops_.getNextLoop();
            loop->runInLoop([this, loop]() {
                std::lock_guard<std::mutex> lock(connectionsMutex_);
                connections_.insert(newConnection(loop));
            });
        }
    }).detach();
}

CouchBaseConnectionPtr CouchBaseClientImpl::newConnection(
    trantor::EventLoop *loop)
{
    return std::make_shared<CouchBaseConnection>(
        connectString_, userName_, password_, bucket_, loop);
}