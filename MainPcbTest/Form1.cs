using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Management;
using System.IO.Ports;
using System.IO;
using System.Threading;
using System.Collections;

namespace USB2AX_Test
{
    

    public partial class Form1 : Form
    {
        string under_test_DeviceId = "";
        string under_test_port = "";

        SerialPort under_test = new SerialPort();

        Int16 Atmel_VID = 0x03EB;
        Int16 ATmega32u2_DFU_PID = 0x2FF0;
        Int16 ATmega32u4_DFU_PID = 0x2FF4;

        List<TestStep> steps = new List<TestStep>();


        public Form1(){
            InitializeComponent();

            // initialize test step list
            svAtmegaDetect.myFunc = DetectATmega32u4;
            steps.Add(svAtmegaDetect);
            svProgramming.myFunc = RunBatchisp;
            steps.Add(svProgramming);
            svDeviceDetect.myFunc = DetectNewDevice;
            steps.Add(svDeviceDetect);
            svSerialOpen.myFunc = OpenNewDevice;
            steps.Add(svSerialOpen);
            svSerialTest.myFunc = RunSerialTest;
            steps.Add(svSerialTest);
            svSerialClose.myFunc = CloseDevice;
            steps.Add(svSerialClose);
            //svUninstall.myFunc = RunDevcon;
            //steps.Add(svUninstall);

            start_logging();
        }

        private void Form1_Load(object sender, EventArgs e){
            tbBatchispPath.Text = Properties.Settings.Default.BatchispPath;
            tbFirmwarePath.Text = Properties.Settings.Default.FirmwarePath;
        }

        //******
        // logging
        //******

        string log_file_path = "main_pcb_test_log.txt";
                    
        public void start_logging() {
            DateTime now = DateTime.Now;
            log("MainPcbTest started: " + now.ToLongDateString() + " @ " + now.ToLongTimeString());
        }

        public void log(string str) {
            try {
                File.AppendAllText(log_file_path, DateTime.Now.ToLongTimeString() + " " + str + Environment.NewLine);
                add_to_output(DateTime.Now.ToLongTimeString() + " " + str + Environment.NewLine);
            }
            catch (Exception) {
                // whatever
            }
        }

        public void err(string str) {
            log("ERROR: " + str);
        }
        public void warn(string str) {
            log("Warning: " + str);
        }


        /**********************************************************************
         * 
         * Test and setup
         * 
         * ********************************************************************/

        /// <summary>
        /// Run all tests and setup.
        /// </summary>
        /// <returns> true if all setup and tests are OK. </returns>
        public bool TestAll() {
            log("TEST All:");
            lBatchispOk.Visible = TestBatchisp(tbBatchispPath.Text);
            lFirmwareOk.Visible = TestFirmware(tbFirmwarePath.Text);

            return lBatchispOk.Visible && lFirmwareOk.Visible;
        }

