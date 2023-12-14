//----------------------------------------------------------------------------
#include "cmd_def.h"
#include "blueStates.h"
#include "const.h"
#include "MCU_Layer.h"

#include <xc.h>
#include <string.h>
#include <stdint.h> // for Uint8/16/32 and Int8/16/32 data types
#include "apitypes.h"
//----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------
typedef struct
{
    uint8      connection;
    uint16     start;
    uint16     end;
    uint8      uuidLength;
    uint8      uuid[16];

} BLE_SERVICE;
//----------------------------------------------------------------------------
extern uint8               blueState;
extern volatile uint16     blueTimer;
extern const uint8         uuIdCommon[16];
extern uint8               serialNumbers[3];
extern uint8               blueSensorAddress[6];
extern uint8               blueSensorAddressFound;
extern uint8               blueSensorAddressType;
extern CONNECTION          blueSensor;
extern BLE_SERVICE         blueService[64];
extern uint8               blueServicesFound;
extern uint8               blueServiceCount;
extern uint8               UUIDserial[16];
extern uint8               serialConnection;
extern uint16              serialHandle;
extern uint8               serialLength;
extern uint8               serialData[20];
extern uint8               deviceNameConnection;
extern uint16              deviceNameHandle;
extern uint8               deviceNameLength;
extern uint8               deviceNameUUID[2];
extern uint8               deviceNameData[20];
extern unsigned char       sensorStatus;
extern volatile uint16     sensorDataTimerCount;

//----------------------------------------------------------------------------
uint8                      UUIDdeviceName[] = {0x00, 0x2A};
//----------------------------------------------------------------------------
uint16 convertSignedToUnsigned(uint16 msb, uint16 lsb)
{
   uint16 value;
   
   value = lsb + (256 * msb);
   if (value >= 32768)
      value = value - 32768;
   else
      value = value + 32768;
   return value;
}
//----------------------------------------------------------------------------
void ble_default(const void*v)
{
}

void ble_rsp_system_reset(const void* nul)
{
}

void ble_rsp_system_hello(const void* nul)
{
}

void ble_rsp_system_address_get(const struct ble_msg_system_address_get_rsp_t *msg)
{
}

void ble_rsp_system_reg_write(const struct ble_msg_system_reg_write_rsp_t *msg)
{
}

void ble_rsp_system_reg_read(const struct ble_msg_system_reg_read_rsp_t *msg)
{
}

void ble_rsp_system_get_counters(const struct ble_msg_system_get_counters_rsp_t *msg)
{
}

void ble_rsp_system_get_connections(const struct ble_msg_system_get_connections_rsp_t *msg)
{
}

void ble_rsp_system_read_memory(const struct ble_msg_system_read_memory_rsp_t *msg)
{
}

void ble_rsp_system_get_info(const struct ble_msg_system_get_info_rsp_t *msg)
{
}

void ble_rsp_system_endpoint_tx(const struct ble_msg_system_endpoint_tx_rsp_t *msg)
{
}

void ble_rsp_system_whitelist_append(const struct ble_msg_system_whitelist_append_rsp_t *msg)
{
}

void ble_rsp_system_whitelist_remove(const struct ble_msg_system_whitelist_remove_rsp_t *msg)
{
}

void ble_rsp_system_whitelist_clear(const void* nul)
{
}

void ble_rsp_system_endpoint_rx(const struct ble_msg_system_endpoint_rx_rsp_t *msg)
{
}

void ble_rsp_system_endpoint_set_watermarks(const struct ble_msg_system_endpoint_set_watermarks_rsp_t *msg)
{
}

void ble_rsp_system_aes_setkey(const void* nul)
{
}

void ble_rsp_system_aes_encrypt(const struct ble_msg_system_aes_encrypt_rsp_t *msg)
{
}

void ble_rsp_system_aes_decrypt(const struct ble_msg_system_aes_decrypt_rsp_t *msg)
{
}

void ble_rsp_flash_ps_defrag(const void* nul)
{
}

