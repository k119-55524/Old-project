using System;

using System.Data;
//using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;
using System.Collections.Generic;

namespace Mathematics
{
    public partial class FormMathem : Form
    {
        private TimeSpan TestTime;
		private TimeSpan FreeTimeTest;
		private TimeSpan IndivTimeTest;
		private TimeSpan IndivRealTimeTest;
        private string StringPrimer;
        private Random Rnd = new Random();
        private FormSetup FormSet = new FormSetup();
        private bool[] aSig = new bool[4];
		private String sgnStr = "";
		private FormAbout FormAb = new FormAbout();
		private FormResult FormRes = new FormResult();
		private bool fEndTest;

        int a1, a2, numTests, numCheckTest;

        public FormMathem()
        {
            InitializeComponent();
        }

        private void FormMathem_Shown(object sender, EventArgs e)
        {
            toolStripText.Text = "Тест отключен.";
			labelCountRes.Text = "";
			labelCheckTest.Text = "";
			labelPercentTest.Text = "";
			labelRealTimeTest.Text = "";

            numericRes.Value = 0;
            numericRes.ResetText();

            toolStripStart.Enabled = true;
            toolStripStop.Enabled = false;
            toolStripSetup.Enabled = true;
            buttonRes.Enabled = false;
            numericRes.Enabled = false;
			groupBoxStatistics.Enabled = false;

			FormRes.Init();
            FormSet.Setup();
        }

        // Start
        private void toolStripStart_Click(object sender, EventArgs e)
        {
            toolStripStart.Enabled = false;
            toolStripStop.Enabled = true;
            toolStripSetup.Enabled = false;
			toolStripButtonEndTest.Enabled = false;
			toolStripButtonAbout.Enabled = false;
            buttonRes.Enabled = true;
            numericRes.Enabled = true;
			groupBoxStatistics.Enabled = true;

			numTests = numCheckTest = 0;
			labelCountRes.Text = "Дано ответов на " + numTests + " примеров";
			if (FormSet.checkResCount.Checked)
				labelCountRes.Text += " из " + FormSet.numericUpDownResCount.Value + " заданных";
			labelCountRes.Text += ".";

			labelCheckTest.Text = "Правильных ответов " + numCheckTest + " из " + numTests + ".";
			labelPercentTest.Text = "Процент правильных ответов: 100%.";

            TestTime = new TimeSpan(0);
			FreeTimeTest = new TimeSpan(0,(int)FormSet.numericUpDownTime.Value,0);
			IndivTimeTest = new TimeSpan(0, 0, (int)FormSet.numericUpDownResTime.Value);
            numericRes.Value = 0;
            numericRes.ResetText();

			if (FormSet.checkResTime.Checked)
			{
				labelRealTimeTest.Text = "Осталось времени на ответ " + IndivTimeTest + " из" + IndivTimeTest + ".";
			}
			else
				labelRealTimeTest.Text = "";

            toolStripText.Text = "Прошло времени: 00:00:00.";
			if (FormSet.checkAllTime.Checked)
				toolStripText.Text += ". Осталось времени на ответы: " + FreeTimeTest + " из " + FreeTimeTest;
			toolStripText.Text += ".";

			fEndTest = false;
            CreatePrimer();
            Timer.Start();
        }

        // Stop
		private void toolStripStop_Click(object sender, EventArgs e)
		{
			if (!FormSet.checkAllTime.Checked && !FormSet.checkResCount.Checked)
				fEndTest = true;

			Stop();
		}