        /// <summary>
        /// Tests if BatchISP can be ran successfully.
        /// </summary>
        /// <param name="path"> Path to batchisp.exe </param>
        /// <returns> true if BatchISP can be ran successfully. </returns>
        public bool TestBatchisp(string path) {
            log("TEST BatchISP");
            Process p = new Process();
            p.StartInfo.FileName = path;
            p.StartInfo.Arguments = "-version";
            p.StartInfo.CreateNoWindow = true; // don't show any window
            p.StartInfo.UseShellExecute = false;   // needed to redirect ant output
            p.StartInfo.RedirectStandardOutput = true; // redirect output
            string output;
            try {
                p.Start();
                if (!p.WaitForExit(5000)) {
                    p.Kill();
                }
                output = p.StandardOutput.ReadToEnd();
                Console.WriteLine(output);
            }
            catch (Exception e) {
                err("Failed running BatchISP. Exception was:");
                log(e.ToString() + Environment.NewLine);
                MessageBox.Show(
                            "An error occured while trying to run BatchISP.\nPlease verify the path to batchisp.exe and that it can be run.",
                            "Error",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
                return false;
            }
            if (!output.StartsWith("batchisp")) {
                err("BatchISP ran, but did not answer as expected. Answer was:");
                log(output + Environment.NewLine);
                MessageBox.Show(
                            "BatchISP did not answer as expected. Please verify the path to batchisp.exe and that it can be run.",
                            "Error",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
                return false;
            }
            log("Test BatchISP OK");
            return true;
        }

        /// <summary>
        /// Tests if the firmware is OK.
        /// </summary>
        /// <param name="path"> Path to the firmware. </param>
        /// <returns> true if the firmware is OK (exists, is a file, and is a .hex). </returns>
        public bool TestFirmware(string path) {
            log("TEST Firmware");
            try {
                File.Open(path, FileMode.Open).Close();
                if (path.EndsWith(".hex")) {
                    log("Test Firmware OK. File is : " + path);
                    return true;
                }
                else {
                    err("Firmware is not a .hex... Selected file was: " + path);
                    MessageBox.Show(
                            "The Firmware is not a .hex file.",
                            "Error",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);

                    return false;
                }
            }catch(Exception e){
                err("Firmware could not be opened... Selected file was: " + path);
                log(e.ToString() + Environment.NewLine);
                MessageBox.Show(
                            "The Firmware file could not be opened.",
                            "Error",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
                return false;
            }
        }


        /// <summary>
        /// Get a list of Plug'n'Play devices matching the pattern
        /// </summary>
        /// <param name="pattern"> Pattern contained in the device name. </param>
        /// <returns> Dictionnary of DeviceID, Device Name. </returns>
        public Dictionary<string,string> GetDevices(string pattern){
            SelectQuery query = new SelectQuery("Select * from Win32_PnPEntity");
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(query);

            Dictionary<string,string> list = new Dictionary<string,string>();
            foreach (ManagementObject res in searcher.Get())
            {
                if (res["Name"].ToString().Contains(pattern))
                {
                    list.Add(res["DeviceID"].ToString(),res["Name"].ToString());
                }
            }
            return list;
        }

        /// <summary>
        /// Detect if there is an ATmega32u4 DFU connected.
        /// </summary>
        /// <returns> true if a blank ATmega32u4 is successfully detected. </returns>
        public bool DetectATmega32u4() {
            return GetDevices("ATmega32U4").Count > 0;
        }

        /// <summary>
        /// Program the ATmega32u4 with the firmware using BatchISP.
        /// </summary>
        /// <returns> true if the ATmega32u4 has been successfully programmed. </returns>
        public bool RunBatchisp() {
            string args = "-device atmega32u4 -hardware usb "
                + "-operation erase f memory flash blankcheck loadbuffer "
                + "\"" + tbFirmwarePath.Text + "\"" + " program verify start reset 0";

            log("Running BatchISP.");

            // run batchisp to program the device
            Process p = new Process();
            p.StartInfo.FileName = tbBatchispPath.Text;
            p.StartInfo.Arguments = args;
            p.StartInfo.CreateNoWindow = true; // don't show any window
            p.StartInfo.UseShellExecute = false;   // needed to redirect any output
            p.StartInfo.RedirectStandardOutput = true; // redirect output
            p.StartInfo.RedirectStandardInput = true;
            string output;
            try {
                p.Start();

                if (!p.WaitForExit(5000)) {
                    p.Kill();
                }
                output = p.StandardOutput.ReadToEnd();
                //Console.WriteLine("***" + p.ExitCode + "***");
                //log("Exit code: " + p.ExitCode);
                //Console.WriteLine(output);
            }
            catch (Exception e) {
                err("");
                log(e.ToString() + Environment.NewLine);
                MessageBox.Show(
                            "An error occured while trying to run BatchISP.\n",
                            "Error",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
                return false;
            }
            if (!output.Trim().EndsWith("Summary:  Total 11   Passed 11   Failed 0")) {
                //Console.WriteLine("...." + output + "....");
                err("Not the expected output. Output :");
                log(output + Environment.NewLine);
                MessageBox.Show(
                            "En error occured while programming.\nTODO manage this error.",
                            "Error",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
                return false;
            }
            else {
                log("BatchISP OK.");
                return true;
            }
        }

        /// <summary>
        /// Detect the new device, and wait for its driver to be installed.
        /// </summary>
        /// <returns> true if a new device has been successfully detected. </returns>
        public bool DetectNewDevice() {
            DateTime stop_time = DateTime.Now + new TimeSpan(0, 0, 60);
            log("Detecting new Device...");

            string device_signature = "LUFA CDC-ACM Virtual Serial Port (";

            do {
                var devices = GetDevices(device_signature);
                switch (devices.Count) {
                    case 0:
                        // That's OK, Waiting to detect the new device...
                        break;
                    case 1:
                        under_test_DeviceId = devices.Keys.ElementAt(0);
                        under_test_port = devices[under_test_DeviceId].Substring(device_signature.Length).Trim(')');
                        log("Detected new device : " + under_test_port);
                        return true;
                        break;
                    default:
                        err("Too many devices detected: " + devices.Count);
                        // Error too many devices at once!
                        MessageBox.Show(
                                "Found too many Devices...\nPlease connect only one at a time.",
                                "Error",
                                MessageBoxButtons.OK,
                                MessageBoxIcon.Error);
                        return false;
                }
                Thread.Sleep(250);
            } while (DateTime.Now < stop_time);
            err("Timeout.");
            return false; //Timeout
        }

        /// <summary>
        /// Open the newly detected device.
        /// </summary>
        /// <returns> true if the serial port has been opened successfully. </returns>
        public bool OpenNewDevice() {
            log("Opening new device : " + under_test_port);
            // open serial port, get it from devices[device_tester]
            try {
                under_test.PortName = under_test_port;
                under_test.BaudRate = 1000000;
                under_test.ReadTimeout = 2000;
                under_test.Open();
            }
            catch (Exception e) {
                err("Could not open. Exception was :");
                log(e.ToString() + Environment.NewLine);
                MessageBox.Show(
                    "Found a new device on port \"" + under_test_port + "\", but could not open it.\nMaybe it's already in use?",
                    "Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                return false;
            }
            log("Opened successfully.");
            return true;
        }

        /// <summary>
        ///  Test serial communications.
        /// </summary>
        /// <returns> true if serial communication is OK. </returns>
        public bool RunSerialTest() {
            // send and receive stuff to test that it works. Retry if needed.

            DialogResult res = DialogResult.None;
            do {
                log("Testing serial port...");
                try {
                    
                    under_test.DiscardInBuffer();
                    under_test.DiscardOutBuffer();

                    under_test.Write("a"); // request a complete test
                    Thread.Sleep(100);
                    // the test should return "MMA8435 OK", "CC2D23 OK" and "CC3000 OK" one after the other
                    // the first two should take <1s
                    // the third one can take around 5s
                    // let's spread them over 4.5s and the serial timeout will do the rest
                    for (int i = 0; i < 3; i++) {
                        Thread.Sleep(1500);
                        var ret = under_test.ReadLine().Trim();
                        add_to_output(DateTime.Now.ToLongTimeString() + " " + ret + Environment.NewLine);
                        if (ret.EndsWith("error")) {
                            string msg = "Error while testing a component: " + ret + Environment.NewLine;
                            if (ret.StartsWith("MMA")) {
                                msg += "Please make sure the board is not moveded while the test is performed";
                            }
                            err("Component testing error...");
                            res = MessageBox.Show(
                                            msg,
                                            "Error",
                                            MessageBoxButtons.RetryCancel,
                                            MessageBoxIcon.Error);
                            break;
                        }
                    }
                    if (res == DialogResult.None){
                        return true;
                    }
                }
                catch (TimeoutException) {
                    err("Timeout...");
                    res = MessageBox.Show(
                                    "Timeout when trying to communicate. Please report this error and try the Communiaiton Test later in manual mode.",
                                    "Error",
                                    MessageBoxButtons.RetryCancel,
                                    MessageBoxIcon.Error);
                }
                catch (InvalidOperationException) {
                    err("Serial port not opened anymore.");
                    MessageBox.Show(
                                    "The serial port is not opened anymore...\nPlease report this error to nicolas.saugnier@gmail.com and restart the program.",
                                    "Error",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                    return false;
                }
                catch (Exception e) {
                    err("Failed. Exception was:");
                    log(e.ToString() + Environment.NewLine);
                    MessageBox.Show(
                                    "An error occured :" + e.Message + "\n\nPlease report this error to nicolas.saugnier@gmail.com and restart the program.",
                                    "Error",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                    return false;
                }
            } while (res == DialogResult.Retry);
            return false;
        }

        /// <summary>
        /// Close the Serial port of the Device under test.
        /// </summary>
        /// <returns> Always return true. Errors are silently ignored. </returns>
        public bool CloseDevice() {
            try {
                under_test.Close();
            } catch(Exception e){
                warn("Could not close properly. Exception was :");
                log(e.ToString() + Environment.NewLine);
               // silently ignore errors, they should not matter for the rest of the test procedure.
            }
            return true;
        }



        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e) {
            if (rbTestsAll.Checked) {
                EnableSteps(true);
            }
            else {
                EnableSteps(true); // TODO only enable the second part
            }
            while (!backgroundWorker1.CancellationPending) {

                InitSteps();

                foreach (var s in steps) {
                    if (backgroundWorker1.CancellationPending) {
                        return;
                    }
                    if (!s.Run()) {
                        break;
                    }
                    SetProgress(s.PercentUponCompletion);
                }
                if (pbTesting.Value == 100) {
                    log("----------------  TEST OK ------------------");
                    Thread.Sleep(4000);
                }
            }
            EnableSteps(false);
            set_gui_stopped();
        }


        /********************************************************************
         * 
         * Menus and GUI stuff
         * 
         * ******************************************************************/

        public void add_to_output(string str) {
            if (tbOutput.InvokeRequired) {
                Invoke(new Action(()=>add_to_output(str)));
            }
            else {
                tbOutput.AppendText(str);
                tbOutput.SelectionStart = tbOutput.Text.Length;
                tbOutput.ScrollToCaret();
                tbOutput.Refresh();
            }
        }

        public void set_gui_running() {
            if (tbOutput.InvokeRequired) {
                Invoke(new Action(() => set_gui_running()));
            }
            else {
                bRun.Enabled = false;
                bStop.Enabled = true;
                gbTests.Enabled = false;
            }
        }
        public void set_gui_stopped() {
            if (tbOutput.InvokeRequired) {
                Invoke(new Action(() => set_gui_stopped()));
            }
            else {
                bRun.Enabled = true;
                bStop.Enabled = false;
                gbTests.Enabled = true;
            }
        }

        private void InitSteps() {
            foreach (var s in steps) {
                s.Reset();
            }
            SetProgress(0);
        }
        
        public void EnableSteps(bool value) {
            if (gbSteps.InvokeRequired) {
                Invoke(new Action(() => EnableSteps(value)));
            }
            else {
                gbSteps.Enabled = value;
            }
        }

        protected void SetProgress(int percentage) {
            if (lProgressPercent.InvokeRequired) {
                Invoke(new Action(() => SetProgress(percentage)));
            }
            else {
                lProgressPercent.Text = percentage.ToString() + "%";
                pbTesting.Value = percentage;
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e){
            MessageBox.Show("Airboxlab Automated Test Application\nv2.2\nby Nicolas Saugnier \ne-mail: nicolas.saugnier@gmail.com", "About...", MessageBoxButtons.OK ,MessageBoxIcon.Information);
        }

        private void bStartTesting_Click(object sender, EventArgs e) {
            if (panelTest.Enabled) {
                tabControl1.SelectedTab = tabPageTest;
            }
            else {
                // Save paths, it's annoying to have to configure stuff over and over again.
                //Properties.Settings.Default.DevconPath = tbDevconPath.Text;
                Properties.Settings.Default.BatchispPath = tbBatchispPath.Text;
                Properties.Settings.Default.FirmwarePath = tbFirmwarePath.Text;
                Properties.Settings.Default.Save();

                // Check if everything is setup properly
                if (TestAll()) {
                    panelTest.Enabled = true;
                    panelManualTest.Enabled = true;
                    tabControl1.SelectedTab = tabPageTest;
                    panelSetup.Enabled = false;
                    bStartTesting.Text = "Resume Testing";
                }
            }
        }

        private void bSearch_Click(object sender, EventArgs e) {
            // Try to search some files
            switch (openFileDialog1.ShowDialog()) {
                case DialogResult.OK:
                    if (sender == bSearchBatchisp) {
                        tbBatchispPath.Text = openFileDialog1.FileName;
                    }
                    else if (sender == bSearchFirmware) {
                        tbFirmwarePath.Text = openFileDialog1.FileName;
                    }
                    break;
                default:
                    break;
            }
        }

        private void bRun_Click(object sender, EventArgs e) {
            backgroundWorker1.RunWorkerAsync();
            set_gui_running();

        }

        private void bStop_Click(object sender, EventArgs e) {
            backgroundWorker1.CancelAsync();
        }

        private void bUpload_Click(object sender, EventArgs e) {
            
        }

        private void bErase_Click(object sender, EventArgs e) {

        }

        private void bGetVersion_Click(object sender, EventArgs e) {

        }



    }
}


