/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gmock/gmock.h>

#include <process/gtest.hpp>

#include <stout/gtest.hpp>

#include "master/master.hpp"

#include "messages/messages.hpp"

#include "slave/flags.hpp"
#include "slave/slave.hpp"

#include "tests/mesos.hpp"

using namespace process;

using mesos::internal::master::Master;

using mesos::internal::slave::Slave;

namespace mesos {
namespace internal {
namespace tests {

class OversubscriptionSlaveTest : public MesosTest {};


// This test verifies that slave will forward the estimation of the
// available oversubscribed resources to the master.
TEST_F(OversubscriptionSlaveTest, UpdateOversubcribedResourcesMessage)
{
  Try<PID<Master>> master = StartMaster();
  ASSERT_SOME(master);

  Future<UpdateOversubscribedResourcesMessage> message =
    FUTURE_PROTOBUF(UpdateOversubscribedResourcesMessage(), _, _);

  MockResourceEstimator resourceEstimator;

  EXPECT_CALL(resourceEstimator, oversubscribed())
    .WillRepeatedly(Return(Resources()));

  Try<PID<Slave>> slave = StartSlave(&resourceEstimator);
  ASSERT_SOME(slave);

  AWAIT_READY(message);

  Shutdown();
}

} // namespace tests {
} // namespace internal {
} // namespace mesos {
