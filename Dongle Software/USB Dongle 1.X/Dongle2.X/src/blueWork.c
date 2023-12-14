//---------------------------------------------
#include <xc.h>
#include "apitypes.h"
#include "blueStates.h"
#include "cmd_def.h"
#include "const.h"
#include "MCU_Layer.h"
//---------------------------------------------
typedef struct
{  
   uint8    connection;
   uint8    flags;
   bd_addr  address;
   uint8    AddressType;
   uint16   connectionInterval;
   uint16   timeout;
   uint16   latency;
   uint8    bonding;  
    
} CONNECTION;
//---------------------------------------------
typedef struct
{
    uint8      connection;
    uint16     start;
    uint16     end;
    uint8      uuidLength;
    uint8      uuid[16];

} BLE_SERVICE;
//---------------------------------------------
const uint8             NotificationsSetting[]  = {0x01, 0x00};
const uint8             IndicationsSetting[]    = {0x02, 0x00};
//---------------------------------------------
extern unsigned int     datas[1024];
extern uint16           dataCount;
extern uint8            bleJustReset;
extern uint8            blueState;
extern volatile uint16  blueTimer;
extern const uint8      uuIdCommon[13];
extern uint8            serialNumbers[3];
extern uint8            blueSensorAddress[6];
extern uint8            blueSensorAddressFound;
extern uint8            blueSensorAddressType;
extern volatile uint16  bleRxBufferCount;
extern volatile uint16  bleRxBufferMax;
extern CONNECTION       blueSensor;
extern const uint8      UUIDdeviceInfo[2];
extern uint8            blueServicesFound;
extern BLE_SERVICE      blueService[10];
extern uint8            blueServiceCount;

extern uint16           serialHandle;
extern uint8            serialData[20];
extern uint16           deviceNameHandle;
extern uint8            deviceNameUUID[2];
extern uint8            deviceNameData[20];
extern const uint8      UUIDserial[16];

extern unsigned char    blueyCount;

extern volatile uint16  disData1;
extern volatile uint16  disData2;
extern uint16           resetsCount;
extern volatile uint16  rawPressure;

extern unsigned char    sensorCalibrationPointRequest; 
extern unsigned char    sensorCalibrationPointRequestFlag;
extern volatile uint16  calibrationPoints[8];

extern uint8            bleModuleHasBooted;

extern unsigned char    sensorIdToCalibrate;
extern unsigned char    sensorStatus;
extern uint8            shootPacketFlag;
extern uint8            constructedPacket[20];

