namespace Mathematics
{
    partial class FormMathem
    {
        /// <summary>
        /// Требуется переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Обязательный метод для поддержки конструктора - не изменяйте
        /// содержимое данного метода при помощи редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMathem));
			this.toolStrip = new System.Windows.Forms.ToolStrip();
			this.toolStripStart = new System.Windows.Forms.ToolStripButton();
			this.toolStripStop = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripSetup = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripButtonEndTest = new System.Windows.Forms.ToolStripButton();
			this.toolStripButtonAbout = new System.Windows.Forms.ToolStripButton();
			this.statusStrip = new System.Windows.Forms.StatusStrip();
			this.toolStripText = new System.Windows.Forms.ToolStripStatusLabel();
			this.Timer = new System.Windows.Forms.Timer(this.components);
			this.panelPrint = new System.Windows.Forms.Panel();
			this.numericRes = new System.Windows.Forms.NumericUpDown();
			this.buttonRes = new System.Windows.Forms.Button();
			this.labelCountRes = new System.Windows.Forms.Label();
			this.groupBoxStatistics = new System.Windows.Forms.GroupBox();
			this.labelRealTimeTest = new System.Windows.Forms.Label();
			this.labelPercentTest = new System.Windows.Forms.Label();
			this.labelCheckTest = new System.Windows.Forms.Label();
			this.toolStrip.SuspendLayout();
			this.statusStrip.SuspendLayout();
			this.panelPrint.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericRes)).BeginInit();
			this.groupBoxStatistics.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStrip
			// 
			this.toolStrip.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(227)))), ((int)(((byte)(208)))), ((int)(((byte)(145)))));
			this.toolStrip.ImageScalingSize = new System.Drawing.Size(48, 48);
			this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStart,
            this.toolStripStop,
            this.toolStripSeparator1,
            this.toolStripSetup,
            this.toolStripSeparator2,
            this.toolStripButtonEndTest,
            this.toolStripButtonAbout});
			this.toolStrip.Location = new System.Drawing.Point(0, 0);
			this.toolStrip.Name = "toolStrip";
			this.toolStrip.Size = new System.Drawing.Size(574, 55);
			this.toolStrip.TabIndex = 0;
			this.toolStrip.Text = "toolStrip1";
			// 
			// toolStripStart
			// 
			this.toolStripStart.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripStart.Image = ((System.Drawing.Image)(resources.GetObject("toolStripStart.Image")));
			this.toolStripStart.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripStart.Name = "toolStripStart";
			this.toolStripStart.Size = new System.Drawing.Size(52, 52);
			this.toolStripStart.Text = "toolStripButton1";
			this.toolStripStart.ToolTipText = "Запустить новый тест";
			this.toolStripStart.Click += new System.EventHandler(this.toolStripStart_Click);
			// 
			// toolStripStop
			// 
			this.toolStripStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripStop.Image = ((System.Drawing.Image)(resources.GetObject("toolStripStop.Image")));
			this.toolStripStop.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripStop.Name = "toolStripStop";
			this.toolStripStop.Size = new System.Drawing.Size(52, 52);
			this.toolStripStop.Text = "toolStripButton1";
			this.toolStripStop.ToolTipText = "Остановить тест";
			this.toolStripStop.Click += new System.EventHandler(this.toolStripStop_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 55);
			// 
			// toolStripSetup
			// 
			this.toolStripSetup.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripSetup.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSetup.Image")));
			this.toolStripSetup.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripSetup.Name = "toolStripSetup";
			this.toolStripSetup.Size = new System.Drawing.Size(52, 52);
			this.toolStripSetup.Text = "toolStripButton1";
			this.toolStripSetup.ToolTipText = "Настройки теста";
			this.toolStripSetup.Click += new System.EventHandler(this.toolStripSetup_Click);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 55);
			// 
			// toolStripButtonEndTest
			// 
			this.toolStripButtonEndTest.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButtonEndTest.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonEndTest.Image")));
			this.toolStripButtonEndTest.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButtonEndTest.Name = "toolStripButtonEndTest";
			this.toolStripButtonEndTest.Size = new System.Drawing.Size(52, 52);
			this.toolStripButtonEndTest.Text = "toolStripButton1";
			this.toolStripButtonEndTest.ToolTipText = "Результаты последнего теста";
			this.toolStripButtonEndTest.Click += new System.EventHandler(this.toolStripButtonEndTest_Click);
			// 
			// toolStripButtonAbout
			// 
			this.toolStripButtonAbout.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButtonAbout.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonAbout.Image")));
			this.toolStripButtonAbout.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButtonAbout.Name = "toolStripButtonAbout";
			this.toolStripButtonAbout.Size = new System.Drawing.Size(52, 52);
			this.toolStripButtonAbout.Text = "toolStripButton2";
			this.toolStripButtonAbout.ToolTipText = "Об авторе";
			this.toolStripButtonAbout.Click += new System.EventHandler(this.toolStripButtonAbout_Click);
			// 
			// statusStrip
			// 
			this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripText});
			this.statusStrip.Location = new System.Drawing.Point(0, 350);
			this.statusStrip.Name = "statusStrip";
			this.statusStrip.Size = new System.Drawing.Size(574, 22);
			this.statusStrip.SizingGrip = false;
			this.statusStrip.TabIndex = 1;
			this.statusStrip.Text = "statusStrip1";
			// 
			// toolStripText
			// 
			this.toolStripText.Name = "toolStripText";
			this.toolStripText.Size = new System.Drawing.Size(25, 17);
			this.toolStripText.Text = "123";
			// 
			// Timer
			// 
			this.Timer.Interval = 1000;
			this.Timer.Tick += new System.EventHandler(this.Timer_Tick);
			// 
			// panelPrint
			// 
			this.panelPrint.BackColor = System.Drawing.Color.Khaki;
			this.panelPrint.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panelPrint.Controls.Add(this.numericRes);
			this.panelPrint.Location = new System.Drawing.Point(0, 58);
			this.panelPrint.Name = "panelPrint";
			this.panelPrint.Size = new System.Drawing.Size(574, 112);
			this.panelPrint.TabIndex = 2;
			// 
			// numericRes
			// 
			this.numericRes.Font = new System.Drawing.Font("Microsoft Sans Serif", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
			this.numericRes.ForeColor = System.Drawing.Color.DodgerBlue;
			this.numericRes.Location = new System.Drawing.Point(391, 15);
			this.numericRes.Name = "numericRes";
			this.numericRes.Size = new System.Drawing.Size(170, 80);
			this.numericRes.TabIndex = 0;
			this.numericRes.KeyUp += new System.Windows.Forms.KeyEventHandler(this.numericRes_KeyUp);
			// 
			// buttonRes
			// 
			this.buttonRes.Image = ((System.Drawing.Image)(resources.GetObject("buttonRes.Image")));
			this.buttonRes.Location = new System.Drawing.Point(339, 176);
			this.buttonRes.Name = "buttonRes";
			this.buttonRes.Size = new System.Drawing.Size(223, 167);
			this.buttonRes.TabIndex = 3;
			this.buttonRes.UseVisualStyleBackColor = true;
			this.buttonRes.Click += new System.EventHandler(this.buttonRes_Click);
			// 
			// labelCountRes
			// 
			this.labelCountRes.AutoSize = true;
			this.labelCountRes.Location = new System.Drawing.Point(12, 26);
			this.labelCountRes.Name = "labelCountRes";
			this.labelCountRes.Size = new System.Drawing.Size(25, 13);
			this.labelCountRes.TabIndex = 4;
			this.labelCountRes.Text = "234";
			// 
			// groupBoxStatistics
			// 
			this.groupBoxStatistics.Controls.Add(this.labelRealTimeTest);
			this.groupBoxStatistics.Controls.Add(this.labelPercentTest);
			this.groupBoxStatistics.Controls.Add(this.labelCheckTest);
			this.groupBoxStatistics.Controls.Add(this.labelCountRes);
			this.groupBoxStatistics.Location = new System.Drawing.Point(0, 176);
			this.groupBoxStatistics.Name = "groupBoxStatistics";
			this.groupBoxStatistics.Size = new System.Drawing.Size(333, 167);
			this.groupBoxStatistics.TabIndex = 5;
			this.groupBoxStatistics.TabStop = false;
			this.groupBoxStatistics.Text = "Статистика";
			// 
			// labelRealTimeTest
			// 
			this.labelRealTimeTest.AutoSize = true;
			this.labelRealTimeTest.Location = new System.Drawing.Point(12, 93);
			this.labelRealTimeTest.Name = "labelRealTimeTest";
			this.labelRealTimeTest.Size = new System.Drawing.Size(25, 13);
			this.labelRealTimeTest.TabIndex = 4;
			this.labelRealTimeTest.Text = "234";
			// 
			// labelPercentTest
			// 
			this.labelPercentTest.AutoSize = true;
			this.labelPercentTest.Location = new System.Drawing.Point(12, 71);
			this.labelPercentTest.Name = "labelPercentTest";
			this.labelPercentTest.Size = new System.Drawing.Size(25, 13);
			this.labelPercentTest.TabIndex = 4;
			this.labelPercentTest.Text = "234";
			// 
			// labelCheckTest
			// 
			this.labelCheckTest.AutoSize = true;
			this.labelCheckTest.Location = new System.Drawing.Point(12, 48);
			this.labelCheckTest.Name = "labelCheckTest";
			this.labelCheckTest.Size = new System.Drawing.Size(25, 13);
			this.labelCheckTest.TabIndex = 4;
			this.labelCheckTest.Text = "234";
			// 
			// FormMathem
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(574, 372);
			this.Controls.Add(this.groupBoxStatistics);
			this.Controls.Add(this.buttonRes);
			this.Controls.Add(this.panelPrint);
			this.Controls.Add(this.statusStrip);
			this.Controls.Add(this.toolStrip);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormMathem";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Mathematics 2012.";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMathem_FormClosing);
			this.Shown += new System.EventHandler(this.FormMathem_Shown);
			this.toolStrip.ResumeLayout(false);
			this.toolStrip.PerformLayout();
			this.statusStrip.ResumeLayout(false);
			this.statusStrip.PerformLayout();
			this.panelPrint.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.numericRes)).EndInit();
			this.groupBoxStatistics.ResumeLayout(false);
			this.groupBoxStatistics.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.ToolStripButton toolStripStart;
        private System.Windows.Forms.ToolStripButton toolStripStop;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripSetup;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel toolStripText;
        private System.Windows.Forms.Timer Timer;
        private System.Windows.Forms.Panel panelPrint;
        private System.Windows.Forms.Button buttonRes;
        private System.Windows.Forms.NumericUpDown numericRes;
		private System.Windows.Forms.Label labelCountRes;
		private System.Windows.Forms.GroupBox groupBoxStatistics;
		private System.Windows.Forms.Label labelCheckTest;
		private System.Windows.Forms.Label labelPercentTest;
		private System.Windows.Forms.Label labelRealTimeTest;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripButton toolStripButtonEndTest;
		private System.Windows.Forms.ToolStripButton toolStripButtonAbout;
    }
}

