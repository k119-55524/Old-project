
using System;
using System.IO;
using System.Data;
//using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;
using System.Collections.Generic;

namespace Mathematics
{
    public partial class FormSetup : Form
    {
		bool fSet;

        public FormSetup()
        {
            InitializeComponent();
        }

        public void Setup()
        {
			BinaryReader file = null;

			try
			{
				fSet = true;

				file = new BinaryReader(File.Open("ini.ini", FileMode.Open));

				checkAllTime.Checked = file.ReadBoolean();
				numericUpDownTime.Value = file.ReadDecimal();
				checkResTime.Checked = file.ReadBoolean();
				numericUpDownResTime.Value = file.ReadDecimal();
				checkResCount.Checked = file.ReadBoolean();
				numericUpDownResCount.Value = file.ReadDecimal();
				checkSumm.Checked = file.ReadBoolean();
				numericUpDownMinSumm1.Value = file.ReadDecimal();
				numericUpDownMaxSumm1.Value = file.ReadDecimal();
				numericUpDownMinSumm2.Value = file.ReadDecimal();
				numericUpDownMaxSumm2.Value = file.ReadDecimal();
				checkSub.Checked = file.ReadBoolean();
				numericUpDownMinSub1.Value = file.ReadDecimal();
				numericUpDownMaxSub1.Value = file.ReadDecimal();
				numericUpDownMinSub2.Value = file.ReadDecimal();
				numericUpDownMaxSub2.Value = file.ReadDecimal();
				checkMul.Checked = file.ReadBoolean();
				numericUpDownMinMul1.Value = file.ReadDecimal();
				numericUpDownMaxMul1.Value = file.ReadDecimal();
				numericUpDownMinMul2.Value = file.ReadDecimal();
				numericUpDownMaxMul2.Value = file.ReadDecimal();
				checkDiv.Checked = file.ReadBoolean();
				numericUpDownMinDiv1.Value = file.ReadDecimal();
				numericUpDownMaxDiv1.Value = file.ReadDecimal();
				numericUpDownMinDiv2.Value = file.ReadDecimal();
				numericUpDownMaxDiv2.Value = file.ReadDecimal();

				file.Close();

				fSet = false;
			}
			catch (SystemException /*e*/)
			{
				if (file != null)
					file.Close();

				Reset();
			}
        }

		public void SaveIni()
		{
			BinaryWriter file = null;

			try
			{
				file = new BinaryWriter(File.Open("ini.ini", FileMode.OpenOrCreate));

				file.Write(checkAllTime.Checked);
				file.Write(numericUpDownTime.Value);
				file.Write(checkResTime.Checked);
				file.Write(numericUpDownResTime.Value);
				file.Write(checkResCount.Checked);
				file.Write(numericUpDownResCount.Value);
				file.Write(checkSumm.Checked);
				file.Write(numericUpDownMinSumm1.Value);
				file.Write(numericUpDownMaxSumm1.Value);
				file.Write(numericUpDownMinSumm2.Value);
				file.Write(numericUpDownMaxSumm2.Value);
				file.Write(checkSub.Checked);
				file.Write(numericUpDownMinSub1.Value);
				file.Write(numericUpDownMaxSub1.Value);
				file.Write(numericUpDownMinSub2.Value);
				file.Write(numericUpDownMaxSub2.Value);
				file.Write(checkMul.Checked);
				file.Write(numericUpDownMinMul1.Value);
				file.Write(numericUpDownMaxMul1.Value);
				file.Write(numericUpDownMinMul2.Value);
				file.Write(numericUpDownMaxMul2.Value);
				file.Write(checkDiv.Checked);
				file.Write(numericUpDownMinDiv1.Value);
				file.Write(numericUpDownMaxDiv1.Value);
				file.Write(numericUpDownMinDiv2.Value);
				file.Write(numericUpDownMaxDiv2.Value);

				file.Close();
			}
			catch (InvalidCastException /*e*/)
			{
				if(file != null)
					file.Close();
			}
		}

