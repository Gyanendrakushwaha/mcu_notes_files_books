#if CONFIG_FREETOS_UNICORE
  static const BaseType_t app_cpu =0;
#else
  static const BaseType_t app_cpu =1;
#endif

//setting length of queue
static const uint8_t msg_queue_len=5;

//globals
static QueueHandle_t msg_queue;


//task1 of gettin the msg
void printmessage(void *parameters){
    int item;

    //loop forever
    while(1){
      //see if there's a message in the queue (do not block)
      if(xQueueReceive(msg_queue,(void *)&item,0)==pdTRUE){
        Serial.println(item);
      }

      //wait before trying again
      vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

// //task to receive messages
// void receivemessages(void *parameters){

// }

//**************************************************************
//main runs as its own task with priority 1 on core 1
void setup(){
  //put your setup code here, to run once

    // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Heap Demo---");
  
  //create queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));
  if(msg_queue==NULL){
    Serial.println("error while creating queue");
  }

  //start print task
  xTaskCreatePinnedToCore(printmessage,
                          "Print Messages",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int num =0;

  //try to add item to queue for 10 ticks, fail if queue is full
  if(xQueueSend(msg_queue,(void *)&num,0)!= pdTRUE){
    Serial.println("Queue full");
  }

  num++;

  //wait before trying again
  vTaskDelay(1000/portTICK_PERIOD_MS);
}
