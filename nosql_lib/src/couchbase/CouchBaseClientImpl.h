/**
 *
 *  @file CouchBaseClientImpl.h
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
#pragma once
#include <drogon/nosql/CouchBaseClient.h>
#include <trantor/net/EventLoopThreadPool.h>
#include <memory>
#include <unordered_set>
#include <mutex>

namespace drogon
{
namespace nosql
{
class CouchBaseConnection;
using CouchBaseConnectionPtr= std::shared_ptr<CouchBaseConnection>;
class CouchBaseClientImpl : public CouchBaseClient
{
  public:
    CouchBaseClientImpl(const std::string &connectString,
                        const std::string &userName,
                        const std::string &password,
                        const std::string &bucket,
                        size_t connNum);

  private:
    const std::string connectString_;
    const std::string userName_;
    const std::string password_;
    const std::string bucket_;
    const size_t connectionsNumber_;
    std::mutex connectionsMutex_;
    std::unordered_set<CouchBaseConnectionPtr> connections_;
    trantor::EventLoopThreadPool loops_;
    CouchBaseConnectionPtr newConnection(trantor::EventLoop *loop);

};
}  // namespace nosql
}  // namespace drogon