		private void Stop()
		{
            Timer.Stop();

            toolStripStart.Enabled = true;
            toolStripStop.Enabled = false;
            toolStripSetup.Enabled = true;
			toolStripButtonEndTest.Enabled = true;
			toolStripButtonAbout.Enabled = true;
			buttonRes.Enabled = false;
            numericRes.Enabled = false;
			groupBoxStatistics.Enabled = false;

            panelPrint.Refresh();

			toolStripText.Text = "Тест отключен.";
			labelCountRes.Text = "";
			labelCheckTest.Text = "";
			labelPercentTest.Text = "";
			labelRealTimeTest.Text = "";

			numericRes.ResetText();

			// Print result.
			DateTime thisDay = DateTime.Now;

			FormRes.label1.Text = "Последний тест был закончен " + thisDay.Date + " в " +
				new TimeSpan(thisDay.TimeOfDay.Hours,thisDay.TimeOfDay.Minutes,thisDay.TimeOfDay.Seconds) + ".";

			if (fEndTest)
			{
				FormRes.label2.ForeColor = Color.Green;
				FormRes.label2.Text = "Тест был полностью завершён.";
			}
			else
			{
				FormRes.label2.ForeColor = Color.Red;
				FormRes.label2.Text = "Тест не был полностью завершён.";
			}

			FormRes.label3.Text = "Время потраченное на тестирование: " + TestTime + ".";

            TestTime = new TimeSpan(0);

			FormRes.label4.Text = "Было правильно отвечено на " + numCheckTest + " примеров из " + numTests + ".";
			FormRes.label5.Text = "Процент правильных ответов: " + (uint)((100.0f / (float)(numTests + 0.0000001f)) * (float)numCheckTest) + "%.";
        }

        // Tick
        private void Timer_Tick(object sender, EventArgs e)
        {
			if (FormSet.checkAllTime.Checked)
				if (TestTime >= FreeTimeTest)
				{
					fEndTest = true;
					Stop();
					return;
				}

			TimeSpan t1 = new TimeSpan(0, 0, 1);
			TestTime += t1;
			toolStripText.Text = "Прошло времени: " + TestTime;
			if (FormSet.checkAllTime.Checked)
				toolStripText.Text += ". Осталось времени на ответы: " + (FreeTimeTest - TestTime) + " из " + FreeTimeTest;
			toolStripText.Text += ".";

			if (FormSet.checkResTime.Checked)
			{
				IndivRealTimeTest += t1;

				labelRealTimeTest.Text = "Осталось времени на ответ " + (IndivTimeTest - IndivRealTimeTest) + " из " + IndivTimeTest + ".";
				if(IndivRealTimeTest>=IndivTimeTest)
				{
					if (!CalcResults())
						return;

					CreatePrimer();
					return;
				}
			}
        }

        // Setup
        private void toolStripSetup_Click(object sender, EventArgs e)
        {
            FormSet.ShowDialog();
        }