void ble_rsp_flash_ps_dump(const void* nul)
{
}

void ble_rsp_flash_ps_erase_all(const void* nul)
{
}

void ble_rsp_flash_ps_save(const struct ble_msg_flash_ps_save_rsp_t *msg)
{
}

void ble_rsp_flash_ps_load(const struct ble_msg_flash_ps_load_rsp_t *msg)
{
}

void ble_rsp_flash_ps_erase(const void* nul)
{
}

void ble_rsp_flash_erase_page(const struct ble_msg_flash_erase_page_rsp_t *msg)
{   
}

void ble_rsp_flash_write_data(const struct ble_msg_flash_write_data_rsp_t *msg)
{  
}

void ble_rsp_flash_read_data(const struct ble_msg_flash_read_data_rsp_t *msg)
{  
}

void ble_rsp_attributes_write(const struct ble_msg_attributes_write_rsp_t *msg)
{  
}

void ble_rsp_attributes_read(const struct ble_msg_attributes_read_rsp_t *msg)
{
}

void ble_rsp_attributes_read_type(const struct ble_msg_attributes_read_type_rsp_t *msg)
{
}

void ble_rsp_attributes_user_read_response(const void* nul)
{
}

void ble_rsp_attributes_user_write_response(const void* nul)
{
}

void ble_rsp_attributes_send(const struct ble_msg_attributes_send_rsp_t *msg)
{
}

void ble_rsp_connection_disconnect(const struct ble_msg_connection_disconnect_rsp_t *msg)
{
}

void ble_rsp_connection_get_rssi(const struct ble_msg_connection_get_rssi_rsp_t *msg)
{
}

void ble_rsp_connection_update(const struct ble_msg_connection_update_rsp_t *msg)
{
}

void ble_rsp_connection_version_update(const struct ble_msg_connection_version_update_rsp_t *msg)
{
}

void ble_rsp_connection_channel_map_get(const struct ble_msg_connection_channel_map_get_rsp_t *msg)
{
}

void ble_rsp_connection_channel_map_set(const struct ble_msg_connection_channel_map_set_rsp_t *msg)
{
}

void ble_rsp_connection_features_get(const struct ble_msg_connection_features_get_rsp_t *msg)
{
}

void ble_rsp_connection_get_status(const struct ble_msg_connection_get_status_rsp_t *msg)
{
}

void ble_rsp_connection_raw_tx(const struct ble_msg_connection_raw_tx_rsp_t *msg)
{
}

void ble_rsp_connection_slave_latency_disable(const struct ble_msg_connection_slave_latency_disable_rsp_t *msg)
{
}

void ble_rsp_attclient_find_by_type_value(const struct ble_msg_attclient_find_by_type_value_rsp_t *msg)
{
}

void ble_rsp_attclient_read_by_group_type(const struct ble_msg_attclient_read_by_group_type_rsp_t *msg)
{
   //uint8 connection,
   //uint16 result
   //displayValues(67, msg->result);
   //displayValues(68, msg->connection);
}

void ble_rsp_attclient_read_by_type(const struct ble_msg_attclient_read_by_type_rsp_t *msg)
{
}

void ble_rsp_attclient_find_information(const struct ble_msg_attclient_find_information_rsp_t *msg)
{
}

void ble_rsp_attclient_read_by_handle(const struct ble_msg_attclient_read_by_handle_rsp_t *msg)
{
   //displayValues(blueState, 707); // Craig leaves this blank
   //displayValues(78, msg->connection);
   //displayValues(79, msg->result);
}

void ble_rsp_attclient_attribute_write(const struct ble_msg_attclient_attribute_write_rsp_t *msg)
{
   blueState = BLE_WAITING_FOR_SERVER_ATTRIBUTES_RESPONSE;  // this is the main write back !!!!!
}

void ble_rsp_attclient_write_command(const struct ble_msg_attclient_write_command_rsp_t *msg)
{
   //blueState = BLE_IDLE;
}

