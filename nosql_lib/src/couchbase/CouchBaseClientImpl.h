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
#include <deque>

namespace drogon
{
namespace nosql
{
class CouchBaseConnection;
using CouchBaseConnectionPtr = std::shared_ptr<CouchBaseConnection>;
class CouchBaseCommand;
using CouchBaseCommandPtr = std::shared_ptr<CouchBaseCommand>;
class CouchBaseClientImpl
    : public CouchBaseClient,
      public std::enable_shared_from_this<CouchBaseClientImpl>
{
  public:
    CouchBaseClientImpl(const std::string &connectString,
                        const std::string &userName,
                        const std::string &password,
                        const std::string &bucket,
                        size_t connNum);
    virtual void get(const std::string &key,
                     CBCallback &&callback,
                     ExceptionCallback &&errorCallback) override;

  private:
    const std::string connectString_;
    const std::string userName_;
    const std::string password_;
    const std::string bucket_;
    const size_t connectionsNumber_;
    std::mutex connectionsMutex_;
    std::unordered_set<CouchBaseConnectionPtr> connections_;
    std::unordered_set<CouchBaseConnectionPtr> readyConnections_;
    std::unordered_set<CouchBaseConnectionPtr> busyConnections_;
    std::deque<CouchBaseCommandPtr> commandsBuffer_;
    trantor::EventLoopThreadPool loops_;
    CouchBaseConnectionPtr newConnection(trantor::EventLoop *loop);
    CouchBaseConnectionPtr getIdleConnection();
    void handleNewTask(const CouchBaseConnectionPtr &connPtr);
};
}  // namespace nosql
}  // namespace drogon