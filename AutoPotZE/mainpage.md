
/*! \mainpage AutoPot
This project is an automated plant watering system build using STM32F4 uc platform as hardware base and STM32 HAL library. <br/>
The program allows user to controll water level of plant and watering schedule using BLUETOOTH module.

- \ref bluetooth "Comunicating with board using BlueTooth"
- \ref pump "Controling the pump"
- \ref measureParameters "Measuremnt parameters"
- \ref measurement "Performing measurement"
- \ref demoApp "Demo app for Android"

\page bluetooth Comunicating with board using BlueTooth
User can communicate with board using simple protocol. Command should follow specific format, explained below:<br/>
In general command of constant length 18 starts with capital letter indicating specific action followed by /, every meaningful information should be separated by '/'. <br/>
Command list:

- Setting RTC time - "T/HOURS/MINUTES/WEEKDAY/" For instance "T/9/59/3/" sets current time to 9:59 Wednesday. <br/>

- Setting days between measurements - "F/HOUR_OF_MEASUREMENT/DAYS_BETWEEN_MEASUREMENTS/". Only 1-7 day periods are allowed. For instance "F/12/2/" will perform automatic measurements every other day at 12 AM. <br/>

- Setting amount of time that pump should water the plant - "A/AMOUNT/". For instance "A/20/59/3/" will make pump turn on for 20s. <br/>

- Requesting data from sensors - "D/" performs asynchronic measuremnet. For instance "D/" will start asynchronic measurement. <br/>

- Sending data - Sending data form uc to Bluetooth device - "S/SOIL_MOISTURE/TEMPERTURE/AIR_MOISTURE/" For instance "S/70/29/30/" means 70% soil moisture, 29 Celcius degrees and 30% air humidity. <br/>
Board uses UART Bluetooth module with baud rate 9600

\page pump Controling the pump
User can control two meaningful aspects of pump over Bluetooth: <br/>
Minimum water level that won't trigger pump - by default 30% soil moisture <br/>
How long watering should last - by default 10s<br/>
See \ref bluetooth for more information.<br/>

\page measureParameters Measuremnt parameters

The sensors used in this project can measure soil moisture, air humidity and air temperature. <br/>
Air humidity is represented in percent ranging from 30 to 90 in 0 C, 20 to 80 in 25 C and 20 to 80 in 50 C.<br/>
Temperature is represented in Celcius degrees ranging from 0 to 50 Celcius.<br/>
Soil moisture is represented in percent ranging from 0 to 100 percent.<br/>

\page measurement Performing measurement
To perform soil moisture call start_soil_moisture_measurement(ADC_HandleTypeDef* adc, DMA_HandleTypeDef* hdma_adc, uint32_t* value). If initilization was successful a positive integer is returned. End of measurement cause interput handler HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) to be called. <br/>
To perform humidity and temperature of air call DHT11_get_data_from_sensor(DHT_Data *DHT_Data, DHT_Config *config). The measurement is perform in polling mode takes about 19ms. Longer measurement indicate faulty read. Funtion returns DHT_OK if measurement was successful and DHT_ERROR otherwise. <br/>
See \ref measureParameters to get more information about data returned from sensors. <br/>

\page demoApp Demo app for Android
The demo app for Android allows to connect to board over bluetooth to check current soil and air parameters and force watering of the plant.
\image html scandevices.png width=400px
\image html device.png width=400px