void ble_rsp_attclient_indicate_confirm(const struct ble_msg_attclient_indicate_confirm_rsp_t *msg)
{
}

void ble_rsp_attclient_read_long(const struct ble_msg_attclient_read_long_rsp_t *msg)
{
}

void ble_rsp_attclient_prepare_write(const struct ble_msg_attclient_prepare_write_rsp_t *msg)
{
}

void ble_rsp_attclient_execute_write(const struct ble_msg_attclient_execute_write_rsp_t *msg)
{
}

void ble_rsp_attclient_read_multiple(const struct ble_msg_attclient_read_multiple_rsp_t *msg)
{
}

void ble_rsp_sm_encrypt_start(const struct ble_msg_sm_encrypt_start_rsp_t *msg)
{
}

void ble_rsp_sm_set_bondable_mode(const void* nul)
{
}

void ble_rsp_sm_delete_bonding(const struct ble_msg_sm_delete_bonding_rsp_t *msg)
{
}

void ble_rsp_sm_set_parameters(const void* nul)
{
}

void ble_rsp_sm_passkey_entry(const struct ble_msg_sm_passkey_entry_rsp_t *msg)
{
}

void ble_rsp_sm_get_bonds(const struct ble_msg_sm_get_bonds_rsp_t *msg)
{
}

void ble_rsp_sm_set_oob_data(const void* nul)
{
}

void ble_rsp_sm_whitelist_bonds(const struct ble_msg_sm_whitelist_bonds_rsp_t *msg)
{
}

void ble_rsp_gap_set_privacy_flags(const void* nul)
{
}

void ble_rsp_gap_set_mode(const struct ble_msg_gap_set_mode_rsp_t *msg)
{  
}

void ble_rsp_gap_discover(const struct ble_msg_gap_discover_rsp_t *msg)
{
   blueState = BLE_DISCOVERY_MODE;
}

void ble_rsp_gap_connect_direct(const struct ble_msg_gap_connect_direct_rsp_t *msg)
{
   blueState = BLE_CONNECTING; //11
   //displayValues(blueState, NO_SECOND_CHAR);
}

void ble_rsp_gap_end_procedure(const struct ble_msg_gap_end_procedure_rsp_t *msg)
{
   if (blueSensorAddressFound == NO)
   {
      blueState = BLE_DISCOVERY_STOPPED;
      blueTimer = 0;
   }
   else
      blueState = BLE_START_CONNECTION;
}

void ble_rsp_gap_connect_selective(const struct ble_msg_gap_connect_selective_rsp_t *msg)
{
}

void ble_rsp_gap_set_filtering(const struct ble_msg_gap_set_filtering_rsp_t *msg)
{
}

void ble_rsp_gap_set_scan_parameters(const struct ble_msg_gap_set_scan_parameters_rsp_t *msg)
{
}

void ble_rsp_gap_set_adv_parameters(const struct ble_msg_gap_set_adv_parameters_rsp_t *msg)
{
}

void ble_rsp_gap_set_adv_data(const struct ble_msg_gap_set_adv_data_rsp_t *msg)
{
}

void ble_rsp_gap_set_directed_connectable_mode(const struct ble_msg_gap_set_directed_connectable_mode_rsp_t *msg)
{
}

void ble_rsp_gap_set_initiating_con_parameters(const struct ble_msg_gap_set_initiating_con_parameters_rsp_t *msg)
{
}

void ble_rsp_hardware_io_port_config_irq(const struct ble_msg_hardware_io_port_config_irq_rsp_t *msg)
{
}

void ble_rsp_hardware_set_soft_timer(const struct ble_msg_hardware_set_soft_timer_rsp_t *msg)
{ 
}

void ble_rsp_hardware_adc_read(const struct ble_msg_hardware_adc_read_rsp_t *msg)
{
}

void ble_rsp_hardware_io_port_config_direction(const struct ble_msg_hardware_io_port_config_direction_rsp_t *msg)
{
}

void ble_rsp_hardware_io_port_config_function(const struct ble_msg_hardware_io_port_config_function_rsp_t *msg)
{
}