//---------------------------------------------
void ReceiveBTMessage (uint8 BTdata)
{
   static uint16       byteCount = 0;
   const struct        ble_msg     *BTMessage;    //holds BLE message
   static struct       ble_header  BTHeader;      //holds header of message
   static uint8        payload[60];               //holds payload of message
   static uint16       pLength;
   
   //--------------------------------------------
   // this removes unwanted "0" at power up
   //--------------------------------------------
   if (bleJustReset == YES)
   {
      if (BTdata == 0x80)
         bleJustReset = NO;
      else
         return;
   }
   //--- 1st BYTE ---
   if (byteCount == 0)
   {
      BTHeader.type_hilen = BTdata;
      ++byteCount;
      return;
   }
   //--- 2nd BYTE ---
   if (byteCount == 1)
   {
      BTHeader.lolen = BTdata;   
      ++byteCount;
      return;
   }   
   //--- 3rd BYTE ---
   if (byteCount == 2)
   {
      BTHeader.cls = BTdata; 
      ++byteCount;
      return;
   }
   //--- 4th BYTE ---
   if (byteCount == 3)
   {
      //---
      BTHeader.command = BTdata;
      //---
      // Calculate data length
      //---
      pLength = ((uint16)(BTHeader.type_hilen & 0x07) * 256) + (uint16)BTHeader.lolen;
      //---
      if (pLength == 0) // we have the entire message now as there are no data bytes
      {
         BTMessage = ble_get_msg_hdr(BTHeader);
         if (!BTMessage) 
         {
            //handle error here
            //displayValues(blueState, 667);
            byteCount = 0x00;
            return;
         }
         //call the handler for the received message, passing in the received payload data
         BTMessage->handler(payload);
         byteCount = 0x00;
         return;            
      }
      else
      {
         ++byteCount;
         return;
      }
   }
   //--- PAYLOAD BYTES ---
   if (byteCount >= 4)
   {
      if (pLength <= 60) 
      {
         // this is an acceptable packet length
         if (byteCount == (pLength + 3))
         {
            // we have received all the data bytes now
            payload[byteCount - 4] = BTdata; 
            BTMessage = ble_get_msg_hdr(BTHeader);
            if (!BTMessage) 
            {
               //handle error here
               //displayValues(blueState, 668);
               byteCount = 0x00;
               return;
            }
            //call the handler for the received message, passing in the received payload data
            BTMessage->handler(payload);
            byteCount = 0x00;
            //displayValues(blueState, pLength);
            return;
         }
         else
         {
            // still loading in data
            payload[byteCount - 4] = BTdata;
            ++byteCount;
            return;
         }
      }
      else
      {
         // this is along packet length which we ignore
         if (byteCount == (pLength + 3))
            byteCount = 0x00;
         else
            ++byteCount;
         return;
      }
   } 
}
//---------------------------------------------
void SendBTMessage (uint8 len1, uint8* data1, uint16 len2, uint8* data2)
{  
   uint8 n;
   
   //---
   for (n = 0; n < len1; n++)
      shootBleData(data1[n]);
   //---
   for (n = len1; n < len1 + len2; n++)   
      shootBleData(data2[n - len1]);
   //---
}
//---------------------------------------------
void BLE_state_manage(void)
{
   //shootCommsData(blueState);
   switch(blueState)
   {
      //-----------------------------------------------------------------------
      case BLE_IN_RESET:   // blueState = 9
      {
         if (blueTimer > 1)
            blueState = BLE_START;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_START:      // blueState = 1
      { 
         bglib_output = &SendBTMessage;
         bleJustReset = YES;
         _LATB13 = 1; // take BLE121LR out of reset 
         // command.c response to this hardware reset is "ble_evt_system_boot"
         blueTimer = 0;
         blueState = BLE_WAITING_FOR_RESET_CALL_BACK;
         break;  
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_RESET_CALL_BACK:     // blueState = 2
      {
         break;
      }   
      //-----------------------------------------------------------------------
      case BLE_START_DISCOVERY:  // blueState = 3
      {
         //sensorSearchId = sensorSetupId;
         /**************************************************
         * We have received the start up event from the 
         * BlE module. We can now start to searching for
         * advertising Sensors  
         *************************************************/ 
         ble_cmd_gap_discover(2); // we are looking for advertised sensors
         // command.c response to this call is "ble_rsp_gap_discover"
         blueState = BLE_WAITING_FOR_START_DISCOVERY_RESPONSE;
         blueTimer = 0;
         //tested looks like a problem here ???
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_START_DISCOVERY_RESPONSE: // blueState = 4
      {
         // we are waiting for "ble_rsp_gap_discover" response
         // need timeout here ?
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_DISCOVERY_MODE:      // blueState = 5
      {
         // waiting for a response from ble_cmd_gap_discover(2) ie "ble_rsp_gap_discover"
         // basically we have a timer here and if a suitable device is not found in a time period
         // the discovery period ends. 
         if (blueTimer > BLUETIMER_2_SECOND) // was 1
         { 
            ble_cmd_gap_end_procedure(); 
            // command.c response to this call is "ble_rsp_gap_end_procedure"
            blueState = BLE_WAITING_FOR_GAP_END_PROCEDURE;
            blueServicesFound = 0;  // I think this is the best place for it too be ???
            blueServiceCount = 0;   // I think this is the best place for it too be ???
         }    
         break;
       }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_GAP_END_PROCEDURE://???
      {
         // we do nothing here - it helps with reading the flow
         // do we need a timeout???
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_DISCOVERY_STOPPED:
      {
         //----------------------------------------------------------
         // We have not found a Sensor so jump to BLE_START_DISCOVERY  
         // wait 250 milliseconds before doing so
         //----------------------------------------------------------
         //here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         if (blueTimer > 1) //BLUETIMER_0d5_SECOND
         {
            blueState = BLE_START_DISCOVERY;
         }
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_START_CONNECTION://
      { 
           //---
           uint8 blueAddress[6];
           //---
           blueAddress[0] = blueSensorAddress[0];
           blueAddress[1] = blueSensorAddress[1];
           blueAddress[2] = blueSensorAddress[2];
           blueAddress[3] = blueSensorAddress[3];
           blueAddress[4] = blueSensorAddress[4];
           blueAddress[5] = blueSensorAddress[5];
           //---
           ble_cmd_gap_connect_direct  (blueAddress,        //0 xxxx
                                        blueSensorAddressType,    //0 xxxx
                                        CONN_INTERVAL_MIN,
                                        CONN_INTERVAL_MAX,
                                        TIME_OUT,
                                        LATENCY);
           // command.c response to this call is "ble_rsp_gap_connect_direct"
           blueTimer = 0;      
           blueState = BLE_WAITING_FOR_CONNECTION_CALL_BACK;//10
           break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_CONNECTION_CALL_BACK://???
      {
         if (blueTimer > BLUETIMER_5_SECOND)
         {
            blueTimer = 0;
         }
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_CONNECTING:
      {
         if (blueTimer > BLUETIMER_5_SECOND)
         {
            blueTimer = 0;
            ble_cmd_gap_end_procedure();    
            // we have given up or what ???
            // where do we go ???
         }   
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_CONNECTED:
      {
         static uint8_t connected_seq = 0;
         
         switch (connected_seq)
         {
            //------------------------
            case 0:
            {
               ble_cmd_attclient_read_by_group_type  (blueSensor.connection,   //0 xxxx
                                                      FIRST_HANDLE, 
                                                      LAST_HANDLE, 
                                                      2, 
                                                      UUIDdeviceInfo); //uint8_t UUIDdeviceInfo[] = {0x00, 0x28}; // Service
               // command.c response to this call is "ble_rsp_attclient_read_by_group_type"
               blueState = BLE_FINDING_SERVICES;
               connected_seq = 1;
               break;
            } 
            //------------------------
            case 1:
            {
               blueState =  BLE_DISCOVERING_CHARACTERISTICS;
               connected_seq = 0;
               break;
            }
            //------------------------
         }
         blueTimer = 0;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_FINDING_SERVICES:
      {
         // Just Hold here, waiting for completion          
         //TODO: add time out error functions
         break;
      } 
      //-----------------------------------------------------------------------
      case BLE_DISCOVERING_CHARACTERISTICS://14
      {
         if (blueServiceCount < blueServicesFound)
         {
            ble_cmd_attclient_find_information (blueService[blueServiceCount].connection, 
                                                blueService[blueServiceCount].start, 
                                                blueService[blueServiceCount].end);
            // command.c response to this call is "ble_evt_attclient_find_information_found"
            blueState = BLE_FINDING_CHARACTERISTICS;
            ++blueServiceCount;
            break;
         }
         break;
      }      
      //-----------------------------------------------------------------------
      case BLE_FINDING_CHARACTERISTICS:
      {
         // Just Hold here, waiting for completion 
         //TODO: add time out error functions  
         break;
      } 
      //-----------------------------------------------------------------------
      case BLE_INITIALIZE_DATA:
      {
         ble_cmd_attclient_read_by_handle(blueSensor.connection, deviceNameHandle); //0 0xxxx
         blueState = BLE_INITIALIZING_DATA;
         blueTimer = 0;
         break;   
       }
      //-----------------------------------------------------------------------
      case BLE_INITIALIZING_DATA:
      {
         break;              
      }
      //-----------------------------------------------------------------------
      case BLE_SETUP_STD_NOTIFICATIONS:
      { 
         ble_cmd_attclient_attribute_write(blueSensor.connection, serialHandle + 1, 2, &NotificationsSetting); // 0 xxxx
         blueState = BLE_CONFIG_NOTIFICATIONS;
         blueTimer = 0;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_CONFIG_NOTIFICATIONS:
      {
         // waiting
         break;
      }      
      //-----------------------------------------------------------------------
      case BLE_IDLE:
      {
         if (sensorStatus == CONNECTED)
         {
            if (shootPacketFlag == SET)
            {
               shootPacketFlag = RESET;
               // Here we transmit the 20 byte packet
               blueTimer = 0;
               ble_cmd_attclient_attribute_write(blueSensor.connection, serialHandle, 20, &constructedPacket);
               blueState = BLE_WAITING_FOR_WRITE_ATTRIBUTES_RESPONSE;
            }
         }
         break;     
      }       
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_WRITE_ATTRIBUTES_RESPONSE:
      {
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_SERVER_ATTRIBUTES_RESPONSE:
      {
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_DISCONNECTED:
      {
           blueState = BLE_START_DISCOVERY;
           break;
      }
      //-----------------------------------------------------------------------
      default:
      {
         // this is an error state as we have basically found a blueState that is not defined
         break;
      }
      //-----------------------------------------------------------------------
   }
}
//---------------------------------------------