        private void checkAllTime_CheckedChanged(object sender, EventArgs e)
        {
            if (checkAllTime.Checked)
            {
                checkAllTime.Checked = true;
                numericUpDownTime.Enabled = true;
                label1.Enabled = true;
            }
            else
            {
                checkAllTime.Checked = false;
                numericUpDownTime.Enabled = false;
                label1.Enabled = false;
            }
        }

        private void checkResTime_CheckedChanged(object sender, EventArgs e)
        {
            if (checkResTime.Checked)
            {
                checkResTime.Checked = true;
                numericUpDownResTime.Enabled = true;
                label3.Enabled = true;
            }
            else
            {
                checkResTime.Checked = false;
                numericUpDownResTime.Enabled = false;
                label3.Enabled = false;
            }
        }

        private void checkResCount_CheckedChanged(object sender, EventArgs e)
        {
            if (checkResCount.Checked)
            {
                checkResCount.Checked = true;
                numericUpDownResCount.Enabled = true;
                label4.Enabled = true;
            }
            else
            {
                checkResCount.Checked = false;
                numericUpDownResCount.Enabled = false;
                label4.Enabled = false;
            }
        }

        private void checkSumm_CheckedChanged(object sender, EventArgs e)
        {
            if (checkSumm.Checked)
            {
                numericUpDownMinSumm1.Enabled = true;
                numericUpDownMaxSumm1.Enabled = true;
                numericUpDownMinSumm2.Enabled = true;
                numericUpDownMaxSumm2.Enabled = true;
                label2.Enabled = true;
                label5.Enabled = true;
                label16.Enabled = true;
                label17.Enabled = true;
            }
            else
            {
                numericUpDownMinSumm1.Enabled = false;
                numericUpDownMaxSumm1.Enabled = false;
                numericUpDownMinSumm2.Enabled = false;
                numericUpDownMaxSumm2.Enabled = false;
                label2.Enabled = false;
                label5.Enabled = false;
                label16.Enabled = false;
                label17.Enabled = false;
            }

            CheckCheck();
        }

        private void checkSub_CheckedChanged(object sender, EventArgs e)
        {
            if (checkSub.Checked)
            {
                numericUpDownMinSub1.Enabled = true;
                numericUpDownMaxSub1.Enabled = true;
                numericUpDownMinSub2.Enabled = true;
                numericUpDownMaxSub2.Enabled = true;
                label6.Enabled = true;
                label7.Enabled = true;
                label18.Enabled = true;
                label19.Enabled = true;
            }
            else
            {
                numericUpDownMinSub1.Enabled = false;
                numericUpDownMaxSub1.Enabled = false;
                numericUpDownMinSub2.Enabled = false;
                numericUpDownMaxSub2.Enabled = false;
                label6.Enabled = false;
                label7.Enabled = false;
                label18.Enabled = false;
                label19.Enabled = false;
            }

            CheckCheck();
        }

        private void checkMul_CheckedChanged(object sender, EventArgs e)
        {
            if (checkMul.Checked)
            {
                numericUpDownMinMul1.Enabled = true;
                numericUpDownMaxMul1.Enabled = true;
                numericUpDownMinMul2.Enabled = true;
                numericUpDownMaxMul2.Enabled = true;
                label8.Enabled = true;
                label9.Enabled = true;
                label20.Enabled = true;
                label21.Enabled = true;
            }
            else
            {
                numericUpDownMinMul1.Enabled = false;
                numericUpDownMaxMul1.Enabled = false;
                numericUpDownMinMul2.Enabled = false;
                numericUpDownMaxMul2.Enabled = false;
                label8.Enabled = false;
                label9.Enabled = false;
                label20.Enabled = false;
                label21.Enabled = false;
            }

            CheckCheck();
        }

        private void checkDiv_CheckedChanged(object sender, EventArgs e)
        {
            if (checkDiv.Checked)
            {
                numericUpDownMinDiv1.Enabled = true;
                numericUpDownMaxDiv1.Enabled = true;
                numericUpDownMinDiv2.Enabled = true;
                numericUpDownMaxDiv2.Enabled = true;
                label10.Enabled = true;
                label11.Enabled = true;
                label12.Enabled = true;
                label13.Enabled = true;
                label14.Enabled = true;
                label15.Enabled = true;
            }
            else
            {
                numericUpDownMinDiv1.Enabled = false;
                numericUpDownMaxDiv1.Enabled = false;
                numericUpDownMinDiv2.Enabled = false;
                numericUpDownMaxDiv2.Enabled = false;
                label10.Enabled = false;
                label11.Enabled = false;
                label12.Enabled = false;
                label13.Enabled = false;
                label14.Enabled = false;
                label15.Enabled = false;
            }

            CheckCheck();
        }