void ble_rsp_hardware_io_port_config_pull(const struct ble_msg_hardware_io_port_config_pull_rsp_t *msg)
{
}

void ble_rsp_hardware_io_port_write(const struct ble_msg_hardware_io_port_write_rsp_t *msg)
{
}

void ble_rsp_hardware_io_port_read(const struct ble_msg_hardware_io_port_read_rsp_t *msg)
{
}

void ble_rsp_hardware_spi_config(const struct ble_msg_hardware_spi_config_rsp_t *msg)
{
}

void ble_rsp_hardware_spi_transfer(const struct ble_msg_hardware_spi_transfer_rsp_t *msg)
{
}

void ble_rsp_hardware_i2c_read(const struct ble_msg_hardware_i2c_read_rsp_t *msg)
{
}

void ble_rsp_hardware_i2c_write(const struct ble_msg_hardware_i2c_write_rsp_t *msg)
{
}

void ble_rsp_hardware_set_txpower(const void* nul)
{
}

void ble_rsp_hardware_timer_comparator(const struct ble_msg_hardware_timer_comparator_rsp_t *msg)
{
}

void ble_rsp_hardware_io_port_irq_enable(const struct ble_msg_hardware_io_port_irq_enable_rsp_t *msg)
{
}

void ble_rsp_hardware_io_port_irq_direction(const struct ble_msg_hardware_io_port_irq_direction_rsp_t *msg)
{
}

void ble_rsp_hardware_analog_comparator_enable(const void* nul)
{
}

void ble_rsp_hardware_analog_comparator_read(const struct ble_msg_hardware_analog_comparator_read_rsp_t *msg)
{
}

void ble_rsp_hardware_analog_comparator_config_irq(const struct ble_msg_hardware_analog_comparator_config_irq_rsp_t *msg)
{
}

void ble_rsp_hardware_set_rxgain(const void* nul)
{
}

void ble_rsp_hardware_usb_enable(const struct ble_msg_hardware_usb_enable_rsp_t *msg)
{
}

void ble_rsp_test_phy_tx(const void* nul)
{
}

void ble_rsp_test_phy_rx(const void* nul)
{
}

void ble_rsp_test_phy_end(const struct ble_msg_test_phy_end_rsp_t *msg)
{
}

void ble_rsp_test_phy_reset(const void* nul)
{
}

void ble_rsp_test_get_channel_map(const struct ble_msg_test_get_channel_map_rsp_t *msg)
{
}

void ble_rsp_test_debug(const struct ble_msg_test_debug_rsp_t *msg)
{
}

void ble_rsp_test_channel_mode(const void* nul)
{
}

void ble_rsp_dfu_reset(const void* nul)
{
}

void ble_rsp_dfu_flash_set_address(const struct ble_msg_dfu_flash_set_address_rsp_t *msg)
{
}

void ble_rsp_dfu_flash_upload(const struct ble_msg_dfu_flash_upload_rsp_t *msg)
{
}

void ble_rsp_dfu_flash_upload_finish(const struct ble_msg_dfu_flash_upload_finish_rsp_t *msg)
{
}

void ble_evt_system_boot(const struct ble_msg_system_boot_evt_t *msg)
{
   blueState = BLE_START_DISCOVERY;
}

void ble_evt_system_debug(const struct ble_msg_system_debug_evt_t *msg)
{
}

void ble_evt_system_endpoint_watermark_rx(const struct ble_msg_system_endpoint_watermark_rx_evt_t *msg)
{
}

void ble_evt_system_endpoint_watermark_tx(const struct ble_msg_system_endpoint_watermark_tx_evt_t *msg)
{
}

void ble_evt_system_script_failure(const struct ble_msg_system_script_failure_evt_t *msg)
{
}

void ble_evt_system_no_license_key(const void* nul)
{  
}

void ble_evt_system_protocol_error(const struct ble_msg_system_protocol_error_evt_t *msg)
{
}

