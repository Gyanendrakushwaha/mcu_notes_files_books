/**
* Solution to 03 - preemtion test
* 
* 
* 
*/
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//some string message to print
const char msg[]="this is string";

//task handlers
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// delay rates
static const int rate_1 = 500;  // ms
static const int rate_2 = 500;  // ms


///tasks
//Task: print to serial terminla with lower priority
void startTask1(void *parameter){
  //count number of characters in string
  int msg_len = strlen(msg);

  //print string to terminal
  while(1){
    for(int i=0;i<msg_len; i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//TASK: print to serial terminal with heigher priority, so that it some time preemt the lower priority task
void startTask2(void *parameter){
  while(1){
    Serial.print('*');
    vTaskDelay(100/ portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  //configure the seral baud rate and go slow so that we can watch preemtion
  Serial.begin(300);

  //wait a moment to start (so we do not miss serial output)
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FREERTOS TASK Demo---");


  //print self priority
  Serial.print("setup and loop task running on core");
  Serial.print(xPortGetCoreID());  //Get the current core's ID
  Serial.print("with priority");
  Serial.println(uxTaskPriorityGet(NULL));

  //Task to run forever
  xTaskCreatePinnedToCore(startTask1, "Task 1",1024,NULL,1,&task_1,app_cpu);

  //task to run once with heigher priority
  xTaskCreatePinnedToCore(startTask2,"Task 2",1024,NULL,2,&task_2,app_cpu);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<3;i++){
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  //first check task_t exist or not then Delay the lower priority task
  if(task_1 != NULL){
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}
