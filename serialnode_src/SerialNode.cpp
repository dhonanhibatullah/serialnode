#include "SerialNode.h"
#include <iostream>


/*----------- SerialNode Scope -----------*/

/*
  * Note that refresh_rate must be less than or equal to 1000.
  * And queue_size must be less than or equal to 50.
  * Or else, throw an error.
*/
SerialNode::SerialNode(unsigned short refresh_rate, unsigned short queue_size) {
    try {
        this->refresh_rate  = refresh_rate;
        this->queue_size    = queue_size;
        this->started_t     = millis();
        
        if (this->refresh_rate > 1000) {
            throw(this->refresh_rate);
        }

        if (this-> queue_size > 50) {
            throw(this->queue_size);
        }
    }
    catch(unsigned short e) {
        std::cout << "Refresh rate must be less than or equal to 1000" << std::endl;
        std::cout << "and queue size must be less than or equal to 50" << std::endl;
    }

    /*
      * Take the period from refresh_rate
    */
    this->period = (unsigned long)(1000.0/(float)refresh_rate);
}


/*
  * SerialNode::spin() method should be placed at the very bottom of a loop().
  * It is responsible for controlling the publish refresh rate and taking the incoming message queue.
*/
void SerialNode::spin() {
    /*
      * The following will control the refresh rate of publish based on refresh_rate
      * If spin() is 
    */
    this->elapsed_t = millis();
    if (this->elapsed_t - this->started_t >= this->period) {
        this->pub_allow = true;
        this->started_t = this->elapsed_t;
    }
    else {
        this->pub_allow = false;
    }

    /*
      * Retrieve the incoming message
      * Each message starts with a topic name as a head
    */
    if (Serial.available()) {
        topic_name = TopicList();
        topic_data = TopicList();

        for (unsigned short i = 0; i < queue_size; ++i) {
            /* get the topic name */
            String temp = Serial.readStringUntil('/');
            temp.trim();
            temp.remove(temp.lastIndexOf('/'));
            topic_name.append(temp);

            /* get the topic data */
            temp = Serial.readStringUntil('\n');
            temp.trim();
            topic_data.append(temp);
        }
    }
}