void ble_evt_flash_ps_key(const struct ble_msg_flash_ps_key_evt_t *msg)
{
}

void ble_evt_attributes_value(const struct ble_msg_attributes_value_evt_t *msg)
{
}

void ble_evt_attributes_user_read_request(const struct ble_msg_attributes_user_read_request_evt_t *msg)
{
}

void ble_evt_attributes_status(const struct ble_msg_attributes_status_evt_t *msg)
{  
}

void ble_evt_connection_status(const struct ble_msg_connection_status_evt_t *msg)
{ 
   blueSensor.connection = msg->connection;
   blueSensor.flags = msg->flags;
   blueSensor.address = msg->address;
   blueSensor.AddressType = msg->address_type;
   blueSensor.connectionInterval = msg->conn_interval;
   blueSensor.timeout = msg->timeout;
   blueSensor.latency = msg->latency;
   blueSensor.bonding = msg->bonding;
   blueState = BLE_CONNECTED; //12 
}

void ble_evt_connection_version_ind(const struct ble_msg_connection_version_ind_evt_t *msg)
{
}

void ble_evt_connection_feature_ind(const struct ble_msg_connection_feature_ind_evt_t *msg)
{
}

void ble_evt_connection_raw_rx(const struct ble_msg_connection_raw_rx_evt_t *msg)
{
}

void ble_evt_connection_disconnected(const struct ble_msg_connection_disconnected_evt_t *msg)
{
   //---
   blueState = BLE_DISCONNECTED;
   //---
   if (msg->connection == blueSensor.connection)
      sensorStatus = NOT_CONNECTED;
   //---
}

void ble_evt_attclient_indicated(const struct ble_msg_attclient_indicated_evt_t *msg)
{
}

void ble_evt_attclient_procedure_completed(const struct ble_msg_attclient_procedure_completed_evt_t *msg)
{
   switch(blueState)
   {
      //--------------------------------------
      case BLE_FINDING_SERVICES:
      {
         blueState = BLE_CONNECTED;
         break;
      }
      //--------------------------------------
      case BLE_FINDING_CHARACTERISTICS:
      {
         if (blueServiceCount < 2)
            blueState = BLE_DISCOVERING_CHARACTERISTICS;
         else 
            blueState = BLE_INITIALIZE_DATA;
         break;
      } 
      //--------------------------------------
      case BLE_INITIALIZE_DATA:
        {
            blueState = BLE_SETUP_STD_NOTIFICATIONS;
            break;
        }
      //--------------------------------------
      case BLE_CONFIG_NOTIFICATIONS:
      {
         blueState = BLE_IDLE;
         blueTimer = 0;      
         break;   
      } 
      //--------------------------------------
      case BLE_WAITING_FOR_SERVER_ATTRIBUTES_RESPONSE: // this is the second condition after a write
      {
         blueState = BLE_IDLE;
         blueTimer = 0;          
         sensorStatus = CONNECTED;                      
         break;   
      } 
      //--------------------------------------
      default:
      {
         break;
      }
      //--------------------------------------
   }
}

void ble_evt_attclient_group_found(const struct ble_msg_attclient_group_found_evt_t *msg)
{
   uint8 n;
   
   blueService[blueServicesFound].connection = msg->connection;
   blueService[blueServicesFound].start = msg->start;
   blueService[blueServicesFound].end = msg->end;
   blueService[blueServicesFound].uuidLength = msg->uuid.len;
   for (n = 0; n < msg->uuid.len; ++n)
      blueService[blueServicesFound].uuid[n] = msg->uuid.data[n];
   ++blueServicesFound;
}

void ble_evt_attclient_attribute_found(const struct ble_msg_attclient_attribute_found_evt_t *msg)
{
}

