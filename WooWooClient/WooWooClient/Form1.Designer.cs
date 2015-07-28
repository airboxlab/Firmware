namespace WifiConfigClient {
    partial class Form1 {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent() {
            this.tbSsid = new System.Windows.Forms.TextBox();
            this.tbPassword = new System.Windows.Forms.TextBox();
            this.rbEncryptNone = new System.Windows.Forms.RadioButton();
            this.gbEncrypt = new System.Windows.Forms.GroupBox();
            this.rbEncryptWPA2 = new System.Windows.Forms.RadioButton();
            this.rbEncryptWPA = new System.Windows.Forms.RadioButton();
            this.rbEncryptWEP = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.cbComPort = new System.Windows.Forms.ComboBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.tssl1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.bUpdate = new System.Windows.Forms.Button();
            this.gbEncrypt.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbSsid
            // 
            this.tbSsid.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbSsid.Location = new System.Drawing.Point(12, 53);
            this.tbSsid.MaxLength = 31;
            this.tbSsid.Name = "tbSsid";
            this.tbSsid.Size = new System.Drawing.Size(363, 20);
            this.tbSsid.TabIndex = 1;
            // 
            // tbPassword
            // 
            this.tbPassword.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbPassword.Location = new System.Drawing.Point(12, 99);
            this.tbPassword.MaxLength = 31;
            this.tbPassword.Name = "tbPassword";
            this.tbPassword.Size = new System.Drawing.Size(363, 20);
            this.tbPassword.TabIndex = 2;
            // 
            // rbEncryptNone
            // 
            this.rbEncryptNone.AutoSize = true;
            this.rbEncryptNone.Location = new System.Drawing.Point(6, 19);
            this.rbEncryptNone.Name = "rbEncryptNone";
            this.rbEncryptNone.Size = new System.Drawing.Size(51, 17);
            this.rbEncryptNone.TabIndex = 3;
            this.rbEncryptNone.Text = "None";
            this.rbEncryptNone.UseVisualStyleBackColor = true;
            // 
            // gbEncrypt
            // 
            this.gbEncrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gbEncrypt.Controls.Add(this.rbEncryptWPA2);
            this.gbEncrypt.Controls.Add(this.rbEncryptWPA);
            this.gbEncrypt.Controls.Add(this.rbEncryptWEP);
            this.gbEncrypt.Controls.Add(this.rbEncryptNone);
            this.gbEncrypt.Location = new System.Drawing.Point(381, 37);
            this.gbEncrypt.Name = "gbEncrypt";
            this.gbEncrypt.Size = new System.Drawing.Size(84, 120);
            this.gbEncrypt.TabIndex = 4;
            this.gbEncrypt.TabStop = false;
            this.gbEncrypt.Text = "Encryption";
            // 
            // rbEncryptWPA2
            // 
            this.rbEncryptWPA2.AutoSize = true;
            this.rbEncryptWPA2.Checked = true;
            this.rbEncryptWPA2.Location = new System.Drawing.Point(6, 88);
            this.rbEncryptWPA2.Name = "rbEncryptWPA2";
            this.rbEncryptWPA2.Size = new System.Drawing.Size(56, 17);
            this.rbEncryptWPA2.TabIndex = 6;
            this.rbEncryptWPA2.TabStop = true;
            this.rbEncryptWPA2.Text = "WPA2";
            this.rbEncryptWPA2.UseVisualStyleBackColor = true;
            // 
            // rbEncryptWPA
            // 
            this.rbEncryptWPA.AutoSize = true;
            this.rbEncryptWPA.Location = new System.Drawing.Point(6, 65);
            this.rbEncryptWPA.Name = "rbEncryptWPA";
            this.rbEncryptWPA.Size = new System.Drawing.Size(50, 17);
            this.rbEncryptWPA.TabIndex = 5;
            this.rbEncryptWPA.Text = "WPA";
            this.rbEncryptWPA.UseVisualStyleBackColor = true;
            // 
            // rbEncryptWEP
            // 
            this.rbEncryptWEP.AutoSize = true;
            this.rbEncryptWEP.Location = new System.Drawing.Point(6, 42);
            this.rbEncryptWEP.Name = "rbEncryptWEP";
            this.rbEncryptWEP.Size = new System.Drawing.Size(50, 17);
            this.rbEncryptWEP.TabIndex = 4;
            this.rbEncryptWEP.Text = "WEP";
            this.rbEncryptWEP.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 37);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(134, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "SSID (max length 31 char):";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 83);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(152, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Password (max length 31 char)";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "COM Port:";
            // 
            // cbComPort
            // 
            this.cbComPort.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cbComPort.FormattingEnabled = true;
            this.cbComPort.Location = new System.Drawing.Point(74, 6);
            this.cbComPort.Name = "cbComPort";
            this.cbComPort.Size = new System.Drawing.Size(392, 21);
            this.cbComPort.TabIndex = 8;
            this.cbComPort.DropDown += new System.EventHandler(this.comboBox1_DropDown);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tssl1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 169);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(478, 22);
            this.statusStrip1.TabIndex = 11;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // tssl1
            // 
            this.tssl1.Name = "tssl1";
            this.tssl1.Size = new System.Drawing.Size(166, 17);
            this.tssl1.Text = "To begin, select the COM port";
            // 
            // bUpdate
            // 
            this.bUpdate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bUpdate.Location = new System.Drawing.Point(259, 134);
            this.bUpdate.Name = "bUpdate";
            this.bUpdate.Size = new System.Drawing.Size(116, 23);
            this.bUpdate.TabIndex = 7;
            this.bUpdate.Text = "Update credentials";
            this.bUpdate.UseVisualStyleBackColor = true;
            this.bUpdate.Click += new System.EventHandler(this.bUpdate_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(478, 191);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cbComPort);
            this.Controls.Add(this.bUpdate);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.gbEncrypt);
            this.Controls.Add(this.tbPassword);
            this.Controls.Add(this.tbSsid);
            this.MinimumSize = new System.Drawing.Size(494, 229);
            this.Name = "Form1";
            this.Text = "Airboxlab WiFi Configuration";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.gbEncrypt.ResumeLayout(false);
            this.gbEncrypt.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tbSsid;
        private System.Windows.Forms.TextBox tbPassword;
        private System.Windows.Forms.RadioButton rbEncryptNone;
        private System.Windows.Forms.GroupBox gbEncrypt;
        private System.Windows.Forms.RadioButton rbEncryptWPA2;
        private System.Windows.Forms.RadioButton rbEncryptWPA;
        private System.Windows.Forms.RadioButton rbEncryptWEP;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cbComPort;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel tssl1;
        private System.Windows.Forms.Button bUpdate;
    }
}

