using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
//using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Mathematics
{
	public partial class FormResult : Form
	{
		bool fInit = false;

		public FormResult()
		{
			InitializeComponent();
		}

		public void Init()
		{
			if (fInit)
				return;

			label1.Text = "Пока тесты не проводились.";
			label2.Text = "";
			label3.Text = "";
			label4.Text = "";
			label5.Text = "";

			fInit = true;
		}
	}
}