void ble_evt_attclient_find_information_found(const struct ble_msg_attclient_find_information_found_evt_t *msg)
{
   static uint8 n = 0;
   uint8 match;
   
   switch(msg->uuid.len)
   {
      //----------------------------------------------
      // Short UUIDs
      //----------------------------------------------
      case 2:
      {       
         if ((msg->uuid.data[0] == 0x00) && (msg->uuid.data[1] == 0x2A)) // DeviceName
         {
            deviceNameConnection = msg->connection;
            deviceNameHandle = msg->chrhandle;
            deviceNameLength = msg->uuid.len;
            for (n = 0 ; n < 2 ; n++)
               deviceNameUUID[n] = msg->uuid.data[n];
         }
         break;
      }
      //----------------------------------------------
      // Long UUIDs
      //----------------------------------------------
      case 16:
      {
         match = YES;
         for (n = 0; n < 16; n++)
         {
            if (UUIDserial[n] != msg->uuid.data[15 - n])
               match = NO;
         }
         if (match == YES)
         {
            serialConnection = msg->connection;
            serialHandle = msg->chrhandle;
            serialLength = msg->uuid.len;
         }
         else
         {
            // we have a problem, what do we do ????
         }
         break;
      }
      //----------------------------------------------
   }
}

void ble_evt_attclient_attribute_value(const struct ble_msg_attclient_attribute_value_evt_t *msg)
{  
   unsigned int i;
   
   //---
   if (msg->atthandle == deviceNameHandle)  
   {
      if (blueState == BLE_INITIALIZING_DATA)
         blueState = BLE_SETUP_STD_NOTIFICATIONS;
   }
   //---
   sensorDataTimerCount = 0;
   //---
      shootCommsData(START_HEADER);
      for (i = 0 ; i < 20 ; i++)
      {
         serialData[i] = msg->value.data[i];
         shootCommsData(serialData[i] >> 4);
         shootCommsData(serialData[i] & 0x0F);
      }
      shootCommsData(STOP_HEADER);
   //---
}

void ble_evt_attclient_read_multiple_response(const struct ble_msg_attclient_read_multiple_response_evt_t *msg)
{
}

void ble_evt_sm_smp_data(const struct ble_msg_sm_smp_data_evt_t *msg)
{
}

void ble_evt_sm_bonding_fail(const struct ble_msg_sm_bonding_fail_evt_t *msg)
{
}

void ble_evt_sm_passkey_display(const struct ble_msg_sm_passkey_display_evt_t *msg)
{
}

void ble_evt_sm_passkey_request(const struct ble_msg_sm_passkey_request_evt_t *msg)
{
}

void ble_evt_sm_bond_status(const struct ble_msg_sm_bond_status_evt_t *msg)
{
}

void ble_evt_gap_scan_response(const struct ble_msg_gap_scan_response_evt_t *msg)
{ 
   unsigned char n, match;
   
   //---
   match = YES;
   //---
   for (n = 0; n < 3; n++)
   {
      if (serialNumbers[2 - n] != msg->data.data[5 + n])  // 0
         match = NO;
   }       
   //---
   for (n = 0; n < 13; n++)
   {
      if (uuIdCommon[12 - n] != msg->data.data[8 + n])
         match = NO;
   }
   //---
   if (match == YES)   
   {
      //---
      // we have found the correct sensor ie serial numbers match
      //---
      for (n = 0 ; n < 6 ; n++)
         blueSensorAddress[n] = msg->sender.addr[n];
      //---
      blueSensorAddressType = msg->address_type;
      blueSensorAddressFound = YES;
   }    
   //---
}

void ble_evt_gap_mode_changed(const struct ble_msg_gap_mode_changed_evt_t *msg)
{
}

void ble_evt_hardware_io_port_status(const struct ble_msg_hardware_io_port_status_evt_t *msg)
{
}

void ble_evt_hardware_soft_timer(const struct ble_msg_hardware_soft_timer_evt_t *msg)
{
}

void ble_evt_hardware_adc_result(const struct ble_msg_hardware_adc_result_evt_t *msg)
{
}

void ble_evt_hardware_analog_comparator_status(const struct ble_msg_hardware_analog_comparator_status_evt_t *msg)
{
}

void ble_evt_dfu_boot(const struct ble_msg_dfu_boot_evt_t *msg)
{
}
