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
extern uint8            blueSensorAddress[6];
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
extern uint8            UUIDserial[16];

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

extern uint8            runBluetooth;
extern uint8            addressSelect;

//extern uint16           resetFlagCount;
extern uint8            bleResetRequest;

extern uint8            uuIdCommonBuffer[13];
extern uint8            serialNumbersBuffer[3];
extern uint8            UUIDserialBuffer[16];

extern uint8            uuIdCommon[13];
extern uint8            serialNumbers[3];
extern uint8            blueSensorAddressFound;

extern uint8            bleDisconnectRequest;
extern uint8            connected_seq;
extern uint8            bleConnectRequestFlag1;
extern uint8            bleConnectRequestFlag2;
//---------------------------------------------
void ReceiveBTMessage (uint8 BTdata)
{
   static uint16       byteCount = 0;
   const struct        ble_msg     *BTMessage;    //holds BLE message
   static struct       ble_header  BTHeader;      //holds header of message
   static uint8        payload[256];               //holds payload of message
   static uint16       pLength;
   
   //--------------------------------------------
   // this removes unwanted "0" at power up - DO NOT REMOVE
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
            // still loading in data
            payload[byteCount - 4] = BTdata;
            if (++byteCount >= 255)
               byteCount = 266;
            return;
         }
      }
      else
      {
         // this is along packet length which we ignore
         if (byteCount == (pLength + 3))
            byteCount = 0x00;
         else
            if(++byteCount >= 255)
               byteCount = 255;
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
//void trace(unsigned char t)
//{
//   static unsigned long counter = 0;
//   
//   if (++counter > 30000)
//   {
//      counter = 0;
//      shootCommsData(TRACE_HEADER);
//      shootCommsData(t);
//   }
//}
//---------------------------------------------
//void traceImmediate(unsigned char t)
//{
//      shootCommsData(TRACE_HEADER);
//      shootCommsData(t);
//}
//---------------------------------------------
void BLE_state_manage(void)
{
   static unsigned char blueStow = 0xFF;
   unsigned char n;
   
   //---
   if (blueState != blueStow)
   {
      blueStow = blueState;
      //shootCommsData(BLE_STATES_HEADER);
      //shootCommsData(blueState);
   } 
   //---
   switch(blueState)
   {
      //-----------------------------------------------------------------------
      case BLE_IN_RESET:                                       // blueState = 9
      {
         //---
         _LATB13 = 0; // BLE121LR reset to zero
         //---
         for (n = 0 ; n < 200 ; n++)
            Nop();
         //---
         _LATB13 = 1;
         //---   
         bglib_output = &SendBTMessage;
         //---
         for (n = 0 ; n < 13 ; n++)
            uuIdCommon[n] = uuIdCommonBuffer[n];
         for (n = 0 ; n < 3 ; n++)
            serialNumbers[n] = serialNumbersBuffer[n];
         for (n = 0 ; n < 16 ; n++)
            UUIDserial[n] = UUIDserialBuffer[n];
         //---
         bleJustReset = YES;
         blueTimer = 0;
         blueSensorAddressFound = NO;
         blueServicesFound = 0;
         blueServiceCount = 0;
         connected_seq = 0;
         if (bleConnectRequestFlag1 == SET)
         {
            bleConnectRequestFlag1 = RESET;
            bleConnectRequestFlag2 = RESET;
         }
         blueState = BLE_WAITING_FOR_RESET_CALL_BACK;
         //---
         break;
         //---
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_RESET_CALL_BACK:                    // blueState = 2
      {
         break;
      }   
      //-----------------------------------------------------------------------
      case BLE_START_DISCOVERY:                                // blueState = 3
      {
         //---
         if (bleResetRequest == SET)
         {
            bleResetRequest = RESET;
            blueState = BLE_IN_RESET;
         }
         else
         {
            /**************************************************
            * We have received the start up event from the 
            * BlE module. We can now start to searching for
            * advertising Sensors  
            *************************************************/ 
            ble_cmd_gap_discover(2); // we are looking for advertised sensors
            // command.c response to this call is "ble_rsp_gap_discover"
            blueState = BLE_WAITING_FOR_START_DISCOVERY_RESPONSE;
            blueTimer = 0;
         }
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_START_DISCOVERY_RESPONSE:           // blueState = 4
      {
         // we are waiting for "ble_rsp_gap_discover" response
         // need timeout here ?
         //---
         if (blueTimer > BLUETIMER_5_SECOND) 
            //blueState = BLE_START_DISCOVERY;
            blueState = BLE_IN_RESET;
         //---
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_DISCOVERY_MODE:                                 // blueState = 5
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
            connected_seq = 0; // new
         }    
         break;
       }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_GAP_END_PROCEDURE:                  // blueState = 6
      {
         // we do nothing here - it helps with reading the flow
         // do we need a timeout???
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_DISCOVERY_STOPPED:                              // blueState = 7
      {
         //----------------------------------------------------------
         // We have not found a Sensor so jump to BLE_START_DISCOVERY  
         // wait 250 milliseconds before doing so
         //----------------------------------------------------------
         if (blueTimer > 1) //BLUETIMER_0d5_SECOND
            //blueState = BLE_START_DISCOVERY;
            blueState = BLE_IN_RESET;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_START_CONNECTION:                               // blueState = 8
      { 
           uint8 blueAddress[6];
           
           //---
           blueAddress[0] = blueSensorAddress[0];
           blueAddress[1] = blueSensorAddress[1];
           blueAddress[2] = blueSensorAddress[2];
           blueAddress[3] = blueSensorAddress[3];
           blueAddress[4] = blueSensorAddress[4];
           blueAddress[5] = blueSensorAddress[5];
           //---
           ble_cmd_gap_connect_direct  (blueAddress, 
                                        blueSensorAddressType,
                                        CONN_INTERVAL_MIN,
                                        CONN_INTERVAL_MAX,
                                        TIME_OUT,
                                        LATENCY);
           // command.c response to this call is "ble_rsp_gap_connect_direct"
           blueTimer = 0;      
           blueState = BLE_WAITING_FOR_CONNECTION_CALL_BACK;
           break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_CONNECTION_CALL_BACK:              // blueState = 10
      {
         if (blueTimer > BLUETIMER_5_SECOND)
         {
            blueTimer = 0;
            blueState = BLE_IN_RESET;
         }
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_CONNECTING:                                    // blueState = 11
      {
         if (blueTimer > BLUETIMER_5_SECOND)
         {
            blueTimer = 0;
            blueState = BLE_IN_RESET;
         }   
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_CONNECTED:                                     // blueState = 12
      {        
         switch (connected_seq)
         {
            //------------------------
            case 0:
            {
               ble_cmd_attclient_read_by_group_type  (blueSensor.connection,
                                                      FIRST_HANDLE, 
                                                      LAST_HANDLE, 
                                                      2, 
                                                      UUIDdeviceInfo);
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
      case BLE_FINDING_SERVICES:                              // blueState = 13
      {
         break;
      } 
      //-----------------------------------------------------------------------
      case BLE_DISCOVERING_CHARACTERISTICS:                   // blueState = 14
      {
         if (blueServiceCount < blueServicesFound)
         {
            ble_cmd_attclient_find_information (blueService[blueServiceCount].connection, 
                                                blueService[blueServiceCount].start, 
                                                blueService[blueServiceCount].end);
            // command.c response to this call is "ble_evt_attclient_find_information_found"
            blueState = BLE_FINDING_CHARACTERISTICS;
            ++blueServiceCount;
            blueTimer = 0;
         }
         else
         {
            if (blueTimer > BLUETIMER_5_SECOND)
            {
               blueTimer = 0;
               blueState = BLE_IN_RESET;
               bleResetRequest = SET;
            }
         }
         break;
      }      
      //-----------------------------------------------------------------------
      case BLE_FINDING_CHARACTERISTICS:                       // blueState = 15
      {
         break;
      } 
      //-----------------------------------------------------------------------
      case BLE_INITIALIZE_DATA:                               // blueState = 16
      {
         ble_cmd_attclient_read_by_handle(blueSensor.connection, deviceNameHandle);
         blueState = BLE_INITIALIZING_DATA;
         blueTimer = 0;
         break;   
       }
      //-----------------------------------------------------------------------
      case BLE_INITIALIZING_DATA:                             // blueState = 17
      {
         break;              
      }
      //-----------------------------------------------------------------------
      case BLE_SETUP_STD_NOTIFICATIONS:                       // blueState = 18
      { 
         ble_cmd_attclient_attribute_write(blueSensor.connection, serialHandle + 1, 2, &NotificationsSetting);
         blueState = BLE_CONFIG_NOTIFICATIONS;
         blueTimer = 0;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_CONFIG_NOTIFICATIONS:                          // blueState = 19
      {
         break;
      }      
      //-----------------------------------------------------------------------
      case BLE_IDLE:                                          // blueState = 20
      {
         //---------------------------
         if (sensorStatus == CONNECTED)
         {
            //---
            if (shootPacketFlag == SET)
            {
               shootPacketFlag = RESET;
               blueTimer = 0;
               ble_cmd_attclient_attribute_write(blueSensor.connection, serialHandle, 20, &constructedPacket);
               blueState = BLE_WAITING_FOR_WRITE_ATTRIBUTES_RESPONSE;
            }
            else
            {
               //---
               if (bleDisconnectRequest == SET)
               {
                  bleDisconnectRequest = RESET;
                  ble_cmd_connection_disconnect(blueSensor.connection);
                  blueState = BLE_WAITING_FOR_DISCONNECTION;
                  blueTimer = 0;
               }
               //---
            }
            //---
         }
         //---------------------------
         bleConnectRequestFlag2 = SET;
         //---------------------------
         break;     
      }       
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_WRITE_ATTRIBUTES_RESPONSE:         // blueState = 22
      {
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_SERVER_ATTRIBUTES_RESPONSE:        // blueState = 23
      {
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_DISCONNECTED:                                  // blueState = 21
      {   
         if (bleResetRequest == SET)
         {
            bleResetRequest = RESET;
            blueState = BLE_IN_RESET;
         }
         blueTimer = 0;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_DISCONNECTION:                     // blueState = 24
      {
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

