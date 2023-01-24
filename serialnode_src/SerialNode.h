#include "Arduino.h"

/*
  * A simple list object only for a string
  * Will be used for listing existing topics
*/
struct TopicList {
    /*
      * Element structure will define each element inside a list
    */
    struct Element {
        Element* next;
        String data;

        Element() {
            this->next = nullptr;
            this->data = "_";
        }
    };
    
    /*
      * The following is the members and methods of List structure
    */
    Element* root;
    unsigned short length;

    TopicList() {
        root = new Element();
        length = 0;
    }

    void append(String val) {
        Element* temp = root;
        while (true) {
            if (temp->next == nullptr) {
                temp->data = val;
                temp->next = new Element();
                break;
            }
            else if (temp->next != nullptr) {
                temp = temp->next;
            }
        }
        ++length;
    }

    String getElement(unsigned short key) {
        if (key < length) {
            Element* temp = root;
            for (unsigned short i = 0; i < key; ++i) {
                temp = temp->next;
            }
            return temp->data;
        }

        else {
            return "0";
        }
    }
};


/*
  * NodeHandler handles the quasi-two-way-communication from the Arduino side.
  * refresh_rate determines the publishing frequency (<= 1000 Hz).
  * queue_size determines how many incoming topics will be received at one spin().
*/
class SerialNode {


    private:
        unsigned short  refresh_rate,
                        queue_size;

        unsigned long   started_t,
                        elapsed_t,
                        period;

        TopicList   topic_name,
                    topic_data;

        bool  pub_allow = true;
    

    public:
        SerialNode(unsigned short refresh_rate, unsigned short queue_size);
        void spin();


    public:
        /*
          * publish() template method is basically a Serial.println().
          * Each message starts with a topic name as a head separated with a slash '/'
        */
        template <class T> void publish(String topic, T msg) {
            if (this->pub_allow) {
                Serial.print(topic + "/");
                Serial.println(msg);
            }
        }


        /*
          * subscribe() template method returns the message based on given topic.
          * The message is in String object.
        */
        String subscribe(String topic) {
            if (topic_name.root->data == "_") {
                return "_";
            }

            else {
                TopicList::Element* temp = topic_name.root;
                for (unsigned short i = 0; i < topic_name.length; ++i) {
                    if (temp->data == topic) {
                        return topic_data.getElement(i);
                    }
                    else if (temp->next != nullptr) {
                        temp = temp->next;
                    }
                    else {
                      return "_";
                    }
                }
            }    
        }
};