using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Management;
using System.IO.Ports;

namespace WifiConfigClient {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e) {
            load_pref();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
            save_pref();
        }


            // errors that can be returned during the process
        const char CONFIG_ERROR_NO_ERROR = '0';     // OK!
        const char CONFIG_ERROR_SSID_LEN                   ='1';     // SSID len is either null or too long
        const char CONFIG_ERROR_PASSWORD_LEN               ='2';     // Password length is too long, or in WEP it's not 5 or 13 as it should be.
        const char CONFIG_ERROR_BAD_ENCRYPT_MODE           ='3';     // Encrypt mode is not a valid value
        const char CONFIG_ERROR_COULD_NOT_CONNECT          ='4';     // Could not connect to the new AP
        const char CONFIG_ERROR_COULD_NOT_SAVE             ='5';     // Error occurred while trying to save the new config
        const char CONFIG_ERROR_TIMEOUT                    ='6';     // a read operation timeout occurred
        const char CONFIG_ERROR_NO_CONFIG                  ='7';     // no config saved to restore
        const char CONFIG_ERROR_NO_DHCP                    ='8';     // could not get an IP
        const char CONFIG_ERROR_NO_DNS                     ='9';     // could not get the address of the back-end
        const char CONFIG_ERROR_NO_SERVER_CONNECTIVITY     ='a';     // could not ping the back-end
        const char CONFIG_ERROR_COULD_NOT_CONNECT_TIMEOUT  ='b';     // timeout while trying to connect to new AP
        const char CONFIG_ERROR_COULD_NOT_SET_PROFILE      ='c';     // setting the profile into the CC3000 failed


        /// <summary>
        /// Get a list of Plug'n'Play devices matching the pattern
        /// </summary>
        /// <param name="pattern"> Pattern contained in the device name. </param>
        /// <returns> Dictionnary of DeviceID, Device Name. </returns>
        public List<string> GetDevices(string pattern) {
            SelectQuery query = new SelectQuery("Win32_PnPEntity");
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(query);

            List<string> list = new List<string>();
            foreach (ManagementObject res in searcher.Get()) {
                if (res["Name"].ToString().Contains(pattern)) {
                    list.Add(res["Name"].ToString());
                }
            }
            return list;
        }

        public void save_pref() {
            if (cbComPort.Text != "") {
                Properties.Settings.Default.Com_Port_Name = cbComPort.Text;
            }
            Properties.Settings.Default.Ssid = tbSsid.Text;
            Properties.Settings.Default.Password = tbPassword.Text;
            Properties.Settings.Default.Encrypt_mode = gbEncrypt.Controls.OfType<RadioButton>().FirstOrDefault(r => r.Checked).Text;
            Properties.Settings.Default.Save();
        }

        public void load_pref() {
            cbComPort.Text = Properties.Settings.Default.Com_Port_Name;
            tbSsid.Text = Properties.Settings.Default.Ssid;
            tbPassword.Text = Properties.Settings.Default.Password;
            if (Properties.Settings.Default.Encrypt_mode != "") {
                var rb = gbEncrypt.Controls.OfType<RadioButton>().FirstOrDefault(r => r.Text == Properties.Settings.Default.Encrypt_mode);
                if (rb != null) {
                    rb.Checked = true;
                }
            }
        }


        private void comboBox1_DropDown(object sender, EventArgs e) {
            cbComPort.Items.Clear();
            var ports = GetDevices("(COM");
            cbComPort.Items.AddRange(ports.ToArray());
        }

        SerialPort ser = new SerialPort();

        public void update_status(string str) {
            tssl1.Text = str;
        }


        private bool openSerial() {
            if (ser.IsOpen) {
                update_status("Serial is already open.");
                return false;
            }
            update_status("Opening serial port");
            int com_pos = cbComPort.Text.ToUpper().LastIndexOf("COM");
            if (com_pos >= 0) {
                string number_str = cbComPort.Text.Substring(com_pos + 3);
                if (number_str.Length > 0) {
                    foreach (var c in number_str) {
                        if (!char.IsDigit(c)) {
                            number_str = number_str.Substring(0, number_str.IndexOf(c));
                            break;
                        }
                    }
                    int com_number;
                    if (int.TryParse(number_str, out com_number)) {
                        string com_name = "COM" + com_number;
    
                        try {
                            ser.PortName = com_name;
                            ser.Open();
                            ser.DtrEnable = true; // needed for the device to know the port is open.
                            return true;
                        }
                        catch (Exception) {
                            MessageBox.Show(
                                "Could not open port " + com_name + ". Please check that it is available and not already in use.",
                                "Error",
                                MessageBoxButtons.OK,
                                MessageBoxIcon.Error);
                        }
                        
                        return false;
                    }
                }
            }

            MessageBox.Show(
                "\"" + cbComPort.Text + "\" is not a valid COM port name.",
                "Error",
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);
            return false;
        }


        private char wait_for_answer(int timeout_s)
        {
            ser.ReadTimeout = timeout_s*1000;
            int buf; 

            try
            {
                ser.ReadTo("#");
                buf = ser.ReadByte();
                
                ser.ReadByte();
                ser.ReadByte();
                
                // TODO add debug output and logging

                return (char)buf;
            }
            catch (TimeoutException)
            {
                return CONFIG_ERROR_TIMEOUT;
            }

        }

        private bool print_error( char val ){
            string err_str = "Unknown error";
            bool ret_val = true;

            switch(val){
                case CONFIG_ERROR_NO_ERROR:
                    return true;
                case CONFIG_ERROR_SSID_LEN:
                    err_str = "Error: SSID len";
                    ret_val = false;
                    break;
                case CONFIG_ERROR_PASSWORD_LEN:
                    err_str = "Error: Password len";
                    ret_val = false;
                    break;
                case CONFIG_ERROR_BAD_ENCRYPT_MODE:
                    err_str = "Error: Bad encrypt mode";
                    ret_val = false;
                    break;
                case (CONFIG_ERROR_COULD_NOT_CONNECT):
                    err_str = "Error: Could not connect";
                    break;
                case (CONFIG_ERROR_COULD_NOT_SAVE):
                    err_str = "Error: Could not save";
                    break;
                case (CONFIG_ERROR_TIMEOUT):
                    err_str = "Error: Timeout";
                    ret_val = false;
                    break;
                case (CONFIG_ERROR_NO_CONFIG):
                    err_str = "Error: No config";
                    break;
                case (CONFIG_ERROR_NO_DHCP):
                    err_str = "Error: No DHCP";
                    break;
                case (CONFIG_ERROR_NO_DNS):
                    err_str = "Error: No DNS";
                    break;
                case (CONFIG_ERROR_NO_SERVER_CONNECTIVITY):
                    err_str = "Error: No server connectivity";
                    break;
                case (CONFIG_ERROR_COULD_NOT_CONNECT_TIMEOUT):
                    err_str = "Error: Timeout while trying to connect to AP";
                    break;
                case (CONFIG_ERROR_COULD_NOT_SET_PROFILE):
                    err_str = "Error: Could not set profile to verify it";
                    break;
            }

            update_status("An error occured : " + err_str + ".");
            MessageBox.Show(
                "An error occured : " + err_str + ".",
                "Error",
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);

            return ret_val;
        }


        private bool updateConfig()
        {
            if (!ser.IsOpen)
            {
                return false;
            }
            
            update_status("Sending new wifi config to the airboxlab...");

            try
            {
                ser.Write("w"); // get in wifi config mode

                char c = wait_for_answer(15);
                if (!print_error(c)) return false; 

                ser.Write(tbSsid.Text);
                ser.Write("\r"); // separator
                ser.Write(tbPassword.Text);
                ser.Write("\r"); // separator
                ser.Write(getEncryptionMode().ToString());

                c = wait_for_answer(5);
                if (!print_error(c)) return false;

                update_status("Waiting for information validation...");

                c = wait_for_answer(60);
                if (!print_error(c)) return false;
                if ( c == CONFIG_ERROR_NO_ERROR ){
                    update_status("Saving config...");
                    c = wait_for_answer(10);
                    if (!print_error(c)) return false;
                } else if ( c == CONFIG_ERROR_COULD_NOT_CONNECT
                    || c == CONFIG_ERROR_COULD_NOT_CONNECT_TIMEOUT
                    || c == CONFIG_ERROR_NO_DHCP
                    || c == CONFIG_ERROR_NO_DNS
                    || c == CONFIG_ERROR_NO_SERVER_CONNECTIVITY)
                {
                    // the device needs to be told if it needs to save the config anyway or not
                    DialogResult res = MessageBox.Show(
                       "The device cold not connect to the AP."
                       + Environment.NewLine
                       + "You can choose to force the use of these parameters if you know what you are doing. (60s remaining to choose)."
                       + Environment.NewLine
                       + "Do you want to save this configuration anyway?",
                       "Save configuration anyway?",
                       MessageBoxButtons.YesNo,
                       MessageBoxIcon.Error);
                    if (res == DialogResult.Yes ){
                        ser.Write("s"); 
                    }
                    else
                    {
                        ser.Write("n"); 
                    }
                    c = wait_for_answer(10);
                    if (!print_error(c)) return false;
                }
            }
            catch (Exception)
            {
                // TODO
                update_status("An error occured while trying to write the new configuration.");
                return false;
            }
            update_status("Done.");
            return true;
        }

        private void closeSerial()
        {
            if (!ser.IsOpen)
            {
                return;
            }

            try
            {
                ser.DtrEnable = false;
                ser.Close();
            }
            catch (Exception)
            {
                // the board rebooted before we could close, no big deal.
            }

        }

        int ENCRYPT_NONE = 0;
        int ENCRYPT_WEP = 1;
        int ENCRYPT_WPA = 2;
        int ENCRYPT_WPA2 = 3;

        private void bUpdate_Click(object sender, EventArgs e) {
            bool res = validateCredentials()
                    && openSerial()
                    && updateConfig();
            closeSerial();
            
        }

        private bool validateCredentials()
        {
            string key = tbPassword.Text.Trim();
            string ssid = tbSsid.Text.Trim();
            int encrypt = getEncryptionMode();

            if (ssid.Length == 0)
            {
                update_status("SSID cannot be empty.");
                return false;
            }
            if (encrypt >= ENCRYPT_WEP && key.Length == 0)
            {
                update_status("In this mode, the Password cannot be empty.");
                return false;
            }
            if (encrypt == ENCRYPT_WEP && key.Length != 5 && key.Length != 13 ) // WEP keys are 5 or 13 ASCII char long
            {
                update_status("WEP key must be in ASCII (5 or 13 characters long). Hex keys are not supported.");
                return false;
            }
            // SSID length and key are limited by the textbox

            return true;
        }

        private int getEncryptionMode()
        {
            var rb = gbEncrypt.Controls.OfType<RadioButton>().FirstOrDefault(r => r.Checked);

            if ( rb == rbEncryptNone ){
                return ENCRYPT_NONE;
            } else if ( rb == rbEncryptWEP ) {
                return ENCRYPT_WEP;
            } else if ( rb == rbEncryptWPA ) {
                return ENCRYPT_WPA;
            } else if ( rb == rbEncryptWPA2 ) {
                return ENCRYPT_WPA2;
            }
            else
            {
                throw new Exception();
            }
        }

        

    }
}
