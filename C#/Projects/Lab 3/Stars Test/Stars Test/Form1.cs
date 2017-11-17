using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Stars_Test
{
    public partial class Form1 : Form
    {
        Stars stars;
        Random random = new Random();
        public Form1()
        {
            InitializeComponent();
            stars = new Stars(ClientRectangle, random);
            timer1.Start();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            g.FillRectangle(Brushes.Black, ClientRectangle);
            stars.Draw(g);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            stars.Twinkle(random);
            Refresh();
        }
    }
}
