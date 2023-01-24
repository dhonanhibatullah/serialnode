#include "SerialNode.h"

SerialNode serial_node = SerialNode(1, 0);

void setup() {
  Serial.begin(115200);
}

void loop() {
  String pub_topic = "test_pub";
  String pub_msg   = "Publish testing";
  serial_node.publish<String>(pub_topic, pub_msg);

  String sub_topic = "test_sub";
  String res = serial_node.subscribe(sub_topic);
  if (res != "_") {
    Serial.print("\nSubscribe message = ");
    Serial.println(res + "\n");
  }

  serial_node.spin();
}