        private void buttonReset_Click(object sender, EventArgs e)
        {
            Reset();
        }

        private void CheckCheck()
        {
            if (!checkSumm.Checked && !checkSub.Checked && !checkMul.Checked && !checkDiv.Checked )
                checkSumm.Checked = true;
        }

        public void Reset()
        {
			fSet = true;

            // All time
            checkAllTime.Checked = false;
			numericUpDownTime.Value = 1;
            numericUpDownTime.Enabled = false;
            label1.Enabled = false;

            // Time result
            checkResTime.Checked = false;
			numericUpDownResTime.Value = 5;
            numericUpDownResTime.Enabled = false;
            label3.Enabled = false;

            // Result count
            checkResCount.Checked = false;
            numericUpDownResCount.Value = 5;
            numericUpDownResCount.Enabled = false;
            label4.Enabled = false;

            // Summ
            checkSumm.Checked = true;
            numericUpDownMinSumm1.Enabled = true;
            numericUpDownMinSumm1.Value = 0;
            numericUpDownMaxSumm1.Enabled = true;
            numericUpDownMaxSumm1.Value = 10;
            numericUpDownMinSumm2.Enabled = true;
            numericUpDownMinSumm2.Value = 0;
            numericUpDownMaxSumm2.Enabled = true;
            numericUpDownMaxSumm2.Value = 10;
            label2.Enabled = true;
            label5.Enabled = true;
            label16.Enabled = true;
            label17.Enabled = true;

            // Sub
            checkSub.Checked = false;
            numericUpDownMinSub1.Enabled = false;
            numericUpDownMinSub1.Value = 0;
            numericUpDownMaxSub1.Enabled = false;
            numericUpDownMaxSub1.Value = 10;
            numericUpDownMinSub2.Enabled = false;
            numericUpDownMinSub2.Value = 0;
            numericUpDownMaxSub2.Enabled = false;
            numericUpDownMaxSub2.Value = 10;
            label6.Enabled = false;
            label7.Enabled = false;
            label18.Enabled = false;
            label19.Enabled = false;

            // Mul
            checkMul.Checked = false;
            numericUpDownMinMul1.Enabled = false;
            numericUpDownMinMul1.Value = 0;
            numericUpDownMaxMul1.Enabled = false;
            numericUpDownMaxMul1.Value = 10;
            numericUpDownMinMul2.Enabled = false;
            numericUpDownMinMul2.Value = 0;
            numericUpDownMaxMul2.Enabled = false;
            numericUpDownMaxMul2.Value = 10;
            label8.Enabled = false;
            label9.Enabled = false;
            label20.Enabled = false;
            label21.Enabled = false;

            // Div
            checkDiv.Checked = false;
            numericUpDownMinDiv1.Enabled = false;
            numericUpDownMinDiv1.Value = 0;
            numericUpDownMaxDiv1.Enabled = false;
            numericUpDownMaxDiv1.Value = 10;
            numericUpDownMinDiv2.Enabled = false;
            numericUpDownMinDiv2.Value = 1;
            numericUpDownMaxDiv2.Enabled = false;
            numericUpDownMaxDiv2.Value = 10;
            label10.Enabled = false;
            label11.Enabled = false;
            label12.Enabled = false;
            label13.Enabled = false;
            label14.Enabled = false;
            label15.Enabled = false;

			fSet = false;
        }

        private void buttonReset_Click_1(object sender, EventArgs e)
        {
            Reset();
        }

		// Summ
		private void numericUpDownMinSumm1_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinSumm1.Value > numericUpDownMaxSumm1.Value)
				numericUpDownMaxSumm1.Value = numericUpDownMinSumm1.Value;

