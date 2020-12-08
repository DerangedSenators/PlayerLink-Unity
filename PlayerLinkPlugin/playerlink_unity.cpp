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

#include "playerlink_unity.h"


DLLEXPORT void startDaemon(char* username,char* hostname){
    NetClient constructor(hostname);
    player_usr = username;
    mClient= &constructor; // Allow address to escape the function. This is required for the plugin to function as Unity will not allow class instances to be accessed
    runtimeThreads->create_thread(daemonRT);
    // Don't wait to join
}

DLLEXPORT bool stopDaemon(){
    runtimeMutex->lock();
    runtimeThreads->interrupt_all(); //Force Interrupts all threads
    mClient->logout();
    runtimeMutex->unlock();
}
DLLEXPORT void logout(){
    mClient->logout();
}

void daemonRT(){
    active = true;
    if(mClient != nullptr){
        mClient->login(player_usr);
        active = false; // Thread has timed out
    }
}
DLLEXPORT bool isActive(){
    return active;
}

DLLEXPORT void sendMessage(unsigned char* message){
    mClient->addMsgToBuffer(message);
}

DLLEXPORT unsigned char* getLatestMessage(){
    return mClient->getLatestMessage();
}
