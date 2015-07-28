
#if 0



void process_bytes(){
	uint16_t nb_received = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);
	uint8_t c;
	
    
	for( uint16_t i = 0; i < nb_received ; i++ ){
		c = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
		
		switch (c){
			case 'l':
				CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
				USB_USBTask();
				for (uint8_t x = 0; x < 255; x++){
					led_set(255,20,x);
					Delay_MS(10);
				}
				led_clear();
				println("LED test finished");
				break;
            #ifdef ONBOARD_STUFF
			case 'm':{
				switch (motor_get()){
					case 0: // TODO use ranges instead of exact values
					println("Motor ON (PWM @255)");
					motor_set(255);
					break;
					case 255:
					println("Motor ON (PWM @100)");
					motor_set(100);
					break;
					case 100:
					println("Motor OFF");
					motor_set(0);
					break;
				}
			}break;
			case 'd':{
				fprintf(&USBSerialStream, "GP2Y PM: %.1f ug/m3\r\n", gp2y_get_pm() );
			}break;
			case 'k':
				fprintf(&USBSerialStream, "humidity: %.0f %%\r\n", cc2dxx_get_humidity());
				Delay_MS(100);
				fprintf(&USBSerialStream, "temp: %.2f deg C\r\n", cc2dxx_get_temperature());
			break;
			#endif
            
            
            #ifdef SENSORBOARD
			case 'i':
				fprintf(&USBSerialStream, "TCOV: %d\r\n", iaqengine_get_ppm());
			break;
			#endif
			
            
            
            #ifdef WIFI
			case 'w':
				wifi_print_MAC();
                wifi_print_ip();
                
			break;
			case 'y':
                wifi_config_set_dev();
			    wifi_connect_to_ap(wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode);
                wifi_print_ip();
                wifi_ping(192,168,1,1);
                
			break;
			case 's':
				wifi_resolve_backend_ip();
			break;
            /*case 'e':
                fprintf(&USBSerialStream, "is set? %u\r\n", wifi_config_is_set());
                fprintf(&USBSerialStream, "set dev %u\r\n", wifi_config_set_dev());
                fprintf(&USBSerialStream, "is set? %u\r\n", wifi_config_is_set());
                fprintf(&USBSerialStream, "save %u\r\n", wifi_config_save());
                                
                fprintf(&USBSerialStream, "%s %s %u\r\n", wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode);
                fprintf(&USBSerialStream, "set lol %u\r\n", wifi_config_set("lol", "lolilol", 2));
                fprintf(&USBSerialStream, "%s %s %u\r\n", wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode);

                fprintf(&USBSerialStream, "load %u\r\n", wifi_config_load());
                fprintf(&USBSerialStream, "%s %s %u\r\n", wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode);
                
                wifi_config_mem_clear();
                fprintf(&USBSerialStream, "clear then load %u\r\n", wifi_config_load());
            break;*/
            case 'p':{
                println("Trying to POST");
                CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                USB_USBTask();
                
                wifi_config_set_dev();
                /* // There is a problem with that, it hangs sometimes ><
                if (!wifi_config_is_set()){
                    if (!wifi_config_load()){
                        #ifdef DEV
                        printf("setting up DEV config");
                        wifi_config_set_dev();
                        #else
                        printf("No config anywhere!");
                        return;
                        // TODO what do we do if it's not configured?
                        #endif
                    }                        
                }*/
                
                fprintf(&USBSerialStream, "%s %s %u\r\n", wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode);
                CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                USB_USBTask();
                
                // TODO check if the SSID is available before trying to connect or it might hang... :/
                    
                if( !wifi_connect_to_ap(wifi_ap_ssid, wifi_ap_password, wifi_ap_encrypt_mode) == WERR_OK){
                    println("AP NOK");
                } else {
                    println("AP OK");
                    wifi_print_ip();

                    uint32_t backend_ip;
                    uint16_t backend_port = 80;
                        
                    //backend_ip = cc3000_IP2U32(54,221,218,154);
                    //backend_ip = cc3000_IP2U32(67,215,65,132);
                    //backend_ip = cc3000_IP2U32(192,168,1,11);
                    //backend_ip = cc3000_IP2U32(192,168,42,102);
                    backend_ip = wifi_resolve_backend_ip();
                    
                    print_ip_helper("backend", backend_ip);
                    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                    USB_USBTask();
                    
                    if (cc3000_ping(backend_ip, 3, 500, 32)){
                        fprintf(&USBSerialStream, "ping OK\r\n");
                    } else {
                        fprintf(&USBSerialStream, "ping timeout\r\n");
                    }
                    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                    USB_USBTask();
                    void* sock = cc3000_connectTCP(backend_ip, backend_port);
                        
                    if (cc3000_cli_connected(sock)){
                        println("cli connected");
                        
                        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                        USB_USBTask();
                        uint16_t res = 0;
                        
                        res += cc3000_cli_fastrprint(sock, "POST /v1/airboxlab/ HTTP/1.1\n");
                        res += cc3000_cli_fastrprint(sock, "User-Agent: abl1.0\n");
                        res += cc3000_cli_fastrprint(sock, "Host: api.airboxlab.com\n");
                        res += cc3000_cli_fastrprint(sock, "Accept: */*\n");
                        //res += cc3000_cli_fastrprint(sock, "Content-Type: application/json\n");
                        res += cc3000_cli_fastrprint(sock, "Content-Length: 35\n\n");
                        res += cc3000_cli_fastrprint(sock, "5040f3bd20c8,20.7,53.9,1255,11.1,78\n");
                        
                        fprintf(&USBSerialStream, "written: %u / %u\r\n", res, 171);
                        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                        USB_USBTask();
                        
                        Delay_MS(1000);
                        
                        uint8_t nb_available = cc3000_cli_available(sock);
                        fprintf(&USBSerialStream, "Returned %u\r\n", nb_available);
                        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                        USB_USBTask();
                        
                        for (int i = 0; i<cc3000_cli_available(sock); i++){
                            fprintf(&USBSerialStream, "%c" ,cc3000_cli_read(sock));
                        }
                        println("");
                        
                        cc3000_cli_close(sock);
                        println("closed");
                        
                    } else {
                        println("cli not connected");
                    }
                        
                        
                }
                                
                
                }break;/*
            case 'W':{ // setup wifi
                
                int nb_returned;
                println("SSID?");
                
                
                println("password?");
                println("encryption mode number?[0:none, 1:WEP, 2:WPA, 3:WPA2]");
                uint16_t _mode;
                
                CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                USB_USBTask();
                
                while(1){ // TODO timeout? opt-out?
                    _mode = fgetc(&USBSerialStream);
                    printf("read: %i", _mode);
                    if(_mode <= 3){
                        wifi_ap_encrypt_mode = _mode;
                        fprintf(&USBSerialStream, "Mode = %u\r\n", _mode);
                        break;
                        } else {
                        fprintf(&USBSerialStream, "Not a valid mode. Please choose number between [0:none, 1:WEP, 2:WPA, 3:WPA2]\r\n");
                        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                        USB_USBTask();
                    }
                }
                
                
            }break;*/
			#endif
            
			default:
				println("Airboxlab serial mode: press 'l' key to test LEDs");
				break;
		}
	}
}
#endif