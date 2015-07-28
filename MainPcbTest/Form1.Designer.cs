namespace USB2AX_Test
{
    partial class Form1
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gbSteps = new System.Windows.Forms.GroupBox();
            this.lProgressPercent = new System.Windows.Forms.Label();
            this.pbTesting = new System.Windows.Forms.ProgressBar();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageSetup = new System.Windows.Forms.TabPage();
            this.panelSetup = new System.Windows.Forms.Panel();
            this.label20 = new System.Windows.Forms.Label();
            this.lFirmwareOk = new System.Windows.Forms.Label();
            this.bSearchFirmware = new System.Windows.Forms.Button();
            this.tbFirmwarePath = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.lBatchispOk = new System.Windows.Forms.Label();
            this.bSearchBatchisp = new System.Windows.Forms.Button();
            this.tbBatchispPath = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.bStartTesting = new System.Windows.Forms.Button();
            this.tabPageTest = new System.Windows.Forms.TabPage();
            this.panelTest = new System.Windows.Forms.Panel();
            this.bStop = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.tbOutput = new System.Windows.Forms.TextBox();
            this.bRun = new System.Windows.Forms.Button();
            this.tabPageManualTest = new System.Windows.Forms.TabPage();
            this.panelManualTest = new System.Windows.Forms.Panel();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.bErase = new System.Windows.Forms.Button();
            this.bUpload = new System.Windows.Forms.Button();
            this.tbConsole = new System.Windows.Forms.TextBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.bSerialTest = new System.Windows.Forms.Button();
            this.bUninstall = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.bBootloader = new System.Windows.Forms.Button();
            this.bGetVersion = new System.Windows.Forms.Button();
            this.bUpdate = new System.Windows.Forms.Button();
            this.cbPort = new System.Windows.Forms.ComboBox();
            this.label19 = new System.Windows.Forms.Label();
            this.rbTestsAll = new System.Windows.Forms.RadioButton();
            this.gbTests = new System.Windows.Forms.GroupBox();
            this.rbTestsSerialOnly = new System.Windows.Forms.RadioButton();
            this.svUninstall = new USB2AX_Test.TestStep();
            this.svSerialClose = new USB2AX_Test.TestStep();
            this.svSerialTest = new USB2AX_Test.TestStep();
            this.svSerialOpen = new USB2AX_Test.TestStep();
            this.svDeviceDetect = new USB2AX_Test.TestStep();
            this.svAtmegaDetect = new USB2AX_Test.TestStep();
            this.svProgramming = new USB2AX_Test.TestStep();
            this.menuStrip1.SuspendLayout();
            this.gbSteps.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPageSetup.SuspendLayout();
            this.panelSetup.SuspendLayout();
            this.tabPageTest.SuspendLayout();
            this.panelTest.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.tabPageManualTest.SuspendLayout();
            this.panelManualTest.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.gbTests.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(677, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(24, 20);
            this.toolStripMenuItem1.Text = "?";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // gbSteps
            // 
            this.gbSteps.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbSteps.Controls.Add(this.svUninstall);
            this.gbSteps.Controls.Add(this.svSerialClose);
            this.gbSteps.Controls.Add(this.svSerialTest);
            this.gbSteps.Controls.Add(this.svSerialOpen);
            this.gbSteps.Controls.Add(this.svDeviceDetect);
            this.gbSteps.Controls.Add(this.svAtmegaDetect);
            this.gbSteps.Controls.Add(this.svProgramming);
            this.gbSteps.Controls.Add(this.lProgressPercent);
            this.gbSteps.Controls.Add(this.pbTesting);
            this.gbSteps.Enabled = false;
            this.gbSteps.Location = new System.Drawing.Point(3, 51);
            this.gbSteps.Name = "gbSteps";
            this.gbSteps.Size = new System.Drawing.Size(655, 338);
            this.gbSteps.TabIndex = 4;
            this.gbSteps.TabStop = false;
            this.gbSteps.Text = "Device under test";
            // 
            // lProgressPercent
            // 
            this.lProgressPercent.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lProgressPercent.AutoSize = true;
            this.lProgressPercent.BackColor = System.Drawing.Color.LimeGreen;
            this.lProgressPercent.Font = new System.Drawing.Font("Microsoft Sans Serif", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lProgressPercent.Location = new System.Drawing.Point(435, 262);
            this.lProgressPercent.Name = "lProgressPercent";
            this.lProgressPercent.Size = new System.Drawing.Size(125, 73);
            this.lProgressPercent.TabIndex = 11;
            this.lProgressPercent.Text = "0%";
            // 
            // pbTesting
            // 
            this.pbTesting.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pbTesting.Location = new System.Drawing.Point(9, 272);
            this.pbTesting.MarqueeAnimationSpeed = 0;
            this.pbTesting.Name = "pbTesting";
            this.pbTesting.Size = new System.Drawing.Size(420, 60);
            this.pbTesting.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.pbTesting.TabIndex = 10;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPageSetup);
            this.tabControl1.Controls.Add(this.tabPageTest);
            this.tabControl1.Controls.Add(this.tabPageManualTest);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.ItemSize = new System.Drawing.Size(42, 18);
            this.tabControl1.Location = new System.Drawing.Point(0, 24);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(677, 538);
            this.tabControl1.TabIndex = 5;
            // 
            // tabPageSetup
            // 
            this.tabPageSetup.Controls.Add(this.panelSetup);
            this.tabPageSetup.Controls.Add(this.bStartTesting);
            this.tabPageSetup.Location = new System.Drawing.Point(4, 22);
            this.tabPageSetup.Name = "tabPageSetup";
            this.tabPageSetup.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageSetup.Size = new System.Drawing.Size(669, 512);
            this.tabPageSetup.TabIndex = 1;
            this.tabPageSetup.Text = "Setup";
            this.tabPageSetup.UseVisualStyleBackColor = true;
            // 
            // panelSetup
            // 
            this.panelSetup.Controls.Add(this.label20);
            this.panelSetup.Controls.Add(this.lFirmwareOk);
            this.panelSetup.Controls.Add(this.bSearchFirmware);
            this.panelSetup.Controls.Add(this.tbFirmwarePath);
            this.panelSetup.Controls.Add(this.label17);
            this.panelSetup.Controls.Add(this.lBatchispOk);
            this.panelSetup.Controls.Add(this.bSearchBatchisp);
            this.panelSetup.Controls.Add(this.tbBatchispPath);
            this.panelSetup.Controls.Add(this.label15);
            this.panelSetup.Controls.Add(this.label13);
            this.panelSetup.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelSetup.Location = new System.Drawing.Point(3, 3);
            this.panelSetup.Name = "panelSetup";
            this.panelSetup.Size = new System.Drawing.Size(663, 371);
            this.panelSetup.TabIndex = 4;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(5, 112);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(468, 13);
            this.label20.TabIndex = 20;
            this.label20.Text = "The firmware is named MainPcb<version>.hex, where version is the release verion o" +
                "f the firmware.";
            // 
            // lFirmwareOk
            // 
            this.lFirmwareOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lFirmwareOk.AutoSize = true;
            this.lFirmwareOk.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lFirmwareOk.Location = new System.Drawing.Point(601, 112);
            this.lFirmwareOk.Name = "lFirmwareOk";
            this.lFirmwareOk.Size = new System.Drawing.Size(53, 31);
            this.lFirmwareOk.TabIndex = 18;
            this.lFirmwareOk.Text = "OK";
            this.lFirmwareOk.Visible = false;
            // 
            // bSearchFirmware
            // 
            this.bSearchFirmware.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bSearchFirmware.Location = new System.Drawing.Point(576, 126);
            this.bSearchFirmware.Name = "bSearchFirmware";
            this.bSearchFirmware.Size = new System.Drawing.Size(26, 23);
            this.bSearchFirmware.TabIndex = 17;
            this.bSearchFirmware.Text = "...";
            this.bSearchFirmware.UseVisualStyleBackColor = true;
            this.bSearchFirmware.Click += new System.EventHandler(this.bSearch_Click);
            // 
            // tbFirmwarePath
            // 
            this.tbFirmwarePath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbFirmwarePath.Location = new System.Drawing.Point(5, 128);
            this.tbFirmwarePath.Name = "tbFirmwarePath";
            this.tbFirmwarePath.Size = new System.Drawing.Size(565, 20);
            this.tbFirmwarePath.TabIndex = 16;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label17.Location = new System.Drawing.Point(5, 88);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(158, 24);
            this.label17.TabIndex = 15;
            this.label17.Text = "2. Select firmware";
            // 
            // lBatchispOk
            // 
            this.lBatchispOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lBatchispOk.AutoSize = true;
            this.lBatchispOk.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lBatchispOk.Location = new System.Drawing.Point(601, 24);
            this.lBatchispOk.Name = "lBatchispOk";
            this.lBatchispOk.Size = new System.Drawing.Size(53, 31);
            this.lBatchispOk.TabIndex = 13;
            this.lBatchispOk.Text = "OK";
            this.lBatchispOk.Visible = false;
            // 
            // bSearchBatchisp
            // 
            this.bSearchBatchisp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bSearchBatchisp.Location = new System.Drawing.Point(576, 63);
            this.bSearchBatchisp.Name = "bSearchBatchisp";
            this.bSearchBatchisp.Size = new System.Drawing.Size(26, 23);
            this.bSearchBatchisp.TabIndex = 11;
            this.bSearchBatchisp.Text = "...";
            this.bSearchBatchisp.UseVisualStyleBackColor = true;
            this.bSearchBatchisp.Click += new System.EventHandler(this.bSearch_Click);
            // 
            // tbBatchispPath
            // 
            this.tbBatchispPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbBatchispPath.Location = new System.Drawing.Point(9, 65);
            this.tbBatchispPath.Name = "tbBatchispPath";
            this.tbBatchispPath.Size = new System.Drawing.Size(561, 20);
            this.tbBatchispPath.TabIndex = 9;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(5, 24);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(424, 39);
            this.label15.TabIndex = 6;
            this.label15.Text = "batchisp.exe is found in the folder where Atmel FLIP is installed.\r\nThe path usua" +
                "lly looks like C:\\Program Files (x86)\\Atmel\\Flip <version>\\bin\\batchisp.exe\r\nwhe" +
                "re <version> is the version of FLIP.\r\n";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(5, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(261, 24);
            this.label13.TabIndex = 4;
            this.label13.Text = "1. Set the path to batchisp.exe";
            // 
            // bStartTesting
            // 
            this.bStartTesting.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.bStartTesting.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bStartTesting.Location = new System.Drawing.Point(3, 374);
            this.bStartTesting.Name = "bStartTesting";
            this.bStartTesting.Size = new System.Drawing.Size(663, 135);
            this.bStartTesting.TabIndex = 1;
            this.bStartTesting.Text = "Check Setup and Start Testing";
            this.bStartTesting.UseVisualStyleBackColor = true;
            this.bStartTesting.Click += new System.EventHandler(this.bStartTesting_Click);
            // 
            // tabPageTest
            // 
            this.tabPageTest.Controls.Add(this.panelTest);
            this.tabPageTest.Location = new System.Drawing.Point(4, 22);
            this.tabPageTest.Name = "tabPageTest";
            this.tabPageTest.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTest.Size = new System.Drawing.Size(669, 512);
            this.tabPageTest.TabIndex = 0;
            this.tabPageTest.Text = "Automated Tests";
            this.tabPageTest.UseVisualStyleBackColor = true;
            // 
            // panelTest
            // 
            this.panelTest.Controls.Add(this.gbTests);
            this.panelTest.Controls.Add(this.bStop);
            this.panelTest.Controls.Add(this.groupBox5);
            this.panelTest.Controls.Add(this.bRun);
            this.panelTest.Controls.Add(this.gbSteps);
            this.panelTest.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelTest.Enabled = false;
            this.panelTest.Location = new System.Drawing.Point(3, 3);
            this.panelTest.Name = "panelTest";
            this.panelTest.Size = new System.Drawing.Size(663, 506);
            this.panelTest.TabIndex = 5;
            // 
            // bStop
            // 
            this.bStop.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bStop.Enabled = false;
            this.bStop.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bStop.Location = new System.Drawing.Point(583, 3);
            this.bStop.Name = "bStop";
            this.bStop.Size = new System.Drawing.Size(75, 42);
            this.bStop.TabIndex = 9;
            this.bStop.Text = "Stop";
            this.bStop.UseVisualStyleBackColor = true;
            this.bStop.Click += new System.EventHandler(this.bStop_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox5.Controls.Add(this.tbOutput);
            this.groupBox5.Location = new System.Drawing.Point(5, 395);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(653, 106);
            this.groupBox5.TabIndex = 8;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Output (for information only)";
            // 
            // tbOutput
            // 
            this.tbOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbOutput.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbOutput.Location = new System.Drawing.Point(7, 21);
            this.tbOutput.Multiline = true;
            this.tbOutput.Name = "tbOutput";
            this.tbOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbOutput.Size = new System.Drawing.Size(640, 79);
            this.tbOutput.TabIndex = 1;
            // 
            // bRun
            // 
            this.bRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bRun.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bRun.Location = new System.Drawing.Point(502, 3);
            this.bRun.Name = "bRun";
            this.bRun.Size = new System.Drawing.Size(75, 42);
            this.bRun.TabIndex = 6;
            this.bRun.Text = "Run";
            this.bRun.UseVisualStyleBackColor = true;
            this.bRun.Click += new System.EventHandler(this.bRun_Click);
            // 
            // tabPageManualTest
            // 
            this.tabPageManualTest.Controls.Add(this.panelManualTest);
            this.tabPageManualTest.Location = new System.Drawing.Point(4, 22);
            this.tabPageManualTest.Name = "tabPageManualTest";
            this.tabPageManualTest.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageManualTest.Size = new System.Drawing.Size(669, 512);
            this.tabPageManualTest.TabIndex = 2;
            this.tabPageManualTest.Text = "Manual Tests";
            this.tabPageManualTest.UseVisualStyleBackColor = true;
            // 
            // panelManualTest
            // 
            this.panelManualTest.Controls.Add(this.groupBox3);
            this.panelManualTest.Controls.Add(this.tbConsole);
            this.panelManualTest.Controls.Add(this.groupBox4);
            this.panelManualTest.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelManualTest.Enabled = false;
            this.panelManualTest.Location = new System.Drawing.Point(3, 3);
            this.panelManualTest.Name = "panelManualTest";
            this.panelManualTest.Size = new System.Drawing.Size(663, 506);
            this.panelManualTest.TabIndex = 8;
            this.panelManualTest.Visible = false;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.bErase);
            this.groupBox3.Controls.Add(this.bUpload);
            this.groupBox3.Location = new System.Drawing.Point(5, 3);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(250, 90);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "ATmega32u2 Bootloader";
            // 
            // bErase
            // 
            this.bErase.Location = new System.Drawing.Point(6, 57);
            this.bErase.Name = "bErase";
            this.bErase.Size = new System.Drawing.Size(232, 23);
            this.bErase.TabIndex = 1;
            this.bErase.Text = "Erase flash memory";
            this.bErase.UseVisualStyleBackColor = true;
            this.bErase.Click += new System.EventHandler(this.bErase_Click);
            // 
            // bUpload
            // 
            this.bUpload.Location = new System.Drawing.Point(6, 28);
            this.bUpload.Name = "bUpload";
            this.bUpload.Size = new System.Drawing.Size(232, 23);
            this.bUpload.TabIndex = 0;
            this.bUpload.Text = "Upload Firmware";
            this.bUpload.UseVisualStyleBackColor = true;
            this.bUpload.Click += new System.EventHandler(this.bUpload_Click);
            // 
            // tbConsole
            // 
            this.tbConsole.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbConsole.Location = new System.Drawing.Point(5, 180);
            this.tbConsole.Multiline = true;
            this.tbConsole.Name = "tbConsole";
            this.tbConsole.Size = new System.Drawing.Size(653, 321);
            this.tbConsole.TabIndex = 5;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.bUpdate);
            this.groupBox4.Controls.Add(this.label19);
            this.groupBox4.Controls.Add(this.bGetVersion);
            this.groupBox4.Controls.Add(this.cbPort);
            this.groupBox4.Controls.Add(this.bBootloader);
            this.groupBox4.Controls.Add(this.bSerialTest);
            this.groupBox4.Controls.Add(this.bUninstall);
            this.groupBox4.Location = new System.Drawing.Point(261, 3);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(266, 171);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "USB2AX";
            // 
            // bSerialTest
            // 
            this.bSerialTest.Location = new System.Drawing.Point(13, 104);
            this.bSerialTest.Name = "bSerialTest";
            this.bSerialTest.Size = new System.Drawing.Size(232, 23);
            this.bSerialTest.TabIndex = 3;
            this.bSerialTest.Text = "Serial Test";
            this.bSerialTest.UseVisualStyleBackColor = true;
            // 
            // bUninstall
            // 
            this.bUninstall.Location = new System.Drawing.Point(13, 133);
            this.bUninstall.Name = "bUninstall";
            this.bUninstall.Size = new System.Drawing.Size(232, 23);
            this.bUninstall.TabIndex = 4;
            this.bUninstall.Text = "Uninstall peripheral";
            this.bUninstall.UseVisualStyleBackColor = true;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // backgroundWorker1
            // 
            this.backgroundWorker1.WorkerReportsProgress = true;
            this.backgroundWorker1.WorkerSupportsCancellation = true;
            this.backgroundWorker1.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
            // 
            // bBootloader
            // 
            this.bBootloader.Location = new System.Drawing.Point(13, 75);
            this.bBootloader.Name = "bBootloader";
            this.bBootloader.Size = new System.Drawing.Size(232, 23);
            this.bBootloader.TabIndex = 2;
            this.bBootloader.Text = "Run Bootloader";
            this.bBootloader.UseVisualStyleBackColor = true;
            // 
            // bGetVersion
            // 
            this.bGetVersion.Location = new System.Drawing.Point(13, 46);
            this.bGetVersion.Name = "bGetVersion";
            this.bGetVersion.Size = new System.Drawing.Size(232, 23);
            this.bGetVersion.TabIndex = 5;
            this.bGetVersion.Text = "Get Firmware Version";
            this.bGetVersion.UseVisualStyleBackColor = true;
            this.bGetVersion.Click += new System.EventHandler(this.bGetVersion_Click);
            // 
            // bUpdate
            // 
            this.bUpdate.Location = new System.Drawing.Point(188, 17);
            this.bUpdate.Name = "bUpdate";
            this.bUpdate.Size = new System.Drawing.Size(57, 23);
            this.bUpdate.TabIndex = 7;
            this.bUpdate.Text = "Update";
            this.bUpdate.UseVisualStyleBackColor = true;
            // 
            // cbPort
            // 
            this.cbPort.FormattingEnabled = true;
            this.cbPort.Location = new System.Drawing.Point(68, 19);
            this.cbPort.Name = "cbPort";
            this.cbPort.Size = new System.Drawing.Size(114, 21);
            this.cbPort.TabIndex = 1;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(10, 22);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(52, 13);
            this.label19.TabIndex = 6;
            this.label19.Text = "COM port";
            // 
            // rbTestsAll
            // 
            this.rbTestsAll.AutoSize = true;
            this.rbTestsAll.Checked = true;
            this.rbTestsAll.Location = new System.Drawing.Point(9, 19);
            this.rbTestsAll.Name = "rbTestsAll";
            this.rbTestsAll.Size = new System.Drawing.Size(36, 17);
            this.rbTestsAll.TabIndex = 10;
            this.rbTestsAll.TabStop = true;
            this.rbTestsAll.Text = "All";
            this.rbTestsAll.UseVisualStyleBackColor = true;
            // 
            // gbTests
            // 
            this.gbTests.Controls.Add(this.rbTestsSerialOnly);
            this.gbTests.Controls.Add(this.rbTestsAll);
            this.gbTests.Location = new System.Drawing.Point(3, 3);
            this.gbTests.Name = "gbTests";
            this.gbTests.Size = new System.Drawing.Size(141, 42);
            this.gbTests.TabIndex = 11;
            this.gbTests.TabStop = false;
            this.gbTests.Text = "Tests";
            this.gbTests.Visible = false;
            // 
            // rbTestsSerialOnly
            // 
            this.rbTestsSerialOnly.AutoSize = true;
            this.rbTestsSerialOnly.Location = new System.Drawing.Point(51, 19);
            this.rbTestsSerialOnly.Name = "rbTestsSerialOnly";
            this.rbTestsSerialOnly.Size = new System.Drawing.Size(73, 17);
            this.rbTestsSerialOnly.TabIndex = 11;
            this.rbTestsSerialOnly.Text = "Serial only";
            this.rbTestsSerialOnly.UseVisualStyleBackColor = true;
            // 
            // svUninstall
            // 
            this.svUninstall.Description = "Uninstalling device...";
            this.svUninstall.Location = new System.Drawing.Point(99, 230);
            this.svUninstall.Name = "svUninstall";
            this.svUninstall.PercentUponCompletion = 100;
            this.svUninstall.Size = new System.Drawing.Size(442, 25);
            this.svUninstall.State = USB2AX_Test.StepViewState.Error;
            this.svUninstall.TabIndex = 33;
            this.svUninstall.Visible = false;
            // 
            // svSerialClose
            // 
            this.svSerialClose.Description = "Closing Serial Port...";
            this.svSerialClose.Location = new System.Drawing.Point(99, 199);
            this.svSerialClose.Name = "svSerialClose";
            this.svSerialClose.PercentUponCompletion = 100;
            this.svSerialClose.Size = new System.Drawing.Size(442, 25);
            this.svSerialClose.State = USB2AX_Test.StepViewState.Error;
            this.svSerialClose.TabIndex = 32;
            // 
            // svSerialTest
            // 
            this.svSerialTest.Description = "Testing Serial Communication..";
            this.svSerialTest.Location = new System.Drawing.Point(99, 165);
            this.svSerialTest.Name = "svSerialTest";
            this.svSerialTest.PercentUponCompletion = 70;
            this.svSerialTest.Size = new System.Drawing.Size(442, 25);
            this.svSerialTest.State = USB2AX_Test.StepViewState.Error;
            this.svSerialTest.TabIndex = 31;
            // 
            // svSerialOpen
            // 
            this.svSerialOpen.Description = "Opening Serial Port...";
            this.svSerialOpen.Location = new System.Drawing.Point(99, 134);
            this.svSerialOpen.Name = "svSerialOpen";
            this.svSerialOpen.PercentUponCompletion = 50;
            this.svSerialOpen.Size = new System.Drawing.Size(442, 25);
            this.svSerialOpen.State = USB2AX_Test.StepViewState.Error;
            this.svSerialOpen.TabIndex = 30;
            // 
            // svDeviceDetect
            // 
            this.svDeviceDetect.Description = "Detecting Device...";
            this.svDeviceDetect.Location = new System.Drawing.Point(99, 103);
            this.svDeviceDetect.Name = "svDeviceDetect";
            this.svDeviceDetect.PercentUponCompletion = 40;
            this.svDeviceDetect.Size = new System.Drawing.Size(442, 25);
            this.svDeviceDetect.State = USB2AX_Test.StepViewState.Error;
            this.svDeviceDetect.TabIndex = 29;
            // 
            // svAtmegaDetect
            // 
            this.svAtmegaDetect.Description = "Waiting for blank device...";
            this.svAtmegaDetect.Location = new System.Drawing.Point(99, 41);
            this.svAtmegaDetect.Name = "svAtmegaDetect";
            this.svAtmegaDetect.PercentUponCompletion = 10;
            this.svAtmegaDetect.Size = new System.Drawing.Size(442, 25);
            this.svAtmegaDetect.State = USB2AX_Test.StepViewState.Error;
            this.svAtmegaDetect.TabIndex = 28;
            // 
            // svProgramming
            // 
            this.svProgramming.Description = "Programming Firmware...";
            this.svProgramming.Location = new System.Drawing.Point(99, 72);
            this.svProgramming.Name = "svProgramming";
            this.svProgramming.PercentUponCompletion = 30;
            this.svProgramming.Size = new System.Drawing.Size(442, 25);
            this.svProgramming.State = USB2AX_Test.StepViewState.Error;
            this.svProgramming.TabIndex = 27;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(677, 562);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "AirBoxLab Automated Test Application";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.gbSteps.ResumeLayout(false);
            this.gbSteps.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPageSetup.ResumeLayout(false);
            this.panelSetup.ResumeLayout(false);
            this.panelSetup.PerformLayout();
            this.tabPageTest.ResumeLayout(false);
            this.panelTest.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.tabPageManualTest.ResumeLayout(false);
            this.panelManualTest.ResumeLayout(false);
            this.panelManualTest.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.gbTests.ResumeLayout(false);
            this.gbTests.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.GroupBox gbSteps;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPageTest;
        private System.Windows.Forms.TabPage tabPageSetup;
        private System.Windows.Forms.Button bStartTesting;
        private System.Windows.Forms.Panel panelTest;
        private System.Windows.Forms.Panel panelSetup;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox tbBatchispPath;
        private System.Windows.Forms.Button bSearchBatchisp;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Label lBatchispOk;
        private System.Windows.Forms.Label lFirmwareOk;
        private System.Windows.Forms.Button bSearchFirmware;
        private System.Windows.Forms.TextBox tbFirmwarePath;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Button bRun;
        public System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.TabPage tabPageManualTest;
        private System.Windows.Forms.Button bUpload;
        private System.Windows.Forms.Button bUninstall;
        private System.Windows.Forms.Button bSerialTest;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button bErase;
        private System.Windows.Forms.TextBox tbConsole;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Panel panelManualTest;
        private System.Windows.Forms.ProgressBar pbTesting;
        private System.Windows.Forms.Button bStop;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label lProgressPercent;
        private TestStep svAtmegaDetect;
        private TestStep svProgramming;
        private TestStep svUninstall;
        private TestStep svSerialClose;
        private TestStep svSerialTest;
        private TestStep svSerialOpen;
        private TestStep svDeviceDetect;
        private System.Windows.Forms.TextBox tbOutput;
        private System.Windows.Forms.Button bUpdate;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Button bGetVersion;
        private System.Windows.Forms.ComboBox cbPort;
        private System.Windows.Forms.Button bBootloader;
        private System.Windows.Forms.GroupBox gbTests;
        private System.Windows.Forms.RadioButton rbTestsSerialOnly;
        private System.Windows.Forms.RadioButton rbTestsAll;
    }
}

