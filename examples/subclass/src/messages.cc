// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// https://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or https://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

// This example shows Rust subclasses of C++ classes.
// See messages.h and main.rs for most of the interesting code.

#include "messages.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>

class CppExampleProducer : public MessageProducer {
public:
    CppExampleProducer() {}
    std::string get_message() const {
        std::time_t result = std::time(nullptr);
        std::ostringstream st;
        st << std::asctime(std::localtime(&result))
           << result << " seconds since the Epoch";
        return st.str();
    }
};

class CppExampleDisplayer : public MessageDisplayer {
public:
    CppExampleDisplayer() {}
    void display_message(const std::string& msg) const {
        std::cout << "Message: " << msg << std::endl;
    }

    void do_something_pod(MyPod &pod) const {
        std::cout << "From C++: " << pod.data[0][0] << std::endl;
    }
};

std::vector<std::reference_wrapper<const MessageProducer>> producers;
std::vector<std::reference_wrapper<const MessageDisplayer>> displayers;
CppExampleProducer cpp_producer;
CppExampleDisplayer cpp_displayer;


// Maybe we should use a language which tracks lifetimes
// better than this. If only such a language existed.
void register_displayer(const MessageDisplayer& displayer) {
    displayers.push_back(displayer);
}

void register_producer(const MessageProducer& producer) {
    producers.push_back(producer);
}

void register_cpp_thingies() {
    register_producer(cpp_producer);
    register_displayer(cpp_displayer);
}

void run_demo() {
    for (auto& producer: producers) {
        auto msg = producer.get().get_message();
        for (auto& displayer: displayers) {
            displayer.get().display_message(msg);
            MyPod pod;
            pod.foo = 1;
            pod.data = new float*[1];
            pod.data[0] = new float[1];
            pod.data[0][0] = 101.0;
            displayer.get().do_something_pod(pod);
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

