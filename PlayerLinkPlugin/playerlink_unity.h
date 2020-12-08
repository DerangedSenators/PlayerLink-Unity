/*
 * Copyright (C) 2020 Deranged Senators
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PLAYERLINK_UNITY_H
#define PLAYERLINK_UNITY_H
#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec (dllimport)
#elif unix
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLIMPORT __attribute__((visibility("default")))
#endif
#include <boost/thread.hpp>
#include <mutex>
#include "Client.h"
#include "net/NetConfig.h"
/**
 * @file UnityAPI.h
 * @brief Provides an interface to allow usage of PlayerLink API tools as a Unity Plugin
 * @author Hanzalah Ravat
 * This header program holds static members and methods that can be called from Unity into PlayerLink and vice-versa.
 */
using namespace PlayerLink::Client;
using namespace PlayerLink::Core;
/**
 * The client instance used for this session
 */
static NetClient* mClient = nullptr;
static bool active = false; // Starts as false and flipped as required
static boost::thread_group* runtimeThreads;
static std::mutex* runtimeMutex;
static char* player_usr;
extern "C"{
/**
 * Starts a Daemon to connect to the server
 * @param username The username of this player
 * @param hostname The Host address
 */
DLLEXPORT void startDaemon(char* username,char* hostname);
/**
 * Attempts to force-stop the daemon. Not reccomended unless absolutely neccessary as it may lead to memory mis-management as items aren't managed. Use Logout for best results.
 * @return True if the daemon was stopped successfully and false if there was an error
 */
DLLEXPORT bool stopDaemon();
/**
 * Denotes if the daemon is active
 * @return True if active and false if the daemon has been terminated
 */
DLLEXPORT bool isActive();
/**
 * Logs the user out and closes connection to the server. This is the prefered method to end a connection over stopDaemon() which forces Daemon threads to force-stop
 */
DLLEXPORT void logout();
/**
 * Attempts to send a message to other players
 * @param message The message in a C# byte[] format
 */
DLLEXPORT void sendMessage(unsigned char* message);
/**
 * The method that is called when a message is recieved by PlayerLink
 * This method must be handled within Unity so that the Game or Application is able to read the message
 * @param message a C# byte[] containing the message
 */
DLLIMPORT void onMessageRecieved(unsigned char* message);
/**
 * Gets the message at the top of the message queue. You should use this implementation over onMessageRecieved if you cannot define the method in C#.
 * Ideally, you should have a seperate thread which calls this method on a whileloop based on the condition isActive() evaluating true
 * @return The latest message that was recieved by the PlayerLink daemon
 */
DLLEXPORT unsigned char* getLatestMessage();
/**
 * Looped method which forms the basis of the daemon at runtime
 */
void daemonRT();

}

#endif //PLAYERLINK_UNITY_H