			fSet = false;
		}

		private void numericUpDownMaxSumm1_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMaxSumm1.Value < numericUpDownMinSumm1.Value)
				numericUpDownMinSumm1.Value = numericUpDownMaxSumm1.Value;

			fSet = false;
		}

		private void numericUpDownMinSumm2_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinSumm2.Value > numericUpDownMaxSumm2.Value)
				numericUpDownMaxSumm2.Value = numericUpDownMinSumm2.Value;

			fSet = false;
		}

		private void numericUpDownMaxSumm2_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMaxSumm2.Value < numericUpDownMinSumm2.Value)
				numericUpDownMinSumm2.Value = numericUpDownMaxSumm2.Value;

			fSet = false;
		}

		// Sub
		private void numericUpDownMinSub1_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinSub1.Value > numericUpDownMaxSub1.Value)
				numericUpDownMaxSub1.Value = numericUpDownMinSub1.Value;

			fSet = false;
		}

		private void numericUpDownMaxSub1_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMaxSub1.Value < numericUpDownMinSub1.Value)
				numericUpDownMinSub1.Value = numericUpDownMaxSub1.Value;

			if (numericUpDownMaxSub1.Value < numericUpDownMinSub2.Value)
				numericUpDownMinSub2.Value = numericUpDownMaxSub1.Value;

			if (numericUpDownMaxSub1.Value < numericUpDownMaxSub2.Value)
				numericUpDownMaxSub2.Value = numericUpDownMaxSub1.Value;

			fSet = false;
		}

		private void numericUpDownMinSub2_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinSub2.Value > numericUpDownMaxSub2.Value)
				numericUpDownMaxSub2.Value = numericUpDownMinSub2.Value;

			if (numericUpDownMinSub2.Value > numericUpDownMaxSub1.Value)
				numericUpDownMaxSub1.Value = numericUpDownMinSub2.Value;

			fSet = false;
		}

		private void numericUpDownMaxSub2_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMaxSub2.Value < numericUpDownMinSub2.Value)
				numericUpDownMinSub2.Value = numericUpDownMaxSub2.Value;

			if (numericUpDownMaxSub2.Value > numericUpDownMaxSub1.Value)
				numericUpDownMaxSub1.Value = numericUpDownMaxSub2.Value;

			fSet = false;
		}

		// Mul
		private void numericUpDownMinMul1_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinMul1.Value > numericUpDownMaxMul1.Value)
				numericUpDownMaxMul1.Value = numericUpDownMinMul1.Value;

			fSet = false;
		}

		private void numericUpDownMaxMul1_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinMul1.Value > numericUpDownMaxMul1.Value)
				numericUpDownMinMul1.Value = numericUpDownMaxMul1.Value;

			fSet = false;
		}

		private void numericUpDownMinMul2_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinMul2.Value > numericUpDownMaxMul2.Value)
				numericUpDownMaxMul2.Value = numericUpDownMinMul2.Value;

			fSet = false;
		}

		private void numericUpDownMaxMul2_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinMul2.Value > numericUpDownMaxMul2.Value)
				numericUpDownMinMul2.Value = numericUpDownMaxMul2.Value;

			fSet = false;
		}

		// Sub
		private void numericUpDownMinDiv1_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinDiv1.Value > numericUpDownMaxDiv1.Value)
				numericUpDownMaxDiv1.Value = numericUpDownMinDiv1.Value;

			fSet = false;
		}

		private void numericUpDownMaxDiv1_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinDiv1.Value > numericUpDownMaxDiv1.Value)
				numericUpDownMinDiv1.Value = numericUpDownMaxDiv1.Value;

			fSet = false;
		}

		private void numericUpDownMinDiv2_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinDiv2.Value > numericUpDownMaxDiv2.Value)
				numericUpDownMaxDiv2.Value = numericUpDownMinDiv2.Value;

			fSet = false;
		}

		private void numericUpDownMaxDiv2_ValueChanged(object sender, EventArgs e)
		{
			if (fSet)
				return;

			fSet = true;

			if (numericUpDownMinDiv2.Value > numericUpDownMaxDiv2.Value)
				numericUpDownMinDiv2.Value = numericUpDownMaxDiv2.Value;

			fSet = false;
		}
    }
}