        // Create primer
        private void CreatePrimer()
        {
            aSig[0] = FormSet.checkSumm.Checked;
            aSig[1] = FormSet.checkSub.Checked;
            aSig[2] = FormSet.checkMul.Checked;
            aSig[3] = FormSet.checkDiv.Checked;

            // Create
            int nc = 0;
            if (FormSet.checkSumm.Checked)
                nc++;

            if (FormSet.checkSub.Checked)
                nc++;

            if (FormSet.checkMul.Checked)
                nc++;

            if (FormSet.checkDiv.Checked)
                nc++;

            int ncRes = Rnd.Next(0, nc);

            uint i, iS;
            for (i = 0, iS = 0; i < nc + 1; i++)
            {
                while (!aSig[i])
                    i++;

                if (iS == ncRes)
                    break;

				iS++;
            }

             switch (i)
            {
                case 0:
                    sgnStr = "+";
					a1 = Rnd.Next((int)FormSet.numericUpDownMinSumm1.Value, (int)FormSet.numericUpDownMaxSumm1.Value + 1);
					a2 = Rnd.Next((int)FormSet.numericUpDownMinSumm2.Value, (int)FormSet.numericUpDownMaxSumm2.Value + 1);					
                    break;
				case 1:
                    sgnStr = "-";
					a1 = Rnd.Next((int)FormSet.numericUpDownMinSub1.Value, (int)FormSet.numericUpDownMaxSub1.Value + 1);
					a2 = Rnd.Next((int)FormSet.numericUpDownMinSub2.Value, a1 + 1);	
					break;
				case 2:
                    sgnStr = "*";
					a1 = Rnd.Next((int)FormSet.numericUpDownMinMul1.Value, (int)FormSet.numericUpDownMaxMul1.Value + 1);
					a2 = Rnd.Next((int)FormSet.numericUpDownMinMul2.Value, (int)FormSet.numericUpDownMaxMul2.Value + 1);
					break;
				case 3:
                    sgnStr = "/";
					int res = Rnd.Next((int)FormSet.numericUpDownMinDiv1.Value, (int)FormSet.numericUpDownMaxDiv1.Value + 1);
					a2 = Rnd.Next((int)FormSet.numericUpDownMinDiv2.Value, (int)FormSet.numericUpDownMaxDiv2.Value + 1);
					a1 = res * a2;
					break;
            }

            // Print
            numericRes.Value = 0;
            numericRes.ResetText();
            StringPrimer = a1 + sgnStr + a2 + "=";

            System.Drawing.Graphics graphics = panelPrint.CreateGraphics();
            System.Drawing.Font drawFont = new System.Drawing.Font("Arial", 72);
            System.Drawing.SolidBrush drawBrush = new System.Drawing.SolidBrush(System.Drawing.Color.DodgerBlue);

            panelPrint.Refresh();
            graphics.DrawString(StringPrimer, drawFont, drawBrush, 1.0f, 1.0f);

            drawFont.Dispose();
            drawBrush.Dispose();
            graphics.Dispose();

			IndivRealTimeTest = new TimeSpan(0, 0, 0);

            numericRes.Focus();
        }

        // Check result
        bool CheckResult()
        {
			switch (sgnStr)
			{
				case "+":
					if ((a1 + a2) == numericRes.Value)
						return true;
					break;
				case "-":
					if ((a1 - a2) == numericRes.Value)
						return true;
					break;
				case "*":
					if ((a1 * a2) == numericRes.Value)
						return true;
					break;
				case "/":
					if ((a1 / a2) == numericRes.Value)
						return true;
					break;
			}

			return false;
        }

		private void numericRes_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyValue == (int)Keys.Return)
			{
				e.Handled = true;
				if (!CalcResults())
					return;

				CreatePrimer();
				return;
			}

			if (CheckResult())
				numericRes.ForeColor = Color.Green;
			else
				numericRes.ForeColor = Color.Red;

			numericRes.Invalidate();
			numericRes.Refresh();
		}

		private bool CalcResults()
		{
			if(CheckResult())
				numCheckTest++;

			numTests++;
			labelCountRes.Text = "Дано ответов на " + numTests + " примеров";
			if (FormSet.checkResCount.Checked)
			{
				labelCountRes.Text += " из " + FormSet.numericUpDownResCount.Value + " заданных";
				if (numTests == FormSet.numericUpDownResCount.Value)
				{
					fEndTest = true;
					Stop();
					return false;
				}
			}
			labelCountRes.Text += ".";

			labelCheckTest.Text = "Правильных ответов " + numCheckTest + " из " + numTests + ".";
			labelPercentTest.Text = "Процент правильных ответов: " + (uint)((100.0f / (float)(numTests + 0.0000001f)) * (float)numCheckTest) + "%.";

			return true;
		}

		private void buttonRes_Click(object sender, EventArgs e)
		{
			if (!CalcResults())
				return;

			CreatePrimer();
		}

		// Form closing
		private void FormMathem_FormClosing(object sender, FormClosingEventArgs e)
		{
			FormSet.SaveIni();
		}

		// About
		private void toolStripButtonAbout_Click(object sender, EventArgs e)
		{
			FormAb.ShowDialog();
		}

		// View result
		private void toolStripButtonEndTest_Click(object sender, EventArgs e)
		{
			FormRes.ShowDialog();
		}
    }
}
